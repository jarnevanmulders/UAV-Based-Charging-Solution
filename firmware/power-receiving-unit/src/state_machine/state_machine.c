#include "state_machine.h"

// *** Variables *** //
uint16_t timer_check = 0;
state_type current_state = INIT;

uint8_t pulse_width_value = PID_UPPER_THRESHOLD;
uint8_t check = 0;

SYS_INIT(disable_ds_1, PRE_KERNEL_2, 0);

void Enable_buck_converter(void){
	printk("Enable BUCK\n");
    pin_set(&ls_en_1_pin);
    pin_set(&ls_en_2_pin);
	k_msleep(50);
	pin_set(&pg_pin);
	k_msleep(50);
	pin_set(&buck_en_pin);

}

void Disable_buck_converter(void){
	printk("Disable BUCK\n");
	pin_reset(&buck_en_pin);
	k_msleep(50);
	pin_reset(&pg_pin);
	k_msleep(50);
	pin_reset(&ls_en_1_pin);
  pin_reset(&ls_en_2_pin);
}

void state_machine(void){
    
    //	Check input voltage exceed max voltage
	if(input_voltage_mv > MAX_VOLTAGE){
      current_state = OVER_VOLTAGE;
    }

    // Check input voltage in not below minimum voltage
    if(input_voltage_mv < SLEEP_VOLTAGE){
      current_state = SLEEP;
    }

    switch (current_state){
      /*******************************************************************************************/
      case INIT: //0

        // No charging may occur in the INIT phase
        Disable_buck_converter();

        // Reset pulse_width_value
        #ifdef PWM_ENABLE
            pulse_width_value = PID_INIT;
        #endif

        // Check input voltage before start charging
        if(input_voltage_mv < MAX_VOLTAGE && input_voltage_mv > MIN_VOLTAGE){
          current_state = START_CHARGING;
          break;
        }

        // Blink led
        change_blink_delay(500, 500);
      break;
      /*******************************************************************************************/
      case START_CHARGING: //1
        // Enable buck converter safely
        Enable_buck_converter();

        printk("Start charging!");

        change_pwm_pulse_size(0);

        k_msleep(100);

        printk("Current = %d \n", buck_current_ma);

        if(buck_current_ma > MIN_CURRENT && buck_current_ma < 1000){
            // Change state
            current_state = CHARGING;
            printk("Current state changed");
        }

        printk("Current state = %d \n", current_state);

      break;
      /*******************************************************************************************/
      case CHARGING: //2

        // Blink led
        change_blink_delay(100, 900);
        
        #ifdef PWM_ENABLE
            if(check == 5){
                // Check input voltage with PID controller
                pulse_width_value = pwm_value_controller(input_voltage_mv, pulse_width_value);

                // Compensate programmable potentiometer
                change_pwm_pulse_size(pulse_width_value);
                check = 0;
            }
            check++;
        #endif

        // If under voltage occurs, wait for enough voltage to resume
        if(input_voltage_mv < MIN_VOLTAGE){
          current_state = INIT;
          break;
        }

        // Check every 10 seconds if current is lower then MIN_CURRENT and voltage reached target level.
        if(buck_current_ma < MIN_CURRENT && timer_check > 10*1000 && supply_voltage_mv > MIN_CHARGE_CUTTOF_VOLTAGE){
          timer_check = 0;
          current_state = STOP_CHARGING;
        }

        timer_check++;

      break;
      /*******************************************************************************************/
      case STOP_CHARGING: //3

        // Disable buck conveter safely
        Disable_buck_converter();

        // Reset pulse_width_value
        #ifdef PWM_ENABLE
            pulse_width_value = PID_INIT;
        #endif

        // TODO SEND MESSAGE TO TRANSMITTER

        // if(buck_current_ma < MIN_CURRENT && supply_voltage_mv > MIN_CHARGE_CUTTOF_VOLTAGE){
        //   current_state = SLEEP;
        // }
        // else{
        //   current_state = INIT;
        // }
        current_state = SLEEP;
      break;
      /*******************************************************************************************/
      case OVER_VOLTAGE: //4

        // Blink led
        change_blink_delay(100, 100);

        // Disable buck conveter safely
        Disable_buck_converter();

        // Check for safe voltage
        if(input_voltage_mv < MAX_VOLTAGE){
          current_state = INIT;
        }
      break;
      /*******************************************************************************************/
      case SLEEP: //5

	  	// To be really sure, disable buck converter
		Disable_buck_converter();

		printk(" *** I am going to sleep! *** \n");
        
        k_msleep(1000);

        // ??? Disable all threads ???

        // Disable USER LED
       	pin_reset(&led_pin);

        // ??? Check if input voltage is above 2 V and enable timer interrupt ???

        // ??? Reconfigure GPIO pins for sleep mode: EN1_Pin EN2_Pin PG_Pin BUCK_EN_Pin ???

		#ifdef DEEP_SLEEP_ENABLE
			/* Force entry to deep sleep on any delay. */
			pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});

			/* Go to sleep. Idle thread keeps running. */
			k_sleep(K_SECONDS(SLEEP_S));
		#endif

        // Change state
        current_state = INIT;

      break;
      /*******************************************************************************************/
      default:
      current_state = INIT;
      break;
    }
}

// Loop
void sm_start(void){

    // Wait for other threads to start up
    k_msleep(1000);

    while(1){
        k_msleep(100);
        state_machine();
    }
}

/* Prevent deep sleep (system off) from being entered on long timeouts
 * or `K_FOREVER` due to the default residency policy.
 *
 * This has to be done before anything tries to sleep, which means
 * before the threading system starts up between PRE_KERNEL_2 and
 * POST_KERNEL.  Do it at the start of PRE_KERNEL_2.
 */

uint8_t disable_ds_1(void){
	pm_policy_state_lock_get(PM_STATE_SOFT_OFF, PM_ALL_SUBSTATES);
	return 0;
}

static uint8_t pwm_value_controller(int32_t measured_value, uint8_t value){

    if(measured_value < 0){
        return PID_UNDER_THRESHOLD;
    }

    if(measured_value > PID_SETPOINT){
        value--;
    }
    else{
        // uint16_t res = (PID_SETPOINT - measured_value)/1000;
        // value = value - res;
        value++;
    }

    if(value > PID_UPPER_THRESHOLD){value = PID_UPPER_THRESHOLD;}
    if(value < PID_UNDER_THRESHOLD){value = PID_UNDER_THRESHOLD;}

    return value;
}




