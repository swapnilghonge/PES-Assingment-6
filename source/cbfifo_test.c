/***********************************************************************
 * @file      cbfifo_test.c
 * @version   1.0
 * @brief     This file contains the function circular buffer FIFO
 *
 * @author    Swapnil Ghonge swapnil.ghonge@colorado.edu
 * @date       April 1, 2022
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
*@brief Function contains test cases , by doing
*  several enqueue and dequeue operations.
* Test cases include checks on capacity, length, max_size of buffer.
* Assert function is used  to validate the output
*/

void test_cbfifo()
{
	char *str ="University of Colorado Boulder, #Ralphie #SKOBUFFS!!";

	//temporary buffer used for dequeue
	char cbuf[1024];

	//initialization of Tx buffer for test as multiple instance
	circularbuffer_t Tx;
	cbfifo_init(&Tx);

	assert(cbfifo_enqueue(&Tx, str, 5) == 5);
	assert(cbfifo_dequeue(&Tx,cbuf ,5) == 5);
	assert(cbfifo_length(&Tx)==0);

	//enqueue 5 element and then dequeue
	    assert(cbfifo_enqueue(&Tx,str, 5) == 5);
	    assert(cbfifo_dequeue(&Tx,cbuf, 5) == 5);
	    assert(cbfifo_length(&Tx)==0);
	    //enqueue 10 element and then dequeue 5 and check length
	    assert(cbfifo_enqueue(&Tx,str, 10) == 10);
	    assert(cbfifo_dequeue(&Tx,cbuf, 5) == 5);
	    assert(cbfifo_length(&Tx)==5);
	    assert(cbfifo_dequeue(&Tx,cbuf, 5) == 5);
	    assert(cbfifo_length(&Tx)==0);
	    //enqueue 10 element and then dequeue 5 and check length
	    assert(cbfifo_enqueue(&Tx,str, 5) == 5);
	    assert(cbfifo_dequeue(&Tx,cbuf, 5) == 5);
	    assert(cbfifo_length(&Tx)==0);

	    //enqueue 128 elements and dequeue 123
	    assert(cbfifo_enqueue(&Tx,str, 128) == 128);
	    assert(cbfifo_dequeue(&Tx,cbuf, 5) == 5);
	    assert(cbfifo_length(&Tx)==123);
	    assert(cbfifo_dequeue(&Tx, cbuf, 123) == 123);

	    //dequeue more elements then required
	     assert(cbfifo_enqueue(&Tx,str, 128) == 128);
	    assert(cbfifo_dequeue(&Tx,cbuf, 129) != 129);
	    assert(cbfifo_length(&Tx)!=128);
	    //assert(cbfifo_capacity()==128);


	printf("%s: Congratulations!! you passed all the test cases passed\n\n\r",__FUNCTION__);
}

