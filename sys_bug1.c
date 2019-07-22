// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>

// REF: https://elixir.bootlin.com/linux/v4.20.6/ident
// REF: https://www.kernel.org/doc/html/latest/dev-tools/kmemleak.html
asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug1(void *arg)
{
	char *name;

	// allocate memory to the name variable
	name = kmalloc(sizeof(char)*10, GFP_KERNEL);
	// if memory allocation failed, return ENOMEM
	if (name == NULL)
		return -ENOMEM;
	pr_info("memory successfully allocated\n");
	// not using kfree to free up the allocated
	// memory before returning back control to
	// the user space. This is the BUG here
	// which gets caught only if the kleak
	// memory KHO detector is turned on
	return 0;
}

static int __init init_sys_bug1(void)
{
	pr_info("installed new sys_bug1 module\n");
	if (sysptr == NULL)
		sysptr = bug1;
	return 0;
}
static void  __exit exit_sys_bug1(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug1 module\n");
}
module_init(init_sys_bug1);
module_exit(exit_sys_bug1);
MODULE_LICENSE("GPL");
