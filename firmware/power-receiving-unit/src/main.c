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

/* Custom libs include */
#include "advertisement/advertisement.h"
#include "adc_update/adc_update.h"

// DISABLE/ENABLE
#define DEEP_SLEEP_ENABLE

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define ADV_PRIORITY 7
#define ADC_PRIORITY 8

// Define GPIO states
#define DISABLE	0
#define ENABLE 	1

// *** GPIO ALIAS *** //
#define USRLED0_NODE 	DT_ALIAS(usrled0)	// The devicetree node identifier for the "led0" alias.
#define EN1_NODE 		DT_ALIAS(en1)		// The devicetree node identifier for the "en1" alias.
#define EN2_NODE 		DT_ALIAS(en2)		// The devicetree node identifier for the "en2" alias.
#define PG0_NODE 		DT_ALIAS(pg0)		// The devicetree node identifier for the "pg0" alias.
#define BUCKEN0_NODE 	DT_ALIAS(bucken0)	// The devicetree node identifier for the "bucken0" alias.
#define SENSEN0_NODE 	DT_ALIAS(sensen0)	// The devicetree node identifier for the "sens_en" alias.

// *** STATIC constants *** //
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(USRLED0_NODE, gpios);
static const struct gpio_dt_spec ls_en_1 = GPIO_DT_SPEC_GET(EN1_NODE, gpios);
static const struct gpio_dt_spec ls_en_2 = GPIO_DT_SPEC_GET(EN2_NODE, gpios);
static const struct gpio_dt_spec pg = GPIO_DT_SPEC_GET(PG0_NODE, gpios);
static const struct gpio_dt_spec buck_en = GPIO_DT_SPEC_GET(BUCKEN0_NODE, gpios);
static const struct gpio_dt_spec sens_en = GPIO_DT_SPEC_GET(SENSEN0_NODE, gpios);

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
int32_t supply_voltage_mv = 0;
int32_t buck_current_ma = 0;
int32_t* adc_results_m [3] = {&input_voltage_mv, &supply_voltage_mv, &buck_current_ma}; // Create pointer array

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
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);			// Default LED off
	gpio_pin_configure_dt(&ls_en_2, GPIO_OUTPUT_INACTIVE);		// Default load switch disabled (no leakage current to charger circuit)
	gpio_pin_configure_dt(&pg, GPIO_OUTPUT_INACTIVE);			// Default voltage WPT receiver blocked
	gpio_pin_configure_dt(&buck_en, GPIO_OUTPUT_INACTIVE);		// 
	gpio_pin_configure_dt(&sens_en, GPIO_OUTPUT_INACTIVE);		// 

	gpio_pin_set_dt(&sens_en, 0);

	// K_SYSCALL_LED_BLINK

	//led_set_brightness(led, led, level);


	// *** Configure to generate PORT event (wakeup) on button 1 press. *** //
	// !!! >>>usrbutton0<<< NEED TO BE CHANGED TO >>>wakeup1<<< !!! //
	nrf_gpio_cfg_input(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(wakeup1), gpios), NRF_GPIO_PIN_PULLDOWN);
	nrf_gpio_cfg_sense_set(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(wakeup1), gpios), NRF_GPIO_PIN_SENSE_LOW);

	// *** To read supply voltage, load switch 2 should be enabled. ***
	// gpio_pin_set_dt(&ls_en_2, 1);

	// gpio_pin_set_dt(&pg, 1);

	// gpio_pin_set_dt(&buck_en, 1);


	// *** MAIN LOOP *** //
	while(1){

		// Print readed values
		printk("input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
		printk("supply_voltage_mv: %"PRId32" mV\n", supply_voltage_mv);
		printk("buck_current_ma: %"PRId32" mA\n", buck_current_ma);


		k_msleep(1000);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_CHG);
		k_msleep(500);
		gpio_pin_toggle_dt(&led);
		adv_change_status(STAT_RDY);

		if(input_voltage_mv > 6000){
			gpio_pin_set_dt(&ls_en_1, ENABLE);
			gpio_pin_set_dt(&ls_en_2, ENABLE);
			k_msleep(50);
			gpio_pin_set_dt(&pg, ENABLE);
			k_msleep(50);
			gpio_pin_set_dt(&buck_en, ENABLE);
		}
		else{
			gpio_pin_set_dt(&buck_en, DISABLE);
			k_msleep(50);
			gpio_pin_set_dt(&pg, DISABLE);
			k_msleep(50);
			gpio_pin_set_dt(&ls_en_1, DISABLE);
			gpio_pin_set_dt(&ls_en_2, DISABLE);
		}

		#ifdef DEEP_SLEEP_ENABLE
			/* Force entry to deep sleep on any delay. */
			pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});

			/* Go to sleep. Idle thread keeps running. */
			k_sleep(K_SECONDS(SLEEP_S));
		#endif

	}

	return 0;
}


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