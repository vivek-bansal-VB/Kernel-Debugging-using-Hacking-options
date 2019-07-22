// SPDX-License-Identifier: GPL-2.0
#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/list.h>
#include <linux/slab.h>

struct k_list {
	int temp;
	struct list_head test_list;
};

asmlinkage extern long (*sysptr)(void *arg);

//Bug 5: CONFIG_DEBUG_LIST
asmlinkage long bug5(void *arg)
{
	int ret = 0;
	struct k_list *one = NULL, *two = NULL, *three = NULL;
	LIST_HEAD(test_head);

	//Initializing list element
	one = kmalloc(sizeof(struct k_list), GFP_KERNEL);
	if (!one)
		return -ENOMEM;
	one->temp = 10;
	two = kmalloc(sizeof(struct k_list), GFP_KERNEL);
	if (!two) {
		ret = -ENOMEM;
		goto out;
	}
	two->temp = 20;
	three = kmalloc(sizeof(struct k_list), GFP_KERNEL);
	if (!three) {
		ret = -ENOMEM;
		goto out;
	}
	three->temp = 30;

	//Creating linked list
	list_add_tail(&(one->test_list), &test_head);
	list_add_tail(&(two->test_list), &test_head);

	//Corrupting Linked List by change next of one to three and
	//prev of two already points to 1
	(&(one->test_list))->next = NULL;

	//Adding new node between node one and three
	//Throws linked list add corruption
	__list_add(&(three->test_list), &(one->test_list), &(two->test_list));
out:
	kfree(one);
	kfree(two);
	kfree(three);
	return ret;
}

static int __init init_sys_bug5(void)
{
	pr_info("installed new sys_bug5 module\n");
	if (sysptr == NULL)
		sysptr = bug5;
	return 0;
}

static void  __exit exit_sys_bug5(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	pr_info("removed sys_bug5 module\n");
}
module_init(init_sys_bug5);
module_exit(exit_sys_bug5);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sruti Kumari");
MODULE_DESCRIPTION("New system call to reproduce linked list corruption");
