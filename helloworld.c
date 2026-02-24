#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/device.h>
#define MUTEX_LOCK
static int major = 0;
static int minor = 0;
struct device_private
{
    dev_t chrdev_num;
    int major;
    int minor;
    struct cdev my_cdev;
    struct device *hello_devices;
    struct class *hello_class;
    struct semaphore sema;
    struct mutex mutex_lock;
    int lock_flag;
} hello_data;
module_param(major,int ,S_IRUGO);
module_param(minor,int ,S_IRUGO);
// MODULE_DESCRIPTION(a,)
// static struct file_operations test_fops;
static int hello_open(struct inode *inode ,struct file *file)
{
#ifdef SEMAPHORE_LOCK
    int ret = 0;
    ret = down_interruptible(&hello_data.sema);
    if (ret == -EINTR )
    {
        printk("");
        return -EINTR;
    }
    file->private_data = (struct device_private *)&hello_data;
#endif
#ifdef MUTEX_LOCK
    mutex_lock(&hello_data.mutex_lock);
    if (hello_data.lock_flag != 1)
    {
        mutex_unlock(&hello_data.mutex_lock);
        return -EBUSY;   
    }
    hello_data.lock_flag = 0;
    mutex_unlock(&hello_data.mutex_lock);
#endif // MUTEX_LOCK


    printk("this hello device is opend \n");
    return 0;
    // int a;
}
static ssize_t hello_read(struct file  *file ,char __user *buf, size_t size, loff_t *off){
    return 0;
}

static ssize_t hello_write(struct file *file ,const char __user *buf ,size_t size ,loff_t *off){
    // struct device_private * hello_dev = (struct device_private *) file->private_data;
    return 0;
}

static int hello_release(struct inode * inode , struct file *file){
    printk("close hello dev\n");
#ifdef SEMAPHORE_LOCK
    up(&hello_data.sema);
#endif

#ifdef MUTEX_LOCK
    mutex_lock(&hello_data.mutex_lock);
    hello_data.lock_flag = 1;
    mutex_unlock(&hello_data.mutex_lock);
#endif // MUTEX_LOCK
    return 0;
}
struct file_operations Hello_FileOperations = 
{
    .owner = THIS_MODULE,
    .open = hello_open,
    .read = hello_read,
    .write = hello_write,
    .release = hello_release,
};
/* 驱动入口函数 */
static int __init helloworld_init(void)
{
    int ret;
#ifdef SEMAPHORE_LOCK
    sema_init(&hello_data.sema,1);
#endif // SEMAPHORE_LOCK
    
#ifdef MUTEX_LOCK
    mutex_init(&hello_data.mutex_lock);
#endif // MUTEX_LOCK

    /* 入口函数具体内容 */
    // int retvalue = 0;
    // int ret;
    if (major)
    {
        printk("major dev num = %d",major);
        printk("minor dev num = %d",minor);

        hello_data.chrdev_num = MKDEV(major,minor);
        hello_data.major = major;
        hello_data.minor = minor;
        printk("chrdev_num = %d",hello_data.chrdev_num);
        printk("hello woorld\n");
        ret = register_chrdev_region(hello_data.chrdev_num,1,"hello_chrdev");
        if (ret < 0)
        {
            printk("register chrdev fail !\n");
            ret = -1;
            goto err_chrdev_unregion;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&hello_data.chrdev_num,0,1,"hello_chrdev");
        if (ret < 0)
        {
            printk("register chrdev fail !\n");
            ret = -1;
            goto err_chrdev_unregion;
        }
        hello_data.major = MAJOR(hello_data.chrdev_num);
        hello_data.minor = MINOR(hello_data.chrdev_num);
        printk("major dev num = %d\n",hello_data.major);
        printk("minor dev num = %d\n",hello_data.minor);
    }
    //初始化结构体成员。
    hello_data.my_cdev.owner = THIS_MODULE;
    cdev_init(&hello_data.my_cdev, &Hello_FileOperations);
    ret = cdev_add(&hello_data.my_cdev, hello_data.chrdev_num, 1);
    if(ret < 0)
    {
        goto err_chrdev_add;
    }

    hello_data.hello_class = class_create(THIS_MODULE,"hello_test");
    if (IS_ERR(hello_data.hello_class))
    {
        ret = PTR_ERR(hello_data.hello_class);
        goto err_class_create;
    }
    
    hello_data.hello_devices = device_create( hello_data.hello_class ,NULL,  hello_data.chrdev_num, NULL,"hello_devices");
    if(IS_ERR(hello_data.hello_devices)){
        ret = PTR_ERR(hello_data.hello_devices);
        goto err_device_create;
    }
    printk("init ok hello devices\n");
    return 0;

err_device_create:
    class_destroy(hello_data.hello_class);
err_class_create:
    cdev_del(&hello_data.my_cdev);
err_chrdev_add:
    unregister_chrdev_region(hello_data.chrdev_num,1);
err_chrdev_unregion:
    return ret;
}


 /* 驱动出口函数 */
static void __exit helloworld_exit(void)
{
/* 注销字符设备驱动 */
//  unregister_chrdev(200, "chrtest");
   device_destroy(hello_data.hello_class,hello_data.chrdev_num);
   class_destroy(hello_data.hello_class);
   cdev_del(&hello_data.my_cdev);
   unregister_chrdev_region(hello_data.chrdev_num,1);
   printk("hello world bye \n");
}

 /* 将上面两个函数指定为驱动的入口和出口函数 */
 module_init(helloworld_init);
 module_exit(helloworld_exit);
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("harn");