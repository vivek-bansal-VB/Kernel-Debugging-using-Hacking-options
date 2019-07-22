obj-m += sys_bug1.o
obj-m += sys_bug2.o
obj-m += sys_bug3.o
obj-m += sys_bug4.o
obj-m += sys_bug5.o
obj-m += sys_bug6.o
obj-m += sys_bug7.o
obj-m += sys_bug8.o
obj-m += sys_bug9.o
obj-m += sys_bug10.o
obj-m += sys_bug11.o
obj-m += sys_bug12.o

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: xhw3 bug

xhw3: xhw3.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi xhw3.c -o xhw3

bug:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f xhw3

