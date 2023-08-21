/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/gpio.h>

int observer_start(void);

#define LED0_NODE DT_ALIAS(led0)	// The devicetree node identifier for the "led0" alias.

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

	k_msleep(1000);


	printk("Exiting %s thread.\n", __func__);
	return 0;
}
