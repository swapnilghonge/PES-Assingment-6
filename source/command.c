/***********************************************************************
 * @file      command.c
 * @version   1.0
 * @brief     This file contains two function process command and process log
				process log function keeps the log of the key when it is pressed on the keyboard
				process command function process the command from the log function.
				This function keeps the value of flag and calls UART IRQhandler
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


/*@brief: This function keeps the log of which value is pressed on the Keyboard
 *			this function checks enter key, backspace key and other input character
 *
 *@output: None
 */
void process_logger()
{
	char temp_buffer[700];
	char* ptr = &temp_buffer[0]; // initializing the buffer with pointer, pointing to first value


	char key_press; //key press denotes when button is pressed
	while(key_press!=0x0D) // when enter key is pressed/detected
	{
		while (ifEmpty(&Rxbuf)); // checking if the buffer is empty or not
		cbfifo_dequeue(&Rxbuf, &key_press, 1);
		putchar(key_press); //echo the characters on UART
		if(key_press!=0x0D || key_press!=0x0A) // if the key pressed is not ENter button
		{
			if(key_press!=0x08) // copy the value to temp buffer if backspace is not pressed
			{
				*ptr++ = key_press;
			}
			else
			{
				printf(" \b"); // Pritf statement when buffer is pressed
				ptr--; // decrese the pointer value in the buffer.
			}
		 }
		if(key_press=='\r') //when enter key is pressed
		{
			key_press=0x0A;
			printf("\r\n");// GO to  next line
			break;
		}
	}
	*ptr='\0';

	process_command(temp_buffer); // calling process_command function

	ptr =& temp_buffer[0]; // Now the pointer is pointing to the first value of buffer.
}
/*
 * Use of Function pointer
 */
typedef void (*command_handler)(const char* command); //Using function pointer in order to handle the command.
/*
 * structure for command lookup table
 * with front pointer and function name
 */
typedef struct
{
	const char *front;
	command_handler function_name;
} command_lut;

/**
 * @brief: function prints string for hexdump
 *
 * @param: takes the command from the Lookuptable
 * @return: prints hexdump of the given address and nbytes
 */
static void hex(const char* command)
{
	uint32_t address;
	size_t bytes;
	sscanf(command,"dump %x %i",&address,&bytes); // scan the address and number of bytes from command
	Hexdump((void*)address, bytes);
}

/**
 * @brief: This function prints Author's name on UART when function is called
 *
 * @param: takes command according to look up table
 * @return: prints "Swapnil Ghonge" as autor
 */
static void author_name(const char* command)
{
	printf("Swapnil Ghonge\r\n");
}


/**
 * @brief: function prints "Command Unknown"
 *
 *
 * @param: takes command from the lookup table
 *
 * @return: prints "Command unknown"
 */
static void invalid(const char* command)
{
	printf("Command Unknown, type help for help menu: %s\r\n", command);
}

/**
 * @brief: prints help menu when string 'help' is typed
 *
 * @param: takes command from the lookup table
 * @return: prints the help menu
 */
static void help(const char* command)
{
	printf("Type 'author' to prints the author's name.\r\n");
	printf("Type 'dump' (start address) (ending address) values between 0-640 to print hexdump.\r\n");
}


static const command_lut commands[] =
{
		{"author", author_name},
		{"dump", hex},
		{"help", help},
		{"", invalid}
};

/**
 * @brief: function processes the string
 *
 *
 * @param: string is passed as coming from logger function.
 * @return: NULL
 */
void process_command(char *input)
{
	char process_buff[2000];
	memset(process_buff,0,2000); // set all value to 0 in the temp buffer
	char* ptr1 = &process_buff[0]; // prt1-> pointer to first value of thebuffer
	char* ptr2 = &input[0]; // ptr2-> pinter to first value of input whcih is received for processing.
	bool flag = true; // initialing the token
	char front[50];

	while(*ptr2 != '\0')
	{
		if(flag==true && isspace(*ptr2)) // if flag is true and space allocated by first string
		{
			ptr2++;
		}
		else if(flag==false && isspace(*ptr2)) // if flag is false and space allocated by first string
		{
			*ptr1++ = ' '; // if true increment the pointer  to next element of the temp buff
			flag=true; // set it to be true
		}
		else
		{
			*ptr1++ = tolower(*ptr2++); // conver input string to lower case.
			flag = false; // flag is set to false
		}
	}
	*ptr2 = '\0'; // last elememnt is the NULL character
	sscanf(process_buff,"%s",front);

	int num = sizeof(commands)/sizeof(command_lut); // calculates the size of lookup table (4 in this case)

	int i=0;
	while(i<num-1)
	{
		if(strcmp(front, commands[i].front)==0) // compare the pointer front to the command from the lookup table
		{
			commands[i].function_name(process_buff); // Calling the UART handler
			ptr1 = &process_buff[0]; //pointer, pointing to first value of the process command buffer
			ptr2 = &input[0]; // pointer pointing to first value of the string
			return;
		}
		i++;
	}
	invalid(process_buff); // calling the invalid input function
	ptr1 = &process_buff[0]; //pointer, pointing to first value of the process command buffer
	ptr2 = &input[0]; // pointer pointing to first value of the string
	return;
}
