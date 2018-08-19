#include "keyboard.h"
#include "briupDelay.h"
#include "briupUsart.h"
volatile u8 keyValue =0;

void keyboardInit(void)
{
	NVIC_InitTypeDef nvic_init;
	
	RCC->APB2ENR|=1<<4 | 1<<0;
	GPIOC->CRH&=0xff0fffff;
	GPIOC->CRH|=0x00800000;
	
	AFIO->EXTICR[3] &= ~(0x0F << 4);
	AFIO->EXTICR[3] |= 0x02<<4;
	
	EXTI->RTSR |= 1<<13;
	EXTI->FTSR &= ~(1<<13);
	EXTI->IMR |= 1<<13;
	
	nvic_init.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvic_init);
	_delay_ms(10);
	IIC_M_Init();
	_delay_ms(10);
}

u8 getKeyValue(void)
{
	u8 temp = keyValue;
	keyValue = 0;
	return temp;
}


//外部中断服务程序
void EXTI15_10_IRQHandler(void)
{ 
	EXTI->PR=1<<13;  //清除LINE上的中断标志位  
	IIC_M_Start();
	IIC_M_Send_Byte(0XBA);
	if(IIC_M_Wait_Ack()){IIC_M_Stop(); return ;}
	keyValue = IIC_M_Read_Byte(0);
	IIC_M_Stop();
	EXTI->PR=1<<13;  //清除LINE上的中断标志位 
}





//============================================================

//初始化IIC
void IIC_M_Init(void)
{					     
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	  
	GPIOB->CRL&=0XFFFFFF00;
	GPIOB->CRL|=0X00000033;//PB0/PB1设置推挽输出 
	IIC_M_SCL_H();
	IIC_M_SDA_H();
}
//产生IIC起始信号
void IIC_M_Start(void)
{
	//SDA_M_OUT();     //sda线输出
	IIC_M_SDA_H();	  	  
	IIC_M_SCL_H();
	_delay_us(4);
 	IIC_M_SDA_L();//START:when CLK is high,DATA change form high to low 
	_delay_us(4);
	IIC_M_SCL_L();//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_M_Stop(void)
{
	SDA_M_OUT();//sda线输出
	IIC_M_SCL_L();
	IIC_M_SDA_L();//STOP:when CLK is high DATA change form low to high
 	_delay_us(4);
	IIC_M_SCL_H(); 
	_delay_us(1);
	IIC_M_SDA_H();//发送I2C总线结束信号
	_delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_M_Wait_Ack(void)
{
	u16 ucErrTime=0;
	SDA_M_IN();      //SDA设置为输入  
	IIC_M_SDA_H();_delay_us(1);	   
	IIC_M_SCL_H();_delay_us(1);	
	while(READ_M_SDA)
	{
		ucErrTime++;
		if(ucErrTime>2500)
		{
			//IIC_Stop();
			return 1;
		}
	}
	IIC_M_SCL_L();//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_M_Ack(void)
{
	IIC_M_SCL_L();
	SDA_M_OUT();
	IIC_M_SDA_L();
	_delay_us(2);
	IIC_M_SCL_H();
	_delay_us(2);
	IIC_M_SCL_L();
}
//不产生ACK应答		    
void IIC_M_NAck(void)
{
	IIC_M_SCL_L();
	SDA_M_OUT();
	IIC_M_SDA_H();
	_delay_us(2);
	IIC_M_SCL_H();
	_delay_us(2);
	IIC_M_SCL_L();
}					 				     
//IIC发送一个字节
//返回从机有无应答		  
void IIC_M_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_M_OUT(); 	    
    IIC_M_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
		if( txd & 0x80)
			IIC_M_SDA_H();
		else
			IIC_M_SDA_L();
		
        txd<<=1; 	  
		_delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_M_SCL_H();
		_delay_us(2); 
		IIC_M_SCL_L();	
		_delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_M_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_M_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_M_SCL_L(); 
        _delay_us(2);
				IIC_M_SCL_H();
        receive<<=1;
        if(READ_M_SDA)receive++;   
				_delay_us(1); 
    }	
    if (!ack)
        IIC_M_NAck();//发送nACK
    else
        IIC_M_Ack(); //发送ACK   
    return receive;
}

