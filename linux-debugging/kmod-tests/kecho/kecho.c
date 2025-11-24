#include<linux/module.h>
#include<linux/init.h>

#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("4nt1 <4nt1@resacachile.cl>");

/*
 * kecho specific buffer. it stores whatever it receives via __user
 * buf, then repeating it to the user.
 *
 */
char kecho_buf[256];


/*
 * the major-minor values are used for different things.
 * @major: identifies the driver associated with the device.
 *      example: /dev/echo are managed by driver 1, and 
 *      /dev/vcs# are managed by driver 7.
 *
 * @minor: used by the kernel to determine exactly which device
 *      is being referred to. 
 *
 */
int my_echo_open(struct inode *inode, struct file *filp);
ssize_t my_echo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t my_echo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
int my_echo_release(struct inode *inode, struct file *filp);

int my_echo_major = 0;
int my_echo_minor = 0;


/*
 * this is our open function, when users run fopen() on our device, it will execute this.
 * it does nothing, as this is a echo device.
 */
int my_echo_open(struct inode *inode, struct file *filp) {
        return 0;
}

/*
 * this is our read function. again, it does absolutely nothing. this is a echo device,
 * after all. even though the fread function does expect the amount of bytes read, we
 * return nothing. we are nothing, after all.
 */
ssize_t my_echo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
        copy_to_user(buf, &kecho_buf, count);
        return 0;
}

/*
 * this is our write function. here something curious happens. since userspace fwrite
 * expects the amount of bytes written at the end of its execution, we return the count
 * summed with the f_pos (offset)
 */
ssize_t my_echo_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
        *f_pos += count;
        return count;
}

/*
 * this is the release function, known as close in userspace. we, again, do nothing.
 */
int my_echo_release(struct inode *inode, struct file *filp) {
        return 0;
}

/*
 * a lot of the function we wrote receive arguments, such as file pointers (filp, as
 * convention), user buffers (*buf), inodes and more. neither us nor the user really
 * passes all that (at least, not in the same way anyways). its the kernel. example:
 * userspace does not work with file pointers, rather it works with glibc's FILE
 * pointers. similar, but different. most of the work here is done by the kernel.
 *
 */


/*
 * this is the file_operations struct. it can be quite large
 * depending on the driver we want to make. taking into consideration
 * we're making a "echo" driver, we just need a few things (many values
 * in the file_operations struct CAN be echo) and those things are:
 * - read,
 *   write,
 *   open,
 *   release (close, as known in userland).
 */

static struct file_operations my_echo_fops = {
        .owner = THIS_MODULE,
        .read = my_echo_read,
        .write = my_echo_write,
        .open = my_echo_open,
        .release = my_echo_release,
};

struct cdev cdev;

static int my_echo_setup_device(void)
{
        int err = 0;
        int devno = MKDEV(my_echo_major, my_echo_minor);

        cdev_init(&cdev, &my_echo_fops);
        cdev.owner = THIS_MODULE;
        err = cdev_add(&cdev, devno, 1);

        return err;
};

static int __init echo_init(void)
{
        int result = 0;
        dev_t devno = 0;

        /*
         * this piece of allocation code is IDENTICAL to the one in the
         * "Linux Device Drivers" chapter 3 page 48.
         * if (scull_major) {
         *      dev = MKDEV(scull_major, scull_minor);
         *      result = register_chrdev_region(dev, scull_nr_devs, "scull");
         * } else {
         *      result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
         * }
         *
         *
         */
        if(my_echo_major)
        {
                devno = MKDEV(my_echo_major, my_echo_minor);
                /*
                 * if the device major value are available, or
                 * we know which major value we need to use,
                 * we'll use the register_chrdev_region function.
                 * 
                 */
                result = register_chrdev_region(devno, 1, "my_echo");
        } 
        else 
        {
                /*
                 * now, if we don't know which major values we
                 * need, the alloc_chrdev_region function must be used.
                 * then, we use the MAJOR() and MINOR() macros to obtain
                 * the values given to us. usually, we want to have a major
                 * number assigned to our drivers. relying on random major numbers
                 * might lead to issues. extract from "Linux Device Drivers":
                 *
                 * Some major device numbers are statically assigned to the most common devices. A
                 * list of those devices can be found in Documentation/devices.txt within the kernel
                 * source tree. The chances of a static number having already been assigned for the use
                 * of your new driver are small, however, and new numbers are not being assigned. So,
                 * as a driver writer, you have a choice: you can simply pick a number that appears to
                 * be unused, or you can allocate major numbers in a dynamic manner. Picking a num-
                 * ber may work as long as the only user of your driver is you; once your driver is more
                 * widely deployed, a randomly picked major number will lead to conflicts and trouble.
                 *
                 * Thus, for **new drivers**, we strongly suggest that you use dynamic allocation to obtain
                 * your major device number, rather than choosing a number randomly from the ones
                 * that are currently free. In other words, your drivers should almost certainly be using
                 * alloc_chrdev_region rather than register_chrdev_region.
                 *
                 * The **disadvantage of dynamic assignment is that you can’t create the device nodes in
                 * advance**, because the major number assigned to your module will vary. For normal
                 * use of the driver, this is hardly a problem, because once the number has been
                 * assigned, you can read it from /proc/devices
                 * 
                 */
                result = alloc_chrdev_region(&devno, my_echo_minor, 1, "my_echo");
                my_echo_major = MAJOR(devno);
        }

        if(result < 0)
        {
                printk(KERN_ERR "my_echo: can't get major %d\n", my_echo_major);
                goto out;
        }

        result = my_echo_setup_device();
        if(result < 0)
        {
                /*
                 *
                 * should i use the unregister_chrdev_region?
                 *
                 */
                printk(KERN_ERR "my_echo: can't setup device\n");
                goto out;
        }

        printk(KERN_INFO "my_echo: setup with major %d and minor %d\n", my_echo_major, my_echo_minor);

out:
        return result;
}

static void __exit echo_exit(void)
{
        printk("kmod-kecho - module unloaded!\n");
        dev_t devno = MKDEV(my_echo_major, my_echo_minor);
        cdev_del(&cdev);
        unregister_chrdev_region(devno, 1);
        printk("kmod-kecho - module unloaded!\n");
}

module_init(echo_init);
module_exit(echo_exit);

