#include <stm32f10x.h>
#include <stdio.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "is.h"
#include "briupTim.h"
#include "highLight.h"
#include "WS2812B.h"
#include "briupZigbee.h"
#include "protocol.h"

//board1可用硬件
//基础硬件
//4组高亮LED
//4*4炫彩LED
//人体红外传感器
int fputc(int ch,FILE * stream)
{
	putChar((uint8_t)ch);
	return ch;
}

 int main(void)
{
	//u8  buf1[20] ={0xaa,0xbb,0x00,0xff,0xff,0x02,0x01,0x01,0x01,0x00,0x00,0xee,0xff};
	u8  buf2[10] = {0x01};
	u8  buf1[10] = {0x00};
	UserProt * cmd;
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	

	
//	//test2
//	protocolSend(0x01,0x01,0x01,buf2);
//	
//	//test3
//	protocolRes(cmd);
	
	

	while(1)
	{
//			//test1
		_delay_sec(5);
		protocolSend(0x02,0x03,0x01,buf2);
		baseLedSet(LED_B);
		_delay_ms(500);
		baseLedSet(LED_R);
		_delay_ms(500);
		_delay_sec(5);
		protocolSend(0x02,0x03,0x01,buf1);
		
	}
	
}

