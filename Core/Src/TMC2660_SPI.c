/*
 * TMC2660_SPI.c
 *
 *  Created on: Feb 19, 2025
 *      Author: tilen
 */


#include "main.h"
#include "TMC2660.h"


extern SPI_HandleTypeDef hspi1;


static uint8_t continuousModeEnabled[7] = {0};

uint8_t tmc2660_getcontinuousModeEnable(uint8_t icID) {
    if(icID >= 7) return 0;  // Safety check for your 7 drivers
    return continuousModeEnabled[icID];
}

void tmc2660_setContinuousMode(uint8_t icID, uint8_t enable) {
    if(icID >= 7) return;
    continuousModeEnabled[icID] = enable;
}



void tmc2660_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength) {
    // 1. Map icID to GPIO CS pins (example for 7 drivers)
    static const GPIO_TypeDef* csPorts[] = {DRIVER1_GPIO_Port, DRIVER2_GPIO_Port, DRIVER3_GPIO_Port, DRIVER4_GPIO_Port,
    										DRIVER5_GPIO_Port, DRIVER6_GPIO_Port, DRIVER7_GPIO_Port};
    static const uint16_t csPins[] = {DRIVER1_Pin, DRIVER2_Pin, DRIVER3_Pin, DRIVER4_Pin, DRIVER5_Pin, DRIVER6_Pin, DRIVER7_Pin};

    // 2. Assert CS
    HAL_GPIO_WritePin((GPIO_TypeDef*)csPorts[icID], csPins[icID], GPIO_PIN_RESET);

    // 3. SPI Transfer (STM32 HAL example)
    HAL_SPI_TransmitReceive(&hspi1, data, data, dataLength, HAL_MAX_DELAY);

    // 4. Release CS
    HAL_GPIO_WritePin((GPIO_TypeDef*)csPorts[icID], csPins[icID], GPIO_PIN_SET);
}
