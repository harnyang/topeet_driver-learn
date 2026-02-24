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
// const struct of_device_id  ft5x06_id[]= {
//     {.compatible = "my-ft5x06"},
//     {}
// };
const struct of_device_id  ft5x06_id[]= {
    {"my-ft5x06"},//因为不使用设别树所以这里直接使用名字来进行匹配。
    {}
};
struct i2c_driver ft5x06_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = "my-ft5x06",
        // .of_match_table = ft5x06_id,//这个是使用设备树匹配规则，如果不使用设备树的话这里需要注释掉使用id
    },
    .probe = ft5x06_probe,
    .remove = ft5x06_remove,
    .id_table = ft5x06_id,//不使用设备树的匹配规则。这里的名字需要和ft5x06_client.c之中的名字一致。
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