// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug12(void *arg)
{
	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	char *c = NULL;
	int copy;

	copy = copy_from_user(c, (char *)arg, strlen(arg));
	if (copy != 0)
		return -EFAULT;
	return 0;
}

static int __init init_sys_bug12(void)
{
	pr_info("installed new sys_bug12 module\n");
	if (sysptr == NULL)
		sysptr = bug12;
	return 0;
}

static void  __exit exit_sys_bug12(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug12 module\n");
}

module_init(init_sys_bug12);
module_exit(exit_sys_bug12);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sruti Kumari");
MODULE_DESCRIPTION("New system call to detect hardening of copy*user methods");
