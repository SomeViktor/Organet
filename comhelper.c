/*
 * OrganetP.c
 *
 * Created: 24.02.2015 10:07:45
 *  Author: Viktor
 */ 

#include "comhelper.h"




void com_helper_init(){
	
	/************************************************************************/
	/* This method initializes all four connection lines.                                                                     */
	/************************************************************************/
	
	initQueue(&incoming_port[0]);
	initQueue(&incoming_port[1]);
	initQueue(&incoming_port[2]);
	initQueue(&incoming_port[3]);
	
	
	port_rotator = 1;				//this couldnt be done in the header file
	
	port[0].uartrx = PORTONERX;	//load the pins for each line.
	port[0].uarttx = PORTONETX;
	port[0].port_number = 1;
	
	port[1].uartrx = PORTTWORX;
	port[1].uarttx = PORTTWOTX;
	port[1].port_number = 2;
	
	
	port[2].uartrx = PORTTHREERX;
	port[2].uarttx = PORTTHREETX;
	port[2].port_number = 3;
 	
	
	port[3].uartrx = PORTFOURRX;		//right now, this pin is still wrong.
	port[3].uarttx = PORTFOURTX; 
	port[3].port_number = 4;
	
	for(int i = 0;i<4;i++){
		switch (i){			//init each com line
			case 1:
				
				uart_init(&port[0]);
								
				break; 
			case 2:
				uart_init(&port[1]);
				break;
			case 3:
				uart_init(&port[2]);
				break;
			case 4:
				uart_init(&port[3]);
				break;
		}
	}
	
	
}

char com_helper_send_message(message msg,comhelper cmhlpr)
{
	
	
	return 0;
}

char com_helper_is_msg(queue *q)
{
	
	return 0;
}


char com_helper_parse_msg(queue *q)
{
	
	
}