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
		// Check every 100 ms
		k_msleep(100);

		if(ap.update){
			ap.update = false;

			// Stop advertising
			bt_le_adv_stop();
			
			// Start advertising 
			bt_le_adv_start(BT_LE_ADV_NCONN, ap.adv_buffer, ARRAY_SIZE(ap.adv_buffer), NULL, 0);
		}
	}
}



void adv_change_status(uint8_t stat){
	ap.usr_tx_buffer[STAT_NR] = stat;

	copy_to_adv_buffer();
}


void adv_update_input_voltage(uint16_t value){
	ap.usr_tx_buffer[VOLT_IN_NR] = (uint8_t)value;
	ap.usr_tx_buffer[VOLT_IN_NR + 1] = (uint8_t)(value >> 8);

	copy_to_adv_buffer();
}