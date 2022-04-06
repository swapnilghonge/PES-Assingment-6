/***********************************************************************
 * @file      uart.c
 * @version   1.0
 * @brief     This file contains two function Init_UART0 and UART0_IRQHandler
 *				Init_UART0  this function initializes the UART0 by configuring the registers
 *				UART0_IRQHandler This function is callled whenever UART is called
 *				the priority of the UART is 2
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
 * @resources Took reference Alexander Dean's code and Prof's Lecture Notes.
 *             Collaborated with Ishaan and Bhargav for understanding the concept.

 * @copyright  All rights reserved. Distribution allowed only for the
 * use of assignment grading. Use of code excerpts allowed at the
 * discretion of author. Contact for permission.
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

/*
 * @brief: This function takes the integer input and return the equivalent hex output
 *	@return: Hexadecimal value
 */

char Convert_to_hex(int n);
/*
 * @brief: This function takes input as address and number of bytes  and prints the hex dump
 *	@return: NULL
 *	@refernce: Took reference from class notes and Prof's Howdy discussion on Hex dump
 */
void Hexdump(void *addr, size_t nbytes);

#endif /* HEXDUMP_H_ */
