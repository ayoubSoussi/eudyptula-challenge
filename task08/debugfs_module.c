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
#include <linux/rwlock.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple debugfs module");
MODULE_AUTHOR("Ayoub SOUSSI");

static char mem_page[PAGE_SIZE];
static rwlock_t memlock = __RW_LOCK_UNLOCKED(memlock);

static ssize_t debugfs_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	ssize_t retval = 0;
	unsigned long flags;

	read_lock_irqsave(&memlock, flags);
	if (copy_to_user(buf, mem_page, sizeof(mem_page))) {
		retval = -EFAULT;
		pr_alert("[DEBUGFS MODULE]Error in copy_to_user function !\n");
		goto out;
	}

	(*f_pos) += sizeof(mem_page);
	retval += sizeof(mem_page);
out:
	read_unlock_irqrestore(&memlock, flags);
	pr_alert("[DEBUGFS MODULE]Read operation succeeded !\n");
	return retval;
}

static ssize_t debugfs_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	unsigned long flags;

	write_lock_irqsave(&memlock, flags);
	if (copy_from_user(mem_page, buf, sizeof(buf))) {
		pr_alert("[DEBUGFS MODULE] Error in copy_from_user function !\n");
		retval = -EFAULT;
		goto out;
	}

	pr_alert("[DEBUGFS MODULE] Write operation succeeded !!\n");

out:
	write_unlock_irqrestore(&memlock, flags);
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
	debugfs_create_file("foo", S_IWUSR | S_IRUGO, dir_parent, NULL,
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
