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

#define NAME_LEN 30

#define RX_DEVICE_NAME "wpt-receiver"

uint16_t input_voltage_mv;
uint16_t supply_voltage_mv;
uint16_t buck_current_ma;

static void extract_data(uint8_t *buffer, uint8_t len){
	input_voltage_mv = *(buffer + 17) << 8 | *(buffer + 18);
	supply_voltage_mv = *(buffer + 19) << 8 | *(buffer + 20);
	buck_current_ma = *(buffer + 21) << 8 | *(buffer + 22);
}

static void print_buffer(uint8_t *buffer, uint8_t len){
	// Print full message
	for (size_t i = 0; i < len; i++) {
        printk("%02X ", *(buffer + i));
    }
	printk("\n");	
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	// Desired - buffer name rx adv
	char ds_name_buffer [] = RX_DEVICE_NAME;
	uint8_t buffer_cmp_len = sizeof(ds_name_buffer) - 1;

	// Scanned - buffer name rx adv
	char sc_name_buffer [buffer_cmp_len];
	memcpy(sc_name_buffer, ad->data + 2, buffer_cmp_len);
	
	// Print buffers to check similarity
	// print_buffer(ds_name_buffer, buffer_cmp_len);
	// print_buffer(sc_name_buffer, buffer_cmp_len);

	// Compare both buffer
	int result = memcmp(ds_name_buffer, sc_name_buffer, buffer_cmp_len);

	if(result == 0){
		bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
		printk("Device found: %s (RSSI %d), type %u, AD data len %u\n",
			addr_str, rssi, type, ad->len);

		print_buffer(ad->data, ad->len);

		extract_data(ad->data, ad->len);

		printk("input_voltage_mv: %"PRId32" mV\n", input_voltage_mv);
		printk("supply_voltage_mv: %"PRId32" mV\n", supply_voltage_mv);
		printk("buck_current_ma: %"PRId32" mA\n", buck_current_ma);
	}
}


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

	// 	k_msleep(10);

	// 	bt_le_scan_stop();

	// 	k_msleep(1000);
	// }

	return 0;
}