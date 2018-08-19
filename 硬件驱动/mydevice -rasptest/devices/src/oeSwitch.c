/***************************************************************************
*FileName:		oeSwitch.c
*Description: 光电开关的初始化函数接口实现
							通过轮询的方式返回光电开关状态
							通过中断的方式检测光电开关状态
*Author:			yhs
*CreatDate:		2018-7-13
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "oeSwitch.h"
#include "briupUsart.h"
#include "briupDelay.h"


//光电开关的初始化接口
void oeSwitchInit(void)
{
	NVIC_InitTypeDef nvicInit;
	
	//初始化光电开关IO引脚
	RCC->APB2ENR |= 1<<3;	//PB时钟
	RCC->APB2ENR |= 1<<0;	//AFIO时钟
	
	//PB0 PB1 上拉输入
	GPIOB->CRL &= ~0x000000FF;
	GPIOB->CRL |=  0x00000088;
	GPIOB->ODR |= 1<<1 | 1<<0;
	
	//外部中断线的端口设置
	AFIO->EXTICR[0] |= 0x0011;
	
	//外部中断的触发模式与使能
	EXTI->RTSR &= ~(1<<1 | 1<<0);
	EXTI->FTSR |= 1<<1 | 1<<0;
	EXTI->IMR |= 1<<1 | 1<<0;
	
	//外部中断线0的NVIC优先级
	nvicInit.NVIC_IRQChannel = EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInit.NVIC_IRQChannelSubPriority = 1;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvicInit);
	
	//外部中断线1的NVIC优先级
	nvicInit.NVIC_IRQChannel = EXTI1_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInit.NVIC_IRQChannelSubPriority = 2;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvicInit);
}

//返回对射开关的当前状态
u8 currSwitchStat(void)
{
	if( GPIOB->IDR & (1<<1))
		return 1;
	else
		return 0;
}

//返回反射开关的当前状态
u8 refSwitchStat(void)
{
	if( GPIOB->IDR & (1<<0))
		return 1;
	else
		return 0;
}

//=======================外部中断服务函数==========
void EXTI0_IRQHandler(void)
{
	_delay_ms(10);
	//响应并清除中断标志
	if( !(GPIOB->IDR & 0x01))
	{
		EXTI->PR |= 1<<0;
		putDataDma("EXTI0\r\n", 7);
	}
}

void EXTI1_IRQHandler(void)
{
	_delay_ms(10);
	//响应并清除中断标志
	if( !(GPIOB->IDR & 0x02))
	{
		EXTI->PR |= 1<<1;
		putDataDma("EXTI1\r\n", 7);
	}
}

