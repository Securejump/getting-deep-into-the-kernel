#!/bin/bash

KERNEL="path/to/kernel/code/arch/x86_64/boot/bzImage"
CPIO="path/to/cpio.cpio"
IMAGE="path/to/kernel/image.img"

qemu-system-x86_64 \
        -kernel $KERNEL \
        -append "nokaslr" \
        -m 2G \
        -s \
        -S \
        -initrd "$CPIO"
