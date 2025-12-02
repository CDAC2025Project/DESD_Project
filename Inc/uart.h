/*
 * uart.h
 *
 *  Created on: Sep 18, 2025
 *      Author: Sunbeam
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

// USART2 - Tx (PA.2) Rx (PA.3)
// OVER8 = 0
#define BRR_9600	0x683
#define BRR_38400	0x1A1
#define BRR_115200	0x8B

void UartInit(uint32_t baud);
void UartPutch(int ch);
void UartPuts(char str[]);
int UartGetch(void);
void UartGets(char str[]);

#endif /* UART_H_ */
