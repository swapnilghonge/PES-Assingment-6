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

#define OVERSAMPLE_RATE (16)
#define SYS_CLOCK (24E6) // system clock of 24MHz

circularbuffer_t Txbuf, Rxbuf;

/**
 * @brief: function configures the UART by configuring registers
 *
 * @param: takes baud rate as input ie as per assignment 38400
 * @return: NULL
 */
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);

	// Setting baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(OVERSAMPLE_RATE-1);

	/* Disable RX Input active edge interrupt and LIN Break Detect Interrupt, select one stop bit*/
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(1) | UART0_BDH_LBKDIE(0);

	/*Don't enable Loopback mode, use 8 data bit mode*/
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);

	/*Don't invert transmit data , do enable interrupts for errors*/
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// LSB bit is transmitted first, receive data not inverted
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// initialize Txbuf and Rxbuf buffers
	cbfifo_init(&Txbuf);
	cbfifo_init(&Rxbuf);

	//UART interrupt has been given second priority
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts
	UART0->C2 |= UART_C2_RIE(1);

	//Receiver and Transmitter are enabled
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);
}

/**
 * @brief: Whenever UART interrupt is called this function goes into Interrupt handler
 *
 * @param: NULL
 * @return: NULL
 */
void UART0_IRQHandler(void)
{
	uint8_t ch;
	if(UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			ch = UART0->D;
	}


	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		ch = UART0->D;
		cbfifo_enqueue(&Rxbuf,&ch,1);
	}

	if((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK))
	{
		if(cbfifo_dequeue(&Txbuf,&ch,1)==1)
		{
			UART0->D = ch;
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK; //clear the TIE mask
		}
	}
}

/**
 * @brief: Writes the value according to the input
 *
 * @param: NULL
 * @return: character
 */
int __sys_write(int handle, char* buf, int size)
{
	if(buf==NULL)
	{
		return -1;
	}
	while(ifFull(&Txbuf));
	if(cbfifo_enqueue(&Txbuf, buf, size) != size)
	{
		return -1;
	}
	if(!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}
	return 0;
}

/**
 * @brief:Read the value according to the input
 *
 * @param: NULL
 * @return: character
 */

int __sys_readc(void)
{
	char ch;
	while(ifEmpty(&Rxbuf));
	if(cbfifo_dequeue(&Rxbuf, &ch, 1) != 1)
	{
		return -1;
	}
	return ch;
}
