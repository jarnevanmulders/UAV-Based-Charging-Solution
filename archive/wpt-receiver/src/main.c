/*  ____  ____      _    __  __  ____ ___
 * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
 * | | | | |_) |  / _ \ | |\/| | |  | | | |
 * | |_| |  _ <  / ___ \| |  | | |__| |_| |
 * |____/|_| \_\/_/   \_\_|  |_|\____\___/
 *                           research group
 *                             dramco.be/
 *
 *  KU Leuven - Technology Campus Gent,
 *  Gebroeders De Smetstraat 1,
 *  B-9000 Gent, Belgium
 *
 *         File: main.c
 *      Created: 2023-06-30
 *       Author: Jarne Van Mulders
 *      Version: major.minor
 *
 *  Description: wpt-receiver
 *      IoT node & wpt receiver
 *
 *  Commissiond by PhD
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>
#include <string.h>


#include "advertisement/advertisement.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

// *** GPIO ALIAS *** //
#define LED0_NODE DT_ALIAS(led0)	// The devicetree node identifier for the "led0" alias.

// *** STATIC constants *** //
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// *** START THREADS *** //
K_THREAD_DEFINE(start_adv_id, STACKSIZE, adv_start, NULL, NULL, NULL, PRIORITY, 0, 0);


int main(void)
{
	// *** SETUP FUNCTIONS *** //
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	// *** MAIN LOOP *** //
	while(1){
		k_msleep(1000);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_CHG);
		k_msleep(1000);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_RDY);

	}

	return 0;
}
