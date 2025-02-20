/*
 * TMC2660_Names.c
 *
 *  Created on: Feb 20, 2025
 *      Author: tilen
 */



#include "TMC2660_HW_Abstraction.h"

const char *tmc2660_getRegisterName(uint8_t address) {
    static const char *names[] = {
        TMC2660_REGISTER_NAME()
    };

    if(address >= sizeof(names)/sizeof(names[0]))
        return "UNKNOWN";

    return names[address] ? names[address] : "UNKNOWN";
}
