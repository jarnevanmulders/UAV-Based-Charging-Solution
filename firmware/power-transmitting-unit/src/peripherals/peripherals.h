/******************************************************************************
 *
 * This is a library to control all peripherals 
 * connected with the power transmitter unit.
 * 
 * Designed by Jarne Van Mulders
 *
 ******************************************************************************/

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "../ADS1115/ADS1115.h"

void perihperals_init();

void peripherals_update_all_ads1115_channels();

int16_t peripherals_ads1115_read_channel(uint8_t channel);

#endif
