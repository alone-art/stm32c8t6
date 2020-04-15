#ifndef _DELAY_H_
#define _DELAY_H_

#include "stm32f10x.h"

extern u32 g_ul_sys_time;

extern void SYS_Time_Init(void);
extern void delay_us(u32 us); //计时器精准延时
extern void delay_ms(u32 ms);
extern void delay_long(unsigned int s);
extern void delay_short(unsigned int s);


#endif
