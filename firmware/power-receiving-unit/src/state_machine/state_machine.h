#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/policy.h>


#include "../gpio_control/gpio_control.h"
#include "../adc_update/adc_update.h"

// DISABLE/ENABLE
// #define DEEP_SLEEP_ENABLE
// #define PWM_ENABLE

// *** SETTINGS CHARGER *** //
#define MIN_VOLTAGE               	5000
#define MAX_VOLTAGE               	22000
#define MIN_CURRENT               	200
#define SLEEP_VOLTAGE             	1000
#define MIN_CHARGE_CUTTOF_VOLTAGE 	2600
#define PID_SETPOINT              	10000

#define PID_UNDER_THRESHOLD         5
#define PID_UPPER_THRESHOLD         70
#define PID_INIT                    25

// *** SLEEP *** //
#define SLEEP_S 2U

// *** State machine states *** //
typedef enum {INIT, START_CHARGING, CHARGING, STOP_CHARGING, OVER_VOLTAGE, SLEEP} state_type;

// *** Extern variables defined in .c file *** //
extern state_type current_state;
extern uint8_t pulse_width_value;

// *** Static Functions *** //
static uint8_t pwm_value_controller(int32_t measured_value, uint8_t value);

// *** Functions *** //
void Disable_buck_converter(void);
void Enable_buck_converter(void);
void state_machine(void);

// State machine thread
void sm_start();

uint8_t disable_ds_1(void);

#endif /* STATE_MACHINE_H_ */


