/*
 * SoftUart2.h
 *
 * Created: 24.02.2015 09:57:33
 *  Author: Viktor
 */ 


#ifndef SOFTUART_H_
#define SOFTUART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/*
################################################################################
This is a full duplex software Uart library.

IMPORTANT: set port_rotator to 1 when doing first init.

Todo: Output buffer
#############################################################################
*/



#define BUFFERSIZE 32
#define txbusy 0				// set if a byte is in transmission
#define rxbusy 1				// set if a byte is being received
#define rxframe 2

static uint8_t uart_bitpattern[9] = {0,1,2,4,8,16,32,64,128}; //this is used to know the actual value of a incoming bit in the final char. Read only, so no need to put it in every uart_port object.

volatile char port_rotator;   // this variable determines on which port the read/send method will be executed at the moment.
							// it is incremented/set to 0 after each interrupt. The interrupt is called by a timer periodically.


//this is the uart_port struct. A port represents a physical uart port, made of two pins on the arduino.
//The set of uart_port attributes are where the state of a port is saved, the method for reading exists only once, but the port which is given as an argument changes.
//Therefore, depending on which uart_port object is given to the read/write method, the according port is used.

//Addition: A uart_message consist of more than just one bit, so the bits have to be saved for each port. This happens in the buffer2 of every uart_port object.
typedef struct {
	char port_number;			//The port number as an actual number.
	volatile uint8_t yeah;		//if true, this port lights the debug led.
	volatile uint8_t	uart_rxd;		// the byte being received
	volatile uint8_t	uart_txd;		// the byte being transmitted
	volatile uint8_t	uart_txtick;	// tick count for the transmitter
	volatile uint8_t	uart_rxtick;	// tick count for the receiver
	volatile uint8_t	uart_txbit;		// bit count for tx
	volatile uint8_t	uart_rxbit;		// bit count for rx
	volatile uint8_t	uart_status;	// uart status
	volatile uint8_t next_read_2;
	volatile uint8_t next_write_2;	// the pointers to the receive2 buffer
	volatile uint8_t uartrx;			//the pin used by the port to receive something.
	volatile uint8_t uarttx;			//the pin used by the port to send something.

	volatile unsigned char buffer2[BUFFERSIZE];	// circular buffer for receiver 2

}uart_port;

//this is our port array:
//it has positions 0-3 althoug the port numbers are 1-4. attention!
//in this port array, all four port objects are saved.
uart_port port[4];

//######################################################################
//
//			METHODS
//
//######################################################################


void uart_init (uart_port *curr_port); //Used for initialization, argument is a port object, in which for example tha pins to use are specified. where specified? at com_helper_init().

char uart_ischar (uart_port *curr_port); //checks if there is a new char to be read.

char uart_get_char (uart_port *curr_port); // gets this char, which is to be read.

void uart_out_char (char ch, uart_port *curr_port); // prints a char via the software Uart. Currently the main routine waits until its printed, in the future abuffer will be implemented

void uart_print (char * text); //prints a whole string, doesnt work currently

void do_what_needs_to_be_done(uart_port *curr_port);






#endif /* SOFTUART_H_ */
