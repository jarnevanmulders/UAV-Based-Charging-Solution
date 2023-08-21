#include "gpio_control.h"

uint16_t delay_on_ms = 100;
uint16_t delay_off_ms = 100;

// *** Variables *** //
struct gpio_dt_spec led_pin = GPIO_DT_SPEC_GET(USRLED0_NODE, gpios);
struct gpio_dt_spec ls_en_1_pin = GPIO_DT_SPEC_GET(EN1_NODE, gpios);
struct gpio_dt_spec ls_en_2_pin = GPIO_DT_SPEC_GET(EN2_NODE, gpios);
struct gpio_dt_spec pg_pin = GPIO_DT_SPEC_GET(PG0_NODE, gpios);
struct gpio_dt_spec buck_en_pin = GPIO_DT_SPEC_GET(BUCKEN0_NODE, gpios);
struct gpio_dt_spec sens_en_pin = GPIO_DT_SPEC_GET(SENSEN0_NODE, gpios);

struct pwm_dt_spec pwm0_pin = PWM_DT_SPEC_GET(DT_ALIAS(pwmled0));

// *** SETUP PINS *** //
void setup_pins(void){
	gpio_pin_configure_dt(&led_pin, GPIO_OUTPUT_INACTIVE);			// Default LED off
    gpio_pin_configure_dt(&ls_en_1_pin, GPIO_OUTPUT_INACTIVE);		// Default load switch disabled (no leakage current to charger circuit)
	gpio_pin_configure_dt(&ls_en_2_pin, GPIO_OUTPUT_INACTIVE);		// Default load switch disabled (no leakage current to charger circuit)
	gpio_pin_configure_dt(&pg_pin, GPIO_OUTPUT_INACTIVE);			// Default voltage WPT receiver blocked
	gpio_pin_configure_dt(&buck_en_pin, GPIO_OUTPUT_INACTIVE);		// 
	gpio_pin_configure_dt(&sens_en_pin, GPIO_OUTPUT_INACTIVE);		// 
}


void pin_set(struct gpio_dt_spec *pin_number){
    gpio_pin_set_dt(pin_number, ENABLE);
}

void pin_reset(struct gpio_dt_spec *pin_number){
    gpio_pin_set_dt(pin_number, DISABLE);
}

void pin_toggle(struct gpio_dt_spec *pin_number){
    gpio_pin_toggle_dt(pin_number);
}

void change_pwm_pulse_size(uint8_t value){
    pwm_set_dt(&pwm0_pin, PWM_USEC(100U), PWM_USEC(100U) / PERIOD_DIVIDER * value);
}

void change_blink_delay(uint16_t value_on, uint16_t value_off){
    delay_on_ms = value_on;
    delay_off_ms = value_off;
}

void led_blink_start(void){
    while(1){
        pin_reset(&led_pin);
        k_msleep(delay_off_ms);
        pin_set(&led_pin);
        k_msleep(delay_on_ms);
    }
}