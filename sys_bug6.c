// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/spinlock.h>

// REF: https://elixir.bootlin.com/linux/v4.20.6/source/arch/alpha/
// kernel/irq_i8259.c
static DEFINE_SPINLOCK(my_lock);
asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long bug6(void *arg)
{
	int critical_section_var;

	// using a spinloc to acquire the lock
	spin_lock(&my_lock);
	// critical section starts below
	critical_section_var = 10;
	// critical section ends here. The bug
	// here is that we are not setting the lock
	// free before returning to the user land.
	// This BUG will only be caught if the
	// detect incorrect freeing of locks is
	// turned on from the kernel hacking options
	return 0;
}

static int __init init_sys_bug6(void)
{
	pr_info("installed new sys_bug6 module\n");
	if (sysptr == NULL)
		sysptr = bug6;
	return 0;
}
static void  __exit exit_sys_bug6(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug6 module\n");
}
module_init(init_sys_bug6);
module_exit(exit_sys_bug6);
MODULE_LICENSE("GPL");
