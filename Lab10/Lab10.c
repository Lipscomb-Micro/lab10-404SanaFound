/*****************************************************************************
 *	    File: Lab10.c
 *  Lab Name: RS232/BT Communication Tester
 *    Author: Dr. Greg Nordstrom & Prof. John Hutton
 *   Created: 12/30/17
 *  Modified: 3/28/19, 1/17/21, 11/13/2023
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server by reading a character
 * from a UART Terminal and reflecting that character to the BT UART
 * device.
 *
 * This program depends on the UARTLibrary used in previous labs, but
 * students are required to create and use their own UARTLibrary.c file,
 * but must use the UARTLibrary.h file that was given to them in Lab07.
 *
 * NOTE:  Since we skipped Lab08 this year (2023-Fall), I'm providing 
 * the USARTLibrary.a file from Lab07.
 *
 * In other words, their routines must conform to the prototypes they
 * have previously used.
 *
 * NB: The ReadyAVR-64 board uses UART1 as the default for programming,
 * freeing uart1 for external RS232 traffic (e.g. like adding the Bluetooth
 * transceivers in Lab09).
 *
 *   UART1 for communication to a Putty Terminal
 *   UART0 for communication across the paired BT channel.
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define END_STRING  '\0'
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  // for sprintf functionality
#include "UARTLibrary.h"
#include "UART0Library.h"

//#define BAUDRATE    19200 // standard values from 1200 to 115200
//#define DATABITS        7 // 5-8
//#define STOPBITS        1 // 1,2
//#define PARITY          0 // 0=none, 1=odd, 2=even

#define BAUDRATE   9600 // standard values from 1200 to 115200
#define DATABITS      8 // 5-8
#define STOPBITS      1 // 1,2
#define PARITY        0 // 0=none, 1=odd, 2=even

// Button Definitions --------------------------------------------------
#define forward_button PB3 
#define backward_button PB1
#define left_button PB2
#define right_button PB0
#define center_button PB4

char text[40];

void send_string(const char *cp) {
	while(*cp != END_STRING) {
		uart_tx(*cp++);
	}
}

int main(void) {
	char rcvdChar;
	const char resetMsg[] = "\r\nRS232/BT Test Reflector.\r\n\0";
	
 // Setting up the buttons for the joystick ------------------------------------
// setting as inputs with pull-ups 
DDRB = DDRB & ~(1 << forward_button);  // PB3 as input button 
DDRB = DDRB & ~(1 << backward_button);  // PB1 as input button 
DDRB = DDRB & ~(1 << left_button);  // PB2 as input button 
DDRB = DDRB & ~(1 << right_button);  // PB0 as input button 
DDRB = DDRB & ~(1 << center_button);  // PB4 as input button 

// buttons should read high when they aren't being pressed (turning on pull up registers)
PORTB = PORTB | (1 << forward_button); // pull up on PB3
PORTB = PORTB | (1 << backward_button); // pull up on PB1
PORTB = PORTB | (1 << left_button); // pull up on PB2
PORTB = PORTB | (1 << right_button); // pull up on PB0
PORTB = PORTB | (1 << center_button); // pull up on PB4

	// For simplicity, use the same setup information for both ports.
	// Init UART1 for debug information to Putty
    uart_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	// Init UART0 for BT/UART path
	uart0_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
 
	send_string(resetMsg);
	
	 char lastSent = 'S';
	 uart0_tx('S');
	 int extra_attack = 0;
	
	while(1)
	{
// Checking if a button was pressed or not (the joystick) to send the command

// Forward button pressed?
if (!(PINB &(1 << forward_button))){
	if (lastSent != 'F'){
	uart0_tx('F');     // this should send an f to the zumo
	uart_tx('F');      // Should show it in putty   
	lastSent = 'F';
}
}
else if (!(PINB &(1 << backward_button))){
	if (lastSent != 'B'){
		
	uart0_tx('B');     // this should send an f to the zumo
	uart_tx('B');      // Should show it in putty 
	lastSent = 'B';   
}
}
else if (!(PINB &(1 << left_button))){
	if (lastSent != 'L'){
		
	uart0_tx('L');     // this should send an f to the zumo
	uart_tx('L');      // Should show it in putty 
	lastSent = 'L';   
}
}
else if (!(PINB &(1 << right_button))){
	if (lastSent != 'R'){
		
	uart0_tx('R');     // this should send an f to the zumo
	uart_tx('R');      // Should show it in putty 
	lastSent = 'R';   
} 
	}
else if (!(PINB &(1 << center_button))){
	if (lastSent != 'C'){
		
	uart0_tx('C');     // this should send an f to the zumo
	uart_tx('C');      // Should show it in putty 
	lastSent = 'C';   
} 
}
	
else {
	// if the button isnt pressed?
	if (lastSent != 'S'){
		uart0_tx('S');
		uart_tx('S');
		lastSent = 'S';
	}
}

		// wait for a character
		if (uart_data_available()){
		rcvdChar = uart_rx();
		
		switch(rcvdChar)
		{
			case CR:
				// send back both CR and LF (makes PuTTY work without having to assume CRs w/ LFs)
				uart_tx(CR);	// echo char back to sender
				uart_tx(LF);	// echo char back to sender
				break;					
			default:
				// Echo normal character back to terminal
				uart_tx(rcvdChar);
				// Send character to BT Device (Zumo)
				uart0_tx(rcvdChar);
		}
		 
	}
	_delay_ms(50);
	
}
	return 0;
}
