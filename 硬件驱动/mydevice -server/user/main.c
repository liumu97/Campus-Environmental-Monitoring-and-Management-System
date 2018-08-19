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

 int main(void)
{
	u16 len;
	UserProt cmd;
	u8 buf1[20] = {0xbb, 0xaa, 0x00, 0xff, 0xff,\
									0x05, 0x02, 0x01, 0x01, \
									0x01,\
									0xee, 0xff};
	u8 buf2[20] = {0xbb, 0xaa, 0x00, 0xff, 0xff,\
									0x05, 0x02, 0x02, 0x01, \
									0x01,\
									0xee, 0xff};

	_delay_init( 72);
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	
//	zigbeeSendBuf( buf1, 12);
//	baseLedSet(LED_R);
//	_delay_sec(1.5);
//	zigbeeSendBuf( buf2, 12);
//	baseLedSet(LED_G);
//	_delay_sec(1.5);								
//	baseLedSet(LED_B);
	while(1)
	{
		len = protocolRes( &cmd);
		if( len == 0)
		{
			_delay_ms(50);
			analysis(&cmd);
		  //putChar(len);
		}
		_delay_ms(100);
	}
	
	
	
	
	
	
	
	
	
	
//	u32 RGB=0x000011;
//	//usart1Init(72,115200);
//	briupGPIOInit();
//  _delay_init(72);
//	//is_Init();
//	timInit(72,1000);
//	lightSet(2,30);
//	WS2812B_Init();
//	pixBuf[2][3].blue = 0x0f;
//	pixBuf[1][2].green = 0x0f;
//	pixBuf[0][1].red = 0x0f;
//	pixBuf[3][0].green = 0xf;
//	pixBuf[3][1].blue = 0xf;
//	ws2812SendData();
//	//WS2812B_Pixel_EN(0,0,16,&RGB,0);
//	while(1)
//	{
//		//putChar(havehuman());
//		//putChar((u8)'1');
//		
//		//_delay_ms(100);
		
	//}
	
}

