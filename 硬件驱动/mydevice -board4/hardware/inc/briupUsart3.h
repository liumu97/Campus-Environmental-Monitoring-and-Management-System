/********************
* FileName: 	briupUsart3.h
* Description: 	
			串口初始化的接口
			接收：按Byte接收、接收一个Str；
			发送：按Byte发送、发送一个Str、发送一个buffer;
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加接口声明
*********************/
#ifndef __USART3_H__
#define __USART3_H__

#include <stm32f10x.h>

//串口3初始化函数接口
void usart3Init( u8 pclk, u32 baud);

//从缓冲区获取最近一次收到的数据
u16 usart3GetBuffer(u8 * buf);

#endif
