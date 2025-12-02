 /*
 * spi.h
 *
 *  Created on: Nov 2025
 *  Author: Sagar
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"

#define BV(x)   (1U << (x))

// -----------------------------
//  SPI Functions
// -----------------------------
void SpiInit(void);

void SpiCSEnable(void);
void SpiCSDisable(void);

uint16_t SpiTransfer(uint16_t data);
void     SpiTransmit(uint8_t data);
uint16_t SpiReceive(void);

void SpiWrite(uint8_t internalAddr, uint8_t data[], uint8_t size);
void SpiRead(uint8_t internalAddr, uint8_t data[], uint8_t size);

#endif /* SPI_H_ */
