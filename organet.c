/*
 * CFile1.c
 *
 * Created: 24.02.2015 23:32:03
 *  Author: Viktor
 */ 
#include "organet.h"
#include <util/delay.h>
int main(){ 
		
	com_helper_init(); //initializes all four ports by calling the uart_init method with each port struct as an argument.
	sei();
	
	while(1){
		
		uart_out_char(0xAA,&port[0]);
		_delay_ms(50);
		if(uart_ischar(&port[0])){			//checks if the is a char to be read on port 1
			//activate debug led.
			
			char x = uart_get_char(&port[0]);
			if(x == 0x04){
				port[0].yeah = 1;
				
			}
			
				
			
						
		}
		
		
	}
	
}

void check_for_new_chars(){
	if(uart_ischar(&port[0])){
		enqueue(&incoming_port[0], uart_get_char(&port[0]));
		
		if(com_helper_is_msg(&incoming_port[0])){
			com_helper_parse_msg(&incoming_port[0]);
		}
	}
	if(uart_ischar(&port[1])){
		enqueue(&incoming_port[1], uart_get_char(&port[1]));
		
		if(com_helper_is_msg(&incoming_port[1])){
			com_helper_parse_msg(&incoming_port[1]);
		}
	}
	if(uart_ischar(&port[2])){
		enqueue(&incoming_port[0], uart_get_char(&port[2]));
		
		if(com_helper_is_msg(&incoming_port[2])){
			com_helper_parse_msg(&incoming_port[2]);
		}
	}
	if(uart_ischar(&port[3])){
		enqueue(&incoming_port[0], uart_get_char(&port[3]));
		
		if(com_helper_is_msg(&incoming_port[3])){
			com_helper_parse_msg(&incoming_port[3]);
		}
	}
	  
}

void parse_message(){
	//check if message is for me
		//if nessesary send a response message
	
	//if not for me, send the message on the other 3 ports.
	
}

void send_message(){
	//on which ports?
	
	//add message to cue
	
}



void manage_transmissions(){
	//is uart currently sending a byte?
		//yes: return
		
		//no: is a message currently being sent?
			//yes: uart_send next byte of message
			
			//no: take next message from message cue, send first byte.
	
}