/***************************************************************************
*FileName:		CO2.h
*Description: CO2传感器的接口声明
*Author:			yhs
*CreatDate:		2018-7-18
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef __CO2_H_
#define __CO2_H_
#include <stm32f10x.h>

void CO2_Init(void);
void CO2_Measure(u8* CO2_Value_H, u8* CO2_Value_L);

#endif

