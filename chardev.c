#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define CHRDEVBASE_MAJOR 200
#define CHRDEVBASE_NAME "yzbDev"
#define CMD 123
#define NEWCHRLED_CNT 1
#define NEWCHRLED_NAME "yzbDev"

typedef struct MyDev{
	struct cdev cdev;
	int major;
	struct device *device;
	dev_t devid;
	struct class *class;
}MyDev;

MyDev mydev;

static int chrdevbase_open(struct inode *inode,struct file *filp){
    return 0;
}

static long yzb_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	switch(cmd){
		case CMD:
			arg=250;
			break;
		default:
			arg=0;
			break;
	}
	return 0;
}


static struct file_operations chrdevbase_fops = {
	.owner = THIS_MODULE,	
	.open = chrdevbase_open,
    .unlocked_ioctl = yzb_ioctl,
	//.release = chrdevbase_release,

};

static int __init chrdevbase_init(void)
{
	
	if (mydev.major) {		/*  定义了设备号 */
		mydev.devid = MKDEV(mydev.major, 0);
		register_chrdev_region(mydev.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&mydev.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);	/* 申请设备号 */
	}
	mydev.cdev.owner = THIS_MODULE;
	cdev_init(&mydev.cdev, &chrdevbase_fops);
	cdev_add(&mydev.cdev, mydev.devid, NEWCHRLED_CNT);

	mydev.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
	if (IS_ERR(mydev.class)) {
		return PTR_ERR(mydev.class);
	}

	mydev.device = device_create(mydev.class, NULL, mydev.devid, NULL, NEWCHRLED_NAME);
	if (IS_ERR(mydev.device)) {
		return PTR_ERR(mydev.device);
	}
	return 0;
}


static void __exit chrdevbase_exit(void)
{
	/* 注销字符设备驱动 */
	unregister_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME);
	printk("chrdevbase exit!\r\n");
}

module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yzb");
