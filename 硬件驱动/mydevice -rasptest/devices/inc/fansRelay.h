/***************************************************************************
*FileName:		fansRelay.h
*Description: 风扇和继电器接口声明
							通过查询方式实现
							通过中断方式实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef	__FANSRELAY_H_
#define __FANSRELAY_H_
#include <stm32f10x.h>

//初始化风扇和继电器
void fanRelayInit(void);

//控制风扇 key:1-开 0-关
void fan(u16 key);

//控制继电器1 key:1-开 0-关
void relay1(u16 key);

//控制继电器2 key:1-开 0-关
void relay2(u16 key);

#endif

