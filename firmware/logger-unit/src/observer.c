/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
// #include <bluetooth/scan.h>

// #define DEBUG_ADV_RAW_DATA

#define NAME_LEN 30

#define RX_DEVICE_NAME "wpt-receiver"
#define TX_DEVICE_NAME "wpt-transmitter"

// Receiver device variables
int16_t input_voltage_mv;
int16_t supply_voltage_mv;
int16_t buck_current_ma;

// Transmitter device variables
int16_t uav_battery_voltage_mv;
int16_t uav_current_ma;
int16_t pre_reg_input_current_ma;
int16_t pre_reg_output_current_ma;
int16_t vamp_supply_voltage_mv;


char rx_device_name_buffer [] = RX_DEVICE_NAME;
char tx_device_name_buffer [] = TX_DEVICE_NAME;
char *device_name_pointer_buffer [] = {rx_device_name_buffer, tx_device_name_buffer};
uint8_t device_name_len_buffer [] = {sizeof(rx_device_name_buffer), sizeof(tx_device_name_buffer)};
typedef enum{RX_DEVICE, TX_DEVICE, NONE} Devices;


// Static functions
static void extract_rx_data(uint8_t *buffer, uint8_t len);
static void extract_tx_data(uint8_t *buffer, uint8_t len);
static void print_buffer(uint8_t *buffer, uint8_t len);


uint8_t check_device(struct net_buf_simple *ad){

	for(uint8_t device = RX_DEVICE; device <= TX_DEVICE; device++){
		// Desired - buffer name rx adv
		// char ds_name_buffer [] = RX_DEVICE_NAME;
		uint16_t buffer_cmp_len = device_name_len_buffer[device] - 1;//sizeof(device_name_pointer_buffer[device]) - 1;

		// Scanned - buffer name rx adv
		char sc_name_buffer [buffer_cmp_len];
		memcpy(sc_name_buffer, ad->data + 2, buffer_cmp_len);
		
		// Print buffers to check similarity
		// print_buffer(ds_name_buffer, buffer_cmp_len);
		// print_buffer(sc_name_buffer, buffer_cmp_len);

		// Compare both buffer
		int result = memcmp(device_name_pointer_buffer[device], sc_name_buffer, buffer_cmp_len);

		if(result == 0){
			return device;
		}
	}
	return NONE;
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad){

	char addr_str[BT_ADDR_LE_STR_LEN];

	uint8_t dev = check_device(ad);

	#ifdef DEBUG_ADV_RAW_DATA 
		if(dev != NONE){
			printk("Device found: %s (RSSI %d), type %u, AD data len %u\n",
			addr_str, rssi, type, ad->len);
			print_buffer(ad->data, ad->len);
		}
	#endif

	switch (dev){
		case RX_DEVICE:
			bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

			extract_rx_data(ad->data, ad->len);

			printk("RECEIVER: input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
			printk("RECEIVER: supply_voltage_mv: %"PRId32" mV\n", supply_voltage_mv);
			printk("RECEIVER: buck_current_ma: %"PRId32" mA\n", buck_current_ma);

			break;

		
		case TX_DEVICE:
			bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

			extract_tx_data(ad->data, ad->len);

			// printk("TRANSMITTER: uav_battery_voltage_mv: %"PRId32" mV\n", uav_battery_voltage_mv);
			// printk("TRANSMITTER: uav_current_ma: %"PRId32" mA\n", uav_current_ma);
			// printk("TRANSMITTER: pre_reg_input_current_ma: %"PRId32" mA\n", pre_reg_input_current_ma);
			// printk("TRANSMITTER: pre_reg_output_current_ma: %"PRId32" mA\n", pre_reg_output_current_ma);
			printk("TRANSMITTER: vamp_supply_voltage_mv: %"PRId32" mV\n", vamp_supply_voltage_mv);

			break;
		
		default:
			break;
	}

	// // Desired - buffer name rx adv
	// char ds_name_buffer [] = RX_DEVICE_NAME;
	// uint8_t buffer_cmp_len = sizeof(ds_name_buffer) - 1;

	// // Scanned - buffer name rx adv
	// char sc_name_buffer [buffer_cmp_len];
	// memcpy(sc_name_buffer, ad->data + 2, buffer_cmp_len);
	
	// // Print buffers to check similarity
	// // print_buffer(ds_name_buffer, buffer_cmp_len);
	// // print_buffer(sc_name_buffer, buffer_cmp_len);

	// // Compare both buffer
	// int result = memcmp(ds_name_buffer, sc_name_buffer, buffer_cmp_len);

	// if(result == 0){
	// 	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
		
	// 	#ifdef(DEBUG_ADV_RAW_DATA)
	// 		printk("Device found: %s (RSSI %d), type %u, AD data len %u\n",
	// 			addr_str, rssi, type, ad->len);

	// 		print_buffer(ad->data, ad->len);
	// 	#endif

	// 	extract_data(ad->data, ad->len);

	// 	printk("input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
	// 	printk("supply_voltage_mv: %"PRId32" mV\n", supply_voltage_mv);
	// 	printk("buck_current_ma: %"PRId32" mA\n", buck_current_ma);
	// }
}


/********************************************************************/
/*							START OBSERVER							*/
/********************************************************************/

int observer_start(void)
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;


	// while(1){
	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Start scanning failed (err %d)\n", err);
		return err;
	}
	printk("Started scanning...\n");

	return 0;
}

/********************************************************************/
/*							OTHER FUNCTIONS							*/
/********************************************************************/


static void extract_rx_data(uint8_t *buffer, uint8_t len){
	uint8_t pos = 4 + device_name_len_buffer[RX_DEVICE];
	input_voltage_mv = *(buffer + pos) << 8 | *(buffer + pos + 1);
	supply_voltage_mv = *(buffer + pos + 2) << 8 | *(buffer + pos + 3);
	buck_current_ma = *(buffer + pos + 4) << 8 | *(buffer + pos + 5);
}

static void extract_tx_data(uint8_t *buffer, uint8_t len){
	int16_t *extract_pointer_buffer [] = {&uav_battery_voltage_mv, &uav_current_ma, &pre_reg_input_current_ma, &pre_reg_output_current_ma, &vamp_supply_voltage_mv};
	
	// Adv start position user data
	uint8_t pos = 4 + device_name_len_buffer[TX_DEVICE];

	for(uint8_t i = 0; i < sizeof(extract_pointer_buffer)/sizeof(extract_pointer_buffer[0]); i++){
		*extract_pointer_buffer[i] = *(buffer + pos + i*2) << 8 | *(buffer + pos + 1 + i*2);
	}
}

static void print_buffer(uint8_t *buffer, uint8_t len){
	// Print full message
	for (size_t i = 0; i < len; i++) {
        printk("%02X ", *(buffer + i));
    }
	printk("\n");	
}