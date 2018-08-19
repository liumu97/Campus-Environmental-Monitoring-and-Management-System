/***************************************************************************
*FileName:		bruipUSART.h
*Description: 声明初始化Dubug串口 -> usart1 的函数接口
							声明串口发送数据：ch / str  /buf
							声明串口接收数据：ch / str
*Author:			yhs
*CreatDate:		2018-7-12
*Copyright		www.yanghs97@163.com
*UpDate:			修改人		修改内容
*2018-7-11		杨淏森	 	添加初始化函数实现
****************************************************************************/
#ifndef __USART1_H_
#define __USART1_H_

#include <stm32f10x.h>

//初始化usart1的函数接口
// pclk：工作时钟
// baud： 要设置的波特率
void usart1Init(u8 pclk, u32 baud);

//发送函数声明
void putChar( u8 ch);
void putStr( char * str);
void putBuffer( u8 * buf,u16 length);

//接收函数声明
u8 getChar( void);
u8 getStr(char * str);

//通过循环队列获取一段数据
// flag: 
//			0 直接获取最近接收到的数据
//			1 判断获取到完整的一帧协议数据再返回
u16 getBuffer( u8 *buf, u8 flag);

//通过DMA发送一段数据
void putDataDma( u8 * buf, u16 len);


#endif



