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
#include "../TPL0401x/TPL0401x.h"

#define LT8330_REFERENCE_VOLTAGE_MV     1600
#define LT8330_FB_RES_H                 10000

void peripherals_set_pre_reg_voltage(uint16_t voltage_mv);

void perihperals_init();

void peripherals_update_all_ads1115_channels();

int16_t peripherals_ads1115_read_channel(uint8_t channel);

void peripherals_convert_readings();

#endif
