#include <stm32f10x.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "oeSwitch.h"
#include "warn.h"
#include "briupZigbee.h"
#include "protocol.h"

//board1可用硬件
//基础硬件
//对射光电传感器
//反射光电传感器
//报警灯
//报警器

 int main(void)
{
	u8 len;
	UserProt cmd;
	extern volatile u8 flag;
	u8 wareSet = 0;
	flag = 0x00;
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	//hardware初始化
	//device初始化
	oeSwitchInit();
	warnInit();
	
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(160);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);

	while(1)
	{
		if(flag)
		{
			if( wareSet == 0)
			{
				warnLed();
				warn();
				wareSet = 1;
			}
			else
			{
				closeWarnLed();
				closeWarn();
				wareSet = 0;
			}
			_delay_ms(1000);
		}
		len = protocolRes(&cmd);
		_delay_ms(50);
		if(len == 0)
		{
			flag = 0x00;
		}
		_delay_ms(100);
	}
	
}

