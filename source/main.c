/***********************************************************************
 * @file      main.c
 * @version   1.0
 * @brief     This file calls the function firstly the circular buffer and the test cases for circular buffer
 *				Secondly this files calls the timer function
 *				Thirdly it calls the process logger function
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
 * @resources Took reference from Prof's notes and the lecture
 *             Collaborated with Ishaan and Bhargav for understanding the concept.

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


int main(void)
{
	sysclock_init(); //initialization of sysclock
	Init_UART0(38400);

	test_cbfifo(); //circular buffer test file

	printf("***************** PES Assignment 6 ****************\r\n");
	printf("Type 'author' to get the Author' Name\r\n");
	printf("Type dump (start address) (stop address) value of address between 0 and 640\r\n");
	printf("Type help to get the help menu\r\n");

	while(1)
	{
		putchar('?');
		putchar(' ');
		process_logger();
	}
	return 0;
}
