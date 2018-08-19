/********************
* FileName: 	veml6070.h
* Description: 	
			紫外线强度检测函数声明文件
* Author:		Dennis Chen
* CreateDate:	2018-7-9
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-9	陈哲	添加初始化接口声明
*********************/
#ifndef __VEML6070_H__
#define __VEML6070_H__

#include <stm32f10x.h>

/*******************************
函数名：VEML6070_Set
函数功能：	初始化紫外线强度传感器
函数返回值：成功返回0
*******************************/
unsigned char VEML6070Init(void);

/*******************************
函数名：VEML6070_Measure
函数功能：获取紫外线强度数据
函数返回值：成功返回0
*******************************/
unsigned char VEML6070Measure(u16* Result);

#endif
