/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
// #include <stderr.h>
#include <stdint.h>

int observer_start(void);

#define LED0_NODE DT_ALIAS(usrled0)	// The devicetree node identifier for the "led0" alias.

static int write(uint8_t devaddr, uint8_t regaddr, uint8_t regval);

static int read(uint8_t devaddr, uint8_t regaddr, uint8_t *regval);

/* STATIC constants */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int err;

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);		// 

	printk("Start");

	gpio_pin_set_dt(&led, 1);

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	
	gpio_pin_toggle_dt(&led);

	(void)observer_start();

	while (1)
	{
		gpio_pin_toggle_dt(&led);
		k_msleep(1000);
		uint8_t buf [2];
		read(0x10, 0x04, buf);
		//write(0x01, 0x01, 0x01);
	}


	printk("Exiting %s thread.\n", __func__);
	return 0;
}

static int read(uint8_t devaddr, uint8_t regaddr, uint8_t *regval)
{
    int ret;

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

    if (!device_is_ready(dev)) {
        // LOG_ERR("Device not ready");
        return -ENODEV;
    }

    ret = i2c_write_read(dev, devaddr, &regaddr, 1, regval, 1);
    if (ret) {
        // LOG_ERR("Call `i2c_write_read` failed: %d", ret);
		printk("Call `i2c_write_read` failed: %d", ret);
        return ret;
    }

    return 0;
}


static int write(uint8_t devaddr, uint8_t regaddr, uint8_t regval){
    int ret;

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

    if (!device_is_ready(dev)) {
        // LOG_ERR("Device not ready");
		printk("Device not ready");
        return -ENODEV;
    }

    uint8_t buf[2] = { regaddr, regval };

    ret = i2c_write(dev, buf, 2, devaddr);
    if (ret) {
        // LOG_ERR("Call `i2c_write` failed: %d", ret);
        return ret;
    }

    return 0;
}