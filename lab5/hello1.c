#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Artem Cherevatov");
MODULE_DESCRIPTION("Lab 5: Hello 1 kernel module (Exports print_hello)");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_time_node {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_time_list);

void print_hello(void)
{
	struct hello_time_node *node;

	node = kmalloc(sizeof(*node), GFP_KERNEL);
	if (!node) {
		pr_err("Memory allocation failed\n");
		return;
	}

	node->time_before = ktime_get();
	pr_info("Hello, world!\n");
	node->time_after = ktime_get();

	list_add_tail(&node->list, &hello_time_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_time_node *pos, *tmp;
	ktime_t duration;

	list_for_each_entry_safe(pos, tmp, &hello_time_list, list) {
		duration = ktime_sub(pos->time_after, pos->time_before);
		pr_info("Time to print: %lld ns\n", ktime_to_ns(duration));
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello1_init);
module_exit(hello1_exit);
