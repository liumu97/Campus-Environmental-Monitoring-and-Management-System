/********************
* FileName: 	highLight.c
* Description: 	驱动高亮LED灯的函数接口
* Author:		Dennis Chen
* CreateDate:	2018-7-6
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-6	陈哲
*********************/

#include "highLight.h"
//高亮LED亮度设置
//	row 指定行		ratio 亮度百分比
void lightSet( u8 row, u8 ratio)
{
	row &= 0x0f;
	ratio = ratio > 100 ? 100:ratio;
	if( row & ROW_1)
		TIM3->CCR1 = ratio * 10;
	
	if( row & ROW_2)
		TIM4->CCR1 = ratio * 10;
	
	if( row & ROW_3)
		TIM3->CCR3 = ratio * 10;
	
	if( row & ROW_4)
		TIM3->CCR4 = ratio * 10;
}


