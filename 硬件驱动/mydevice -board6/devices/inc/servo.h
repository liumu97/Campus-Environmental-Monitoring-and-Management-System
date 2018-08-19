/********************
* FileName: 	servo.h
* Description: 	
			初始化舵机对应的定时器
			舵机的开启和关闭函数接口声明
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#ifndef __SERVO_H__
#define __SERVO_H__

#include <stm32f10x.h>

void Servo_Init(u16 arr, u16 psc);
//闸机打开
void Servo_ON(void);
//闸机关闭
void Servo_OFF(void);

#endif
