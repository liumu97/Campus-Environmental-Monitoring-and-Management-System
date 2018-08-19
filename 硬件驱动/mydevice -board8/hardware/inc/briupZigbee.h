/********************************************************
*FileName:     briupZigbee.h
*Destription:  
*              Zigbee Usart2初始化的接口
*              通过DMA发送数据
*              通过Usart2中断的方式接收数据
*Author:      
*CreateDate:   2018-7-15
*Copyright     
*Update:       修改人    修改内容
*  2018-7-15     	       初始化函数声明
********************************************************/
#ifndef __BRIUPZIGBEE_H__
#define __BRIUPZIGBEE_H__

#include <stm32f10x.h>

//Zigbee串口的初始化
void zigbeeInit( u8 pclk, u32 baud);

//通过DMA发送一个buf数组
void zigbeeSendBuf( u8 * buf, u16 len);

//获取zigbee的数据
u16 zigbeeGetData( u8 * buf, u8 flag);



#endif






