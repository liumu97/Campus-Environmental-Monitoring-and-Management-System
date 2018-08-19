/***************************************************************************
*FileName:		is.h
*Description: 人体红外传感器初始化接口声明
*Author:			yhs
*CreatDate:		2018-7-14
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef _IS_H_
#define	_IS_H_

#include<stm32F10x.h>

//初始化人体红外传感器
void is_Init(void);

//返回人体红外传感器当前状态
u8 havahuman(void);

#endif

