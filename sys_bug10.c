// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>

// REF: https://elixir.bootlin.com/linux/v4.20.6/source/include/linux/string.h
//REF: https://elixir.bootlin.com/linux/v4.20.6/source/arch/x86/lib/memcpy_32.c
asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug10(void *arg)
{
	char a[5];
	char b[] = "abcdefghij";

	pr_info("checking in sys bug 10\n");
	// copying memory contents of b
	// upto lenght strlen(b) to a.
	// this is a bug as variable a is
	// only supposed to have a max of 5
	// characters. This bug is only detected
	// if the config option is turned on.
	memcpy(a, b, strlen(b));
	return 0;
}

static int __init init_sys_bug10(void)
{
	pr_info("installed new sys_bug10 module\n");
	if (sysptr == NULL)
		sysptr = bug10;
	return 0;
}
static void  __exit exit_sys_bug10(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug10 module\n");
}
module_init(init_sys_bug10);
module_exit(exit_sys_bug10);
MODULE_LICENSE("GPL");
