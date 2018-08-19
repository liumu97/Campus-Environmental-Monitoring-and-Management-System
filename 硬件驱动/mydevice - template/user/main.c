#include <stm32f10x.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "oeSwitch.h"
#include "is.h"


 int main(void)
{
	//NVIC_InitTypeDef nvicInit;
//	u8 * buf;
//	u16 len;
	//NVIC÷–∂œ∑÷◊È≈‰÷√
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	
	_delay_init(72);
	briupGPIOInit();
	usart1Init(72,57600);
	is_Init();

	
	while(1)
	{
		//putChar(currSwitchStat());	
//		len = getBuffer(buf , 0);
//		if( len !=0)
//		{
//			putDataDma(buf, len);
//		}
//		putBuffer((u8*)"12345678910JQK",13);
//		_delay_ms(50);
	}
}

