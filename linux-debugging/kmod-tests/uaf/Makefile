obj-m += uaf.o
MY_CFLAGS +=-g -DDEBUG
ccflags-y += ${MY_CFLAGS}
CC += ${MY_CFLAGS}

all:
	make -C /lib/modules/$(shell uname  -r)/build M=$(PWD) modules 

clean:
	make -C /lib/modules/$(shell uname  -r)/build M=$(PWD) clean

debug:
	make -C /lib/modules/$(shell uname  -r)/build M=$(PWD) modules EXTRA_CFLAGS="$(MY_CFLAGS)"
