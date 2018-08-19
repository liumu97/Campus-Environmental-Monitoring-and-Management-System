/***************************************************************************
*FileName:		briupGPIO.c
*Description: 基础IO的初始化接口实现
*Author:			yhs
*CreatDate:		2018-7-11
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "briupGPIO.h"

//基础GPIO初始化引脚函数接口
void briupGPIOInit(void)
{
	//三色LED灯 				蜂鸣器BEEP
	//PA8 PA11 PA12		PA15
	
	//开启外设GPIOA、AFIO的时钟
	RCC->APB2ENR |= 1<<2 | 1<<0;
	//修改PA15的复用功能,使PA15用作通用功能
	AFIO->MAPR |= 0x02<<24;
	
	//将4个引脚配置为通用推挽输出50MHz模式
	GPIOA->CRH &= 0x0FF00FF0;
	GPIOA->CRH |= 0x30033003;
	
	//默认关闭LED和蜂鸣器
	GPIOA->ODR &= ~(1<<15);
	GPIOA->ODR |= 1<<8 | 1<<11 |1<<12;
	
	
}
