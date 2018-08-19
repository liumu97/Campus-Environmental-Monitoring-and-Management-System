
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stm32f10x.h>

void keyboardInit(void);

u8 getKeyValue(void);


//=============================================
//                IIC通信接口，勿动

//IO方向设置
#define SDA_M_IN()  {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=8<<4;}	//PB3输入模式
#define SDA_M_OUT() {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=3<<4;} //PB3输出模式
//IO操作函数	 
#define IIC_M_SCL_H()    GPIOB->ODR |= 0x01 //SCL
#define IIC_M_SDA_H()    GPIOB->ODR |= 0x02 //SDA	 
#define IIC_M_SCL_L()    GPIOB->ODR &= ~0x01 //SCL
#define IIC_M_SDA_L()    GPIOB->ODR &= ~0x02 //SDA	 
#define READ_M_SDA   	((GPIOB->IDR & 0x02)>>1)  //输入SDA 

//初始化IIC
void IIC_M_Init(void);
//产生IIC起始信号
void IIC_M_Start(void);
//产生IIC停止信号
void IIC_M_Stop(void);
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_M_Wait_Ack(void);
//产生ACK应答
void IIC_M_Ack(void);
//不产生ACK应答		    
void IIC_M_NAck(void);
//IIC发送一个字节
//返回从机有无应答		  
void IIC_M_Send_Byte(u8 txd);
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_M_Read_Byte(unsigned char ack);

#endif
