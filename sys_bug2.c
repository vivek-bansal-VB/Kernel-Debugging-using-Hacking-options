// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>

asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug2(void *arg)
{
	long t_start, diff = 0;

	t_start = ktime_get_real_seconds();
	do {
		diff = ktime_get_real_seconds() - t_start;
	} while (diff < 29);
	return 0;
}

static int __init init_sys_bug2(void)
{
	pr_info("installed new sys_bug2 module\n");
	if (sysptr == NULL)
		sysptr = bug2;
	return 0;
}

static void  __exit exit_sys_bug2(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug2 module\n");
}
module_init(init_sys_bug2);
module_exit(exit_sys_bug2);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sruti Kumari");
MODULE_DESCRIPTION("New system call to reproduce soft lockup");

