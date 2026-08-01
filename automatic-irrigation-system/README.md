# Automatic Irrigation System

A two-board Arduino irrigation controller that automatically waters 4 garden plots based on soil moisture readings, with a keypad + LCD interface for manual control and monitoring.

## How it works

- **Arduino Mega** (`arduino_mega.ino`) reads 8 soil moisture sensors (2 per plot across 4 plots), averages readings per plot, tracks a water reservoir level, and drives 4 solenoid valves and 2 pumps to water plots as needed.
- **Arduino UNO** (`arduino_uno_keypad.ino`) drives a 4x4 keypad and an I2C LCD as the user interface, and communicates with the Mega over Serial to display status and accept manual commands.

## Hardware

- Arduino Mega 2560
- Arduino UNO
- 8x soil moisture sensors
- 4x solenoid valves
- 2x water pumps
- 4x4 matrix keypad
- I2C LCD display

## Files

- `arduino_mega.ino` - sensor reading, pump/solenoid control, timing logic
- `arduino_uno_keypad.ino` - keypad input and LCD display, Serial link to the Mega
