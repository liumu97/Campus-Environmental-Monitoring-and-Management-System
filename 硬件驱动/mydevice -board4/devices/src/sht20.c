/********************
* FileName: 	sht20.c
* Description: 	
			温湿度检测函数实现文件
* Author:		Dennis Chen
* CreateDate:	2018-7-9
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-9	陈哲	添加初始化接口声明
*********************/
#include "briupI2C.h"
#include "sht20.h"

#define SHT20_ADDR	0X80	//器件地址
#define SHT20_RT		0XF3	//读温度
#define SHT20_RH		0XF5	//读湿度
/*******************************
函数名：SHT20_Measure
函数功能：获取温湿度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Measure(float* Temperature, float* Humidity)
{
	unsigned char temp[2]={0};
	u16 ST=0;
	if(!Single_Read_Len(SHT20_ADDR,SHT20_RT,2,temp))
	{
		ST = (temp[0]<<8)|temp[1];
		ST&=~0X0003;
		*Temperature = ((float)ST * 0.00268127)-46.85;
	}
	else
	{
		*Temperature = 0;
		return 1;
	}
/******************获取湿度***************/
	if(!Single_Read_Len(SHT20_ADDR,SHT20_RH,2,temp))
	{
		ST = (temp[0]<<8)|temp[1];
		ST&=~0X0003;
		*Humidity = ((float)ST * 0.00190735)-6;
	}
	else
	{
		*Humidity = 0;
		return 1;
	}
	return 0;
}
/*******************************
函数名：SHT20_Original_TEMP
函数功能：获取原始温度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Original_TEMP(u16* Temperature)
{
	unsigned char temp[2]={0};

	if(!Single_Read_Len(SHT20_ADDR,SHT20_RT,2,temp))
	{
		*Temperature = (temp[0]<<8)|temp[1];
		*Temperature&=~0X0003;
	}
	else
	{
		*Temperature = 0;
		return 1;
	}
	return 0;
}
/*******************************
函数名：SHT20_Original_HUM
函数功能：获取原始湿度数据
函数返回值：成功返回0
*******************************/
unsigned char SHT20_Original_HUM(u16* Humidity)
{
	unsigned char temp[2]={0};

/******************获取湿度***************/
	if(!Single_Read_Len(SHT20_ADDR,SHT20_RH,2,temp))
	{
		*Humidity = (temp[0]<<8)|temp[1];
		*Humidity&=~0X0003;
	}
	else
	{
		*Humidity = 0;
		return 1;
	}
	return 0;
}

