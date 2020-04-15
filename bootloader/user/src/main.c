/****************************************************************
����޸ģ�2019-1-16 22��00
*****************************************************************/

#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_flash.h"

typedef  void (*IapFun)(void);                                //����һ���������͵Ĳ���
IapFun JumpToApp; 
void Iap_Load_App(u32 AppAddr)
{
	if(((*(vu32*)AppAddr)&0x3FFF8000)==0x20000000)      //���ջ����ַ�Ƿ�Ϸ�.
	{ 
		JumpToApp = (IapFun)*(vu32*)(AppAddr+4); 	//�û��������ڶ�����Ϊ����ʼ��ַ(�³���λ��ַ)               

		__set_MSP(*(vu32*)AppAddr);                 //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		
		JumpToApp();        						//����PCָ��Ϊ�³���λ�жϺ����ĵ�ַ������ִ��
	}
}
int main()
{
	FLASH_Status status;
	usart_init(115200);
	usart2_init(9600);
	LED_Init();
	//test();
	SYS_Time_Init();
	printf("bootloader\r\n");
	printf("%x\r\n", *(__IO uint32_t*)0x8004000);
	if((*(__IO uint32_t*)0x8004000)==0x12345678) {
		FLASH_Unlock();
		for(int i=0; i<0x5000/1024; i++) {
			do {
				status = FLASH_ErasePage(0x8005000+i*1024);
			} while(status!=FLASH_COMPLETE);
		}
		FLASH_Lock();
		printf("upgrade flag\r\n");
	} else {
		Iap_Load_App(0x8005000);
	}
	uint32_t addr=0x8005000;
	while(1)
	{
		if(g_ul_sys_time%10==0)
		{
//			g_ul_sys_time = 1;
			if(g_ul_sys_time==1000) {
				FLASH_Unlock();
				do {
					status = FLASH_ErasePage(0x8004000);
				} while(status!=FLASH_COMPLETE);
				FLASH_Lock();
				Iap_Load_App(0x8005000);
			}
			LED_REV();
		}
		if(read_flag) {
			
			FLASH_Unlock();
			do {
				status = FLASH_ProgramWord(addr, *(uint32_t *)str_buffer); 
			} while(status!=FLASH_COMPLETE);
			printf("upgrade %d\r\n", addr);
			addr+=4;
			FLASH_Lock();
			str_buffer_len = 0;
			read_flag=0;
		}
	}
	
	/* ��� */
}

