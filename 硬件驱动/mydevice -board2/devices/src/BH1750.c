/***************************************************************************
*FileName:		BH1750.c
*Description: 光照强度传感器的接口实现
*Author:			yhs
*CreatDate:		2018-7-18
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#include "BH1750.h"
#include "briupI2C.h"

/*******************************
函数名：BH1750_Init
函数功能：初始化BH1750
函数返回值：成功返回0
*******************************/
void BH1750_Init(void)
{
	Sing_Write_ND(BH1750_ADDR, BH1750_PWON);		//通电
	Sing_Write_ND(BH1750_ADDR, BH1750_RESET);		//复位
	Sing_Write_ND(BH1750_ADDR, BH1750_CMT_H);	
	Sing_Write_ND(BH1750_ADDR, BH1750_CMT_L);			//设置透光率为100%
	Sing_Write_ND(BH1750_ADDR, BH1750_H_RE_MOD2);	//设置为高精度模式
}	
/*******************************
函数名：BH1750_Init
函数功能：初始化BH1750
函数返回值：成功返回0
*******************************/
void BH1750_Start(void)
{
	Sing_Write_ND(BH1750_ADDR, BH1750_PWON);		//通电
	Sing_Write_ND(BH1750_ADDR, BH1750_RESET);		//复位
	Sing_Write_ND(BH1750_ADDR, BH1750_CMT_H);	
	Sing_Write_ND(BH1750_ADDR, BH1750_CMT_L);			//设置透光率为100%
	Sing_Write_ND(BH1750_ADDR, BH1750_H_RE_MOD2);	//设置为高精度模式
}	

/*******************************
函数名：BH1750_Measure
函数功能：获取一次BH1750的测量值
函数返回值：成功返回0
注意：每次读取光照强度数据至少要间隔120ms
			在高精度模式下传感器转换一次数据需要至少120ms时间
*******************************/
unsigned char BH1750_Measure(float* Result)
{
	unsigned char temp[2]={0};
	u16 ST = 0;
	if(Sing_Read_NR(BH1750_ADDR, 2, temp))
	 {
		ST = (temp[0]<<8)|temp[1];
		*Result = (u16)ST / 0.6;
	 }
	else
	{
		*Result = 0;
		return 1;
	}
	return 0;
}

/*******************************
函数名：BH1750_Measure
函数功能：获取一次BH1750的原始测量值
函数返回值：成功返回0
注意：每次读取光照强度数据至少要间隔120ms
			在高精度模式下传感器转换一次数据需要至少120ms时间
*******************************/
unsigned char BH1750_Original_Measure(u16* Result)
{
	unsigned char temp[2]={0};
	//u16 ST = 0;
	if(Sing_Read_NR(BH1750_ADDR, 2, temp))
	 {
		* Result = (temp[0]<<8)|temp[1];
	 }
	else
	{
		*Result = 0;
		return 1;
	}
	return 0;
}








