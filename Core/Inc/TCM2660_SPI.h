/*
 * TCM2660_SPI.h
 *
 *  Created on: Feb 19, 2025
 *      Author: tilen
 */

#ifndef INC_TCM2660_SPI_H_
#define INC_TCM2660_SPI_H_

void tmc2660_setContinuousMode(uint8_t icID, uint8_t enable);
void tmc2660_readWriteSPI(uint16_t icID, uint8_t *data, size_t dataLength);


#endif /* INC_TCM2660_SPI_H_ */
