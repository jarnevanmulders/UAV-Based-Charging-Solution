# Hardware status
## PRU
Working

Updates from v1.0 to v1.1
- I2C programmable load
- VDD_BLE directly connected to ADC pin P03 (without voltage divider)
- Additional RC filter after current limiter OPAMP
- Button from pull-up to pull down
- Additional filter (ferrite bead) on VDD_BLE supply line
- Other load switch implementation (with dual PMOS PMDPB55XP)
- Diode and shunt resistor location changed
- MIFA antenna tuning
  - Measured impedance 25+j25
    - Measured with R&S ZVL 3GHz VNA
    - Used a calibrated UFL connector cable (calibrated with ZVL one port calibration procedure)
    - Blank PCB board was used for measurement
  - Added 1.2 pF parallel with antenna (as compensation (see Smith V4.1))
  - Total [Pi] matching circuit (MCU - 0.8 pF capacitor - 3.9 nH inductor - 1.2 pF capacitor)
