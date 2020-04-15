#include "delay.h"

void delay_us(u32 us) //1 us��ʱ
{
    SysTick->LOAD= us*9;       //ʱ�����  
	SysTick->VAL= 0;        //��ռ�����      
 	SysTick->CTRL|= ENABLE;            //��ʼ����    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//�ȴ�ʱ�䵽�� 
 	SysTick->CTRL=0X00000000;       //�رռ�����
 	SysTick->VAL= 0;        //��ռ�����  
}

void delay_ms(u32 ms) //1 ms��ʱ
{
//	u32 sys = SYS_Time;
// 	while(sys+ms/10 > SYS_Time);//�ȴ�ʱ�䵽�� 
	
	SysTick->LOAD= ms*9000;       //ʱ�����  
	SysTick->VAL= 0;        //��ռ�����      
 	SysTick->CTRL|= ENABLE;            //��ʼ����    
 	while(SysTick->VAL||!(SysTick->CTRL&(1<<16)));//�ȴ�ʱ�䵽�� 
 	SysTick->CTRL=0X00000000;       //�رռ�����
 	SysTick->VAL= 0;        //��ռ�����  
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
    SysTick->LOAD= 90000;       //ʱ�����  //10ms
	SysTick->VAL= 0;        //��ռ�����      
 	SysTick->CTRL|= ENABLE | 1<<1;            //��ʼ����    
}

u32 g_ul_sys_time = 0;

void SysTick_Handler(void)
{
	if(SysTick->VAL||!(SysTick->CTRL&(1<<16)))//ʱ�䵽�� 
	{
		g_ul_sys_time++;   
//		if(SYS_Time%3==0)
//			WWDG->CR=0x7f;
	}
}





