#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/errno.h>

#define P0_INPUT        0x00
#define P1_INPUT        0x01
#define P0_OUTPUT       0x02
#define P1_OUTPUT       0x03
#define P0_POLARITY_INV 0x04
#define P1_POLARITY_INV 0x05
#define P0_CONFIG       0x06
#define P1_CONFIG       0x07

#define I2C_BUS_AVAILABLE       1              // I2C Bus available in your Raspberry Pi
#define SLAVE_DEVICE_NAME       "TCA9539"      // Device and Driver Name
#define TCA9539_SLAVE_ADDRESS   0x74           // TCA9539 Slave Address

struct i2c_adapter *tca_i2c_adapter = NULL;  // I2C Adapter Structure
struct i2c_client *tca_i2c_client = NULL;    // I2C Client Structure (TCA9539)

/* I2C Board info structure */
  struct i2c_board_info tca_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, TCA9539_SLAVE_ADDRESS)
};

/* Structure that has slave device id */
  const struct i2c_device_id tca_id[] = {
    { SLAVE_DEVICE_NAME, 0 },
    { }
};

/* Read and write tca9539 registers*/
  int tca_read_reg(struct i2c_client *client, int reg, unsigned char *val)
{
	int ret;

	ret = i2c_smbus_read_byte_data(client, reg);
	if (ret < 0) {
		dev_err(&client->dev, "i2c read fail: can't read from %02x: %d\n", reg, ret);
		return ret;
	} else {
		*val = ret;
	}
	return 0;
}

  int tca_write_reg(struct i2c_client *client, int reg, int val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, val);
	if (ret < 0) {
		dev_err(&client->dev, "i2c write fail: can't write %02x to %02x: %d\n",
			val, reg, ret);
		return ret;
	}
	return 0;
}

/*tca9539 formatting register functions*/
// get_reg()
// set_reg()

/* tca9539 driver */

int tca_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    unsigned char val;
    
    pr_info("TCA9539 Probed!!!\n");

    // Add initialization code here if needed
    tca_write_reg(client, P0_OUTPUT, 0x1A);

    tca_read_reg(client, P0_OUTPUT, &val);
    
    pr_info("Register read and write test succesfull!!\n");
    pr_info("Value at P0_OUTPUT %x", val);
    
    return 0;
}

void tca_remove(struct i2c_client *client)
{
    // Include any cleanup code here
    pr_info("TCA9539 Removed!!!\n");
    
}

MODULE_DEVICE_TABLE(i2c, tca_id);

struct i2c_driver tca_driver = {
	.driver = {
		.name = SLAVE_DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = tca_probe,
	.remove = tca_remove,
	.id_table = tca_id,
};

/* Module Init function */
int __init tca_driver_init(void)
{
    int ret = -1;

    pr_info("Driver Intializing....\n");
    tca_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (tca_i2c_adapter != NULL) {
        tca_i2c_client = i2c_new_client_device(tca_i2c_adapter, &tca_i2c_board_info);

        if (tca_i2c_client != NULL) {
            i2c_add_driver(&tca_driver);
            ret = 0;
        }

        i2c_put_adapter(tca_i2c_adapter);
    }

    pr_info("Driver Added!!!\n");
    return ret;
}

/* Module Exit function */
void __exit tca_driver_exit(void)
{
    pr_info("Removing Driver....\n");

    i2c_unregister_device(tca_i2c_client);
    i2c_del_driver(&tca_driver);
    pr_info("Driver Removed!!!\n");
}

module_init(tca_driver_init);
module_exit(tca_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAZOUT ELECTRIC");
MODULE_DESCRIPTION("Loadable Kernel Module for TCA9539 - I2C based GPIO expander");
MODULE_VERSION("1.0");
