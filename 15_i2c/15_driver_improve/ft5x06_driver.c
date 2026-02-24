#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

struct gpio_desc *reset_gpio;
struct gpio_desc *irq_gpio;

irqreturn_t ft5x06_handler(int irq, void *args){
    printk("this is gt5x06_handler\n");
    return IRQ_RETVAL(IRQ_HANDLED);
}
int ft5x06_probe(struct i2c_client *client, const struct i2c_device_id *id){

    int ret = 0;
    printk(KERN_INFO"this is ft5x06_probe\n");
    reset_gpio = gpiod_get_optional(&client->dev, "reset", 0);
    if(reset_gpio == NULL)
    {
        printk("gpiod_get_optional reset-gpio is error\n");
        return -1;
    }
    irq_gpio = gpiod_get_optional(&client->dev, "interrupts", 0);
    if(irq_gpio == NULL)
    {
        printk("gpiod_get_optional irq-gpio is error\n");
        return -1;
    }

    gpiod_direction_output(reset_gpio, 0);
    msleep(5);
    gpiod_direction_output(reset_gpio, 1);
    
    ret = request_irq(client->irq, ft5x06_handler, IRQ_TYPE_EDGE_FAILLING | IRQF_ONESHOT, "ftx506_irq", NULL);
    if (ret < 0)
    {
        printk("request_irq is error\n");
        return -2;
    }
    
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