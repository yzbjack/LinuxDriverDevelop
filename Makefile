obj-m = chardev.o

KDIR := /home/yzb/orangepi4Android/RK3399-Android8.1/kernel

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm64  modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
