#include "usart.h"

char str_buffer[512]="1230\r\n",*str_loca=str_buffer,read_flag;

void usart_send_byte(char dat)
{
	while(!((USART1->SR>>6) & 1));  //查忙
	USART1->DR = dat;
}

void usart_send_str(char *str)		//发送字符串
{
	while(*str!='\0')
	{
		usart_send_byte(*str);
		str++;
	}
}


char usart_read_byte()
{
	while(USART1->SR>>5 & 1);  //查忙
	return USART1->DR;
}


char *usart_read_str(char *str)	//接收字符串
{
	//字符串需要#结束
	int i=0;
	do
	{
		str[i]=usart_read_byte();
	}while(str[i++]!='#' && i<data_len);
	str[i-1]='\0';
	return str;
}

unsigned char read_rxne()  //查看是否有数据接收
{
	if(USART1->SR>>5 & 1)
		return 1;
	else
		return 0;
}

void usart_init(u32 baud)
{
	RCC->APB2ENR |= 1<<2 | 1<<14;			//使能A组 usart1
	
	//PA9 TX  PA10 RX
	GPIOA->CRH &= ~(0x0f<<4);
	GPIOA->CRH |= 0x0b<<4;					//1011 复用推挽输出
	
	GPIOA->CRH &= ~(0x0f<<8);
	GPIOA->CRH |= 0x04<<8;					//0100 浮空输入 （复位状态）

	double DIV = 72000000/16/baud;
	USART1->BRR |= (u32)DIV<<4 | (u32)((DIV-(u32)DIV)*16+0.5f);					//配置波特率
	
	USART1->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;		//使能串口 开启发送 开启接收 开启中断位
	NVIC->ISER[1] |= 1<<(37-32);
}

int fputc(int ch,FILE *f)
{
	usart_send_byte(ch);
	return ch;
}


/**********************************************************
函数功能：将数据打包发送
函数名：USART1_Send_Deal
形参：num 语句数 num_count语句序号 *dat 数据

***********************************************************/
void USART1_Send_Deal(u8 num,u8 num_count,u8* dat)
{
	static u8 flag=0;
//	static u8 mas_num=0;
	u8 buf[64],check=0,i=6;
	
	buf[0] = '!';		//起始符
	buf[1] = 'R';		//标识符
	buf[2] = num;		//语句数
	buf[3] = flag;		//语句序号
	buf[4] = 'o';		//标识号
	buf[5] = 7;			//传输方式
	while(*dat != '\0')	//数据域
	{
		buf[i++] = *dat;
		check += *dat;
		dat++;
	}
	buf[i++] = 0x06;	//填充位
	check += 0x06;
	buf[i++] = check;	//校验和
	buf[i] = '\n';		//结束符
	printf("%s",buf);
}

/************************************************************
函数功能：将串口1接收的数据进行处理
函数名：USART1_RD_Deal
形参：无
返回值：无
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
	while(!((USART2->SR>>6) & 1));  //查忙
	USART2->DR = dat;
}

void usart2_send_str(char *str)		//发送字符串
{
	while(*str!='\0')
	{
		usart2_send_byte(*str);
		str++;
	}
}


char usart2_read_byte()
{
	while(USART2->SR>>5 & 1);  //查忙
	return USART2->DR;
}


char *usart2_read_str(char *str)	//接收字符串
{
	//字符串需要#结束
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
	RCC->APB2ENR |= 1<<2;			//使能A组 usart1
	RCC->APB1ENR |= 1<<17;
	
	//PA3 TX  PA3 RX
	GPIOA->CRL &= 0xfffff00f;
	GPIOA->CRL |= 0x00000b40;					//1011 复用推挽输出

	double DIV = 36000000/16/baud;
	USART2->BRR &= 0xffff0000;
	USART2->BRR |= (u32)DIV<<4 | (u32)((DIV-(u32)DIV)*16+0.5f);					//配置波特率
	
	USART2->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;		//使能串口 开启发送 开启接收 开启中断位
	NVIC->ISER[1] |= 1<<(38-32);
}




/************************************************************
函数功能：将串口1接收的数据进行处理
函数名：USART1_RD_Deal
形参：无
返回值：无
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

