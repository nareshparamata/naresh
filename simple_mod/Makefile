obj-m += simpchar.o

all:
  	
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
#	make -C /usr/src/linux-headers-$(shell uname -r) M=$(shell pwd) modules
clean:
	rm -f devread devwrite devtest *.o *.ko 
