/********************
* FileName: 	mini12864.h
* Description: 	
			液晶屏引脚声明
			操作函数接口声明
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#ifndef __MINI12864_H__
#define __MINI12864_H__

#include <stm32f10x.h>

#define Rom_OUT_IN		((GPIOB->IDR & (1<<7))>>7)	//字库 IC 接口定义:Rom_OUT 就是字库 IC 的 SO

#define Lcd_Sclk_H()	GPIOB->ODR |= (1<<14) 			//接口定义:Lcd_Sclk 就是 LCD 的 SCLK
#define Lcd_Sid_H() 	GPIOB->ODR |= (1<<13)				//接口定义:Lcd_Sid 就是 LCD 的 SDA
#define Lcd_RS_H()		GPIOB->ODR |= (1<<12)			//接口定义:Lcd_RS 就是 LCD 的 RS,也叫“CD”
#define Lcd_CS_H()		GPIOB->ODR |= (1<<15) 			//接口定义:Lcd_CS 就是 LCD 的 CS1
#define Lcd_Reset_H()	GPIOB->ODR |= (1<<8)				//接口定义:Lcd_Reset 就是 LCD 的 RESET
#define Rom_IN_H()		GPIOB->ODR |= (1<<6) 			//字库 IC 接口定义:Rom_IN 就是字库 IC 的 SI
#define Rom_SCK_H()		GPIOB->ODR |= (1<<4)  		//字库 IC 接口定义:Rom_SCK 就是字库 IC 的 SCK
#define Rom_CS_H()		GPIOB->ODR |= (1<<5)			//字库 IC 接口定义 Rom_CS 就是字库 IC 的 CS

#define Lcd_Sclk_L()	GPIOB->ODR &= ~(1<<14) 			//接口定义:Lcd_Sclk 就是 LCD 的 SCLK
#define Lcd_Sid_L() 	GPIOB->ODR &= ~(1<<13)				//接口定义:Lcd_Sid 就是 LCD 的 SDA
#define Lcd_RS_L()		GPIOB->ODR &= ~(1<<12)			//接口定义:Lcd_RS 就是 LCD 的 RS,也叫“CD”
#define Lcd_CS_L()		GPIOB->ODR &= ~(1<<15) 			//接口定义:Lcd_CS 就是 LCD 的 CS1
#define Lcd_Reset_L()	GPIOB->ODR &= ~(1<<8)				//接口定义:Lcd_Reset 就是 LCD 的 RESET
#define Rom_IN_L()		GPIOB->ODR &= ~(1<<6) 			//字库 IC 接口定义:Rom_IN 就是字库 IC 的 SI
#define Rom_SCK_L()		GPIOB->ODR &= ~(1<<4)  		//字库 IC 接口定义:Rom_SCK 就是字库 IC 的 SCK
#define Rom_CS_L()		GPIOB->ODR &= ~(1<<5)			//字库 IC 接口定义 Rom_CS 就是字库 IC 的 CS


//=====================================================================================
//
//	入库显示屏函数声明
//
//=====================================================================================

//打印GB2312的字符串
void showStrCn(u8 page,u8 column,u8 *text);
//全屏清屏
void Clear_Screen_IN(void);

//LCD模块GPIO口初始化
void Lcd_GPIO_Iint_IN(void);
//LCD 模块初始化
void Lcd_Init_IN(void);
//写指令到LCD 模块
void Transfer_Cmd_Lcd_IN(int cmd);
//写数据到 LCD 模块
void Transfer_Data_Lcd_IN(int data);
//LCD 数据地址设置
void Lcd_Address_IN(u32 page,u32 column);
//显示 128x64 点阵图像
void display_128x64_IN(u8 *dp);


//送指令到晶联讯字库 IC
void send_command_to_ROM_IN( u8 datu );
//从晶联讯字库 IC 中取汉字或字符数据（1 个字节）
static u8 get_data_from_ROM_IN(void);
//从指定地址读出数据写到液晶屏指定（page, column)座标中
void get_and_write_16x16_IN(long fontaddr,u8 page,u8 column);
//从指定地址读出数据写到液晶屏指定（page, column)座标中
void get_and_write_8x16_IN(long fontaddr,u8 page,u8 column);


#endif
