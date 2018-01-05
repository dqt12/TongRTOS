#ifndef __LED_H
#define __LED_H

#include "stm8s.h"
#include "stm8s_gpio.h"

#define LED_IO  GPIOD
#define LED0    GPIO_PIN_4
#define LED1    GPIO_PIN_5

/*******************�⺯����ʽ*********************/
#define LED1_ON() GPIO_WriteLow(LED_IO, LED1)
#define LED1_OFF() GPIO_WriteHigh(LED_IO, LED1) 

/*******************�Ĵ�����ʽ*********************/
#define LED0_ON()  LED_IO->ODR&=(~LED0) //�͵�ƽ����LED
#define LED0_OFF() LED_IO->ODR|=LED0  

//#define LED1_ON()  GPIOB->ODR&=(~LED1) //�͵�ƽ����LED
//#define LED1_OFF() GPIOB->ODR|=LED1  

void LED_Init(void);

#endif
 