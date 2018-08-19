/********************************************************
*FileName:     briupZigbee.c
*Destription:  
*              Zigbee Usart2初始化的接口实现
*              通过DMA发送数据
*              通过Usart2中断的方式接收数据
*Author:      
*CreateDate:   2018-7-15
*Copyright     
*Update:       修改人    修改内容
*  2018-7-15     	       实现初始化函数
********************************************************/

#include "briupZigbee.h"

//构建循环队列
static volatile u8 zbBuffer[256];
static volatile u16 zbHead, zbTail;
static volatile u16 zbCount=0;


//Zigbee串口的初始化
void zigbeeInit( u8 pclk, u32 baud)
{
	
	NVIC_InitTypeDef nvicInit;
	
	
	double div= (pclk * 1000000) / (16*baud);
	u16 mant = (u16)div;
	u16 frac = (div - mant) * 16;
	mant = mant << 4 |(frac & 0x0f);
	
	//1. RCC 开启GPIOA的外设时钟
	RCC->APB2ENR |= 1<<2;
	RCC->APB1ENR |= 1<<17;       //USART2时钟使能
	RCC->AHBENR  |= 0x01;        //DMA时钟使能
	
	//2. 配置PA2 PA3的工作模式
	//  PA2 -> U2.Tx  PA3 -> U2.Rx
	GPIOA->CRL &= ~0x0000FF00;
	GPIOA->CRL |=  0x00008B00;
	GPIOA->ODR |=  1<<3;
	
	//3.  配置Usart2
	//  配置串口波特率
	USART2->BRR = mant;
	//开启USART2的接收使能、发送使能、串口使能和接收中断使能，DMA发送使能
	USART2->CR1 |= 1<<13 | 1<<5 | 1<<3 | 1<<2;	
	USART2->CR3 |= 1<<7;
	
	//初始化DMA  U2. Tx -> DMA通道1 7
	DMA1_Channel7->CCR   = 0;
	//通道外设地址
	DMA1_Channel7->CPAR  = (u32) &USART2->DR;
	DMA1_Channel7->CMAR  = 0;
	DMA1_Channel7->CNDTR = 0;
	DMA1_Channel7->CCR   |= 1<<13 | 1<<7 | 1<<4;
	
	nvicInit.NVIC_IRQChannel                   = USART2_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 2;
	nvicInit.NVIC_IRQChannelSubPriority        = 3;
	nvicInit.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init( &nvicInit);
	
}

//通过DMA发送一个buf数组
void zigbeeSendBuf( u8 * buf, u16 len)
{
	DMA1_Channel7->CCR   &= ~0x01;
	DMA1_Channel7->CMAR  = (u32) buf;
	DMA1_Channel7->CNDTR = len;
	DMA1_Channel7->CCR   |= 0x01;
}


//获取zigbee的数据
u16 zigbeeGetData( u8 * buf, u8 flag)
{
	u16 temp, i, ck;
	temp = zbCount;
	if( zbCount == 0)
		return 0;

	if(!flag)
	{
		zbCount = 0;
		for( i = 0;i < temp;i ++)
		{
			*buf = zbBuffer[zbHead];
			buf ++;
			zbHead ++;
			zbHead %= 256;
		}
		return temp;
	}
//  对协议的判断
	else
	{
		for(i = zbHead; ;i ++)
		{
			i %= 256;
			if( zbBuffer[i] == 0xee && zbBuffer[i + 1] == 0xff)
			{
				ck = 1;
				break;
			}
			if( i == zbTail)
				break;
		}
		if( ck == 0)
			return 0;
		temp = zbCount;
		zbCount = 0;
		for( i = 0; i < temp; i ++)
		{
			*buf = zbBuffer[zbHead];
			buf ++;
			zbHead ++;
			zbHead %= 256;
		}
		return temp;
	}
}


//----------------------------中断服务函数------------------------------

void USART2_IRQHandler(void)
{
	if( USART2->SR & (1<<5))
	{
		zbBuffer[zbTail] = USART2->DR;
		zbTail ++;
		zbCount ++;
		zbTail %= 256;
	}
}
	



