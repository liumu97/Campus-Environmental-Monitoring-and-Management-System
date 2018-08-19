/********************
* FileName: 	briupUsart3.c
* Description: 	
			串口初始化的接口
			接收：按Byte接收、接收一个Str；
			发送：按Byte发送、发送一个Str、发送一个buffer;
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加接口声明
*********************/

#include "briupUsart3.h"

//声明接收数据的缓冲区及标记
static volatile u8 usart3_buffer[256];
//数据头和数据尾
static volatile u16 bufHead, bufTail;
static volatile u16 bufCount = 0;


//串口3初始化函数接口
//	pclk:输入的时钟频率	MHz
//	baud:目标波特率
void usart3Init( u8 pclk, u32 baud)
{
	NVIC_InitTypeDef nvic_init;
	//计算DIV定点数的值
	double usartdiv = (pclk * 1000000.0)/(16 * baud);
	u16 mant = (u16)usartdiv;
	u16 frac = (usartdiv - mant) * 16;
	mant = mant<<4 | (frac & 0x0f);
	
	//开启USART3和GPIOB的时钟
	//	Tx -> PB10
	//	Rx -> PB11
	RCC->APB2ENR |= 1<<3;	//GPIOB
	RCC->APB1ENR |= 1<<18;	//USART3
	
	//配置GPIO的复用功能模式
	GPIOB->CRH &= ~0x0000FF00;
	GPIOB->CRH |=  0x00008B00;
	GPIOB->ODR |= 1<<11;	//PB11上拉输入
	
	//配置USART波特率
	USART3->BRR = mant;
	//13->串口EN 3->TxEN 2->RxEN  5->接收中断EN
	USART3->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;
	
	nvic_init.NVIC_IRQChannel = USART3_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init.NVIC_IRQChannelSubPriority = 3;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvic_init);
}

u16 usart3GetBuffer(u8 * buf)
{
	//将最近一次获取到的缓冲区数据
	//转移到buf中，并移动缓冲区头部
	u16 temp = bufCount, i;
	if( bufCount == 0)
		return 0;
	bufCount = 0;
	for( i = 0;i < temp;i ++)
	{
		*buf = usart3_buffer[bufHead];
		buf ++;
		bufHead ++;
		bufHead %= 256;
	}
	return temp;
}

//==============中断的分割线==================
//USART接收中断处理程序
void USART3_IRQHandler(void)
{
	u8 temp = 0;
	if( USART3->SR & (1<<5))
	{
		temp = USART3->DR;
		usart3_buffer[bufTail] = temp;
		bufTail ++;
		bufTail %= 256;
		bufCount ++;
	}
}

