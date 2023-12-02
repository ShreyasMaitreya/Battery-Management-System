# Battery Management System (BMS) Code

## Overview
This repository contains the Arduino code for a Battery Management System (BMS) designed to monitor and manage the health and state of a battery pack. The system uses the bq769x0 library for BMS operations and focuses on State of Charge (SOC), State of Health (SOH), and protection against overcurrent, overcharge, and overdischarge. It must be noted that this code is compatible ONLY with the BQ769X0 ICs from Texas Instruments.

## Features
- **SOC Calculation:** Monitors and calculates the State of Charge of the battery.
- **SOH Calculation:** Determines the State of Health of the battery based on internal resistance.
- **Protection Circuits:** Includes overcurrent, overcharge, and overdischarge protection.
- **Data Storage:** Uses EEPROM for storing vital parameters.
- **Hardware Interrupts:** Manages multiple hardware interrupts for overcurrent protection.

## Hardware Requirements
- Any compatible microcontroller (e.g., ATMEGA328P, ATMEGA1280, ATMEGA2560, MSP430 or any other microcontroller compatible with the Arduino development platform.)
- bq769x0 Battery Management IC
- Shunt resistor
- External circuitry for voltage and current measurement

## Installation
1. Clone the repository or download the source code.
2. Install the `bq769x0` library in your Arduino IDE.
3. Connect the BMS hardware as per the schematic (not provided in this README).
4. Upload the code to your microcontroller.

## Usage
1. **Initialization:** The BMS is initialized with the `BatteryManagement` function, setting up various parameters like temperature limits, shunt resistor value, and protection settings.
2. **SOC and SOH Calculation:** The `SOC_Calc` and `SOH_Calc` functions are responsible for calculating the State of Charge and State of Health, respectively.
3. **Data Storage:** The SOC, SOH, and Depth of Discharge (DOD) values are stored in EEPROM for persistence.
4. **Protection Mechanisms:** The code includes functions to handle overcurrent situations and to adjust the battery capacity based on its degradation over time.

## Configuration
- The pins for the BMS alert and boot are defined at the beginning of the code.
- Adjust the `BMS_I2C_ADDRESS` according to your setup.
- Modify the battery capacity, internal resistance, and other parameters as per your specific battery specifications.

## Contributing
Feel free to fork this project and contribute by submitting pull requests with improvements or by reporting issues.

## Disclaimer
This code is provided "as is" and without any warranty. The author is not responsible for any damage caused by the use of this code.
