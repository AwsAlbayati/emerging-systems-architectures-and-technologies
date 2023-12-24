# Smart Thermostat Project

## Overview
This project implements a smart thermostat for three different microcontroller architectures: Texas Instruments, Microchip, and Freescale. The thermostat system integrates various peripherals such as GPIO, UART, and I2C, enabling features like temperature reading, setpoint adjustment, heater control, and cloud communication via Wi-Fi.

The project involved creating a smart thermostat for three microcontroller architectures—Texas Instruments, Microchip, and Freescale. The thermostat, integrating peripherals like GPIO, UART, I2C, and task scheduling, read room temperature, adjusted setpoints via buttons, controlled a heater based on temperature, and communicated with the cloud via Wi-Fi. The goal was to enhance convenience and energy efficiency in heating applications by enabling remote monitoring and control.

To ensure code maintainability, readability, and adaptability. The project featured a modular code structure, clear variable/function names, and comprehensive comments/documentation. I adhered to coding standards, utilized version control (Git), implemented error handling, and established a testing suite for reliability. 

The skills transferable from this project include embedded systems programming, peripheral integration, wireless communication implementation, cloud integration, version control, and problem-solving. The emphasis on documentation and security awareness, alongside exposure to project management tools, adds versatility. The project's maintainability was ensured through organized code, adherence to standards, and thorough testing, fostering readability and adaptability.

## Features
* Read room temperature using I2C-connected sensors.
* Adjust setpoints via button inputs for each microcontroller architecture.
* Control a heater based on the temperature setpoint.
* Communicate with the cloud via Wi-Fi to send temperature data and receive remote commands.
* Modular code structure for easy customization and expansion.

## Getting Started

### Prerequisites
* Microcontroller Hardware - List of supported microcontrollers.
* Development Environment - Set up the development environment for each microcontroller architecture.

### Installation

* Clone the repository:
 ``` bash Copy code git clone https://github.com/yourusername/smart-thermostat.git  ```

* Install dependencies:
 ``` bash Copy code cd smart-thermostat npm install  # or your preferred package manager ```

## Usage

### Configuring Microcontroller
* Navigate to the config directory.
* Edit the configuration file specific to your microcontroller (e.g., ti_config.json, microchip_config.json, freescale_config.json).

### Running the Thermostat
* Compile and flash the code to your microcontroller using your preferred development environment.
* Power on the smart thermostat.

## Task Scheduler
The project includes a task scheduler for managing periodic tasks. Refer to the Task Scheduler Documentation for detailed information.
