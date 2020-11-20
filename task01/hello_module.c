// Licence: GPL-2.0
/*hello_module.c*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	pr_alert("Hello Ayoub, this is your first module. Enjoy ! \n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_alert("Oh ! That's sad, but I think it's time to say Goodbye ! \n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module");
MODULE_AUTHOR("Ayoub SOUSSI");
