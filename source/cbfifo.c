/***********************************************************************
 * @file      cbfifo.c
 * @version   1.0
 * @brief     This file contains the function circular buffer FIFO
 *
 * @author    Swapnil Ghonge swapnil.ghonge@colorado.edu
 * @date      April 1, 2022
 *
 * @institution University of Colorado Boulder (UCB)
 * @course      ECEN 5813: Principles of Embedded Software
 * @instructor  Howdy Pierce
 *
 *
 *
 * @resources https://www.youtube.com/watch?v=uvD9_Wdtjtw&ab_channel=TechVedas.learn
 *             https://www.techiedelight.com/circular-queue-implementation-c/
 *             https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
 *             https://www.programiz.com/dsa/circular-queue
 *             Collaborated with Ishaan and Bhargav for understanding the conept.

 * @copyright  All rights reserved. Distribution allowed only for the
 * use of assignment grading. Use of code excerpts allowed at the
 * discretion of author. Contact for permission.
 */

#include <MKL25Z4.H>
#include <stdio.h>
#include "core_cm0plus.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "sysclock.h"
#include "cbfifo.h"
#include "cbfifo_test.h"
#include "uart.h"
#include "hexdump.h"
#include "command.h"

/*
 * iNITIALIZES THE Circular Buffer
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   None
 */


void cbfifo_init(circularbuffer_t *cb)
{
	for(int i=0;i<MAX_CAPACITY;i++)
	{
		cb->data[i]='_';
	}
	cb->tail=0;
	cb->head=0;
	cb->length=0;
	cb->buffer_capacity=MAX_CAPACITY;
}

/*
 * Returns if the buffer is full
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Returns the buffer is full
 */
uint8_t ifFull(circularbuffer_t *cb)
{
	if(cb->length==MAX_CAPACITY)
		return 1;
	else
		return 0;
}

/*
 * Returns if the buffer is empty
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Returns the buffer is empty
 */

uint8_t ifEmpty(circularbuffer_t *cb)
{
	if(cb->length==0)
		return 1;
	else
		return 0;
}


size_t cbfifo_capacity(circularbuffer_t *cb)
{
	return (cb->buffer_capacity);
}
/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(circularbuffer_t *cb)
{
	return (cb->length);
}
/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns (size_t) -1.
 */

size_t cbfifo_enqueue(circularbuffer_t *cb,void *buffer,size_t nbyte)
{
	size_t flag=0;
	__disable_irq();
	if(ifFull(cb))
	{
		__enable_irq();
		return 0;
	}
	else
	{
		while((cb->length!=cb->buffer_capacity) && flag<nbyte)
		{
			(cb->data[cb->head])=*(uint8_t *)(buffer++);
			cb->length++;
			cb->head=(cb->head+1)%cb->buffer_capacity;
			flag++;
		}
	}
	__enable_irq();
	return flag;
}

/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(circularbuffer_t *cb,void *buffer,size_t nbyte)
{
	size_t flag=0;

	__disable_irq();
	if(ifEmpty(cb))
	{
		__enable_irq();
		return 0;
	}
	else
	{
		while((cb->length!=0) && flag<nbyte)
		{
			*(uint8_t *)(buffer++)=cb->data[cb->tail];
			cb->data[cb->tail]='_';
			cb->length--;
			cb->tail=(cb->tail+1)%cb->buffer_capacity;
			flag++;
		}
	}
	__enable_irq();
	return flag;
}
