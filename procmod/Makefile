obj-m +=procmod.o
#obj-m +=sample.o
#
#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

obj-m += sample.o
 
KDIR = /lib/modules/4.19.9/build
 
 
all:
	make -C $(KDIR)  M=$(shell pwd) modules
	
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
