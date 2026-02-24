#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
struct resource *myresourece;
int platform_driver_probefunc (struct platform_device *dev)
{
    printk(KERN_INFO "my_driver_probe\n");
    myresourece = platform_get_resource(dev, IORESOURCE_MEM, 0);
    printk(KERN_INFO"get memory address is ");
    return 0;
}
int platform_driver_removefunc(struct platform_device * dev)
{
    printk(KERN_INFO "my_driver_remove\n");
    return 0;
}
/* id_table的优先级比dirver 里面的优先级大，匹配优先级 */
const struct platform_device_id mydriver_id_table = {
    .name = "mydevice",
};
struct platform_driver platform_driver_test = {
    .probe = platform_driver_probefunc,
    .remove = platform_driver_removefunc,
    .driver = {
        .name = "mydevice",
        .owner = THIS_MODULE,
    },
    .id_table = &mydriver_id_table,
};

/* 驱动入口函数 */
static int platform_driver_init(void)
{
    platform_driver_register(&platform_driver_test);
    printk(KERN_INFO "Platform device init\n");
    return 0;
}


 /* 驱动出口函数 */
static void platform_driver_exit(void)
{
    platform_driver_unregister(&platform_driver_test);
    printk(KERN_INFO "Platform device eixt \n");

}

 /* 将上面两个函数指定为驱动的入口和出口函数 */
 module_init(platform_driver_init);
 module_exit(platform_driver_exit);
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("harn");