#include "delay.h"

void delay_us(u32 us) //1 us延时
{
    SysTick->LOAD= us*9;       //时间加载  
	SysTick->VAL= 0;        //清空计数器      
 	SysTick->CTRL|= ENABLE;            //开始倒数    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//等待时间到达 
 	SysTick->CTRL=0X00000000;       //关闭计数器
 	SysTick->VAL= 0;        //清空计数器  
}

void delay_ms(u32 ms) //1 ms延时
{
//	u32 sys = SYS_Time;
// 	while(sys+ms/10 > SYS_Time);//等待时间到达 
	
	SysTick->LOAD= ms*9000;       //时间加载  
	SysTick->VAL= 0;        //清空计数器      
 	SysTick->CTRL|= ENABLE;            //开始倒数    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//等待时间到达 
 	SysTick->CTRL=0X00000000;       //关闭计数器
 	SysTick->VAL= 0;        //清空计数器  
}

void delay_long(unsigned int s)
{
	unsigned int i,j;
	for(i=0;i<65535;i++)
		for(j=0;j<s;j++);
}


void delay_short(unsigned int s)
{
	unsigned int i,j;
	for(i=0;i<127;i++)
		for(j=0;j<s;j++);
}


void SYS_Time_Init()
{
    SysTick->LOAD= 90000;       //时间加载  //10ms
	SysTick->VAL= 0;        //清空计数器      
 	SysTick->CTRL|= ENABLE | 1<<1;            //开始倒数    
}

u32 g_ul_sys_time = 0;

void SysTick_Handler(void)
{
	if(SysTick->VAL||!(SysTick->CTRL&(1<<16)))//时间到达 
	{
		g_ul_sys_time++;   
//		if(SYS_Time%3==0)
//			WWDG->CR=0x7f;
	}
}





