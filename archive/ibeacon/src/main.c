/*
 * Copyright (c) 2018 Henrik Brix Andersen <henrik@brixandersen.dk>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#ifndef IBEACON_RSSI
#define IBEACON_RSSI 0xc8
#endif

char name [] = "wpt-transmitter";

/*
 * Set iBeacon demo advertisement data. These values are for
 * demonstration only and must be changed for production environments!
 *
 * UUID:  18ee1516-016b-4bec-ad96-bcb96d166e97
 * Major: 0
 * Minor: 0
 * RSSI:  -56 dBm
 */
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, name, sizeof(name) - 1),
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	// BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA,
	// 	      0x4c, 0x00, /* Apple */
	// 	      0x02, 0x15, /* iBeacon */
	// 	      0x18, 0xee, 0x15, 0x16, /* UUID[15..12] */
	// 	      0x01, 0x6b, /* UUID[11..10] */
	// 	      0x4b, 0xec, /* UUID[9..8] */
	// 	      0xad, 0x96, /* UUID[7..6] */
	// 	      0xbc, 0xb9, 0x6d, 0x16, 0x6e, 0x97, /* UUID[5..0] */
	// 	      0x00, 0x00, /* Major */
	// 	      0x00, 0x00, /* Minor */
	// 	      IBEACON_RSSI) /* Calibrated RSSI @ 1m */
};

// static void bt_ready(int err)
// {
// 	if (err) {
// 		printk("Bluetooth init failed (err %d)\n", err);
// 		return;
// 	}

// 	printk("Bluetooth initialized\n");

// 	/* Start advertising */
// 	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
// 			      NULL, 0);
// 	if (err) {
// 		printk("Advertising failed to start (err %d)\n", err);
// 		return;
// 	}

// 	printk("iBeacon started\n");
// }

int main(void)
{
	int err;

	struct bt_le_adv_param param;


	printk("Starting iBeacon Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	// bt_set_name("wpt-transmitter");

	// param.options = BT_LE_ADV_OPT_USE_NAME;

	// bt_le_ext_adv_create(&param, NULL, &adv_set);


	printk("Bluetooth initialized\n");

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
			      NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("iBeacon started\n");



	return 0;
}
