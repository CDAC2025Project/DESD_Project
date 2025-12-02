 #include "dht11.h"
#include "uart.h"   // For debug output

/********** Local delay in microseconds (approx) **********/
static void delay_us(uint32_t us)
{
    for(volatile uint32_t i=0; i<us*16; i++);  // approx 1us per loop at 16MHz
}

/********** Configure pin as output **********/
static void DHT11_PinOutput(void)
{
    DHT11_PORT->MODER &= ~(3 << (DHT11_PIN*2));
    DHT11_PORT->MODER |=  (1 << (DHT11_PIN*2));  // output mode
}

/********** Configure pin as input with internal pull-up **********/
static void DHT11_PinInput(void)
{
    DHT11_PORT->MODER &= ~(3 << (DHT11_PIN*2));   // input mode
    DHT11_PORT->PUPDR &= ~(3 << (DHT11_PIN*2));   // clear
    DHT11_PORT->PUPDR |=  (1 << (DHT11_PIN*2));   // enable internal pull-up
}

/********** Public functions **********/
void DHT11_Init(void)
{
    DHT11_CLK_EN();
    DHT11_PinInput();   // default input pull-up
    UartPuts("DHT11: Initialized with internal pull-up on DATA pin.\r\n");
}

/*
 * Returns:
 * 0 = OK
 * 1 = No response
 * 2 = Checksum error
 */
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0};
    UartPuts("DHT11: Starting read sequence...\r\n");

    // ---- Start signal ----
    DHT11_PinOutput();
    DHT11_PORT->ODR &= ~(1 << DHT11_PIN);  // Drive LOW
    UartPuts("DHT11: Sent start signal (LOW 18ms)\r\n");
    delay_us(18000);

    DHT11_PORT->ODR |= (1 << DHT11_PIN);   // Drive HIGH
    delay_us(20);

    DHT11_PinInput();                       // Release pin to input with pull-up
    UartPuts("DHT11: Released pin, waiting for sensor response...\r\n");

    // ---- Check response ----
    delay_us(40);
    if ((DHT11_PORT->IDR & (1 << DHT11_PIN)) != 0)
    {
        UartPuts("DHT11: ERROR - no LOW response from sensor.\r\n");
        return 1;
    }

    delay_us(80);
    if ((DHT11_PORT->IDR & (1 << DHT11_PIN)) == 0)
    {
        UartPuts("DHT11: ERROR - no HIGH response from sensor.\r\n");
        return 1;
    }

    delay_us(80);
    UartPuts("DHT11: Sensor response OK, reading 40 bits...\r\n");

    // ---- Read 40 bits ----
    for (int i = 0; i < 40; i++)
    {
        // wait for LOW -> HIGH transition
        while ((DHT11_PORT->IDR & (1 << DHT11_PIN)) == 0);

        delay_us(40); // wait 40us to sample bit

        if (DHT11_PORT->IDR & (1 << DHT11_PIN))
        {
            data[i/8] |= (1 << (7 - (i%8)));
            UartPuts("1");
        }
        else
        {
            UartPuts("0");
        }

        // wait for HIGH -> LOW transition
        while ((DHT11_PORT->IDR & (1 << DHT11_PIN)) != 0);
    }

    UartPuts("\r\nDHT11: Finished reading 40 bits.\r\n");

    // ---- Checksum ----
    if (data[4] != (uint8_t)(data[0]+data[1]+data[2]+data[3]))
    {
        UartPuts("DHT11: ERROR - Checksum mismatch.\r\n");
        return 2;
    }

    *humidity    = data[0];
    *temperature = data[2];

    UartPuts("DHT11: Read successful.\r\n");
    return 0;
}
