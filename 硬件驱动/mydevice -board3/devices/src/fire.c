/***************************************************************************
*FileName:		fire.c
*Description: 烟雾PB6、火光PB0、甲烷PB1传感器接口实现
							通过查询方式实现
							通过中断方式实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "fire.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "briupZigbee.h"
#include "basePeripheral.h"
#include "briupGPIO.h"
#include "protocol.h"

volatile u8 statFlag;

//初始化火警传感器监控接口
void fireInit(void)
{
	NVIC_InitTypeDef nvicInit;
	//开启GPIOB的时钟
	RCC->APB2ENR |= 1<<3;
	//开启复用时钟
	RCC->APB2ENR |= 1<<0;
	
	//使能PB0，PB1，PB6上拉输入 PB7下拉输入
	GPIOB->CRL &= 0X00FFFF00;
	GPIOB->CRL |= 0X88000088;
	GPIOB->ODR |= (1<<6)|(1<<1)|(1<<0);
	GPIOB->ODR &= ~(1<<7);
	
	//外部中断的端口设置
	AFIO->EXTICR[0] |= 0x0011;
	AFIO->EXTICR[1] |= 0x1100;
	
	//外部中断的触发模式与使能
	EXTI->RTSR &= ~(1<<0 | 1<<1 | 1<<6 | 1<<7);
	EXTI->FTSR |= 1<<0 | 1<<1 | 1<<6 | 1<<7;
	EXTI->IMR |= 1<<0 | 1<<1 | 1<<6 | 1<<7;
	
	//外部中断线0、1、6、7的NVIC优先级
	nvicInit.NVIC_IRQChannel = EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInit.NVIC_IRQChannelSubPriority = 2;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
	
	nvicInit.NVIC_IRQChannel = EXTI1_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInit.NVIC_IRQChannelSubPriority = 2;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
	
	nvicInit.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInit.NVIC_IRQChannelSubPriority = 2;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);
	
}

//获取烟雾传感器状态
u8 fogStat(void)
{
	_delay_ms(10);
	if(!(GPIOB->IDR & (1<<6)))
		return '1';
	else
		return '0';
}
//获取火光传感器状态
u8 fireStat(void)
{
	if(!(GPIOB->IDR & (1<<0)))
		return '1';
	else
		return '0';
}
//获取甲烷传感器状态
u8 CH4Stat(void)
{
	{
	if(!(GPIOB->IDR & (1<<1)))
		return '1';
	else
		return '0';
}
}
//获取红外传感器状态
u8 infrared(void)
{
	if( GPIOB->IDR & (1<<7))
		return '1';
	else
		return '0';
}

//=========外部中断服务函数
//烟雾传感器
void EXTI9_5_IRQHandler(void)
{
	_delay_ms(100);
	//响应并清除中断标志
	if( !(GPIOB->IDR & 1<<6))
	{	
		if(!(statFlag & 0x01)) statFlag |= 1<<0;
	}
	EXTI->PR |= 1<<6;
//	if( GPIOB->IDR & 1<<7)
//	{
//		EXTI->PR |= 1<<7;
//		if(!(statFlag & 1<<4)) statFlag |= 1<<4;
//	}
//	
}
//火光传感器
void EXTI0_IRQHandler(void)
{
	
	_delay_ms(100);
	//响应并清除中断标志
	if( !(GPIOB->IDR & 1<<0))
	{					
		if(!(statFlag & 1<<6)) statFlag |= 1<<6;
	}
	EXTI->PR |= 1<<0;
}
//甲烷传感器
void EXTI1_IRQHandler(void)
{
	_delay_ms(100);
	//响应并清除中断标志
	if( !(GPIOB->IDR & 1<<1))
	{
		if(!(statFlag & 1<<2)) statFlag |= 1<<2;
	}
	EXTI->PR |= 1<<1;
}

