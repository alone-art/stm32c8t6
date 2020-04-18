/****************************************************************
����޸ģ�2019-1-16 22��00
*****************************************************************/

#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_flash.h"

uint8_t upgrade_flag=0;
uint8_t led_flag=0;

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
		printf("erase flash\r\n");
		FLASH_Unlock();
		for(int i=0; i<0x5000/1024; i++) {
			do {
				status = FLASH_ErasePage(0x8005000+i*1024);
			} while(status!=FLASH_COMPLETE);
		}
		FLASH_Lock();
		printf("upgrade flag\r\n");
		upgrade_flag = 1;
	} else {
		Iap_Load_App(0x8005000);
	}
	while(1)
	{
		if(g_upgrade_sys_time==100) {
//			g_ul_sys_time = 1;
			if(upgrade_flag==0) {
				FLASH_Unlock();
				do {
					status = FLASH_ErasePage(0x8004000);
				} while(status!=FLASH_COMPLETE);
				FLASH_Lock();
				printf("upgrade end, start app");
				Iap_Load_App(0x8005000);
			}
			printf("wating...\r\n");
			LED_REV();
			g_upgrade_sys_time = 0;
		}
		if(read_flag) {
				read_flag=0;
			
		}
	}
	
	/* ��� */
}

