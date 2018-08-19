#include <stm32f10x.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "briupTim.h"
#include "fansRelay.h"
#include "a4988.h"
#include "briupZigbee.h"
#include "protocol.h"


//board1可用硬件
//基础硬件
//4组高亮LED
//4*4炫彩LED
//人体红外传感器

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
	//timInit(72,1000);	
	//device初始化
	fanRelayInit();
	A4988_Init();
	
	fan(0);
	relay1(0);
	relay2(0);
	
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(190);
//	}
//	
//	_delay_ms(50);
	baseLedSet(LED_G);

	

	while(1)
	{
		len = protocolRes(&cmd);
		//putChar(len);
		if(len == 0)
		{
			switch(cmd.uDevId)
			{
				case 0x01:
					fan(cmd.uData[0]);
					break;
				case 0x02:
					relay2(cmd.uData[0]);
					break;
				case 0x03:
					relay1(cmd.uData[0]);
					break;
				case 0x04:
					if(cmd.uData[0] == 0x00)
						Motor_Control(1800, 380, 1);
					if(cmd.uData[1] == 0x01)
						Motor_Control(1800, 380, 0);
					break;
			}
		}
		_delay_ms(200);
		
	}
	
}

