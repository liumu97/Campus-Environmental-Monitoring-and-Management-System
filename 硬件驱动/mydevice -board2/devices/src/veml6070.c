/********************
* FileName: 	veml6070.c
* Description: 	
			紫外线强度检测函数声明文件
* Author:		Dennis Chen
* CreateDate:	2018-7-9
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-9	陈哲	添加初始化接口声明
*********************/
#include "veml6070.h"
#include "briupI2C.h"


#define VEML6070_ADDR	0X70	//器件地址
#define VEML6070_DH		0X73	//读温度
#define VEML6070_DL		0X71	//读湿度
/*******************************
函数名：VEML6070_Set
函数功能：初始化紫外线传感器
函数返回值：成功返回0
*******************************/
unsigned char VEML6070Init(void)
{
	IIC_Start();
  IIC_Send_Byte(0X70);
  if (IIC_Wait_Ack()) 
		{
      IIC_Stop();
      return 1;
		}
	IIC_Send_Byte(0X0E);//此处模式配置，关闭ACK功能 IT配置为4T
	if (IIC_Wait_Ack()) 
		{
      IIC_Stop();
      return 1;
		}
	IIC_Stop();
	return  0;
}
/*******************************
函数名：VEML6070_Measure
函数功能：获取紫外线强度
函数返回值：成功返回0
*******************************/
unsigned char VEML6070Measure(u16* Result)
{
	u8 tempH,tempL;
	/*********获取高8位数据*********/
	IIC_Start();
  IIC_Send_Byte(0X73);
  if (IIC_Wait_Ack()) 
		{
      IIC_Stop();
      return 1;
		}
	tempH=IIC_Read_Byte(1);//读取总线数据
	IIC_Stop();
	/*********获取低8位数据*********/
	IIC_Start();
  IIC_Send_Byte(0X71);
  if (IIC_Wait_Ack()) 
		{
      IIC_Stop();
      return 1;
		}
	tempL=IIC_Read_Byte(1);//读取总线数据
	IIC_Stop();
	*Result = (tempH<<8)|tempL;
	return 0;
}


