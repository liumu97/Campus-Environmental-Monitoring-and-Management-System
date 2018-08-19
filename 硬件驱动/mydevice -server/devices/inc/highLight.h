/********************
* FileName: 	highLight.h
* Description: 	驱动高亮LED灯的函数接口声明
* Author:		Dennis Chen
* CreateDate:	2018-7-6
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-6	陈哲
*********************/

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stm32f10x.h>

#define ROW_1	0x01
#define ROW_2	0x02
#define ROW_3	0x04
#define ROW_4	0x08
#define ROW_ALL 0x0F
//高亮LED亮度设置
//	row 指定行		ratio 亮度百分比
void lightSet( u8 row, u8 ratio);

#endif
