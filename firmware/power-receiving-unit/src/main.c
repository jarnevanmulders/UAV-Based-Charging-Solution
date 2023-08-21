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

// #include <zephyr/types.h>
// #include <zephyr/sys/printk.h>
// #include <zephyr/sys/util.h>
// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/gpio.h>
// #include <zephyr/sys/__assert.h>
// #include <string.h>

// // // ADC includes
// #include <zephyr/devicetree.h>
// #include <zephyr/drivers/adc.h>

// SLEEP includes
// #include <stdio.h>
// #include <zephyr/init.h>
// #include <zephyr/pm/pm.h>
// #include <zephyr/pm/device.h>
// #include <zephyr/pm/policy.h>
// #include <soc.h>
// #include <hal/nrf_gpio.h>

/* Custom libs include */
#include "advertisement/advertisement.h"
#include "adc_update/adc_update.h"
#include "state_machine/state_machine.h"
#include "gpio_control/gpio_control.h"


/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define ADV_PRIORITY 7
#define ADC_PRIORITY 8


// *** START THREADS *** //
K_THREAD_DEFINE(start_adv_id, STACKSIZE, adv_start, NULL, NULL, NULL, ADV_PRIORITY, 0, 0);
K_THREAD_DEFINE(start_adc_read_id, STACKSIZE, adc_read_start, NULL, NULL, NULL, ADC_PRIORITY, 0, 0);
K_THREAD_DEFINE(start_sm_id, STACKSIZE, sm_start, NULL, NULL, NULL, 6, 0, 0);
K_THREAD_DEFINE(led_blink_id, STACKSIZE, led_blink_start, NULL, NULL, NULL, 9, 0, 0);


// General error
int err;


// *** MAIN *** //
int main(void)
{
	// Start
	printk("Start firmware\n");

	// *** SETUP FUNCTIONS *** //
	// gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);			// Default LED off
	// gpio_pin_configure_dt(&ls_en_2, GPIO_OUTPUT_INACTIVE);		// Default load switch disabled (no leakage current to charger circuit)
	// gpio_pin_configure_dt(&pg, GPIO_OUTPUT_INACTIVE);			// Default voltage WPT receiver blocked
	// gpio_pin_configure_dt(&buck_en, GPIO_OUTPUT_INACTIVE);		// 
	// gpio_pin_configure_dt(&sens_en, GPIO_OUTPUT_INACTIVE);		// 

	setup_pins();

	// gpio_pin_set_dt(&sens_en, 0);

	// K_SYSCALL_LED_BLINK

	// led_set_brightness(&led_pin, &led_pin, 0);


	// *** Configure to generate PORT event (wakeup) on button 1 press. *** //
	// !!! >>>usrbutton0<<< NEED TO BE CHANGED TO >>>wakeup1<<< !!! //
	nrf_gpio_cfg_input(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(wakeup1), gpios), NRF_GPIO_PIN_PULLDOWN);
	nrf_gpio_cfg_sense_set(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(wakeup1), gpios), NRF_GPIO_PIN_SENSE_HIGH);
	nrf_gpio_cfg_input(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(usrbutton0), gpios), NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_sense_set(NRF_DT_GPIOS_TO_PSEL(DT_ALIAS(usrbutton0), gpios), NRF_GPIO_PIN_SENSE_LOW);

	// *** To read supply voltage, load switch 2 should be enabled. ***
	// gpio_pin_set_dt(&ls_en_2, 1);

	// gpio_pin_set_dt(&pg, 1);

	// gpio_pin_set_dt(&buck_en, 1);


	// *** MAIN LOOP *** //
	while(1){

		// Print readed values
		printk("--------------------------------------------\n");
		printk("input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
		printk("supply_voltage_mv: %"PRId32" mV\n", supply_voltage_mv);
		printk("buck_current_ma: %"PRId32" mA\n", buck_current_ma);
		printk("Current state = %d \n", current_state);
		// printk("Pulse value = %d \n", pulse_width_value);

		k_msleep(1000);

		// pin_toggle(&led_pin);

		// change_pwm_pulse_size(2);

		// k_msleep(3000);

		// change_pwm_pulse_size(100);

		// pin_set(&ls_en_1_pin);
		// pin_set(&ls_en_2_pin);

		// k_msleep(3000);

		// pin_reset(&ls_en_1_pin);
		// pin_reset(&ls_en_2_pin);

	}

	return 0;
}


