/*******************************************************************************
 
*******************************************************************************/
#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "delay.h"

#define GPIO_LED	GPIOC
	
#define LED_REV() (GPIO_LED->ODR ^= 1<<13)
#define LED_ON()  (GPIO_LED->ODR &= ~(1<<13))
#define LED_OFF() (GPIO_LED->ODR |= 1<<13)


extern void LED_Init(void);

#endif 

/******************************************/


