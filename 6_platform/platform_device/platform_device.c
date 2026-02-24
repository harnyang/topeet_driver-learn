#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
struct platform_driver ;
static struct resource my_device_resources[] = {
    [0] = {
        .start = 0xfdd60000,
        .end   = 0xfdd60006,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 13,
        .end   = 13,
        .flags = IORESOURCE_IRQ,
    },
};
void mydevice_release(struct device* dev){
    printk(KERN_INFO"this is release \n");
}
struct platform_device platform_device_test = {
    .name = "mydevice",
    .id = -1,
    .resource = my_device_resources,
    .num_resources = ARRAY_SIZE(my_device_resources),
    .dev = {
        .release = mydevice_release,
    },
};
/* 驱动入口函数 */
static int platform_device_init(void)
{
    platform_device_register(&platform_device_test);
    printk(KERN_INFO "Platform device init\n");
    return 0;
}


 /* 驱动出口函数 */
static void platform_device_exit(void)
{
    platform_device_unregister(&platform_device_test);
    printk(KERN_INFO "Platform device eixt \n");

}

 /* 将上面两个函数指定为驱动的入口和出口函数 */
 module_init(platform_device_init);
 module_exit(platform_device_exit);
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("harn");