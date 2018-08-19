/********************
* FileName: 	briupI2C.c
* Description: 	
		软件模拟i2c (iic) 通信的
		接口初始化和通信功能实现函数
* Author:		Dennis Chen
* CreateDate:	2018-7-9
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-9	陈哲	添加初始化接口声明
*********************/
#include "briupI2C.h"
#include "briupDelay.h"

//初始化IIC
void IIC_Init(void)
{					     
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	  
	GPIOB->CRL&=0XFFF00FFF;
	GPIOB->CRL|=0X00033000;;//PB3/PB4设置推挽输出 
	IIC_SCL_H( );
	IIC_SDA_H( );
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_H( );	  	  
	IIC_SCL_H( );
	_delay_us(4);
 	IIC_SDA_L( );//START:when CLK is high,DATA change form high to low 
	_delay_us(4);
	IIC_SCL_L( );//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_L( );
	IIC_SDA_L( );//STOP:when CLK is high DATA change form low to high
 	_delay_us(4);
	IIC_SCL_H( ); 
	_delay_us(1);
	IIC_SDA_H( );//发送I2C总线结束信号
	_delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_H( );_delay_us(1);	   
	IIC_SCL_H( );_delay_us(1);	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			//IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L( );//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_L( );
	SDA_OUT();
	IIC_SDA_L( );
	_delay_us(2);
	IIC_SCL_H( );
	_delay_us(2);
	IIC_SCL_L( );
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_L( );
	SDA_OUT();
	IIC_SDA_H( );
	_delay_us(2);
	IIC_SCL_H( );
	_delay_us(2);
	IIC_SCL_L( );
}					 				     
//IIC发送一个字节
//返回从机有无应答		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_L( );//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
		if( (txd & 0x80) >> 7)
			IIC_SDA_H();
		else
			IIC_SDA_L();
		
        txd<<=1; 	  
		_delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_H( );
		_delay_us(2); 
		IIC_SCL_L( );	
		_delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L( ); 
        _delay_us(2);
				IIC_SCL_H( );
        receive<<=1;
        if(READ_SDA)receive++;   
		_delay_us(1); 
    }	
		//IIC_SCL_L( );	
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
//单字节写入
u8 Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_Date)
{
	IIC_Start();//发送起始信号
	IIC_Send_Byte(SlaveAddress);//发送设备地址
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	IIC_Send_Byte(REG_Address);//发送操作寄存器地址
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Date);//发送写入寄存器的数据
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	IIC_Stop();
	_delay_us(200);
	return 0;
}
//单字节读取
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{
  unsigned char REG_Date;
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);
	if(IIC_Wait_Ack()){IIC_Stop(); return 0;}
	IIC_Send_Byte(REG_Address);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(SlaveAddress+1);
	IIC_Wait_Ack();
	REG_Date = IIC_Read_Byte(0);
	IIC_Stop();
	return REG_Date;
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
unsigned char Single_Read_Len(unsigned char SlaveAddress,unsigned char REG_Address,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte(SlaveAddress);//发送器件地址
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(REG_Address);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
		_delay_ms(100);			//由于SHT20需要一定的时间进行数据测量，此处的延时不能少
    IIC_Start();
	IIC_Send_Byte(SlaveAddress+1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
	return 0;	
}

u8 Sing_Write_ND(u8 addr, u8 reg)
{
	IIC_Start();//发送起始信号
	IIC_Send_Byte(addr);//发送设备地址
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	IIC_Send_Byte(reg);//发送操作寄存器地址
	IIC_Wait_Ack();
	IIC_Stop();
	_delay_ms(5);
	return 1;
}

u8 Sing_Read_NR(u8 addr, u8 len, u8 *buf)
{	
	IIC_Start(); 
	IIC_Send_Byte(addr+1);//发送器件地址
	while(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
		_delay_ms(5);
	return 1;
}

