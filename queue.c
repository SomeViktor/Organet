/*
 * CFile1.c
 *
 * Created: 22.05.2015 14:05:19
 *  Author: Viktor
 */ 

#include "queue.h"

// Declare these as static so no code outside of this source
// can access them.


//--------------------------------------------
// Function: InitQueue()
// Purpose: Initialize queue to empty.
// Returns: void
//--------------------------------------------
void initQueue(queue *q)
{
	q->head = q->tail = -1;
}

//--------------------------------------------
// Function: ClearQueue()
// Purpose: Remove all items from the queue
// Returns: void
//--------------------------------------------
void clearQueue(queue *q)
{
	q->head = q->tail = -1; // Reset indices to start over
}

//--------------------------------------------
// Function: Enqueue()
// Purpose: Enqueue an item into the queue.
// Returns: TRUE if enqueue was successful
//		or FALSE if the enqueue failed.
// Note: We let head and tail continuing
//		increasing and use [head % MAX_SIZE]
//		and [tail % MAX_SIZE] to get the real
//		indices.  This automatically handles
//		wrap-around when the end of the array
//		is reached.
//--------------------------------------------
char enqueue(queue *q,char ch)
{
	// Check to see if the Queue is full
	if(isFull(q)) return 0;

	// Increment tail index
	q->tail++;
	// Add the item to the Queue
	q->theQueue[q->tail % MAX_SIZE] = ch;
	return 1;
}

//--------------------------------------------
// Function: Dequeue()
// Purpose: Dequeue an item from the Queue.
// Returns: TRUE if dequeue was successful
//		or FALSE if the dequeue failed.
//--------------------------------------------
char dequeue(queue *q)
{
	char ch;

	// Check for empty Queue
	if(isEmpty(q)) return '\0';  // Return null character if queue is empty
	else
	{
		q->head++;
		ch = q->theQueue[q->head % MAX_SIZE];		// Get character to return
		return ch;				// Return popped character
	}
}

//--------------------------------------------
// Function: isEmpty()
// Purpose: Return true if the queue is empty
// Returns: TRUE if empty, otherwise FALSE
// Note: C has no boolean data type so we use
//	the defined int values for TRUE and FALSE
//	instead.
//--------------------------------------------
char isEmpty(queue *q)
{
	return (q->head == q->tail);
}

//--------------------------------------------
// Function: isFull()
// Purpose: Return true if the queue is full.
// Returns: TRUE if full, otherwise FALSE
// Note: C has no boolean data type so we use
//	the defined int values for TRUE and FALSE
//	instead.
//--------------------------------------------
char isFull(queue *q)
{
	// Queue is full if tail has wrapped around
	//	to location of the head.  See note in
	//	Enqueue() function.
	return ((q->tail - MAX_SIZE) == q->head);
}