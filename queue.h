/*
 * IncFile1.h
 *
 * Created: 22.05.2015 14:05:09
 *  Author: Viktor
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

//#include <stdio.h>

#define MAX_SIZE    32        // Define maximum length of the queue
#include <stdint.h>
typedef struct{
	uint16_t head, tail;	// Declare global indices to head and tail of queue
	char theQueue[MAX_SIZE];	// The queue
}queue;

// List Function Prototypes
void initQueue(queue *q);             // Initialize the queue
void clearQueue(queue *q);            // Remove all items from the queue
char enqueue(queue *q,char ch);         // Enter an item in the queue
char dequeue(queue *q);               // Remove an item from the queue
char isEmpty(queue *q);                // Return true if queue is empty
char isFull(queue *q);                 // Return true if queue is full






#endif /* INCFILE1_H_ */