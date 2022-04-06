/***********************************************************************
 * @file      command.h
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

#ifndef COMMAND_H_
#define COMMAND_H_


/**
 * @brief: function processes the string
 *
 *
 * @param: string is passed as coming from logger function.
 * @return: NULL
 */

void process_command(char *input);
/*@brief: This function keeps the log of which value is pressed on the Keyboard
 *			this function checks enter key, backspace key and other input character
 *
 *@output: None
 */
void process_logger();

#endif /* COMMAND_H_ */
