#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>


//本文件之中的老的设备还在使用，大部分都使用的是设备树了。
struct i2c_adapter * ft5x06_adapter;
/**
 * struct i2c_board_info - template for device creation
 * @type: chip type, to initialize i2c_client.name
 * @flags: to initialize i2c_client.flags
 * @addr: stored in i2c_client.addr
 * @dev_name: Overrides the default <busnr>-<addr> dev_name if set
 * @platform_data: stored in i2c_client.dev.platform_data
 * @of_node: pointer to OpenFirmware device node
 * @fwnode: device node supplied by the platform firmware
 * @properties: additional device properties for the device
 * @resources: resources associated with the device
 * @num_resources: number of resources in the @resources array
 * @irq: stored in i2c_client.irq
 *
 * I2C doesn't actually support hardware probing, although controllers and
 * devices may be able to use I2C_SMBUS_QUICK to tell whether or not there's
 * a device at a given address.  Drivers commonly need more information than
 * that, such as chip type, configuration, associated IRQ, and so on.
 *
 * i2c_board_info is used to build tables of information listing I2C devices
 * that are present.  This information is used to grow the driver model tree.
 * For mainboards this is done statically using i2c_register_board_info();
 * bus numbers identify adapters that aren't yet available.  For add-on boards,
 * i2c_new_device() does this dynamically with the adapter already known.
 */
// struct i2c_board_info {
// 	char		type[I2C_NAME_SIZE];
// 	unsigned short	flags;
// 	unsigned short	addr;
// 	const char	*dev_name;
// 	void		*platform_data;
// 	struct device_node *of_node;
// 	struct fwnode_handle *fwnode;
// 	const struct property_entry *properties;
// 	const struct resource *resources;
// 	unsigned int	num_resources;
// 	int		irq;
// };
struct i2c_board_info ft5x06[] = {
    {I2C_BOARD_INFO("ft5x06", 0x38)},
    {} 
};
static int ft5x06_clinet_init(void)
{
    //查找对应的i2c适配器（软件）即控制器（硬件）的函数 ,定义在 kernel/drivers/i2c/i2c-core-base.c参数是i2c的编号
    ft5x06_adapter = i2c_get_adapter(1);
    //i2c_new_device - instantiate an i2c device
    i2c_new_device(ft5x06_adapter, ft5x06);
    return 0;
}
static void ft5x06_client_exit(void)
{
    //引用计数减1，防止资源的泄露。
    i2c_put_adapter(ft5x06_adapter);
}
module_init(ft5x06_clinet_init);
module_exit(ft5x06_client_exit);
MODULE_LICENSE("GPL");