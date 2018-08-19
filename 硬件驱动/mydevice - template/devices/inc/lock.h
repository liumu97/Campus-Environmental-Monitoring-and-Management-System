/***************************************************************************
*FileName:		lock.h
*Description: 电磁锁接口声明
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef __LOCK_H_
#define __LOCK_H_
#include <stm32f10x.h>

//初始化电磁锁
void lockInit(void);

//开关电磁锁
void lock(u16 key);

#endif

