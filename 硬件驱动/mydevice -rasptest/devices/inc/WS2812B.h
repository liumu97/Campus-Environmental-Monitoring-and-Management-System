#ifndef __WS2812B_H
#define __WS2812B_H

#include <stm32f10x.h>

#define X_LEN			4	//矩阵LEDX长度
#define Y_LEN			4 //矩阵LEDY长度

//声明单个炫彩LED灯对应的结构体
typedef struct
{
	u8	green;
	u8	red;
	u8	blue;
	u32 color;
}PixColor;

extern PixColor pixBuf[4][4];

//发送设置好的LED灯颜色数据
void  ws2812SendData(void);

//炫彩LED灯的初始化接口
//	初始化SPI对应的RCC、IO和SPI寄存器
void WS2812B_Init(void);
//开启一次DMA传输，将炫彩LED灯的数据缓冲器
//里的数据传输给SPI的DR
void WS2812B_EN(void);
/*********************************************
函数名     WS2812B_Pixel_EN
函数功能：根据起始坐标点点亮LED
输入参数: x,y为起始坐标点 X为列 Y为行
					len：以起始坐标点开始（包括）要点亮LED的个数。
					RGB_Value:将要显示的颜色 bit0-7蓝色 bit8-15红色 bit16-23绿色
					mode：显示模式 
*********************************************/
void WS2812B_Pixel_EN(u8 x,u8 y,u8 len,u32* RGB_Value,u8 mode);
/*********************************************
函数名     WS2812B_Clear
函数功能： 熄灭所有的LED
*********************************************/
void WS2812B_Clear(void);
#endif
