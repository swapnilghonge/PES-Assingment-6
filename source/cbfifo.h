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

#ifndef CBFIFO_H_
#define CBFIFO_H_

#define MAX_CAPACITY (256)


typedef struct
{
	int tail;
	int head;
	size_t buffer_capacity;
	size_t length;
	uint8_t data[MAX_CAPACITY];
}circularbuffer_t;

/*
 * iNITIALIZES THE Circular Buffer
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   None
 */
void cbfifo_init(circularbuffer_t *cb);
/*
 * Returns if the buffer is full
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Returns the buffer is full
 */
uint8_t ifFull(circularbuffer_t *cb);
/*
 * Returns if the buffer is empty
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Returns the buffer is empty
 */

uint8_t ifEmpty(circularbuffer_t *cb);
/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */

size_t cbfifo_length(circularbuffer_t *cb);
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
size_t cbfifo_enqueue(circularbuffer_t *cb, void *buffer, size_t nbyte);

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
size_t cbfifo_dequeue(circularbuffer_t *cb, void *buffer, size_t nbyte);

#endif /* CBFIFO_H_ */
