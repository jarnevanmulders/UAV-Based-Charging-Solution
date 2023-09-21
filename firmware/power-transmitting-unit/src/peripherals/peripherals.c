/******************************************************************************
 *
 * This is a library to control all peripherals 
 * connected with the power transmitter unit.
 * 
 * Designed by Jarne Van Mulders
 *
 ******************************************************************************/

#include "peripherals.h"

int16_t channel_voltage [ADS1115_NUMBER_OF_CHANNELS];
// int16_t channel_result [ADS1115_NUMBER_OF_CHANNELS];

int16_t uav_battery_voltage_mv = 0;
int16_t uav_current_ma = 0;
int16_t pre_reg_input_current_ma = 0;
int16_t pre_reg_output_current_ma = 0; 

void peripherals_set_pre_reg_voltage(uint16_t voltage_mv){
    
    // Calculate lower resistor value
    uint16_t fb_res_l = LT8330_REFERENCE_VOLTAGE_MV*LT8330_FB_RES_H/(voltage_mv - LT8330_REFERENCE_VOLTAGE_MV);

    // Convert to hex value
    uint8_t value = (uint8_t)(fb_res_l*128/TPL_INTERNAL_RES);

    printk("%d \n", value);
    
    // Update TPL --> programmable load
    TPL0401x_update_resistance(value);
}

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
    peripherals_convert_readings();
}

int16_t peripherals_ads1115_read_channel(uint8_t channel){
  int32_t voltage = 0;
  ads1115_set_single_channel(channel);
  ads1115_start_single_measurement();
  while(ads1115_is_busy()){}
  voltage = ads1115_get_result_mV();
  return voltage;
}

void peripherals_convert_readings(){
    for(uint8_t i = 0; i < ADS1115_NUMBER_OF_CHANNELS; i++){
        switch (i){
            case 0: uav_battery_voltage_mv = channel_voltage[i]*110/10;     break;
            case 1: uav_current_ma = channel_voltage[i]*100/50;             break; // multiply by 100   ===== dividing by 0.01
            case 2: pre_reg_input_current_ma = channel_voltage[i]*10/50;    break; // multiply by 10    ===== dividing by 0.1
            case 3: pre_reg_output_current_ma = channel_voltage[i]*10/50;   break; // multiply by 10    ===== dividing by 0.1
            default: break;
        }
    }
}