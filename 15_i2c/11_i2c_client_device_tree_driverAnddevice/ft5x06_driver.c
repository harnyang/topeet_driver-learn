#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
int ft5x06_probe(struct i2c_client *client, const struct i2c_device_id *id){
    printk(KERN_INFO"this is ft5x06_probe\n");
    return 0;
}
int ft5x06_remove(struct i2c_client *client){
    printk(KERN_INFO"this is ft5x06_remove\n");
    return 0;
}
//匹配规则
const struct of_device_id  ft5x06_id[]= {
    {.compatible = "my-ft5x06"},
    {}
};
struct i2c_driver ft5x06_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = "my-ft5x06",
        .of_match_table = ft5x06_id,
    },
    .probe = ft5x06_probe,
    .remove = ft5x06_remove,
};
static int ft5x06_driver_init(void)
{
    int ret = 0;

    ret = i2c_add_driver(&ft5x06_driver);
    if (ret < 0)
    {
        printk(KERN_INFO"add driver failed");
        return ret;
    }
    
    return 0;
}
static int ft5x06_driver_exit(void)
{
    i2c_del_driver(&ft5x06_driver);
}

module_init(ft5x06_driver_init);
module_exit(ft5x06_driver_exit);
MODULE_LICENSE("GPL");