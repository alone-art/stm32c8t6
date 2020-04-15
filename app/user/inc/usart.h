#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"

#define data_len 30

extern void usart_send_byte(char dat);	//����һ���ַ�
extern void usart_send_str(char *str);	//�����ַ���
extern char usart_read_byte(void);		//��ȡһ���ַ�
extern char *usart_read_str(char *str);//��ȡ�ַ���
extern unsigned char read_rxne(void);	//�鿴�Ƿ������ݽ���
extern void usart_init(u32 baud);			//USART1��ʼ��
extern void USART1_RD_Deal(void);

extern void usart2_send_str(char *str);		//�����ַ���
extern void usart2_init(u32 baud);
extern void USART2_RD_Deal(void);

extern char str_buffer[512],*str_loca,read_flag;
extern char str_buffer2[4096],*str_loca2,read_flag2;
#endif
