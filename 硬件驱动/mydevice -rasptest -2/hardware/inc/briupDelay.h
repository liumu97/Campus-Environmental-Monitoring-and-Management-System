/***************************************************************************
*FileName:		briupDelay.h
*Description: 通过SysTick定时器实现延时功能
							包括延时的初始化函数接口
							按us 微秒延时、按ms 毫秒延时、按sec 秒延时
*Author:			yhs
*CreatDate:		2018-7-12
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数声明
****************************************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

#include <stm32f10x.h>

//延时函数的初始化接口
//Input: pclk->芯片工作时钟
void _delay_init(u32 pclk);

//延时函数
void _delay_us(u32 us);
void _delay_ms(u32 ms);
void _delay_sec(double sec);

#endif
