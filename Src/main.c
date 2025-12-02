 #include "stm32f4xx.h"
#include "uart.h"
#include "spi.h"
#include "oled.h"
#include "dht11.h"
#include <stdio.h>

/* tiny delay */
static void delay_ms_approx(uint32_t ms)
{
    for (volatile uint32_t j = 0; j < ms * 2000; j++);
}

int main(void)
{
    char dbg[128];
    uint8_t t = 0, h = 0;
    uint8_t status;

    /* Initialize UART for debug */
    UartInit(115200);
    UartPuts("\r\n========================================\r\n");
    UartPuts("STM32F407 - SSD1306 SPI OLED - DEBUG\n");
    UartPuts("Author: Sagar\n");
    UartPuts("========================================\r\n");

    /* Basic CPU alive check */
    UartPuts("Main: Starting hardware init sequence...\r\n");

    /* Initialize SPI (SpiInit also prints message if coded) */
    UartPuts("Main: Initializing SPI (SpiInit)...\r\n");
    SpiInit();
    UartPuts("Main: SPI initialized.\r\n");

    /* Initialize DHT11 (GPIO clock + default pin mode) */
    UartPuts("Main: Initializing DHT11 (DHT11_Init)...\r\n");
    DHT11_Init();
    UartPuts("Main: DHT11 init done.\r\n");

    /* Initialize OLED */
    UartPuts("Main: Calling OLED_Init()...\r\n");
    OLED_Init();
    UartPuts("Main: OLED_Init() complete.\r\n");

    /* Clear and print greeting */
    UartPuts("Main: Clearing display (OLED_Clear)...\r\n");
    OLED_Clear();
    UartPuts("Main: Display cleared.\r\n");

    /* Print name on top (page 0) */
    UartPuts("Main: Printing 'Hello Sagar' on page 0, col 0...\r\n");
    OLED_SetCursor(0, 0);
    OLED_PrintString("Hello Sagar");
    UartPuts("Main: 'Hello Sagar' printed.\r\n");

    /* Also print an initial placeholder for Temp/Hum */
    OLED_SetCursor(2, 0); OLED_PrintString("Temp: --C");
    OLED_SetCursor(3, 0); OLED_PrintString("Hum:  --%");

    UartPuts("Main: Initial display setup complete. Entering main loop.\r\n");
    UartPuts("========================================\r\n");

    while (1)
    {
        UartPuts("Main: Starting DHT11 read sequence...\r\n");

        /* Read DHT11 */
        status = DHT11_Read(&t, &h);

        if (status == 0)
        {
            /* Format and show on OLED */
            sprintf(dbg, "Main: DHT11 read OK -> Temp=%d C, Hum=%d %%\r\n", t, h);
            UartPuts(dbg);

            /* Update Temp line (page 2) */
            OLED_SetCursor(2, 0);
            sprintf(dbg, "Temp: %dC  ", t);   // trailing spaces to clear previous digits
            OLED_PrintString(dbg);

            /* Update Hum line (page 3) */
            OLED_SetCursor(3, 0);
            sprintf(dbg, "Hum:  %d%%  ", h);
            OLED_PrintString(dbg);

            UartPuts("Main: OLED updated with new values.\r\n");
        }
        else if (status == 1)
        {
            UartPuts("Main: DHT11 read error - no response from sensor.\r\n");
            /* show error on OLED page 2 */
            OLED_SetCursor(2, 0);
            OLED_PrintString("Temp: ERR   ");
            OLED_SetCursor(3, 0);
            OLED_PrintString("Hum:  ERR   ");
        }
        else if (status == 2)
        {
            UartPuts("Main: DHT11 read error - checksum mismatch.\r\n");
            OLED_SetCursor(2, 0);
            OLED_PrintString("Temp: CHK?  ");
            OLED_SetCursor(3, 0);
            OLED_PrintString("Hum:  CHK?  ");
        }
        else
        {
            UartPuts("Main: DHT11 read error - unknown status.\r\n");
        }

        /* Heartbeat / wait */
        UartPuts("Main: Sleeping for 2 seconds before next read...\r\n");
        delay_ms_approx(2000);
    }
}
