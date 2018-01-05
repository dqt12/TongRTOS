#include "led.h"

void LED_Init(void)
{
    GPIO_DeInit(LED_IO); 
    GPIO_Init(LED_IO, LED0, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(LED_IO, LED1, GPIO_MODE_OUT_PP_HIGH_FAST);
}