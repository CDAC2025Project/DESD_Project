 #ifndef DHT11_H_
#define DHT11_H_

#include "stm32f4xx.h"
#include <stdint.h>

/*
 * ----------- User configurable section -------------
 * Change the port/pin to match your hardware connection
 */
#define DHT11_PORT      GPIOC          // e.g., GPIOA, GPIOB, GPIOC, etc.
#define DHT11_PIN       7              // Pin number (0..15)
#define DHT11_CLK_EN()  (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)  // Port clock enable
/* -------------------------------------------------- */

/********** Public functions **********/

/**
 * @brief Initialize DHT11 data pin (input with pull-up)
 */
void DHT11_Init(void);

/**
 * @brief Read temperature and humidity from DHT11
 * @param temperature Pointer to uint8_t for temperature (°C)
 * @param humidity    Pointer to uint8_t for humidity (%)
 * @return 0 = OK, 1 = No response, 2 = Checksum error
 */
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);

#endif /* DHT11_H_ */
