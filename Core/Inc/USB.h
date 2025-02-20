/*
 * USB.h
 *
 *  Created on: Feb 18, 2025
 *      Author: tilen
 */

#ifndef INC_USB_H_
#define INC_USB_H_

#include "main.h"
#include "TMC2660.h"


#define MAX_DRIVERS 7
#define RESPONSE_BUFFER_SIZE 128


#define CMD_READ_REG  "READ_REG"
#define CMD_WRITE_REG "WRITE_REG"
#define CMD_READ_ALL  "READ_ALL"







void parse_command(char* command);
extern char response_buffer[RESPONSE_BUFFER_SIZE];

#endif /* INC_USB_H_ */
