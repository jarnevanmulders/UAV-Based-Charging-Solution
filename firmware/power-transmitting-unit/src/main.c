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

#include "peripherals/peripherals.h"

int observer_start(void);

// #define LED0_NODE DT_ALIAS(usrled0)	// The devicetree node identifier for the "led0" alias.
#define LED0_NODE DT_ALIAS(led0)	// The devicetree node identifier for the "led0" alias.


/* STATIC constants */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// static int16_t read_channel(uint8_t channel);

extern int16_t channel_voltage [4];

extern int16_t uav_battery_voltage_mv;
extern int16_t uav_current_ma;
extern int16_t pre_reg_input_current_ma;
extern int16_t pre_reg_output_current_ma; 

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

	// Set prereg on 10V
	peripherals_set_pre_reg_voltage(10000);

	while (1)
	{
		gpio_pin_toggle_dt(&led);
		k_msleep(2000);

        peripherals_update_all_ads1115_channels();

		printk("uav_battery_voltage_mv: %d\n", uav_battery_voltage_mv);
		printk("uav_current_ma: %d\n", uav_current_ma);
		printk("pre_reg_input_current_ma: %d\n", pre_reg_input_current_ma);
		printk("pre_reg_output_current_ma: %d\n", pre_reg_output_current_ma);

        // for(int i = 0; i < ADS1115_NUMBER_OF_CHANNELS; i++){
        //     //printk("Channel %d: %d mV\n", i, channel_voltage[i]);

		// 	printk("Res %d: %d mV\n", i, channel_result [i]);
		// }

        // test res
        // TPL0401x_update_resistance(100);

        // peripherals_set_pre_reg_voltage(10000);
        
        // k_msleep(2000);
        
        // peripherals_set_pre_reg_voltage(5000);

        // test res
        // TPL0401x_update_resistance(25);

	}


	printk("Exiting %s thread.\n", __func__);
	return 0;
}

