/***************************************************************************
*FileName:		fire.h
*Description: 烟雾PB6、火光PB0、甲烷PB1传感器初始化接口声明
							通过查询方式实现
							通过中断方式实现
*Author:			yhs
*CreatDate:		2018-7-15
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef	__FIRE_H_
#define	__FIRE_H_
#include <stm32f10x.h>

//初始化烟雾、火光、甲烷传感器函数
void fireInit(void);

//获取烟雾传感器状态
u8 fogStat(void);
//获取火光传感器状态
u8 fireStat(void);
//获取甲烷传感器状态
u8 CH4Stat(void);
//监控烟雾
void fogMonitor(void);
//监控火光
void fireMonitor(void);
//监控甲烷
void CH4Monitor(void);


#endif

