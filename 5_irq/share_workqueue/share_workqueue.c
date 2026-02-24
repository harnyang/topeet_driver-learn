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
#include <linux/workqueue.h>/* 包含workqueue的头文件 */

struct work_struct test_share_work_queue;/* 定义一个头文件 */
/**
 * @brief 共享工作队列回调函数
 * 
 * @param work 
 */
void testwork_func(struct work_struct *work){
    msleep(1000);
    printk("this is share testwork func\n");
}
int irq;

irqreturn_t gpio0_c5_interrupt_handler(int irq, void * args)
{
    printk(KERN_ERR"test interrupt\n");
    schedule_work(&test_share_work_queue);/* 共享工作队列的调度函数，调度共享工作队列，来实现中断下文 */
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

    INIT_WORK(&test_share_work_queue,testwork_func);/* 动态初始化一个共享工作队列 */
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