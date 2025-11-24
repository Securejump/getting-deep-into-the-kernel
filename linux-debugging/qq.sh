#!/bin/bash

KERNEL="/home/anti/Documents/Research/linux-debugging/linux-6.17.7/arch/x86_64/boot/bzImage"
CPIO="/home/anti/Documents/Research/linux-debugging/cpio/initrd.cpio"
IMAGE="/home/anti/Documents/Research/linux-debugging/syzkaller/tools/bullseye.img"

qemu-system-x86_64 \
        -kernel $KERNEL \
        -append "nokaslr" \
        -m 2G \
        -s \
        -S \
        -initrd "$CPIO"
