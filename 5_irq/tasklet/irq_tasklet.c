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
void tasklet_func(unsigned long data)
{
    printk("data is %ld",data);
}

#ifdef STATIC_INIT_TASKLET
DECLARE_TASKLET(my_tasklet,tasklet_func,1);
#else
struct tasklet_struct tasklet_dynamic_init_struct;
#endif // STATIC_INIT_TASKLET

irqreturn_t gpio0_c5_interrupt_handler(int irq, void * args)
{
    printk(KERN_ERR"test interrupt\n");
    tasklet_schedule(&tasklet_dynamic_init_struct);//tasklet中断上文之中调度。
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

    tasklet_init(&tasklet_dynamic_init_struct,tasklet_func,1);
    return 0;
}

static void __exit irq_request_exit(void)
{
    free_irq(irq, NULL);
    tasklet_enable(&tasklet_dynamic_init_struct);
    tasklet_kill(&tasklet_dynamic_init_struct);
    printk("irq free and exit\n");
}


module_init(irq_request_init);
module_exit(irq_request_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("harn");