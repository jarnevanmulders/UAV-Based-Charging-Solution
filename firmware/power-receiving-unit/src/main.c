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

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

// // ADC includes
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/adc.h>

// SLEEP includes
#include <stdio.h>
#include <zephyr/init.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>
#include <soc.h>
#include <hal/nrf_gpio.h>

// DISABLE/ENABLE
// #define DEEP_SLEEP_ENABLE

/* Custom libs include */
#include "advertisement/advertisement.h"
#include "adc_update/adc_update.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define ADV_PRIORITY 7
#define ADC_PRIORITY 8

// *** GPIO ALIAS *** //
#define LED0_NODE DT_ALIAS(led0)	// The devicetree node identifier for the "led0" alias.

// *** STATIC constants *** //
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// *** START THREADS *** //
K_THREAD_DEFINE(start_adv_id, STACKSIZE, adv_start, NULL, NULL, NULL, ADV_PRIORITY, 0, 0);
K_THREAD_DEFINE(start_adc_read_id, STACKSIZE, adc_read_start, NULL, NULL, NULL, ADC_PRIORITY, 0, 0);

// // *** ADC - Device three - check *** //
// #if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
// 	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
// #error "No suitable devicetree overlay specified"
// #endif

// #define DT_SPEC_AND_COMMA(node_id, prop, idx) \
// 	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

// /* Data of ADC io-channels specified in devicetree. */
// static const struct adc_dt_spec adc_channels[] = {
// 	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
// 			     DT_SPEC_AND_COMMA)
// };


// *** Global variables *** //
// ADC
// int16_t buf;
int32_t input_voltage_mv = 0;
int32_t buck_output_voltage_mv = 0;
int32_t buck_current_ma = 0;
int32_t* adc_results_m [3] = {&input_voltage_mv, &buck_output_voltage_mv, &buck_current_ma}; // Create pointer array

// extern int32_t* adc_results_m [];

// General error
int err;

// *** Static fuctions *** //
// static void update_adc_readings(struct adc_sequence *sequence);
static int disable_ds_1(void);

// *** SLEEP *** //
#define SLEEP_S 2U
SYS_INIT(disable_ds_1, PRE_KERNEL_2, 0);

// *** MAIN *** //
int main(void)
{
	// Start
	printk("Start firmware\n");

	// *** SETUP FUNCTIONS *** //
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	// K_SYSCALL_LED_BLINK

	//led_set_brightness(led, led, level);

	// // *** SETUP ADC *** //
	// struct adc_sequence sequence = {
	// 	.buffer = &buf,
	// 	/* buffer size in bytes, not number of samples */
	// 	.buffer_size = sizeof(buf),
	// 	.calibrate = true,
	// };

	// // /* Configure channels individually prior to sampling. */
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

	// *** Configure to generate PORT event (wakeup) on button 1 press. *** //
	// !!! >>>usrbutton0<<< NEED TO BE CHANGED TO >>>wakeup1<<< !!! //
	nrf_gpio_cfg_input(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(usrbutton0), gpios), NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_sense_set(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(usrbutton0), gpios), NRF_GPIO_PIN_SENSE_LOW);
	

	// *** MAIN LOOP *** //
	while(1){

		// Update ADC readings
		//update_adc_readings(&sequence);

		// Print readed values
		printk("input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
		printk("buck_output_voltage_mv: %"PRId32" mV\n", buck_output_voltage_mv);
		printk("buck_current_ma: %"PRId32" mV\n", buck_current_ma);


		k_msleep(1000);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_CHG);
		k_msleep(500);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_RDY);

		#ifdef DEEP_SLEEP_ENABLE
			/* Force entry to deep sleep on any delay. */
			pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});

			/* Go to sleep. Idle thread keeps running. */
			k_sleep(K_SECONDS(SLEEP_S));
		#endif

	}

	return 0;
}

// Update ADC voltages
// void update_adc_readings(struct adc_sequence *sequence){
// 	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {

// 		// Init ADC channel
// 		(void)adc_sequence_init_dt(&adc_channels[i], sequence);

// 		// Read ADC channel
// 		err = adc_read(adc_channels[i].dev, sequence);
// 		if (err < 0) {
// 			printk("Could not read (%d)\n", err);
// 			continue;
// 		}

// 		// Convert to mv
// 		int32_t val_mv;
// 		val_mv = (int32_t)buf;
// 		err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);

// 		// Convert ??? voltage divider ToDo

// 		// Store in global variable via pointer array
// 		*adc_results_m [i] = val_mv;
// 	}
// }

/* Prevent deep sleep (system off) from being entered on long timeouts
 * or `K_FOREVER` due to the default residency policy.
 *
 * This has to be done before anything tries to sleep, which means
 * before the threading system starts up between PRE_KERNEL_2 and
 * POST_KERNEL.  Do it at the start of PRE_KERNEL_2.
 */


static int disable_ds_1(void){
	pm_policy_state_lock_get(PM_STATE_SOFT_OFF, PM_ALL_SUBSTATES);
	return 0;
}