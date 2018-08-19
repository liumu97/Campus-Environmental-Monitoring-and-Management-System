/*******************************************************
器件型号：KS6812 (WS2812B与之操作类似)
通信方式：SPI(单线发送)+DMA
文件功能：控制KS6812显示
传输数据顺序：G R  B 即先传出绿色 再传出红色 再传出蓝色
*******************************************************/

#include "WS2812B.h"
#include <string.h>

PixColor pixBuf[4][4] = {0};

#define WS_ONE		0xFFC0	//10 H    6L
#define WS_ZERO		0xF000	// 4 H	 12L

u16 PixelBuffer[485];//100+16*24+1 0~99永远为0 用于初始化 1636永远为0 用于传输完成拉低总线

void WS2812B_Init(void)
{
	u16 i;
	//SPI配置
	
	RCC->APB2ENR|=1<<12;//SPI1时钟使能 
	RCC->APB2ENR|=1<<2; //PROTA时钟使能
	RCC->AHBENR|=1<<0;  //DMA1时钟使能
	
	GPIOA->CRL&=0X0FFFFFFF;		 
	GPIOA->CRL|=0XB0000000;		 //PA7 复用 
	
	//GPIOA->ODR|=0X7<<5;			 //初始化高电平
	
	SPI1->CR1|=1<<15;
	SPI1->CR1|=1<<14;//单线只发模式
	SPI1->CR1|=1<<11;//16位数据格式
	//SPI1->CR1|=1<<10;
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  
	SPI1->CR1|=0<<7; //MSBfirst 
	SPI1->CR1|=1<<3; //Fsck=Fcpu/4
	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1&=~0x02;//空闲模式下SCK为0 CPOL=0
	SPI1->CR1&=~0x01;//数据采样从第一个时间边沿开始,CPHA=0  

  SPI1->CR2|=1<<1;//启动发送缓冲区DMA 
	SPI1->CR1|=1<<6; //SPI设备使能
	
	DMA1_Channel3->CCR|=1<<12;//中优先级
  DMA1_Channel3->CCR|=1<<10;//存储器数据宽度为16位
	DMA1_Channel3->CCR|=1<<8;//外设数据宽度为16位
	DMA1_Channel3->CCR|=1<<7;//存储器地址增量模式
	DMA1_Channel3->CCR|=1<<4;//从存储器读
	DMA1_Channel3->CNDTR=0;//传输数据量为0
	DMA1_Channel3->CPAR = (u32)&(SPI1->DR);//外设地址
	DMA1_Channel3->CMAR = (u32)(PixelBuffer);//存储器地址
	for(i=0;i<485;i++)
	{
		PixelBuffer[i]=0X0000;
	}
}


//发送设置好的LED灯颜色数据
//	直接设置指定行列值的LED颜色，
//	将结构体数组中的数据 装填到颜色数组中
//	将颜色数组通过DMA发送
void  ws2812SendData(void)
{
	//结构体 G R B -> color
	u16 i,j,k, flag = 100;
	//初始化颜色数组
	for( i = 0;i < 485;i ++)
		PixelBuffer[i] = 0;
	
	for( i = 0;i < 4;i ++)
	{
		for( j = 0;j < 4;j ++)
		{
			pixBuf[i][j].color = pixBuf[i][j].green <<16;
			pixBuf[i][j].color |= pixBuf[i][j].red << 8;
			pixBuf[i][j].color |= pixBuf[i][j].blue;
			
			for( k = 0;k < 24;k ++)
			{
				//PixelBuffer[flag + (i*4+j)*24 + k]
				if( pixBuf[i][j].color & (1<<23))
				{
					PixelBuffer[flag + (i*4+j)*24 + k] = WS_ONE;
				}
				else
					PixelBuffer[flag + (i*4+j)*24 + k] = WS_ZERO;
				pixBuf[i][j].color <<= 1;
			}
		}
	}
	WS2812B_EN();
}


void WS2812B_EN(void)
{
	DMA1_Channel3->CCR&=~(1<<0);//关闭DMA1通道3
	DMA1_Channel3->CNDTR = 485;//初始化计数器的值
	DMA1_Channel3->CCR|=1<<0;//使能DMA传输
}
/*********************************************
函数名     WS2812B_Pixel_EN
函数功能：根据起始坐标点点亮LED
输入参数: x,y为起始坐标点 X为列 Y为行
					len：以起始坐标点开始（包括）要点亮LED的个数。
					RGB_Value:将要显示的颜色 bit0-7蓝色 bit8-15红色 bit16-23绿色
					mode：显示模式 
*********************************************/
void WS2812B_Pixel_EN(u8 x,u8 y,u8 len,u32* RGB_Value,u8 mode)
{
	u16 Pixel_Point = 0;
	u16 One=0XFFC0;
	u16 Zero=0XF000;
	u32 i=0;
	Pixel_Point = (Y_LEN*y+x)*24+100;
	while(len--)
	{
		for(i=0X800000;i>=0X01;i>>=1)
		{
			PixelBuffer[Pixel_Point]= (*RGB_Value)&i ? One : Zero;
			Pixel_Point++;
		}
		if(mode == 1)
			RGB_Value++;
	}
	WS2812B_EN();
}
/*********************************************
函数名     WS2812B_Clear
函数功能： 熄灭所有的LED
*********************************************/
void WS2812B_Clear(void)
{
	u16 i;
	memset( pixBuf, 0, sizeof( pixBuf));
	for(i=0;i<485;i++)
	{
		PixelBuffer[i+100]=0XF000;
	}
	WS2812B_EN();
}



