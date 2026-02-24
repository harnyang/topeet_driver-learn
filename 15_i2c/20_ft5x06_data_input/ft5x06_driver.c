#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/input.h>

struct gpio_desc *reset_gpio;
struct gpio_desc *irq_gpio;
struct i2c_client * ft5x06_client;
struct input_dev *ft5x06_input_dev;


int ft5x06_read_reg(u8 reg_addr){
    u8 data;
    struct i2c_msg msgs[] = {
        [0] = {
            .addr = ft5x06_client->addr,
            .flags = 0,
            .len = sizeof(reg_addr),
            .buf = &reg_addr,
        },
        [1] = {
            .addr = ft5x06_client->addr,
            .flags = 1,//读
            .len = sizeof(data),
            .buf = &data,
        }
    };
    i2c_transfer(ft5x06_client->adapter, msgs, 2);//读取的封包。

    return data;
}
void ft5x06_wirte_reg(u8 reg_addr, u8 data, u16 len)
{
    u8 buff[256];
    struct i2c_msg msgs[] ={
        [0] = {
            .addr = ft5x06_client->addr,
            .flags = 0,
            .len = len + 1,
            .buf = buff,
        }
    };
    buff[0] = reg_addr;
    memcpy(&buff[1], &data, len);
    i2c_transfer(ft5x06_client->adapter, msgs, 1);
}
irqreturn_t ft5x06_handler(int irq, void *args){
    printk("this is gt5x06_handler\n");
    return IRQ_RETVAL(IRQ_HANDLED);
}
int ft5x06_probe(struct i2c_client *client, const struct i2c_device_id *id){

    int ret = 0;
    int value = 0;
    printk(KERN_INFO"this is ft5x06_probe\n");
    //获取client结构体。
    ft5x06_client = client;
    
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
    
    ret = request_irq(client->irq, ft5x06_handler, IRQ_TYPE_EDGE_FAILLING | IRQF_ONESHOT, "ftx506_irq", NULL);//这里的中断的触发需要和设备树之中的触发方式一样。
    if (ret < 0)
    {
        printk("request_irq is error\n");
        return -2;
    }
    ft5x06_input_dev = input_allocate_device();
    if (ft5x06_input_dev == NULL)
    {
        printk("input_allocate_device is error \n");
        return -2;
    }
    ft5x06_input_dev->name = "ft5x06_dev";
    __set_bit(EV_KEY, ft5x06_input_dev->evbit);
    __set_bit(BTN_TOUCH, ft5x06_input_dev->keybit);

    __set_bit(EV_ABS, ft5x06_input_dev->evbit);
    __set_bit(ABS_X, ft5x06_input_dev->absbit);
    __set_bit(ABS_Y, ft5x06_input_dev->absbit);

    input_set_abs_params(ft5x06_input_dev, ABS_X, 0, 1024, 0, 0);
    input_set_abs_params(ft5x06_input_dev, ABS_Y, 0, 600, 0, 0);

    ret = input_register_device(ft5x06_input_dev);
    if (ret < 0)
    {
        printk("input register_device is error\n");
        goto error_0;
    }
    
    
error_0:
    input_free_device(ft5x06_input_dev);
    free_irq(client->irq, NULL);
    gpiod_put(reset_gpio);
    gpiod_put(irq_gpio);
;    return 0;
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
    free_irq(ft5x06_client->irq, NULL);
    input_unregister_device(ft5x06_input_dev);
    gpiod_put(reset_gpio);
    gpiod_put(irq_gpio);
    i2c_del_driver(&ft5x06_driver);
}

module_init(ft5x06_driver_init);
module_exit(ft5x06_driver_exit);
MODULE_LICENSE("GPL");