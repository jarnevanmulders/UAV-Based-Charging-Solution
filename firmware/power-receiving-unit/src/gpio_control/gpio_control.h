#ifndef GPIO_CONTROL_H_
#define GPIO_CONTROL_H_

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

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

// *** Defines *** //
#define PERIOD_DIVIDER 1000

// *** Extern variables defined in .c file *** //
extern struct gpio_dt_spec led_pin;
extern struct gpio_dt_spec ls_en_1_pin;
extern struct gpio_dt_spec ls_en_2_pin;
extern struct gpio_dt_spec pg_pin;
extern struct gpio_dt_spec buck_en_pin;
extern struct gpio_dt_spec sens_en_pin;

// *** Extern pwm variable in .c file *** //
extern struct pwm_dt_spec pwm0_pin;

void setup_pins(void);
void pin_set(struct gpio_dt_spec *pin_number);
void pin_reset(struct gpio_dt_spec *pin_number);
void pin_toggle(struct gpio_dt_spec *pin_number);
void change_pwm_pulse_size(uint8_t value);
void change_blink_delay(uint16_t value_on, uint16_t value_off);
void led_blink_start(void);

#endif /* GPIO_CONTROL_H_ */


