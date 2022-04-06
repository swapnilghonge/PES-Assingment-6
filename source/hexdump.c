


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
 * @brief: This function takes the integer input and return the equivalent hex output
 *	@return: Hexadecimal value
 */

char Convert_to_hex(int n)
{
	if (n>=0 && n<10)
		return '0' + n;
	else if (n>=10 && n<16)
		return 'A' + n - 10;
}

/*
 * @brief: This function takes input as address and number of bytes  and prints the hex dump
 *	@return: NULL
 *	@refernce: Took reference from class notes and Prof's Howdy discussion on Hex dump
 */

void Hexdump(void *addr, size_t nbytes)
{
	  uint8_t *address = (uint8_t*)addr;
	  uint8_t *maximum = (uint8_t*)addr + nbytes;

	  if(nbytes>640)
	  {
		  printf("Enter value of length between 0-640\r\n");
		  return;
	  }

	  while(address<maximum)
	  {
		  putchar(Convert_to_hex(((uint32_t)(address) & 0xF0000000)>>28));
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x0F000000)>>24));
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x00F00000)>>20));
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x000F0000)>>16));
		  putchar('_');
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x0000F000)>>12));
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x00000F00)>>8));
		  putchar(Convert_to_hex(((uint32_t)(address) & 0x000000F0)>>4));
		  putchar(Convert_to_hex((uint32_t)(address) & 0x0000000F));
		  putchar(' ');
		  putchar(' ');

		  int k=0;
		  while((k<16) && ((address+k) < maximum))
		  {
			  putchar(Convert_to_hex(address[k]>>4));
			  putchar(Convert_to_hex(address[k] & 0x0F));
			  putchar(' ');
			  k++;
		  }
		  address=address+16;
		  putchar('\r');
		  putchar('\n');
	  }
}
