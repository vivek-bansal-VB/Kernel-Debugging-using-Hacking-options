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

asmlinkage extern long (*sysptr)(void *arg);
static DEFINE_SPINLOCK(lock);

/* bug for sleep inside atomic section in spinlock */
asmlinkage long bug3(void *arg)
{
	void *kargs;

	spin_lock(&lock); /* holding the lock */
	pr_info("Before sleep\n");
	kargs = kmalloc(40, GFP_KERNEL);
	if (copy_from_user(kargs, arg, 40)) {
		kfree(kargs);
		return -EFAULT;
	}
	pr_info("After sleep\n");
	spin_unlock(&lock); /* releasing the lock */
	if (arg == NULL)
		return -EINVAL;
	else
		return 0;
}

static int __init init_sys_bug3(void)
{
	pr_info("installed new sys_bug3 module\n");
	if (sysptr == NULL)
		sysptr = bug3;
	return 0;
}
static void  __exit exit_sys_bug3(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug3 module\n");
}
module_init(init_sys_bug3);
module_exit(exit_sys_bug3);
MODULE_LICENSE("GPL");

