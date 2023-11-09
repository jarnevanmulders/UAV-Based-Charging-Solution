#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include "advertisement.h"


// Global adv params buffer
struct adv_params ap;

static void copy_to_adv_buffer(){
	struct bt_data ad_variable;
	ad_variable.type = BT_DATA_MANUFACTURER_DATA;
	ad_variable.data = ap.usr_tx_buffer;
	ad_variable.data_len = sizeof(ap.usr_tx_buffer);

	memcpy(ap.adv_buffer, ad, sizeof(ad));
	ap.adv_buffer[DEFAULT_SIZE_ADV_MSG] = ad_variable;

	// Update advertisement packet
	ap.update = true;
}

static void adv_init(void){
	ap.update = true;

	copy_to_adv_buffer();

	/* Initialize the Bluetooth Subsystem */
	bt_enable(NULL);
	printk("Bluetooth initialized\n");

}

// Start advertisements
void adv_start(void){

	adv_init();

	while (1)
	{
		// Check every 150 ms * 5
		k_msleep(150*5);

		if(ap.update){
			ap.update = false;

			// Stop advertising
			bt_le_adv_stop();

			// Update measured voltages and currents
			adv_update_adc_readings();
			
			// Start advertising (*** BT_LE_ADV_NCONN --> 150ms *** BT_LE_ADV_CUSTOM --> 1000 ms adv interval)
			bt_le_adv_start(BT_LE_ADV_CUSTOM, ap.adv_buffer, ARRAY_SIZE(ap.adv_buffer), NULL, 0);
		}
	}
}



void adv_change_status(uint8_t stat){
	ap.usr_tx_buffer[0] = stat;

	copy_to_adv_buffer();
}


// void adv_update_input_voltage(uint16_t value){
// 	ap.usr_tx_buffer[VOLT_IN_NR] = (uint8_t)value;
// 	ap.usr_tx_buffer[VOLT_IN_NR + 1] = (uint8_t)(value >> 8);

// 	copy_to_adv_buffer();
// }

void adv_update_adc_readings(void){
	uint16_t adc_send_buf[NO_VARIABLES_TX];
	if(uav_battery_voltage_mv > 0) 		{ adc_send_buf[0] = (uint16_t)(uav_battery_voltage_mv);		} else { adc_send_buf[0] = 0; }
	if(uav_current_ma > 0)				{ adc_send_buf[1] = (uint16_t)(uav_current_ma);				} else { adc_send_buf[1] = 0; }
	if(pre_reg_input_current_ma > 0)	{ adc_send_buf[2] = (uint16_t)(pre_reg_input_current_ma);	} else { adc_send_buf[2] = 0; }
	if(pre_reg_output_current_ma > 0)	{ adc_send_buf[3] = (uint16_t)(pre_reg_output_current_ma);	} else { adc_send_buf[3] = 0; }
	if(vamp_supply_voltage_mv > 0)		{ adc_send_buf[4] = (uint16_t)(vamp_supply_voltage_mv);		} else { adc_send_buf[4] = 0; }

	for(uint8_t i = 0; i < NO_VARIABLES_TX; i++){
		ap.usr_tx_buffer[1 + i*2] = (uint8_t)(adc_send_buf[i] >> 8);
		ap.usr_tx_buffer[1 + 1 + i*2] = (uint8_t)adc_send_buf[i];	
	}

	copy_to_adv_buffer();
}

