/***************************************************************************
*FileName:		fansRelay.c
*Description: 风扇和继电器接口实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "fansRelay.h"

//初始化风扇和继电器  风扇：PA6 继电器1：PA5 继电器2：PA4
void fanRelayInit(void)
{
	//开启GPIOA的时钟
	RCC->APB2ENR |= 1<<2;
	
	//使能PA6、PA5、PA4推挽输出50MHz模式
	GPIOA->CRL &= 0xF000FFFF;
	GPIOA->CRL |= 0x03330000;
	
	//默认关闭风扇，继电器
	GPIOA->ODR &= ~(1<<4| 1<<5 | 1<<6);
	
}

//控制风扇 key:1-开 0-关
void fan(u8 key)
{
	if(key == 0)
		GPIOA->ODR &= ~(1<<6);
	else
		GPIOA->ODR |= 1<<6;
}

//控制继电器1 key:1-开 0-关
void relay1(u8 key)
{
	if(key == 0)
		GPIOA->ODR &= ~(1<<5);
	else
		GPIOA->ODR |= 1<<5;
}

//控制继电器2 key:1-开 0-关
void relay2(u8 key)
{
	if(key == 0)
		GPIOA->ODR &= ~(1<<4);
	else
		GPIOA->ODR |= 1<<4;
}


