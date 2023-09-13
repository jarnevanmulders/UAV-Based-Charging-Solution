/******************************************************************************
 *
 * This is a library to control all peripherals 
 * connected with the power transmitter unit.
 * 
 * Designed by Jarne Van Mulders
 *
 ******************************************************************************/

#include "peripherals.h"

int16_t channel_voltage [4];

void perihperals_init(){
    // Reset ADS1115 ADC device
    ads1115_reset();

    // Initialize ADS1115 ADC device
    ads1115_init();

    // Inititalize programmable load

}

void peripherals_update_all_ads1115_channels(){
    for(uint8_t i = 0; i < ADS1115_NUMBER_OF_CHANNELS; i++){
        channel_voltage[i] = peripherals_ads1115_read_channel(i);
    }
}

int16_t peripherals_ads1115_read_channel(uint8_t channel){
  int32_t voltage = 0;
  ads1115_set_single_channel(channel);
  ads1115_start_single_measurement();
  while(ads1115_is_busy()){}
  voltage = ads1115_get_result_mV();
  return voltage;
}