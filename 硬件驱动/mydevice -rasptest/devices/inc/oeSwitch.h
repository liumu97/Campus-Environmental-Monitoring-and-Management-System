/***************************************************************************
*FileName:		oeSwitch.h
*Description: 光电开关的初始化函数接口声明
							通过轮询的方式返回光电开关状态
							通过中断的方式检测光电开关状态
*Author:			yhs
*CreatDate:		2018-7-13
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef __OESWITCH_H__
#define	__OESWITCH_H__

#include <stm32f10x.h>

//光电开关的初始化
void oeSwitchInit(void);
	
//返回对射开关的当前状态
u8 currSwitchStat(void);

//返回反射开关的当前状态
u8 refSwitchStat(void);


#endif

