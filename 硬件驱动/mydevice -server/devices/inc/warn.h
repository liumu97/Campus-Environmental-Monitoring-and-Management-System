/***************************************************************************
*FileName:		warn.h
*Description: 报警灯PB9、报警器PB8 初始化接口声明
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef __WARN_H_
#define __WARN_H_
#include <stm32f10x.h>

//初始化报警灯报警器接口
void warnInit(void);

//开启报警灯
void warnLed(void);
	
//开启报警器
void warn(void);

#endif

