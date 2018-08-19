/********************
* FileName: 	pm2_5.c
* Description: 	PM2.5检测接口
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#include "pm2_5.h"
#include "briupUsart3.h"

//返回值为PM2.5的值 ug/m3
u16 pm25Value(void)
{
	u16 len, pm25 = 0;
	u8 buf[300];
	len = usart3GetBuffer( buf);
	if( len != 0)
		pm25 = buf[2] | (buf[3]<<8);
	return pm25;
}
