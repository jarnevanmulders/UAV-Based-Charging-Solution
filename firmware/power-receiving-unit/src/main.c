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

// ADC test
#include <inttypes.h>
#include <stdint.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

/* Custom libs include */
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

// ADC
/* Data of ADC io-channels specified in devicetree. */

// #if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
// 	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
// #error "No suitable devicetree overlay specified"
// #endif

// #define DT_SPEC_AND_COMMA(node_id, prop, idx) \
// 	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

// static const struct adc_dt_spec adc_channels[] = {
// 	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
// 			     DT_SPEC_AND_COMMA)
// };



int main(void)
{
	// Start
	printk("Start firmware");
	
	// *** SETUP FUNCTIONS *** //
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	// K_SYSCALL_LED_BLINK

	//led_set_brightness(led, led, level);

	// ADC
	// int err;
	// uint32_t count = 0;
	// uint16_t buf;
	// struct adc_sequence sequence = {
	// 	.buffer = &buf,
	// 	/* buffer size in bytes, not number of samples */
	// 	.buffer_size = sizeof(buf),
	// };

	// /* Configure channels individually prior to sampling. */
	// for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
	// 	if (!device_is_ready(adc_channels[i].dev)) {
	// 		printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
	// 		return 0;
	// 	}

	// 	err = adc_channel_setup_dt(&adc_channels[i]);
	// 	if (err < 0) {
	// 		printk("Could not setup channel #%d (%d)\n", i, err);
	// 		return 0;
	// 	}
	// }

	// *** MAIN LOOP *** //
	while(1){
		
		// printk("ADC reading[%u]:\n", count++);
		// for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		// 	int32_t val_mv;

		// 	printk("- %s, channel %d: ",
		// 			adc_channels[i].dev->name,
		// 			adc_channels[i].channel_id);

		// 	(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

		// 	err = adc_read(adc_channels[i].dev, &sequence);
		// 	if (err < 0) {
		// 		printk("Could not read (%d)\n", err);
		// 		continue;
		// 	}
		// }


		k_msleep(1000);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_CHG);
		k_msleep(500);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_RDY);

	}

	return 0;
}
