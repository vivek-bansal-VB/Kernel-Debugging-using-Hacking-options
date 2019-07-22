// SPDX-License-Identifier: GPL-2.0
/* Module to detect workqueue Stalls */

#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/swap.h>
#include <linux/uaccess.h>
#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>

/* Reference: http://tuxthink.blogspot.com/2011/02/kernel-thread-creation-1.html */
/* Reference: http://www.cs.fsu.edu/~cop4610t/lectures/project2/kthreads/kthreads.pdf */
/* Reference: https://stackoverflow.com/questions/20892822/how-to-use-lockdep-feature-in-linux-kernel-for-deadlock-detection?rq=1 */
/* Reference: https://kernel.readthedocs.io/en/sphinx-samples/kernel-locking.html */
/* Reference: https://stackoverflow.com/questions/20892822/how-to-use-lockdep-feature-in-linux-kernel-for-deadlock-detection */

static DEFINE_SPINLOCK(lock);
struct task_struct *thread1; /* thread1 */
/* main thread to wait for thread1*/
int sem = 1;
long ctr;

asmlinkage extern long (*sysptr)(void *arg);

/* thread1 run function */
int mythread1(void *arg)
{
	int i = 0, j = 0;

	spin_lock(&lock); /* holding the lock */

	/* Doing intensive computational work to stall workqueue */
	for (i = 0; i < 10000; i++)
		for (j = 0; j < 10000; j++)
			ctr++;

	spin_unlock(&lock); /* releasing the lock */
	pr_info("Thread1 finished its execution\n");
	sem--;   // to signal its complete execution
	return 0;
}

asmlinkage long bug9(void *arg)
{
	char thread1_name[] = "thread1_name";

	if (arg == NULL)
		return -EINVAL;

	/* created thread1 */
	thread1 = kthread_create(&mythread1, NULL, thread1_name);
	if (thread1) {
		/* thread1 scheduled to run */
		wake_up_process(thread1);
		pr_info("Thread1 started\n");
	}

	while (sem > 0)
		;

	return 0;
}

static int __init init_sys_bug9(void)
{
	pr_info("installed module9\n");
	if (sysptr == NULL)
		sysptr = bug9;
	return 0;
}

static void  __exit exit_sys_bug9(void)
{
	int ret = -1;

	ret = kthread_stop(thread1); //  stop thread1
	if (!ret)
		pr_info("Thread1 stopped\n");

	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed module9\n");
}
module_init(init_sys_bug9);
module_exit(exit_sys_bug9);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sruti Kumari");
MODULE_DESCRIPTION("New system call to detect workqueue stalls");

