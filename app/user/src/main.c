/****************************************************************
最后修改：2019-1-16 22：00
*****************************************************************/

#include <string.h>
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_flash.h"

int main()
{
	SCB->VTOR = FLASH_BASE|0x5000;
	FLASH_Status status;
	usart_init(115200);
	usart2_init(9600);
	LED_Init();
	
	SYS_Time_Init();
	printf("app\r\n");
	while(1)
	{
		if(g_ul_sys_time%100==0)
		{
			printf("app version 100\r\n");
			g_ul_sys_time = 1;
			LED_REV();
		}
		if(read_flag) {
			if(strncmp(str_buffer, "upgrade\r\n", 4)==0) {
				FLASH_Unlock();
				do {
					status = FLASH_ErasePage(0x8004000);
				} while(status!=FLASH_COMPLETE);
				do {
					status = FLASH_ProgramWord(0x8004000, 0x12345678); 
				} while(status!=FLASH_COMPLETE);
				FLASH_Lock();
				printf("app reset, start upgrade\r\n");
				NVIC_SystemReset();
			}
			read_flag=0;
		}
	}
	
	/* 深海区 */
}

