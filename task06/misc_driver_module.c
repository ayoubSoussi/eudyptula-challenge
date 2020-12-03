// Licence: GPL-2.0
/*
 * hello_module.c
 */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/string.h>

#define DEVEUDYID "EudyID!"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple misc character device driver");
MODULE_AUTHOR("Ayoub SOUSSI");

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	ssize_t retval = 0;
	char *id = DEVEUDYID;

	if (copy_to_user(buf, id, sizeof(id))) {
		retval = -EFAULT;
		pr_alert("[MISC MODULE]Error in copy_to_user function !\n");	
		goto out;
	}

	(*f_pos) += sizeof(id);
	retval += sizeof(id);
out:
	pr_alert("[MISC MODULE]Read operation succeeded !\n");
	return retval;

}

static ssize_t misc_write(struct file *filp, const char __user *buf,
	       	size_t count, loff_t *f_pos)
{
	char *id = kcalloc(sizeof(buf), sizeof(char), GFP_KERNEL);
	ssize_t retval = 0;
	const char *DEV_ID = DEVEUDYID;

	if (copy_from_user(id, buf, sizeof(DEV_ID))) {
		pr_alert("[MISC MODULE] Error in copy_from_user function !\n");
		retval = -EFAULT;
		goto out;	
	}
	pr_alert("[MISC MODULE] %s, %s \n", DEV_ID, id);
	if (strncmp(DEV_ID, id, strlen(DEV_ID))) {
		pr_alert("[MISC MODULE] ID IS NOT CORRECT !\n");
		retval = -EINVAL;
	       	goto out;
	}

	pr_alert("[MISC MODULE] Coongrats ! ID IS CORRECT!!\n");

out:
	return retval;
}

static const struct file_operations misc_fops = {
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_name",
	.fops = &misc_fops,
	.nodename = "misc_node_name",
};

static int __init misc_init(void)
{
	pr_alert("[MISC_MODULE] Module loaded !\n");
	return misc_register(&misc_dev);
}

static void __exit misc_exit(void)
{
	misc_deregister(&misc_dev);
	pr_alert("[MISC_MODULE] Module unloaded !\n");
}

module_init(misc_init);
module_exit(misc_exit);
