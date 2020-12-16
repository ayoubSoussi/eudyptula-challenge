/*
 * Licence: GPLv2
 *
 * sysfs_module.c
 */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/rwlock.h>
#include <linux/kobject.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple sysfs module");
MODULE_AUTHOR("Ayoub SOUSSI");

static char mem_page[PAGE_SIZE];
static rwlock_t memlock = __RW_LOCK_UNLOCKED(memlock);

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	ssize_t retval = 0;
	unsigned long flags;

	read_lock_irqsave(&memlock, flags);

	strncpy(buf, mem_page, sizeof(mem_page));
	retval += sizeof(mem_page);

	read_unlock_irqrestore(&memlock, flags);
	pr_alert("[SYSFS MODULE]Read operation succeeded !\n");
	return retval;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	ssize_t retval = 0;
	unsigned long flags;

	write_lock_irqsave(&memlock, flags);

	strncpy(mem_page, buf, count);

	pr_alert("[SYSFS MODULE] Write operation succeeded !!\n");

	write_unlock_irqrestore(&memlock, flags);
	return retval < 0 ? retval : count;
}

static ssize_t jiff_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	ssize_t retval = 0;
	char jiffies_str[20];

	snprintf(jiffies_str, sizeof(jiffies_str), "%llu", get_jiffies_64());

	strncpy(buf, jiffies_str, sizeof(jiffies_str));

	retval += sizeof(jiffies_str);

	pr_alert("[DEBUGFS MODULE]Read operation succeeded !\n");
	return retval;
}

struct kobject *task_kobj;
static struct kobj_attribute foo_attr = __ATTR_RW(foo);
static struct kobj_attribute jiff_attr = __ATTR_RO(jiff);

static int __init task_init(void)
{
	int rc = 0;

	task_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!task_kobj) {
		pr_err("[SYSFS MODULE] Failed to create task_kobj\n");
		return 0;
	}

	pr_alert("[SYSFS_MODULE] kobj created !\n");

	rc = sysfs_create_file(task_kobj, &foo_attr.attr);
	if (rc)
		pr_err("[SYSFS MODULE] Unable to create foo sysfs file !\n");

	rc = sysfs_create_file(task_kobj, &jiff_attr.attr);

	if (rc)
		pr_err("[SYSFS MODULE] Unable to create jiff sysfs file !\n");


	return 0;
}

static void __exit task_exit(void)
{
	sysfs_remove_file(task_kobj, &foo_attr.attr);
	sysfs_remove_file(task_kobj, &jiff_attr.attr);

	kobject_del(task_kobj);

	pr_alert("[SYSFS MODULE] Module unloaded !\n");
}

module_init(task_init);
module_exit(task_exit);
