#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/bug.h>

MODULE_AUTHOR("Artem Cherevatov");
MODULE_DESCRIPTION("Lab 6: Kernel Module with BUG_ON and Oops");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

struct hello_time_node {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_time_list);

static int __init hello_init(void)
{
	uint i;
	struct hello_time_node *node;

	BUG_ON(print_count > 10);

	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("Warning: print_count is %u\n", print_count);
	}

	for (i = 0; i < print_count; i++) {
		node = kmalloc(sizeof(*node), GFP_KERNEL);

		if (i == 4) {
			node = NULL;
		}

		if (!node) {
			pr_err("Memory allocation failed. Simulating crash...\n");
		}

		node->time = ktime_get();
		list_add_tail(&node->list, &hello_time_list);
		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time_node *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, &hello_time_list, list) {
		pr_info("Time: %lld ns\n", ktime_to_ns(pos->time));
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello_init);
module_exit(hello_exit);
