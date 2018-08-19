/***************************************************************************
*FileName:		warn.c
*Description: 报警灯PB9、报警器PB8 初始化接口实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "warn.h"
#include "briupDelay.h"

//初始化报警灯报警器接口
void warnInit(void)
{
	//开启GPIOB的时钟
	RCC->APB2ENR |= 1<<3 | 1<<0;
	
	//将PB8、PB9设置成推挽输出50MHz模式
	GPIOB->CRH &= 0xFFFFFF00;
	GPIOB->CRH |= 0x00000033;
	
	//默认关闭报警灯和报警器
	GPIOB->ODR &= ~(1<<9|1<<8); 
}

//开启报警灯
void warnLed(void)
{
	GPIOB->ODR |= 1<<9;
	
}
//关闭报警灯
void closeWarnLed(void)
{
	GPIOB->ODR &= ~(1<<9);
}
	
//开启报警器
void warn(void)
{
	GPIOB->ODR |= 1<<8;
}

//关闭报警器
void closeWarn(void)
{
		GPIOB->ODR &= ~(1<<8);
}


