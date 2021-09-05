#include "main.h"

#define LEDPORT                 (GPIOC)
#define LED1                    (13)
#define ENABLE_GPIO_CLOCK       (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)

static void ms_delay(uint32_t ms);

int main(void)
{
    while (1)
    {
    ENABLE_GPIO_CLOCK;                      // enable the clock to GPIO
    GPIOC->MODER |= GPIO_MODER_MODER13_0;   // set pins to be general purpose output
    while(1)
    {
        ms_delay(500);
        LEDPORT->ODR ^= (1<<LED1);  // toggle diodes
    }
    }
}

static void ms_delay(uint32_t ms)
{
    while (ms-- > 0)
    {
        volatile uint32_t x = 500;
        while (x-- > 0)
            __asm("nop");
    }
}
