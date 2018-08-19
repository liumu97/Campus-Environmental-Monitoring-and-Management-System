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
	u8 len;
	UserProt cmd;
	//NVIC中断分组配置
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	//hardware初始化
	//device初始化
	timInit(72,1000);					//定时器初始化
	WS2812B_Init();						//炫彩LED初始化
	ws2812SendData();
	is_Init();								//人体红外传感器初始化

//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(100);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);
	
	while(1)
	{
		len=protocolRes(&cmd);
		//putChar(len);
		if(len==0)
		{
			putChar(cmd.uDevId);
			switch(cmd.uDevId)
			{
				case 0x01:
					if(cmd.uData[0]==0x00)
					{
						lightSet(0x02,cmd.uData[1]);
						lightSet(0x04,cmd.uData[1]);
					  lightSet(0x08,cmd.uData[1]);
					}
					else
					  lightSet(cmd.uData[0],cmd.uData[1]);
					break;
				case 0x02:
					//炫彩LED逻辑
					WS2812B_Clear();
					if(cmd.uData[0] == 0x01)
					{
						pixBuf[0][0].blue = 0x0f;
					}
					else if(cmd.uData[0] == 0x02)
					{
						pixBuf[0][1].blue = 0x0f;
					}
					else if(cmd.uData[0] == 0x03)
					{
						pixBuf[0][2].blue = 0x0f;
					}
					ws2812SendData();
					break;			
			}
		}
		_delay_ms(500);		
	}
	
}

