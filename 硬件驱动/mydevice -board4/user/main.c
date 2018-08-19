#include <stm32f10x.h>
#include <stdio.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "briupI2C.h"
#include "briupUsart3.h"
#include "pm2_5.h"
#include "sht20.h"
#include "briupZigbee.h"
#include "protocol.h"

//board1可用硬件
//基础硬件
//PM2.5传感器
//温湿度传感器  暂无

int fputc(int ch, FILE* stream)
{
    putChar((uint8_t)ch);
    return ch;
}

 int main(void)
{
	float Tem,Hum;
	u32 pm25,tem,hum;
	u8 VH,VL;
	u8 buf1[5],buf2[10];
	UserProt cmd;
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	
	//hardware初始化
	IIC_Init();
	usart3Init(36,9600);
	//device初始化
	
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(145);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);
	
	while(1)
	{
		pm25=(u32)(pm25Value()*10);
		VH = (u8)(pm25>>8);
		VL = (u8)pm25;
		buf1[0] = VH;
		buf1[1] = VL;
		protocolSend(0x01,0x01,0x02,buf1);
		baseLedSet(LED_B);
		_delay_ms(1500);
		SHT20_Measure(&Tem,&Hum);
		tem = (u16)(Tem*1000);
		hum = (u16)(Hum*100);
		VH = (u8)(tem>>8);
		VL = (u8)tem;
		buf2[0] = VH;
		buf2[1] = VL;
		VH = (u8)(hum>>8);
		VL = (u8)hum;
		buf2[2] = VH;
		buf2[3] = VL;
		protocolSend(0x01,0x02,0x04,buf2);
		baseLedSet(LED_G);
		_delay_sec(5);
	}
}

