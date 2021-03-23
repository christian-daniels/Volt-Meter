# Volt-Meter
An embedded system that implements a volt meter. Using an ATMega32 microcontroller, a keypad as input, and a LCD as output a user is able to view the current voltage output.


## Functions
### Measure Voltage
A user is able to use a jumper cable to measure voltage output from devices. The microcontroller was adjusted to make PinA an input GPIO.
### Calculate parameters
The meter calculates instantaneous voltage, max voltage, min voltage, and average voltage in real time. The LCD displays these values for the user to see. These parameters may also be reset to show most recent data.


## Main Modules Used
* ATMega32 Microcontroller
* 16 input Keypad
* 16 Character LCD
* Jumper cable
