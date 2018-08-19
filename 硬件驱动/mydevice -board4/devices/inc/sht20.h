/********************
* FileName: 	sht20.h
* Description: 	
			温湿度检测函数声明文件
* Author:		Dennis Chen
* CreateDate:	2018-7-9
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-9	陈哲	添加初始化接口声明
*********************/
#ifndef __SHT20_H__
#define __SHT20_H__

#include <stm32f10x.h>

/*******************************
函数名：SHT20_Measure
函数功能：获取温湿度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Measure(float* Temperature, float* Humidity);
/*******************************
函数名：SHT20_Original_TEMP
函数功能：获取原始温度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Original_TEMP(u16* Temperature);
/*******************************
函数名：SHT20_Original_HUM
函数功能：获取原始湿度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Original_HUM(u16* Humidity);


#endif
