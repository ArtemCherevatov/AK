#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Artem Cherevatov");
MODULE_DESCRIPTION("Lab 5: Hello 2 kernel module (Uses print_hello)");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
	uint i;

	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("Warning: print_count is %u\n", print_count);
	} else if (print_count > 10) {
		pr_err("Error: print_count (%u) is greater than 10\n", print_count);
		return -EINVAL;
	}

	for (i = 0; i < print_count; i++) {
		print_hello();
	}

	return 0;
}

static void __exit hello2_exit(void)
{
	/* Nothing specific to clean up here, hello1 handles the list */
}

module_init(hello2_init);
module_exit(hello2_exit);
