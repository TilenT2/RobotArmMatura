/*
 * USB.c
 *
 *  Created on: Feb 18, 2025
 *      Author: tilen
 */


#include "USB.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char response_buffer[RESPONSE_BUFFER_SIZE];


static uint8_t get_register_address(const char* reg_name) {
    // Create a modified version for comparison
    char clean_name[20];
    size_t i;

    // Copy and convert to uppercase
    for(i = 0; reg_name[i] && i < sizeof(clean_name)-1; i++) {
        clean_name[i] = toupper(reg_name[i]);
    }
    clean_name[i] = '\0';

    // Trim trailing whitespace/control characters
    while(i > 0 && (clean_name[i-1] == '\r' || clean_name[i-1] == '\n' || isspace(clean_name[i-1]))) {
        clean_name[--i] = '\0';
    }

    // Now compare
    if(strcmp(clean_name, "DRVCTRL") == 0) return TMC2660_DRVCTRL;
    if(strcmp(clean_name, "CHOPCONF") == 0) return TMC2660_CHOPCONF;
    if(strcmp(clean_name, "SMARTEN") == 0) return TMC2660_SMARTEN;
    if(strcmp(clean_name, "SGCSCONF") == 0) return TMC2660_SGCSCONF;
    if(strcmp(clean_name, "DRVCONF") == 0) return TMC2660_DRVCONF;

    return 0xFF;
}





void handle_read_command(uint8_t driver_id, uint8_t reg_address) {
    if(driver_id >= MAX_DRIVERS) {
        snprintf(response_buffer, sizeof(response_buffer), "ERROR: Invalid driver ID %d\n", driver_id);
    }
    else if(reg_address == 0xFF) {
        snprintf(response_buffer, sizeof(response_buffer), "ERROR: Invalid register name\n");
    }
    else {
        uint32_t value = tmc2660_readRegister(driver_id, reg_address);
        snprintf(response_buffer, sizeof(response_buffer), "DRIVER%d %s: 0x%08lX\n",
                driver_id,
				tmc2660_getRegisterName(reg_address), // Implement this macro if needed
                value);
    }
    CDC_Transmit_FS((uint8_t*)response_buffer, strlen(response_buffer));
}




void handle_write_command(uint8_t driver_id, uint8_t reg_address, uint32_t value) {
    if(driver_id >= MAX_DRIVERS) {
        snprintf(response_buffer, sizeof(response_buffer), "ERROR: Invalid driver ID %d\n", driver_id);
    }
    else if(reg_address == 0xFF) {
        snprintf(response_buffer, sizeof(response_buffer), "ERROR: Invalid register name\n");
    }
    else {
        tmc2660_writeRegister(driver_id, reg_address, value);
        snprintf(response_buffer, sizeof(response_buffer), "ACK: DRIVER%d %s = 0x%08lX\n",
                driver_id,
				tmc2660_getRegisterName(reg_address),
                value);
    }
    CDC_Transmit_FS((uint8_t*)response_buffer, strlen(response_buffer));
}






void parse_command(char* command) {
    char* token = strtok(command, " ");

    if(strcmp(token, CMD_READ_REG) == 0) {
        char* driver_str = strtok(NULL, " ");
        char* reg_str = strtok(NULL, " ");

        if(!driver_str || !reg_str) {
            CDC_Transmit_FS((uint8_t*)"ERROR: Usage: READ_REG [driver] [register]\n", 43);
            return;
        }

        handle_read_command(atoi(driver_str), get_register_address(reg_str));
    }
    else if(strcmp(token, CMD_WRITE_REG) == 0) {
        char* driver_str = strtok(NULL, " ");
        char* reg_str = strtok(NULL, " ");
        char* value_str = strtok(NULL, " ");

        if(!driver_str || !reg_str || !value_str) {
            CDC_Transmit_FS((uint8_t*)"ERROR: Usage: WRITE_REG [driver] [register] [value]\n", 51);
            return;
        }

        handle_write_command(atoi(driver_str),
                            get_register_address(reg_str),
                            strtoul(value_str, NULL, 16));
    }
    else if(strcmp(token, CMD_READ_ALL) == 0) {
        // Implement batch read of all important registers
    }
    else {
        CDC_Transmit_FS((uint8_t*)"ERROR: Unknown command\n", 22);
    }
}
