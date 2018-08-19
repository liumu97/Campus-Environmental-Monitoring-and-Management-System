/********************
* FileName: 	briupTim.h
* Description: 	定时器初始化接口声明
* Author:		Dennis Chen
* CreateDate:	2018-7-6
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-6	陈哲	添加初始化接口声明
*********************/

#ifndef __TIM_H__
#define __TIM_H__

#include <stm32f10x.h>

//定时器初始化函数接口
// pclk 频率	arr 自动装载值
void timInit(u8 pclk, u16 arr);

#endif
