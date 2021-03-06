/*
 * SoftUart.c
 *
 * Created: 22.02.2015 22:19:11
 *  Author: Viktor
 */ 

#include "SoftUart2.h"


////////////////////////////////////////////////////////////////////////////////////////////
//
// Software uart routines
//
// we need two serial lines, and there's only one usart on the M32
// so we implement another in software
//
// we set up a clock with a timeout of 883 cycles - four times baud rate at 2400 baud -
// and use it to sample an input line and clock an output line
// we also use it to maintain a 20mS tick for timouts and such
//
///////////////////////////////////////////////////////////////////////////////////////////




//this is called periodically, then our periodical routine is called to read/write on the current port, which is afterwards incremented.
ISR(TIMER1_COMPA_vect) 
{		
	
			//DEBUG
	 
	switch (port_rotator){
		case 1:
			do_what_needs_to_be_done(&port[0]);
			break;
		case 2:
			do_what_needs_to_be_done(&port[1]);
			break;
		case 3:
			do_what_needs_to_be_done(&port[2]);
			break;
		case 4:
			do_what_needs_to_be_done(&port[3]);
			break;
	}
	
	if(port_rotator == 4){ // <= rotate the port.
		port_rotator = 1;
	}
	else{
		port_rotator +=1;
	}
	
	
}


//this is the main method for reading/writing

//probably doesnt work right now, as interrupt has to be 4 times faster.
void do_what_needs_to_be_done(uart_port *curr_port){

	if(curr_port->yeah){								//First of all, the debug led is lit if yeah==1.
														//has to be improved to all ports
		DDRB |= _BV(PINB3);
		PORTB |= _BV(PINB3);
		
	}
	else{
		PORTB &= ~_BV(PINB3);
		
	}
		
	// the interrupt signal on a compare for the timer 1
	
	// we arrive here four times per baud rate
	// we first send the output signal, if there's anything to send,
	// since it needs to be somewhere close to accurate...
	// then we read the incoming signal, if there is one,
	// and finally we update the nearly-real-time clock
	// (we never display the clock but it timestamps the output data)
	
	if (curr_port->uart_status & _BV(txbusy))
	{
		
		// we're transmitting something
		// increment the tick - we only do things every four ticks
		curr_port->uart_txtick++;
		if (curr_port->uart_txtick == 4)
		{
			// okay, we've work to do
			curr_port->uart_txtick = 0;
			
			// is it the start bit?
			if (curr_port->uart_txbit == 0)
			{
				// yes...
				PORTD &= ~(_BV(curr_port->uarttx));		// clear the start bit output
				curr_port->uart_txbit++;
			}
			else
			{
				if (curr_port->uart_txbit != 9)
				{
					// deal with the data bits
					if (curr_port->uart_txd & 1){// low bit set?
						PORTD |= _BV(curr_port->uarttx);	// then set the data stream bit
					}		
					else{
					PORTD &= ~(_BV(curr_port->uarttx));		//Here the tx pin is set as low. This doesnt work properly.
					//_delay_us(5);
					//PORTD &= ~(_BV(curr_port->uarttx));	//maybe we can set it low twice?
					
					}
					
					// or clear, as required
					curr_port->uart_txbit++;				// increment the bit count
					
					// and shift the data right
					curr_port->uart_txd /= 2;
				}
				else
				{
					// deal with the stop bit (stop bit is allways on high.
					PORTD |= _BV(curr_port->uarttx);
					curr_port->uart_txbit++;
					
				}
			}
			
			// and finally, if txbit is more than 9, we're done
			if (curr_port->uart_txbit > 9)
			{
				curr_port->uart_txbit = 0;					// clear the bit counter
				curr_port->uart_status &= ~(_BV(txbusy)); // and the busy status
			}
		}
	}
	
	// we may be receiving something
	// if we're *not* yet receiving, we check every clock tick to see if the input
	// line has gone into a stop bit
	// if it has, we wait for half a bit period and then sample every four ticks
	// to put together the rx data

		

	if ((curr_port->uart_status & _BV(rxbusy)) == 0)
	{
		
		// check to see if there's a start
		if ((PIND & _BV(curr_port->uartrx)) == 0)
		{
			
			
			
			// we found a start bit!
			// set the tick count to 2, so we get the sample near the middle of the bit
			curr_port->uart_rxtick = 2;
			// and flag that we're now busy
			curr_port->uart_status |= _BV(rxbusy);
			// we're expecting the start bit now...
			curr_port->uart_rxbit = 0;
			// and carry on with life
		}
		
	}
	else
	{
		
		// we're receiving something
		// inc the tick count
		curr_port->uart_rxtick++;
		if (curr_port->uart_rxtick == 4)
		{
			// we only sample when the tick = 0
			curr_port->uart_rxtick = 0;
			
			// what we do depends on the bit count
			// 0 = start,
			// 1-8 = data
			// 9 = stop
			if (curr_port->uart_rxbit == 0)
			{
				
				// start bit
				// it had better be 0 or it was a line glitch
				if ((PIND & _BV(curr_port->uartrx)) == 0)
				{
					// it's a real start bit (probably) so deal with it
					// next bit will be data
					curr_port->uart_rxbit ++;
					curr_port->uart_rxd = 0;
				}
				else
				{
					// bad start bit, flag us back as not busy
					curr_port->uart_status |= ~(_BV(rxbusy));
				}
			}
			else
			{
				if (curr_port->uart_rxbit < 9)
				{
					
					// data bit
					// if the data bit is a one, we add the bit value to the rxd value
					
					if (PIND & _BV(curr_port->uartrx))
					{
						curr_port->uart_rxd += uart_bitpattern[curr_port->uart_rxbit];
						
					}
					curr_port->uart_rxbit ++;
				}
				else
				{
					if (curr_port->uart_rxbit == 9)
					{
						// stop bit
						// we're going to assume it's a valid bit, though we could check for
						// framing error here, and simply use this bit to wait for the first
						// stop bit period
						curr_port->uart_rxbit++;
					}
					else
					{
						// finished, ready to start again
						curr_port->uart_status &= ~(_BV(rxbusy));
						// store the data into the buffer
						curr_port->buffer2[curr_port->next_write_2++] = curr_port->uart_rxd;
						if(curr_port->uart_rxd == 0xF0){
							
						}
						if (curr_port->next_write_2 == BUFFERSIZE)
						curr_port->next_write_2 = 0;
						
					}
				}
			}
		}
	}
}
	

	
	
//will be executed multiple times during startup, but who cares.

void uart_init (uart_port *curr_port)
{
	
	
	curr_port->next_write_2 = 0;	// set up the buffer pointers
	curr_port->next_read_2 = 0;
	
	curr_port->yeah = 0;			//set up debug char
	
	// we wake up the timer, preset the clock and uart variables, and enable the ocr1a interrupt

	
	TCCR1B = 0;					//resetting TCCR1B.
	TCCR1B |= _BV(CS10);		//clock enabled, prescaling = 1
	TCCR1B |= _BV(WGM12);		// ctc mode is enabled
	TCCR1A = 0;					//nothing special is used.
	
	OCR1AH = 0x03;				//for four connections at the same time: has to be D0
	OCR1AL = 0x41;				//The compare A value is set, right now it is the value it used to be for only one port, has to be four times higher.
		
	OCR1AH = 0x00;
	OCR1AL = 0xD0;
	
	//OCR1AH = 0x01;				//bullshit numbers
	//OCR1AL = 0x59;
		
		
	
	TIMSK1 |= _BV(OCIE1A);		//Timer/Counter Compare Match is enabled.
	
	
	
	curr_port->uart_status = 0;		// nothing happening either tx or rx
	
	//Here, the ports are set and the pullups configured. This implementation worked with one port, maybe changes have to be made so that we only edit "our" pin as a initializing port.
	
	
	DDRD = _BV(curr_port->uarttx);		// and set the input and output pins
	
	PORTD |= _BV(curr_port->uarttx);		// we're not using port d for anything else and
	
	PORTD |= _BV(curr_port->uartrx);	// the usart overrides the pin directions anyway, here the pull up resistor on the rx port is enabled.

}

void uart_out_char (char ch, uart_port *curr_port)
{
	// output the character to the software serial port, waiting until the output
	// buffer is available
	
	while ((curr_port->uart_status & _BV(txbusy)) == 1);		// wait...
	{
		// once the system isn't busy...
		// load the register
		curr_port->uart_txd = ch;
		
		// and tell it there's something to send
		curr_port->uart_status |= _BV(txbusy);
	}
}

void uart_print (char * text)
{
	// print a string of characters to the soft uart
	//for (int q=0; q<strlen(text); q++)
	//ser2_out(text[q]);
}

//checks if a char is waiting to be read.
char uart_ischar (uart_port *curr_port)
{
	// returns true if there is a character available from the soft uart
	// false if there is nothing waiting
	// call ser2_in after this to retrieve the data
	
	if (curr_port->next_read_2 != curr_port->next_write_2)
	return 1;
	else
	return 0;
}


//reads this char. Argument in both methods: a port struct, this struct contains the buffer object where the char is read from (if it exists).
char uart_get_char (uart_port *curr_port)
{
	// return a character from the soft uart
	// do *not* call without first testing with ser2_ischar as it modifies the
	// next_read pointer
	
	char ch;
	ch = curr_port->buffer2[curr_port->next_read_2++];
	if (curr_port->next_read_2 == BUFFERSIZE)
	curr_port->next_read_2 = 0;
	return ch;
}

