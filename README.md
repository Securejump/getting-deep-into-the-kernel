# getting-deep-into-the-kernel

welcome to the notes and slides of the "Bajando el nivel: metiendo manos en el kernel Linux" talk. to begin, i advise:

- downloading a release of the linux kernel source code from this [link](https://www.kernel.org/).
- downloading and installing the following packages:
    - gdb
    - make
    - c
    - cmake
    - glibc static libraries
- read the talk and be ready.
    - keep the notes at hand. you'll need them :)

# structure

```
GettingDeepLinuxKernel.pdf -> slides.
linux-debugging/ -> miscelaneous things. check the qq.sh and qemu.sh scripts. might be useful.
linux-debugging/kmod-tests/ -> various drivers.
linux-debugging/kmod-tests/hello/ -> simple hello world driver.
linux-debugging/kmod-tests/uaf/ -> buggy driver, used for testing KASAN (use-after-free, in this case).
linux-debugging/kmod-tests/newnull/ -> "new" null driver. needs manual device registration (via mknod).
linux-debugging/kmod-tests/kecho/ -> unfinished kernel echo.
linux-debugging/ftrace/ -> folder with snippets of code to use with trace-cmd.
linux-debugging/ftrace/code_example/ -> simple C hello world. use it with trace-cmd.
linux-debugging/ftrace/ping/ -> contains a script I used for exploring the ping command and how ping works.
notes/ -> the actual notes.
```
