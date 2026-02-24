#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/device.h>

#include <linux/interrupt.h>
#include <linux/gpio.h>
#define GPIO_0C5_PIN 29

int irq;

irqreturn_t gpio0_c5_interrupt_handler(int irq, void * args)
{
    printk(KERN_ERR"test interrupt\n");
    return IRQ_RETVAL(IRQ_HANDLED);
}
static int __init irq_request_init(void)
{
   
    int ret = 0;
    irq = gpio_to_irq(GPIO_0C5_PIN);//gpio0_c5 8*3+6 -1 =29

    printk("request soft irq is %d\n", irq);
    ret = request_irq(irq, gpio0_c5_interrupt_handler,IRQF_TRIGGER_RISING, "irq_test", NULL);
    if(ret < 0 )
    {
        printk("request—irq is failed\n");
    }
    return 0;
}

static void __exit irq_request_exit(void)
{
    free_irq(irq, NULL);
    printk("irq free and exit\n");
}


module_init(irq_request_init);
module_exit(irq_request_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("harn");