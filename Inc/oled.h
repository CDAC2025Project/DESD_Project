 #ifndef OLED_H_
#define OLED_H_

#include "stm32f4xx.h"
#include "spi.h"
#include "uart.h"
#include "font6x8.h"

// Pin macros (PB0 = DC, PB1 = RES)
#define OLED_DC_LOW()   (GPIOB->ODR &= ~(1U<<0))
#define OLED_DC_HIGH()  (GPIOB->ODR |=  (1U<<0))

#define OLED_RES_LOW()  (GPIOB->ODR &= ~(1U<<1))
#define OLED_RES_HIGH() (GPIOB->ODR |=  (1U<<1))

// API
void OLED_GPIO_Init(void);
void OLED_SendCommand(uint8_t cmd);
void OLED_SendData(uint8_t data);
void OLED_SetCursor(uint8_t page, uint8_t column);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_PrintChar(char c);
void OLED_PrintString(const char *str);

#endif /* OLED_H_ */
