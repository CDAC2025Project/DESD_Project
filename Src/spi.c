 #include "spi.h"

void SpiInit(void)
{
    // ---- Enable Clocks ----
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // PA5 PA6 PA7
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  // PE3 CS
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;   // SPI1 clock

    // ---- Configure PE3 as CS ----
    GPIOE->MODER &= ~(3 << (3 * 2));
    GPIOE->MODER |=  (1 << (3 * 2));  // Output
    GPIOE->OTYPER &= ~(1 << 3);
    GPIOE->OSPEEDR |= (3 << (3 * 2)); // High speed
    GPIOE->ODR |= (1 << 3);           // CS HIGH (inactive)

    // ---- Configure PA5(SCK), PA6(MISO), PA7(MOSI) as AF5 ----
    GPIOA->MODER &= ~((3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
    GPIOA->MODER |=  ((2<<(5*2)) | (2<<(6*2)) | (2<<(7*2)));  // AF mode

    GPIOA->AFR[0] |= (5<<(5*4)) | (5<<(6*4)) | (5<<(7*4));    // AF5 = SPI1

    GPIOA->OSPEEDR |= (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2));   // High speed
    GPIOA->OTYPER &= ~((1<<5) | (1<<6) | (1<<7));

    // ---- SPI1 Config ----
    SPI1->CR1 = 0;
    SPI1->CR1 |= (1<<2);     // Master mode
    SPI1->CR1 |= (3<<3);     // Baud rate = fPCLK/16 (safe)
    SPI1->CR1 |= (1<<1);     // CPOL = 0
    SPI1->CR1 |= (1<<0);     // CPHA = 0  → Mode 0 (SSD1306 supports it)
    SPI1->CR1 &= ~(1<<11);   // 8-bit data frame
    SPI1->CR1 |= (1<<9);     // Software slave mgmt
    SPI1->CR1 |= (1<<8);     // SSI = 1

    SPI1->CR1 |= (1<<6);     // Enable SPI

    UartPuts("SPI1 Init Done\r\n");
}

void SpiCSEnable(void)
{
    GPIOE->ODR &= ~(1<<3);
}

void SpiCSDisable(void)
{
    GPIOE->ODR |= (1<<3);
}

uint16_t SpiTransfer(uint16_t data)
{
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while(!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}

void SpiTransmit(uint8_t data)
{
    SpiTransfer(data);
}

uint16_t SpiReceive(void)
{
    return SpiTransfer(0x00);
}

void SpiWrite(uint8_t internalAddr, uint8_t data[], uint8_t size)
{
    SpiCSEnable();
    SpiTransmit(internalAddr);
    for(int i=0;i<size;i++)
        SpiTransmit(data[i]);
    SpiCSDisable();
}

void SpiRead(uint8_t internalAddr, uint8_t data[], uint8_t size)
{
    SpiCSEnable();
    SpiTransmit(internalAddr);
    for(int i=0;i<size;i++)
        data[i] = SpiReceive();
    SpiCSDisable();
}
