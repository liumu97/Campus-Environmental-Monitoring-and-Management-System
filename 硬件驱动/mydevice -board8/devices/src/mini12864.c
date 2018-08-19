/********************
* FileName: 	mini12864.c
* Description: 	
			操作函数接口实现
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#include "mini12864.h"
#include "briupDelay.h"

long fontaddr_IN=0;

//全屏清屏
void Clear_Screen_IN()
{
 unsigned char i,j;
 for(i=0;i<9;i++)
 {
 Transfer_Cmd_Lcd_IN(0xb0+i);
 Transfer_Cmd_Lcd_IN(0x10);
 Transfer_Cmd_Lcd_IN(0x00);
 for(j=0;j<132;j++)
 {
 Transfer_Data_Lcd_IN(0x00);
 }
 }
}

//打印GB2312的字符串
void showStrCn(u8 page,u8 column,u8 *text)
{
	u8 i= 0;
	//fontaddr_IN=0;
	while((text[i]>0x00))
	{
		
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
			{
				//国标简体（GB2312）汉字在晶联讯字库 IC 中的地址由以下公式来计算：
				//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
				//由于担心 8 位单片机有乘法溢出问题，所以分三部取地址
				fontaddr_IN = (text[i]- 0xb0)*94;
				fontaddr_IN += (text[i+1]-0xa1)+846;
				fontaddr_IN = (long)(fontaddr_IN*32);
				get_and_write_16x16_IN(fontaddr_IN,page,column) ; //从指定地址读出数据写到液晶屏指定（page,column)座标中
				i+=2;
				column+=16;
			}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1 ))
			{
				//国标简体（GB2312）15x16 点的字符在晶联讯字库 IC 中的地址由以下公式来计算：
				//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
				//由于担心 8 位单片机有乘法溢出问题，所以分三部取地址
				fontaddr_IN = (text[i]- 0xa1)*94;
				fontaddr_IN += (text[i+1]-0xa1);
				fontaddr_IN = (long)(fontaddr_IN*32);
				get_and_write_16x16_IN(fontaddr_IN,page,column) ; //从指定地址读出数据写到液晶屏指定（page,column)座标中
				i+=2;
				column+=16;
			}
		 else if((text[i]>=0x20) &&(text[i]<=0x7e))
		 {	 
				 fontaddr_IN = (text[i]- 0x20);
				 fontaddr_IN = (unsigned long)(fontaddr_IN*16);
				 fontaddr_IN = (unsigned long)(fontaddr_IN+0x3cf80);
				 get_and_write_8x16_IN(fontaddr_IN,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
				 i+=1;
				 column+=8;
		 }
		else
		 i++;
	}
}


//写指令到 LCD 模块
void Transfer_Cmd_Lcd_IN(int cmd)
{
	 u8 i;
	 Lcd_CS_L();
	 Lcd_RS_L();
	 for(i=0;i<8;i++)
		 {
			 Lcd_Sclk_L();
			 _delay_us(10); //加少量延时
			 if(cmd&0x80) 
					Lcd_Sid_H();
			 else 
				  Lcd_Sid_L();
			 Lcd_Sclk_H();
			 _delay_us(10); //加少量延时
			 cmd = cmd << 1;
		 }
	 Lcd_CS_H();
}


//写数据到 LCD 模块
void Transfer_Data_Lcd_IN(int data)
{
	 char i;

	 Lcd_CS_L();
	 Lcd_RS_H();
	 for(i=0;i<8;i++)
	 {
			Lcd_Sclk_L();
			if(data&0x80) Lcd_Sid_H();
			else Lcd_Sid_L();
			Lcd_Sclk_H();
			data = data<<1;
		}
	 Lcd_CS_H();
}


//LCD模块GPIO口初始化
void Lcd_GPIO_Iint_IN()
{
	RCC->APB2ENR |= 1<<3;//使能PORTB时钟
	GPIOB->CRL&=0X0000FFFF;
	GPIOB->CRL|=0X83330000;//PB4 5 6 推挽输出 7输入  
	GPIOB->CRH&=0X0000FFF0;
	GPIOB->CRH|=0X33330003;//PB8 12 13 14 15推挽输出
}



//LCD 模块初始化
void Lcd_Init_IN()
{
	 Lcd_GPIO_Iint_IN();//GPIO口初始化
	
	 Lcd_Reset_L();  //低电平复位
	 _delay_ms(100);
	 Lcd_Reset_H() ; //复位完毕
	 _delay_ms(100);
	 Transfer_Cmd_Lcd_IN(0xe2); //软复位
	 _delay_ms(5);
	 Transfer_Cmd_Lcd_IN(0x2c); //升压步聚 1
	 _delay_ms(50);
	 Transfer_Cmd_Lcd_IN(0x2e); //升压步聚 2
	 _delay_ms(50);
	 Transfer_Cmd_Lcd_IN(0x2f); //升压步聚 3
	 _delay_ms(5);
	 Transfer_Cmd_Lcd_IN(0x23);  //粗调对比度，可设置范围 0x20～0x27
	 Transfer_Cmd_Lcd_IN(0x81);  //微调对比度
	 Transfer_Cmd_Lcd_IN(0x28); //微调对比度的值，可设置范围 0x00～0x3f
	 Transfer_Cmd_Lcd_IN(0xa2);  //1/9 偏压比（bias）
	 Transfer_Cmd_Lcd_IN(0xc8);  //行扫描顺序：从上到下
	 Transfer_Cmd_Lcd_IN(0xa0);  //列扫描顺序：从左到右
	 Transfer_Cmd_Lcd_IN(0x40);  //起始行：第一行开始
	 Transfer_Cmd_Lcd_IN(0xaf);  //开显示
	 _delay_ms(100);
	 Clear_Screen_IN();
}


void Lcd_Address(u32 page,u32 column)
{
 column=column-0x01;
 Transfer_Cmd_Lcd_IN(0xb0+page-1); //设置页地址，每 8 行为一页，全屏共 64 行，被分成 8 页
 Transfer_Cmd_Lcd_IN(0x10+(column>>4&0x0f)); //设置列地址的高 4 位
 Transfer_Cmd_Lcd_IN(column&0x0f); //设置列地址的低 4 位
}


//显示 128x64 点阵图像
void display_128x64_IN(u8 *dp)
{
 u32 i,j;
 for(j=0;j<8;j++)
 {
 Lcd_Address(j+1,1);
 for (i=0;i<128;i++)
 {
 Transfer_Data_Lcd_IN(*dp); //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1
 dp++;
 }
 }
}



//送指令到晶联讯字库 IC
void send_command_to_ROM_IN( u8 datu )
{
 u8 i;
 for(i=0;i<8;i++ )
 {
 Rom_SCK_L();
 _delay_us(10);
 if(datu&0x80)Rom_IN_H();
 else Rom_IN_L();
 datu = datu<<1;
 Rom_SCK_H();
 _delay_us(10);
 }
}


//从晶联讯字库 IC 中取汉字或字符数据（1 个字节）
static u8 get_data_from_ROM_IN()
{
 u8 i;
 u8 ret_data=0;
 for(i=0;i<8;i++)
 {
 Rom_SCK_L();
 _delay_us(1);
 ret_data=ret_data<<1;
 if( Rom_OUT_IN )
 ret_data=ret_data+1;
 else
 ret_data=ret_data+0;
 Rom_SCK_H();
 _delay_us(1);
 }
 return(ret_data);
}


//从指定地址读出数据写到液晶屏指定（page, column)座标中
void get_and_write_16x16_IN(long fontaddr_IN,u8 page,u8 column)
{
 u8 i,j,disp_data;
 Rom_CS_L();
 send_command_to_ROM_IN(0x03);
 send_command_to_ROM_IN((fontaddr_IN&0xff0000)>>16); //地址的高 8 位,共 24 位
 send_command_to_ROM_IN((fontaddr_IN&0xff00)>>8) ; //地址的中 8 位,共 24 位
 send_command_to_ROM_IN(fontaddr_IN&0xff); //地址的低 8 位,共 24 位
 for(j=0;j<2;j++)
 {
 Lcd_Address(page+j,column);
 for(i=0; i<16; i++ )
 {
 disp_data=get_data_from_ROM_IN();
 Transfer_Data_Lcd_IN(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
 }
 }
 Rom_CS_H();
}


//从指定地址读出数据写到液晶屏指定（page, column)座标中
void get_and_write_8x16_IN(long fontaddr_IN,u8 page,u8 column)
{
 u8 i,j,disp_data;
 Rom_CS_L();
 send_command_to_ROM_IN(0x03);
 send_command_to_ROM_IN((fontaddr_IN&0xff0000)>>16); //地址的高 8 位,共 24 位
 send_command_to_ROM_IN((fontaddr_IN&0xff00)>>8) ; //地址的中 8 位,共 24 位
 send_command_to_ROM_IN(fontaddr_IN&0xff); //地址的低 8 位,共 24 位
 for(j=0;j<2;j++)

 {
 Lcd_Address(page+j,column);
 for(i=0; i<8; i++ )
 {
 disp_data=get_data_from_ROM_IN();
 Transfer_Data_Lcd_IN(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
 }
 }
 Rom_CS_H();
}
