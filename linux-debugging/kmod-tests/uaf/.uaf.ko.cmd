savedcmd_uaf.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T /usr/src/kernels/6.15.5-200.fc42.x86_64/scripts/module.lds -o uaf.ko uaf.o uaf.mod.o .module-common.o
