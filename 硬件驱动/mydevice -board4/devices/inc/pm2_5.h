/********************
* FileName: 	pm2_5.h
* Description: 	PM2.5检测接口
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#ifndef __PM2_5_H__
#define __PM2_5_H__

#include <stm32f10x.h>

//获取PM2.5的值
u16	pm25Value(void);

#endif
