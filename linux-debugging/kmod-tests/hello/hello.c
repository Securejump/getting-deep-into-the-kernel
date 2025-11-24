#include<linux/module.h>
#include<linux/init.h>

int kmod_init(void)
{
        printk("kmod-hello - module loaded!\n");
        return 0;
}

void kmod_exit(void)
{
        printk("kmod-hello - module unloaded!\n");
}

module_init(kmod_init);
module_exit(kmod_exit);

MODULE_LICENSE("GPL");
