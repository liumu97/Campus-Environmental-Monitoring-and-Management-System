/***************************************************************************
*FileName:		briupDelay.c
*Description: 通过SysTick定时器实现延时功能
							包括延时的初始化函数接口
							按us 微秒延时、按ms 毫秒延时、按sec 秒延时
*Author:			yhs
*CreatDate:		2018-7-12
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "briupDelay.h"

//微秒和毫秒对应的计数次数
static u16 frac_us,frac_ms;

//延时函数的初始化接口
//Input: pclk->芯片工作时钟（MHz）
void _delay_init(u32 pclk)
{
		//配置定时器
	SysTick->CTRL = 0;
	frac_us=pclk/8;
	frac_ms=frac_us*1000;

}

//延时函数
void _delay_us(u32 us)
{
	volatile u32 temp = 0;
	//关闭定时器
	SysTick->CTRL &= ~(1<<0);
	//设置重装载值
	SysTick->LOAD=us;
	//清空当前计数值
	SysTick->VAL = 0;
	//打开定时器
	SysTick->CTRL |= 1<<0;
	
	do
	{
		temp=SysTick->CTRL;
	}while(!(temp & (1<<16)) && (temp & 0x01));
	//关闭SysTick定时器
	SysTick->CTRL &= ~0x01;
}	

void _delay_ms(u32 ms)
{
	volatile u32 temp = 0;
	if( ms > 1800)
		ms = 1800;
	ms *= frac_ms;
	//关闭定时器
	SysTick->CTRL &= ~(1<<0);
	//设置重装载值
	SysTick->LOAD=ms;
	//清空当前计数值
	SysTick->VAL = 0;
	//打开定时器
	SysTick->CTRL |= 1<<0;
	
	do
	{
		temp=SysTick->CTRL;
	}while(!(temp & (1<<16)) && (temp & 0x01));
	//关闭SysTick定时器
	SysTick->CTRL &= ~0x01;
}

//传入的sec保留两位小数
// N sec -> N*1000ms
void _delay_sec(double sec)
{
	u32 count = sec * 1000.0;
	while(count>1800)
	{
		_delay_ms(1800);
		count -= 1800;
	}
	_delay_ms(count);
}

