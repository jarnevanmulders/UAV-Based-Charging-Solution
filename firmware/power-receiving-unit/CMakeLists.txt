# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ibeacon)

target_sources(app PRIVATE src/main.c)

if(IBEACON_RSSI)
  zephyr_compile_definitions(IBEACON_RSSI=${IBEACON_RSSI})
endif()

# NORDIC SDK APP START
target_sources(app PRIVATE src/advertisement/advertisement.c)
target_sources(app PRIVATE src/adc_update/adc_update.c)
target_sources(app PRIVATE src/state_machine/state_machine.c)
target_sources(app PRIVATE src/gpio_control/gpio_control.c)
# NORDIC SDK APP END
