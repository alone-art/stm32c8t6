#include "usart.h"

char str_buffer[512]="1230\r\n",*str_loca=str_buffer,read_flag;

void usart_send_byte(char dat)
{
	while(!((USART1->SR>>6) & 1));  //��æ
	USART1->DR = dat;
}

void usart_send_str(char *str)		//�����ַ���
{
	while(*str!='\0')
	{
		usart_send_byte(*str);
		str++;
	}
}


char usart_read_byte()
{
	while(USART1->SR>>5 & 1);  //��æ
	return USART1->DR;
}


char *usart_read_str(char *str)	//�����ַ���
{
	//�ַ�����Ҫ#����
	int i=0;
	do
	{
		str[i]=usart_read_byte();
	}while(str[i++]!='#' && i<data_len);
	str[i-1]='\0';
	return str;
}

unsigned char read_rxne()  //�鿴�Ƿ������ݽ���
{
	if(USART1->SR>>5 & 1)
		return 1;
	else
		return 0;
}

void usart_init(u32 baud)
{
	RCC->APB2ENR |= 1<<2 | 1<<14;			//ʹ��A�� usart1
	
	//PA9 TX  PA10 RX
	GPIOA->CRH &= ~(0x0f<<4);
	GPIOA->CRH |= 0x0b<<4;					//1011 �����������
	
	GPIOA->CRH &= ~(0x0f<<8);
	GPIOA->CRH |= 0x04<<8;					//0100 �������� ����λ״̬��

	double DIV = 72000000/16/baud;
	USART1->BRR |= (u32)DIV<<4 | (u32)((DIV-(u32)DIV)*16+0.5f);					//���ò�����
	
	USART1->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;		//ʹ�ܴ��� �������� �������� �����ж�λ
	NVIC->ISER[1] |= 1<<(37-32);
}

int fputc(int ch,FILE *f)
{
	usart_send_byte(ch);
	return ch;
}


/**********************************************************
�������ܣ������ݴ������
��������USART1_Send_Deal
�βΣ�num ����� num_count������ *dat ����

***********************************************************/
void USART1_Send_Deal(u8 num,u8 num_count,u8* dat)
{
	static u8 flag=0;
//	static u8 mas_num=0;
	u8 buf[64],check=0,i=6;
	
	buf[0] = '!';		//��ʼ��
	buf[1] = 'R';		//��ʶ��
	buf[2] = num;		//�����
	buf[3] = flag;		//������
	buf[4] = 'o';		//��ʶ��
	buf[5] = 7;			//���䷽ʽ
	while(*dat != '\0')	//������
	{
		buf[i++] = *dat;
		check += *dat;
		dat++;
	}
	buf[i++] = 0x06;	//���λ
	check += 0x06;
	buf[i++] = check;	//У���
	buf[i] = '\n';		//������
	printf("%s",buf);
}

/************************************************************
�������ܣ�������1���յ����ݽ��д���
��������USART1_RD_Deal
�βΣ���
����ֵ����
************************************************************/
void USART1_RD_Deal(void)
{
	if(read_flag)
	{
		usart2_send_str(str_buffer);
//		usart2_send_str(str_buffer);
//		usart_send_str("AT+NAMEMYBLUE\r\n");
		read_flag=0;
	}
}


void USART1_IRQHandler(void)
{
	u16 i=0;
	if(USART1->SR & 1<<5)
	{
		while(++i)
		{
			if(USART1->SR & 1<<5)
			{
				*str_loca=USART1->DR;
				str_loca++;
				i=0;
			}
		}
		*str_loca='\0';
		str_loca=str_buffer;
		read_flag=1;
	}
}


char str_buffer2[4096],*str_loca2=str_buffer2,read_flag2;

void usart2_send_byte(char dat)
{
	while(!((USART2->SR>>6) & 1));  //��æ
	USART2->DR = dat;
}

void usart2_send_str(char *str)		//�����ַ���
{
	while(*str!='\0')
	{
		usart2_send_byte(*str);
		str++;
	}
}


char usart2_read_byte()
{
	while(USART2->SR>>5 & 1);  //��æ
	return USART2->DR;
}


char *usart2_read_str(char *str)	//�����ַ���
{
	//�ַ�����Ҫ#����
	int i=0;
	do
	{
		str[i]=usart2_read_byte();
	}while(str[i++]!='#' && i<data_len);
	str[i-1]='\0';
	return str;
}

void usart2_init(u32 baud)
{
	RCC->APB2ENR |= 1<<2;			//ʹ��A�� usart1
	RCC->APB1ENR |= 1<<17;
	
	//PA3 TX  PA3 RX
	GPIOA->CRL &= 0xfffff00f;
	GPIOA->CRL |= 0x00000b40;					//1011 �����������

	double DIV = 36000000/16/baud;
	USART2->BRR &= 0xffff0000;
	USART2->BRR |= (u32)DIV<<4 | (u32)((DIV-(u32)DIV)*16+0.5f);					//���ò�����
	
	USART2->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;		//ʹ�ܴ��� �������� �������� �����ж�λ
	NVIC->ISER[1] |= 1<<(38-32);
}




/************************************************************
�������ܣ�������1���յ����ݽ��д���
��������USART1_RD_Deal
�βΣ���
����ֵ����
************************************************************/
void USART2_RD_Deal(void)
{
	if(read_flag2)
	{
		printf("%s\r\n",str_buffer2);
		read_flag2=0;
	}
}


void USART2_IRQHandler(void)
{
	u16 i=0;
	NVIC->ICER[1] |= (1<<(38-32));
	if(USART2->SR & 1<<5)
	{
		while(++i>0)
		{
			if(USART2->SR & 1<<5)
			{
				*str_loca2=USART2->DR;
				str_loca2++;
				i=0;
			}
		}
		*str_loca2='\0';
		str_loca2=str_buffer2;
		read_flag2=1;
	}
	NVIC->ISER[1] |= 1<<(38-32);
}

