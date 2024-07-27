# Virtual Pet

This project emulates the charm of the 90’s era Tamagotchi toys, creating a virtual pet experience where users can feed, clean, and play with a digital kitten. The project utilizes an LCD display, IR sensors, and a Bluetooth module to monitor and interact with the pet's status in real time.

## Table of Contents
- [Introduction](#introduction)
- [Project Methodology](#project-methodology)
- [Components List](#components-list)
- [Circuit Schematic](#circuit-schematic)
- [Code Overview](#code-overview)
- [Usage](#usage)
- [Contributors](#contributors)
- [License](#license)

## Introduction

The Virtual Pet project aims to recreate the interactive experience of taking care of a virtual pet. Users can ensure their pet's happiness by maintaining its stats: Fullness (F), Excitement (P), and Cleanliness (C). The pet's emotions change based on these stats, providing a dynamic and engaging user experience.

## Project Methodology

### Why

We were inspired by the nostalgic charm of Tamagotchi toys, where users interact with and care for a virtual pet. The goal was to create a similar experience using modern electronics and coding practices.

### How

- **LCD Screen**: Displays the pet's face and stats.
- **IR Sensors**: Detect user actions (feeding, playing, cleaning).
- **LED Indicators**: Show the pet's emotional state.
- **Bluetooth Module**: Allows remote interaction with the pet via a mobile app.

The pet's stats decrease over time, and users must perform actions to maintain them. Each stat affects the pet's overall happiness, displayed through different facial expressions and LED colors.

## Components List

- 1x PIC16F877A Microcontroller
- 1x LCD Display
- 3x IR Sensors
- 1x Bluetooth Module
- 3x Multicolored LEDs
- 1x Potentiometer
- 3x 330 Ohm Resistors
- 1x 20 MHz Crystal
- 2x 22 pF Capacitors
- Wires and Jumper Wires

## Circuit Schematic

![image](https://github.com/user-attachments/assets/44cee991-c394-4d7a-8af8-5d634596671e)
![image](https://github.com/user-attachments/assets/dd56de7b-4860-49b7-9196-60606b663d82)

## Video


https://github.com/user-attachments/assets/7403b610-c395-465e-af1b-8361e1c1a90c


## Code Overview

The code initializes the LCD display, IR sensors, and Bluetooth module. It uses a timer to decrease the pet's stats and updates the display based on the pet's current state. The Bluetooth module allows for remote interaction, and the LED indicators show the pet's emotional state.

### Main Functions

- **Timer0_Init**: Initializes the timer.
- **interrupt**: Timer interrupt service routine.
- **Lcd_Init_Custom**: Initializes the LCD display.
- **display_cat_face**: Displays the cat's face and stats on the LCD.
- **UART_Init_Custom**: Initializes UART for Bluetooth communication.
- **UART_Send_String**: Sends a string via Bluetooth.
- **UART_Receive**: Receives a string via Bluetooth.

For a detailed look at the code, check the `main.c` file in the repository.

## Usage

1. **Setup**: Assemble the circuit as per the schematic.
2. **Upload Code**: Program the microcontroller with the provided code.
3. **Interact**: Use the IR sensors to feed, play, and clean the pet. Connect via Bluetooth for remote interaction.
4. **Monitor**: Watch the pet's stats and emotions change on the LCD and through LED indicators.

## Contributors

- Ismail Mohamed Waleed 
- Ahmed Kamal Abdallah 
- Alaa Adel Imam 
- Ghada Fares 

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
