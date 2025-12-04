/*****************************************************************************
 * File: FinalProject_AVR.c
 * Project: Zumo Wrestling Bot Controller
 * Author: Sana Ghanem
 * Date: Fall 2025
 * 
 * Description: 
 *   This program controls a Zumo robot via Bluetooth using a joystick.
 *   It uses Timer interrupts to periodically read the joystick and 
 *   UART0 to send commands wirelessly to the Arduino.
 *
 * Hardware:
 *   - ATmega128A on ReadyAVR board
 *   - Joystick connected to ADC pins (analog input)
 *   - HC-05 Bluetooth Master on PE0 (RX) and PE1 (TX)
 * 
 *
 * Communication Protocol:
 *   'F' = Forward, 'B' = Backward, 'L' = Left, 'R' = Right, 'S' = Stop
 *   '+' = Speed up, '-' = Slow down, 'D' = Defense mode
 *****************************************************************************/

#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/intterr

// The set up for serial communication (UART)
#define BAUDRATE 9600
#define DATABITS 8
#define STOPBITS 1
#define PARITY 0

#define center_x 512 // X axis value when the joystick is centered (from 0 - 1023)
#define center_y 512 // Y axis value when the joystick is centered (from 0 - 1023)

// Global variables (between main program and intterrupts )
 
volatile uint8_t timer_flag = 0;      // set to 1 by the timer isr when it's time to read the joystick
volatile char current_command = 'S' ; // current command that is going to send ( 'F', 'B', 'L, 'R', 'S' etc etc)
volatile char last_command = 'S';     // Previous command so it doesn't get sent twice 

void adc_init(void){
	
	// ADMUX controlled the voltage reference and channel s 
	ADMUX = (1 << REFS0); // Use AVCC as a reference volatge 5V
	
	// ADEN - Enable ADC , ADPS2 - 1 sets to 111 prescaler 128
}