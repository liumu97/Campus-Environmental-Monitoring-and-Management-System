#include <stm32f10x.h>
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

 int main(void)
{
	u8  buf1[20] ={0xaa,0xbb,0x00,0xff,0xff,0x01,0x01,0x01,0x01,0x00,0x00,0xee,0xff};
	u8  buf2[1] = {0x00};
	u8 len;
	UserProt  cmd;
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
			//test1
//			zigbeeSendBuf(buf1,13);
//		_delay_ms(200);
		baseLedSet(LED_B);
		len=protocolRes(&cmd);
		//putChar(len);
		_delay_ms(200);
		baseLedSet(LED_R);
		_delay_ms(200);
	}
	
}

