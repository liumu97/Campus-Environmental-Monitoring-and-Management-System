/***************************************************************************
*FileName:		briupUSART.h
*Description: 实现初始化Dubug串口 -> usart1 的函数接口
							实现串口发送数据：ch / str  /buf
							实现串口接收数据：ch / str
*Author:			yhs
*CreatDate:		2018-7-12
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "briupUSART.h"


//声明串口接收的循环队列缓冲区
static volatile u8 Usart1Buf[256];
//队列列首标记和列尾标记
static volatile u16 u1Head,u1Tail;
//统计最后一次接收到的数据长度
static volatile u16 u1Count;

//初始化usart1的函数接口
// pclk：工作时钟
// baud： 要设置的波特率
void usart1Init(u8 pclk, u32 baud)
{
	NVIC_InitTypeDef nvicInit;
	
	double temp = (pclk * 1000000.0)/ (16*baud);
	//计算波特率分频因子的整数部分和小数部分
	u16 mant,frac;
	mant = (u16) temp;
	frac = (temp-mant) * 16;
	//定点数->分数波特率发生器
	mant = mant <<4 |(frac & 0x0f);
	
	//使能USART1的时钟	使能GPIOA的时钟
	RCC->APB2ENR |= 1<<14 | 1<<2;
	RCC->AHBENR |= 1<<0;
	
	//PA9->推挽复用输出 PA10->上拉输入
	GPIOA->CRH &= ~0x00000FF0;
	GPIOA->CRH |= 0x000008B0;
	GPIOA->ODR |= 1<<10;
	
	//配置串口
	USART1->BRR=mant;
	//使能串口的发送和接收
	USART1->CR1 |= 1<<3 | 1<<2;
	//使能串口
	USART1->CR1 |=1<<13;
	//使能串口接收中断
	USART1->CR1 |= 1<<5;
	//使能串口DMA发送
	USART1->CR3 |= 1<<7;
	
	//USART Tx对应的DMA通道
	DMA1_Channel4->CPAR = (u32)&USART1->DR;
	DMA1_Channel4->CMAR = 0;
	DMA1_Channel4->CNDTR = 0;
	DMA1_Channel4->CCR = 0;
	DMA1_Channel4->CCR |= 1<<12 | 1<<7 | 1<<4;
	
	//注册中断优先级和中断编号
	nvicInit.NVIC_IRQChannel = USART1_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 3;
	nvicInit.NVIC_IRQChannelSubPriority = 3;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvicInit);
	
}

//发送函数声明
void putChar( u8 ch)					//发送1Byte数据
{
	while(!(USART1->SR & (1<<7)));
	USART1->DR = ch;
	
}

//发送String数据 ‘\0’
void putStr( char * str)			
{
	while( *str != '\0')
	{
		putChar( *str);
		str++;
	}
}

//发送一个Buffer
void putBuffer( u8 * buf, u16 len)		
{
	u16 i;
	for( i=0; i<len; i++)
	{
		putChar(*buf);
		buf++;
	}
}
//通过DMA发送一段数据
void putDataDma( u8 * buf, u16 len)
{
	DMA1_Channel4->CCR &= ~0x01;
	DMA1_Channel4->CMAR = (u32) buf;
	DMA1_Channel4->CNDTR =len;
	DMA1_Channel4->CCR |= 0x01;
}

//接收函数声明
u8 getChar( void)
{
	while(!(USART1->SR &(1<<5)));
	return USART1->DR;
}
u8 getStr( char * str)		//返回接收到的数据长度
{
	u8 len = 0;
	do
	{
		*str = getChar();
		str++;
		len++;
	}while(*(str-1) != '\0');
	return len;	//接收字符串的长度
}

//通过循环队列获取一段数据
// flag: 
//			0 直接获取最近接收到的数据
//			1 判断获取到完整的一帧协议数据再返回
u16 getBuffer( u8 *buf, u8 flag)
{
	u16 temp, i;
	temp = u1Count;
	if( u1Count == 0)
		return 0;
	if(!flag)		//flag ==0
	{
		u1Count = 0;
		for( i = 0;i < temp;i++)
		{
			*buf =Usart1Buf[u1Head];
			buf ++;
			u1Head ++;
			u1Head %= 256;
		}
		return temp;
	}
//	else				//对协议的判断
//	{
//		
//	}
	return 0;
}

//============中断服务函数===============
void USART1_IRQHandler(void)
{
	//判断是否接收到数据
	if(USART1->SR & (1<<5))
	{
		Usart1Buf[u1Tail] = USART1->DR;
		u1Tail++;
		u1Tail %= 256;
		u1Count ++;
	}
}

