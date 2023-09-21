/******************************************************************************
 *
 * 
 ******************************************************************************/

#ifndef TPL0401X_H_
#define TPL0401X_H_

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#include "../twowire/twowire.h"

#define TPL_ADDRESS_A       0x2E
#define TPL_ADDRESS_B       0x3E

#define TPL_ADDRESS         TPL_ADDRESS_A

#define REG_WIPER           0x00

#define TPL_STEPS           128
#define TPL_INTERNAL_RES    10000

// TODO
// Check connection ACK

void TPL0401x_update_resistance(uint8_t value);


// uint16_t read_register(uint8_t regaddr);
// void write_register(uint8_t regaddr, uint16_t regval);


#endif

