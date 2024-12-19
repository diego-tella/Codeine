obj-m := codeine.o
CC = gcc-12 -Wall
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
 $(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
 $(MAKE) -C $(KDIR) M=$(PWD) clean
