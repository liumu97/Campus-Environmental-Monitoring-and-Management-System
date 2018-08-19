/***************************************************************************
*FileName:		basePeripheral.h
*Description: 三色LED灯的颜色宏定义
							LED灯操作接口
							蜂鸣器操作接口声明
*Author:			yhs
*CreatDate:		2018-7-11
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数声明
****************************************************************************/
#ifndef __BASEPERIPH_H__
#define __BASEPERIPH_H__

#include <stm32f10x.h>

//R G B
#define LED_R				1
#define LED_G				2
#define LED_B				3
#define LED_YELLOW	4
#define LED_PURPLE	5
#define LED_CYAN		6
#define LED_WHITE		7
#define LED_BLACK		0

//声明LED灯的操作接口
void baseLedSet(u8 led);

//声明BEEP的操作接口
void baseBeepSet(u8 set);

#endif

