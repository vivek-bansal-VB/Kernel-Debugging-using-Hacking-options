// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/spinlock.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <asm/unistd.h>
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/file.h>

asmlinkage extern long (*sysptr)(void *arg);
int func_stack_overflow(int x);

/* bug for showing stack corruption */
asmlinkage long bug7(void *arg)
{
	int k;

	/* passing a random integer to the function */
	k = func_stack_overflow(7);
	if (arg == NULL)
		return -EINVAL;
	else
		return 0;
}

int func_stack_overflow(int x)
{
	/* implementing an infinite recursion which is not a tail recursion */
	int y;

	x = x+1;
	y = func_stack_overflow(x);
	return x*x;
}

static int __init init_sys_bug7(void)
{
	pr_info("installed new sys_bug7 module\n");
	if (sysptr == NULL)
		sysptr = bug7;
	return 0;
}
static void  __exit exit_sys_bug7(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug7 module\n");
}
module_init(init_sys_bug7);
module_exit(exit_sys_bug7);
MODULE_LICENSE("GPL");
