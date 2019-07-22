// SPDX-License-Identifier: GPL-2.0
/* Module to detect Deadlock in case of 2 threads */

#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/delay.h>

/* Reference: http://tuxthink.blogspot.com/2011/02/kernel-thread-creation-1.html */
/* Reference: http://www.cs.fsu.edu/~cop4610t/lectures/project2/kthreads/kthreads.pdf */
/* Reference: https://stackoverflow.com/questions/20892822/how-to-use-lockdep-feature-in-linux-kernel-for-deadlock-detection?rq=1 */
/* Reference: https://kernel.readthedocs.io/en/sphinx-samples/kernel-locking.html */
/* Reference: https://stackoverflow.com/questions/20892822/how-to-use-lockdep-feature-in-linux-kernel-for-deadlock-detection */

static DEFINE_MUTEX(lock1);  /* lock1 to provide mutual exclusion on var1 */
static DEFINE_MUTEX(lock2);  /* lock2 to provide mutual exclusion on var2 */

 /* var1 is shared memory accessed by 2 threads in a concurrent manner */
int var1;
/* var2 is shared memory accessed by 2 threads in a concurrent manner */
int var2;
struct task_struct *thread1; /* thread1 */
struct task_struct *thread2; /* thread2 */
/* main thread to wait for thread1 and thread2 */
int sem = 2;

asmlinkage extern long (*sysptr)(void *arg);

/* thread1 run function */
int mythread1(void *arg)
{
	mutex_lock(&lock1);  // acquire lock1
	var1++;		     // increment var1
	pr_info("Thread1 going to sleep for 100 seconds\n");
	/* go to sleep for 100 seconds */
	ssleep(100);
	pr_info("Thread1 wakes up after sleeping for 100 seconds\n");
	mutex_lock(&lock2);   // acquire lock2
	var2++;		      // increment var2
	mutex_unlock(&lock2); // unlock lock2
	mutex_unlock(&lock1); // unlock lock1
	pr_info("Thread1 finished its execution\n");
	sem--;	 // to signal its complete execution
	return 0;
}

/* thread2 run function */
int mythread2(void *arg)
{
	mutex_lock(&lock2);  // acquire lock2
	var2++;		     // increment var2
	pr_info("Thread2 going to sleep for 100 seconds\n");
	ssleep(100);	     // sleep for 100 seconds
	pr_info("Thread2 wakes up after sleeping for 100 seconds\n");
	mutex_lock(&lock1);  // acquire lock1
	var1++;		     // increment var1
	mutex_unlock(&lock1);	 // unlock lock1
	mutex_unlock(&lock2);	 // unlock lock2
	pr_info("var1 = %d\n", var1);
	pr_info("var2 = %d\n", var2);
	pr_info("Thread2 finished its execution\n");
	sem--;	// to signal its complete execution
	return 0;
}

/* syscall to detect deadlock */
asmlinkage long bug4(void *arg)
{
	char thread1_name[] = "thread1_name";
	char thread2_name[] = "thread2_name";

	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	pr_info("sys_bug4 received arg %p\n", arg);
	if (arg == NULL)
		return -EINVAL;

	/* created thread1 */
	thread1 = kthread_create(&mythread1, NULL, thread1_name);
	if (thread1) {
		/* thread1 scheduled to run */
		wake_up_process(thread1);
		pr_info("Thread1 started\n");
	}

	/* created thread2 */
	thread2 = kthread_create(&mythread2, NULL, thread2_name);
	if (thread2) {
		/* thread2 scheduled to run */
		wake_up_process(thread2);
		pr_info("Thread2 started\n");
	}

	/* wait for thread1 and thread2 to finish its execution */
	while (sem > 0)
		;
	return 0;
}

static int __init init_sys_bug4(void)
{
	pr_info("installed new sys_bug4 module\n");
	if (sysptr == NULL)
		sysptr = bug4;
	return 0;
}

static void  __exit exit_sys_bug4(void)
{
	int ret = -1;

	ret = kthread_stop(thread1); //  stop thread1
	if (!ret)
		pr_info("Thread1 stopped\n");
	ret = kthread_stop(thread2);  // stop thread2
	if (!ret)
		pr_info("Thread2 stopped\n");
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug4 module\n");
}
module_init(init_sys_bug4);
module_exit(exit_sys_bug4);
MODULE_LICENSE("GPL");

