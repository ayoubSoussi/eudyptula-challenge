/*
 * Licence: GPLv2
 *
 * debugfs_module.c
 */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/jiffies.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple debugfs module");
MODULE_AUTHOR("Ayoub SOUSSI");

static ssize_t debugfs_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	ssize_t retval = 0;
	char jiffies_str[20];
	
	snprintf(jiffies_str, sizeof jiffies_str, "%llu", get_jiffies_64());

	if (copy_to_user(buf, jiffies_str, sizeof jiffies_str)) {
		retval = -EFAULT;
		pr_alert("[DEBUGFS MODULE]Error in copy_to_user function !\n");
		goto out;
	}

	(*f_pos) += sizeof jiffies_str;
	retval += sizeof jiffies_str;
out:
	pr_alert("[DEBUGFS MODULE]Read operation succeeded !\n");
	return retval;
}

static const struct file_operations debugfs_fops = {
	.read = debugfs_read,
};

static struct dentry *dir_parent;

static int __init dfs_init(void)
{
	pr_alert("[DEBUGFS_MODULE] Module loaded !\n");
	dir_parent = debugfs_create_dir("eudyptula", NULL);
	debugfs_create_file("jiffies", S_IRUGO, dir_parent, NULL,
		       	&debugfs_fops);

	return 0;
}

static void __exit dfs_exit(void)
{
	debugfs_remove_recursive(dir_parent);
	pr_alert("[DEBUGFS MODULE] Module unloaded !\n");
}

module_init(dfs_init);
module_exit(dfs_exit);
