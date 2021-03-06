#ifndef COMHELPER_H_
#define COMHELPER_H_
#include "SoftUart2.h"
#include "queue.h"


/*##############################################################
This is the comhelper. It manages the incoming bytes from the uart_port and returns them as messages to the organet client.

So somehow a layer between the actual software and the uart library. Used fot controlling the uart library.

Todo: 

	-port four pins have to be changed

	-every port should get its own debug led.
	
	-at uart init there is a comment: not using port d for anything else. this is not true anymore with multiple com lines.
	
	-rewrite the print char method.
	
	-the timer interrupt has to be 4 times faster.
################################################################
*/

//here, as a macro the transmission (tx) and receiving (rx) pins are set as macros.
#define PORTONERX PIND2;
#define PORTONETX PIND3;

#define PORTTWORX PIND4;
#define PORTTWOTX PIND5;

#define PORTTHREERX PIND6;
#define PORTTHREETX PIND7;

#define PORTFOURRX PIND0; //probably not a good idea. has to be changed
#define PORTFOURTX PIND1;

//not yet used. maybe not needed after all.
typedef struct{						
	
	uint8_t txpin;

	uint8_t rxpin;

	unsigned char name;

	unsigned char incoming_chars_buffer[32];

	unsigned char outgoin_chars_buffer[32];
} comhelper;
//this as well is not needed. maybe not after all.

//comhelper comhelpers[4];



//a very basic message, with receipient and sender and a short routestack to save the past relay stations.
typedef struct{			
	char receipient;
	char sender;
	unsigned char routestack[5];
}message;

queue incoming_port[4];




void com_helper_init(); //initializes all four connection ports, with the pins set on the macros.

char com_helper_send_message(message,comhelper); //not implemented yet.

char com_helper_is_msg(queue *q); //check if chars in buffer represent message

char com_helper_parse_msg(queue *q);


#endif  /* COMHELPER_H_ */ 
