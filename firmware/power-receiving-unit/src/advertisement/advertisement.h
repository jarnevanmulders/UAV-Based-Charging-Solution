

#ifndef ADVERTISEMENT_H_
#define ADVERTISEMENT_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#define ADV_TX_BUF_LEN 	7

#define STAT_NR				0
#define VOLT_IN_NR			1
#define VOLT_OUT_NR 		3
#define CURR_OUT_NR			5

#define STAT_CHG			0x00
#define STAT_RDY			0x01
#define STAT_NC 			0x02
#define STAT_CHG_THROTTLED	0x03

// Filter name of incoming ADV messages
static char name [] = "wpt-receiver";

// *** Default advertisement message content ***
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, name, sizeof(name) - 1),
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR)
};

// Default size ADV message
#define DEFAULT_STRUCT_SIZE_ADV_MSG 	sizeof(struct bt_data)
#define DEFAULT_SIZE_ADV_MSG 			(uint8_t)(sizeof(ad)/sizeof(struct bt_data))

// Define struct
struct adv_params {
    bool update;
    uint8_t usr_tx_buffer [ADV_TX_BUF_LEN];
	struct bt_data adv_buffer[DEFAULT_SIZE_ADV_MSG + 1];
};

// // Communication
// static void print_buffer(uint8_t *buffer, uint8_t len){
// 	// Print full message
// 	for (size_t i = 0; i < len; i++) {
//         printk("%02X ", *(buffer + i));
//     }
// 	printk("\n");	
// }



void adv_start();

void adv_change_status(uint8_t stat);


#endif /* ADVERTISEMENT_H_ */