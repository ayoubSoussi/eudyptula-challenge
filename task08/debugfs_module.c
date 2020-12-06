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

#define DEVEUDYID "EudyID!"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple debugfs module");
MODULE_AUTHOR("Ayoub SOUSSI");

static ssize_t debugfs_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	ssize_t retval = 0;
	char *id = DEVEUDYID;

	if (copy_to_user(buf, id, sizeof(id))) {
		retval = -EFAULT;
		pr_alert("[DEBUGFS MODULE]Error in copy_to_user function !\n");
		goto out;
	}

	(*f_pos) += sizeof(id);
	retval += sizeof(id);
out:
	pr_alert("[DEBUG MODULE]Read operation succeeded !\n");
	return retval;

}

static ssize_t debugfs_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
	char *id = kcalloc(sizeof(buf), sizeof(char), GFP_KERNEL);
	ssize_t retval = 0;
	const char *DEV_ID = DEVEUDYID;

	if (copy_from_user(id, buf, sizeof(DEV_ID))) {
		pr_alert("[DEBUGFS MODULE] Error in copy_from_user function !\n");
		retval = -EFAULT;
		goto out;
	}
	pr_alert("[DEBUGFS MODULE] %s, %s\n", DEV_ID, id);
	if (strncmp(DEV_ID, id, strlen(DEV_ID))) {
		pr_alert("[DEBUGFS MODULE] ID IS NOT CORRECT !\n");
		retval = -EINVAL;
		goto out;
	}

	pr_alert("[DEBUGFS MODULE] Coongrats ! ID IS CORRECT!!\n");

out:
	kfree(id);
	return retval;
}

static const struct file_operations debugfs_fops = {
	.read = debugfs_read,
	.write = debugfs_write,
};

static struct dentry *dir_parent;

static int __init dfs_init(void)
{
	pr_alert("[DEBUGFS_MODULE] Module loaded !\n");
	dir_parent = debugfs_create_dir("eudyptula", NULL);
	debugfs_create_file("id", S_IRUGO | S_IWUGO, dir_parent, NULL,
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
