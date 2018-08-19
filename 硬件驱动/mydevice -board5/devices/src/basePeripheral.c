/***************************************************************************
*FileName:		basePeripheral.c
*Description: LED灯操作接口、
							蜂鸣器操作接口实现
*Author:			yhs
*CreatDate:		2018-7-11
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/

#include "basePeripheral.h"

//声明LED灯的接口操作
void baseLedSet(u8 led)
{
	if(led>7)
		return;
	switch( led)
	{
		case 0:
			GPIOA->ODR |= (1<<8 | 1<<11 | 1<<12);
			break;
		case LED_R:
			GPIOA->ODR |= (1<<11 | 1<<12);
			GPIOA->ODR &= ~(1<<8);
			break;
		case LED_G:
			GPIOA->ODR |= (1<<8 | 1<<12);
			GPIOA->ODR &= ~(1<<11);
			break;
		case LED_B:
			GPIOA->ODR |= (1<<8 | 1<<11);
			GPIOA->ODR &= ~(1<<12);
			break;
		case LED_YELLOW:
			GPIOA->ODR |= (1<<12);
			GPIOA->ODR &= ~(1<<8 | 1<<11);
			break;
		case LED_PURPLE:
			GPIOA->ODR |= (1<<11);
			GPIOA->ODR &= ~(1<<8 | 1<<12);
			break;
		case LED_CYAN:
			GPIOA->ODR |= (1<<8);
			GPIOA->ODR &= ~(1<<11 | 1<<12);
			break;
		case LED_WHITE:
			GPIOA->ODR &= ~(1<<8 | 1<<11 | 1<<12);
			break;
	}
}


//声明BEEP接口操作
void baseBeepSet(u8 set)
{
	if(set !=0)
		GPIOA->ODR |= 1<<15;
	else
		GPIOA->ODR &= ~(1<<15);
}
