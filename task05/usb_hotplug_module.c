/*
 * Licence: GPL-2.0
 *
 * hello_module.c
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module");
MODULE_AUTHOR("Ayoub SOUSSI");

static struct usb_device_id hello_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
	USB_INTERFACE_SUBCLASS_BOOT,
	USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, hello_id_table);

static int hello_probe(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	pr_debug("[HELLO MODULE] : NEW USB KEYBOARD DETECTED!\n");
	return 0;
}

static void hello_disconnect(struct usb_interface *interface)
{
	pr_debug("[HELLO MODULE] : USB KEYBOARD IS DISCONNECTED !\n");
}

static struct usb_driver hello_driver = {
	.name = "hello_driver",
	.probe = hello_probe,
	.disconnect = hello_disconnect,
	.id_table = hello_id_table
};


static int __init usb_detector_init(void)
{
	int retval = 0;

	pr_alert("Hello Ayoub, this is your first module. Enjoy !\n");
	retval = usb_register(&hello_driver);
	if (retval)
		pr_debug("[HELLO MODULE] :usb_register failed ! Error number %d\n", retval);

	return 0;
}

static void __exit usb_detector_exit(void)
{
	usb_deregister(&hello_driver);
	pr_alert("Oh ! That's sad, but I think it's time to say Goodbye !\n");
}

module_init(usb_detector_init);
module_exit(usb_detector_exit);
