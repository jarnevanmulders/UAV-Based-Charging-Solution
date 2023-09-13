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
#include <stddef.h>

#include "stdio.h"

// #include "ADS1115/ADS1115.h"
#include "peripherals/peripherals.h"

int observer_start(void);

// #define LED0_NODE DT_ALIAS(usrled0)	// The devicetree node identifier for the "led0" alias.
#define LED0_NODE DT_ALIAS(led0)	// The devicetree node identifier for the "led0" alias.

// const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

// static int write(uint8_t devaddr, uint8_t regaddr, uint8_t regval);

// static int read(uint8_t devaddr, uint8_t regaddr, uint8_t *regval);

/* STATIC constants */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// static int16_t read_channel(uint8_t channel);

extern int16_t channel_voltage [4];

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

    // Start observer
	(void)observer_start();

    // Init peripherals (ADS and programmable load)
    perihperals_init();

	while (1)
	{
		gpio_pin_toggle_dt(&led);
		k_msleep(1000);

        peripherals_update_all_ads1115_channels();

        for(int i = 0; i < ADS1115_NUMBER_OF_CHANNELS; i++)
            printk("Channel %d: %d mV\n", i, channel_voltage[i]);

        // init ads1115
        // init();

        // // char buffer[20];
        // // sprintf(buffer, "%.2f", readChannel(0));
        // int16_t value = read_channel(0);
        // // sprintf(buffer, "%.2f", value);
        // // sprintf(value, 6, buffer);
        // // floatToString(buffer, sizeof(buffer), readChannel(0), 2);
        // printk("%d\n", value);

        

        // getConvRate();

		// uint8_t buf [10];
		// read(0x48, 0x01, buf);
		// write(0x4B, 0x01, 0x01);
	}


	printk("Exiting %s thread.\n", __func__);
	return 0;
}

// int16_t read_channel(uint8_t channel) {
//   int32_t voltage = 0;
//   ads1115_set_single_channel(channel);
//   ads1115_start_single_measurement();
//   while(ads1115_is_busy()){}
//   voltage = ads1115_get_result_mV();
//   return voltage;
// }

// int read(uint8_t devaddr, uint8_t regaddr, uint8_t *regval)
// {
//     int ret;

//     if (!device_is_ready(dev)) {
//         // LOG_ERR("Device not ready");
//         printk("Device not ready\n");
//         return -ENODEV;
//     }

//     ret = i2c_write_read(dev, devaddr, &regaddr, 1, regval, 2);
//     if (ret) {
//         // LOG_ERR("Call `i2c_write_read` failed: %d", ret);
// 		printk("Call `i2c_write_read` failed: %d\n", ret);
//         return ret;
//     }

//     return 0;
// }


// int write(uint8_t devaddr, uint8_t regaddr, uint8_t regval){
//     int ret;

//     if (!device_is_ready(dev)) {
//         // LOG_ERR("Device not ready");
// 		printk("Device not ready\n");
//         return -ENODEV;
//     }

//     uint8_t buf[2] = { regaddr, regval };

//     ret = i2c_write(dev, buf, 2, devaddr);
//     if (ret) {
//         // LOG_ERR("Call `i2c_write` failed: %d", ret);
//         printk("Call `i2c_write` failed: %d\n", ret);
//         return ret;
//     }

//     return 0;
// }