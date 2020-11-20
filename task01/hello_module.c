// Licence: GPL-2.0
/*hello_module.c*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	pr_alert("Hello Ayoub, this is your first module. Enjoy !");
	return 0;
}

static int __exit hello_exit(void)
{
	pr_alert("Oh ! That's sad, but I think it's time to say Godbye !");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENCE("GPL");
MODULE_DESCRIPTION("Hello module");
MODULE_AUTHOR("Ayoub SOUSSI");
