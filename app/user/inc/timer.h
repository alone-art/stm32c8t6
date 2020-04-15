/*******************************************************************************

*******************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

extern void IWDG_Start(u16 s);
extern u8 WWDG_Start(u16 Time_T_us,u16 Time_W_us,u8 IR);

//void TIM3_Int_Init(u16 arr,u16 psc);
// void TIM5_Int_Init(u16 arr,u16 psc);
#endif
/******************** **********************/
