/*
 * uart.c
 *
 *  Created on: Sep 18, 2025
 *      Author: Sunbeam
 */

#include "uart.h"

void UartInit(uint32_t baud) {
	// GPIO config (Tx (PA.2) Rx (PA.3)) - clock en, set alt fn, pupd, ...
	// enable clock of gpioa
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set pupd regr -- no pullup/pulldown
	GPIOA->PUPDR &= ~(BV(4)|BV(5)|BV(6)|BV(7));
	// set mode regr - alt fn mode (10)
	GPIOA->MODER &= ~(BV(4)|BV(6));
	GPIOA->MODER |= (BV(5)|BV(7));
	// set alt fn AF7 for USART2
	GPIOA->AFR[0] &= ~(BV(15) | BV(11));
	GPIOA->AFR[0] |= (BV(14)|BV(13)|BV(12)|BV(10)|BV(9)|BV(8));
	// Uart config - clock en, baud rate, enable uart tx & rx, wordlen, parity, stop bits ...
	// enable uart clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// uart enable
	USART2->CR1 = USART_CR1_UE;
	// set the baud rate
	if(baud == 9600)
		USART2->BRR = BRR_9600;
	else if(baud == 38400)
		USART2->BRR = BRR_38400;
	else if(baud == 115200)
		USART2->BRR = BRR_115200;
	// enable uart, tx, rx
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void UartPutch(int ch) {
	// write the char in DR regr
	USART2->DR = ch;
	// wait for TXE bit
	while((USART2->SR & USART_SR_TXE) == 0)
		;
}

void UartPuts(char str[]) {
	int i;
	for(i=0; str[i]!='\0'; i++)
		UartPutch(str[i]);
}

int UartGetch(void) {
	// wait for data to be available in Rx regr (DR) -- RXNE bit
	while((USART2->SR & USART_SR_RXNE) == 0)
		;
	// read data from Rx regr
	return (int)USART2->DR;
}

void UartGets(char str[]) {
	int i = 0;
	char ch;
	do {
		ch = UartGetch();
		str[i] = ch;
		i++;
	}while(ch != '\r');
	str[i] = '\n';
	i++;
	str[i] = '\0';
}



