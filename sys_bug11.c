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
#include <linux/plist.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>

asmlinkage extern long (*sysptr)(void *arg);

//globally defined priority list head and node variable
static struct plist_head list_head;
static struct plist_node list_node[5];
int prio_list_call(void);

asmlinkage long bug11(void *arg)
{
	int ret; /* ret value */

	ret = prio_list_call();
	pr_info("return value of the function is: %d\n", ret);
	if (arg == NULL)
		return -EINVAL;
	else
		return 0;
}

int prio_list_call(void)
{
	struct plist_node *node_pos, *prio_pos;
	int i;

	pr_info("initializing the head\n");
	plist_head_init(&list_head);
	for (i = 0; i < ARRAY_SIZE(list_node); i++)
		/*initializing every node with initial priority 0*/
		plist_node_init(list_node + i, 0);
	// adding 5 nodes to the priority list with different priorities each.
	list_node[0].prio = 5;
	plist_add(list_node + 0, &list_head);
	list_node[1].prio = 10;
	plist_add(list_node + 1, &list_head);
	list_node[2].prio = 20;
	plist_add(list_node + 2, &list_head);
	list_node[3].prio = 30;
	plist_add(list_node + 3, &list_head);
	list_node[4].prio = 40;
	plist_add(list_node + 4, &list_head);
	/* introducing a bug here
	 * Without deleting the node , trying to change the priority of the node
	 * number 3.
	 */
	list_node[3].prio = 5;
	plist_add(list_node + 3, &list_head);
	//prio_pos stores the first node of the list
	prio_pos = plist_first(&list_head);
	// iterating over the lists
	// Its a sorted descending order doubly linked list
	// The node with lowest number is highest priority one.
	plist_for_each(node_pos, &list_head) {
		pr_info("nodes are %d\n", node_pos->prio);
	}
	return 0;
}

static int __init init_sys_bug11(void)
{
	pr_info("installed new sys_bug11 module\n");
	if (sysptr == NULL)
		sysptr = bug11;
	return 0;
}
static void  __exit exit_sys_bug11(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug11 module\n");
}
module_init(init_sys_bug11);
module_exit(exit_sys_bug11);
MODULE_LICENSE("GPL");
