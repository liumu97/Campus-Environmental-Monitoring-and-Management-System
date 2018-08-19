/***************************************************************************
*FileName:		lock.c
*Description: 电磁锁接口实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "lock.h"
#include "briupDelay.h"

//初始化电磁锁 PB9 高电平有效
void lockInit(void)
{
	//初始化GPIOB时钟
	RCC->APB2ENR |= 1<<3;
	
	//将PB9配置为通用推挽输出50MHz
	GPIOB->CRH &= 0xFFFFFF0F;
	GPIOB->CRH |= 0x00000030;
	
	//默认为低电平
	GPIOB->ODR &= ~(1<<9);
}

//开关电磁锁 key=1 开门 key=0 关门
void lock(u16 key)
{
	if(key==1)
	{
		GPIOB->ODR |= 1<<9;
		_delay_sec(2);
		GPIOB->ODR &= ~(1<<9);
	}	
	else
		GPIOB->ODR &= ~(1<<9);
}



