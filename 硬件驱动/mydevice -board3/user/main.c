#include <stm32f10x.h>
#include <stdio.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "fire.h"
#include "briupZigbee.h"
#include "protocol.h"

//board3可用硬件
//烟雾传感器
//甲烷传感器
//人体红外				暂无
//火光传感器
extern volatile u8 statFlag;

int fputc(int ch, FILE* stream)
{
    putChar((uint8_t)ch);
    return ch;
}

int main(void)
{	
	UserProt cmd;
	//四个报警器的报警消息
	u8 fogBuf[1]={0x01};
	u8 fireBuf[1]={0x01};
	u8 CH4Buf[1]={0x01};
	u8 infraredBuf[1]={0x01};
	
	u8 warnflag=0x00;
	
	//初始化报警标志
	statFlag = 0x00;
	
	
	//NVIC中断分组配置
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1);

	//基础初始化
	zigbeeInit( 36, 57600);     //Zigbee默认波特率不可修改
	_delay_init(72);
	briupGPIOInit();
	usart1Init(72,57600);
	//device初始化
	fireInit();
	
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(130);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);
	
	while(1)
	{
		if(statFlag!=0)	//发送警报，且保证每种警报只发送一次
		{
			if(statFlag & 1<<0 && !(statFlag & 1<<1)) 
			{
				protocolSend(0x01,0x01,0x01,fogBuf);
				statFlag |= 1<<1;
				baseLedSet(LED_R);
			}
			if(statFlag & 1<<2 && !(statFlag & 1<<3))
			{
				protocolSend(0x01,0x02,0x01,CH4Buf);
				statFlag |= 1<<3;
				baseLedSet(LED_R);
			}
			if(statFlag & 1<<4 && !(statFlag & 1<<5))
			{	
				protocolSend(0x01,0x03,0x01,infraredBuf);
				statFlag |= 1<<5;
				baseLedSet(LED_R);
			}
			if(statFlag & 1<<6 && !(statFlag & 1<<7))
			{
				protocolSend(0x01,0x04,0x01,fireBuf);
				statFlag |= 1<<7;
				baseLedSet(LED_R);
			}	
			if(!warnflag)
			{
				baseLedSet(LED_R);
				baseBeepSet(1);
				warnflag=0x01;
			}
			else
			{
				baseLedSet(LED_BLACK);
				baseBeepSet(0);
				warnflag=0x00;
			}
		}	
		_delay_ms(100);
		if(protocolRes(&cmd) == 0)
			if(cmd.uFrameType == 0x02)
			{
				statFlag = 0x00;        //解除警报
				baseLedSet(LED_G);
				baseLedSet(LED_BLACK);
				baseBeepSet(0);
			}				
	}
	
}


