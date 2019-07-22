// SPDX-License-Identifier: GPL-2.0
// Slub Debugging

#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/kthread.h>
#include <linux/slab.h>

asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug8(void *arg)
{
	int *ptr;

	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	pr_info("sys_bug8 received arg %p\n", arg);
	if (arg == NULL)
		return -EINVAL;

	ptr = kmalloc(8, GFP_KERNEL);
	if (!ptr)
		return -ENOMEM;

	ptr[0] = 0x10; // 4 bytes written
	ptr[1] = 0x20; // 4 bytes written
	ptr[2] = 0x30; // invalid write
	/* Slub debugging gives an error as write to redzone happened */
	kfree(ptr);
	return 0;
}

static int __init init_sys_bug8(void)
{
	pr_info("installed new sys_bug8 module\n");
	if (sysptr == NULL)
		sysptr = bug8;
	return 0;
}

static void  __exit exit_sys_bug8(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug8 module\n");
}

module_init(init_sys_bug8);
module_exit(exit_sys_bug8);
MODULE_LICENSE("GPL");

