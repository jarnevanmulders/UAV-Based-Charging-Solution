/******************************************************************************
 *
 * 
 ******************************************************************************/

#include "TPL0401x.h"


void TPL0401x_update_resistance(uint8_t value){
    twowire_write_register_byte(TPL_ADDRESS, REG_WIPER, value);
}

// uint16_t read_register(uint8_t regaddr){
//     uint8_t read_buffer [2];
//     twowire_read_register_data(DEVICE_ADDRESS, regaddr, read_buffer);
//     return (uint16_t)(read_buffer[0] << 8 | read_buffer[1]);
// }

// void write_register(uint8_t regaddr, uint16_t regval){
//     twowire_write_register_data(DEVICE_ADDRESS, regaddr, regval);
// }