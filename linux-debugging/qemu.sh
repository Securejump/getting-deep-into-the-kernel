#!/bin/bash

KERNEL="/home/anti/Documents/Research/linux-debugging/linux-6.17.7/arch/x86_64/boot/bzImage"
CPIO="/home/anti/Documents/Research/linux-debugging/cpio/initrd.cpio"
IMAGE="/home/anti/Documents/Research/linux-debugging/syzkaller/tools/bullseye.img"

        #-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0 kasan.fault=panic" \

qemu-system-x86_64 \
        -kernel $KERNEL \
        -m 2G \
        -s \
        -S \
        -append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0 kasan.fault=panic" \
        -drive file=$IMAGE,format=raw \
        -net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10021-:22 \
        -enable-kvm \
        -net nic,model=e1000 \
        -pidfile vm.pid \
        -nographic \
        2>&1 | tee vm.log
