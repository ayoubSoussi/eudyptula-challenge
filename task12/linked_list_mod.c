#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPLv2");
MODULE_DESCRIPTION("Simple linked list module");
MODULE_AUTHOR("Ayoub SOUSSI");

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head mylist;
};

static struct list_head mylinkedlist = LIST_HEAD_INIT(mylinkedlist);

static int identity_create(char *name, int id)
{
	struct identity *elem;	
	
	if (strlen(name) > 20)	
		return -EINVAL;

	elem = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!elem)
		return -ENOMEM;

	strncpy(elem->name, name, strlen(name));
	elem->id = id;

	list_add(&elem->mylist, &mylinkedlist);
	
	return 0;
}

struct identity *identity_find(int id)
{
	struct identity *id_elem = NULL;

	list_for_each_entry(id_elem, &mylinkedlist, mylist)
	{	
		if (id_elem->id == id)
			return id_elem;
	}

	return NULL;
}

void identity_destroy(int id)
{
	struct identity *id_elem = NULL;

        list_for_each_entry(id_elem, &mylinkedlist, mylist)
        {
                if (id_elem->id == id) {
			__list_del_entry(&id_elem->mylist);
			kfree(id_elem);
		}       
        }
}

static int __init my_init(void)
{
	int ret = 0;
	struct identity *temp;
	
        ret = identity_create("Alice", 1);

	if (ret)
		pr_alert("Error while adding Alice : %d\n", ret);

	pr_alert("Success while adding Alice : %d\n", ret);

        ret = identity_create("Bob", 2);
       	
	if (ret)
		pr_alert("Error while adding Bob : %d\n", ret);

	ret = identity_create("Dave", 3);
       	
	if (ret)
		pr_alert("Error while adding Dave : %d\n", ret);

 	ret = identity_create("Gena", 10);
	
	if (ret)
		pr_alert("Error while adding Gena : %d\n", ret);

        temp = identity_find(3);
        pr_alert("id 3 = %s\n", temp->name);

        temp = identity_find(42);
        if (temp == NULL)
                pr_alert("id 42 not found\n");

        identity_destroy(2);
        identity_destroy(1);
        identity_destroy(10);
        identity_destroy(42);
        identity_destroy(3);
	
	temp = identity_find(3);
        if (temp == NULL)
                pr_alert("id 3 not found\n");
	
	return 0;
}

static void __exit my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
