#include <stm32f10x.h>
#include <stdio.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "briupTim.h"
#include "servo.h"
#include "rc522.h"
#include "rc522Add.h"
#include "mini12864.h"
#include "briupZigbee.h"
#include "protocol.h"

//board1可用硬件
//基础硬件
//舵机
//RFID读卡器
//屏幕



 int main(void)
{	
	u8 len;
	UserProt cmd;
	extern u8 flag;
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	//hardware初始化
	timInit(72,1000);
	//device初始化
	Servo_Init(20000,71);
	InitRc522();
	InitAlldata();
	Lcd_Init_IN();
	Clear_Screen_IN();
	
	flag=0;
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(175);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);;
	
	showStrCn(0x01,0x21,(u8*)" forward");
	showStrCn(0x03,0x22,(u8*)"欢迎光临");
	
	//Read_cardID();
	
	while(1)
	{
		Clear_Screen_IN();
		showStrCn(0x01,0x21,(u8*)" forward");
		showStrCn(0x03,0x22,(u8*)"欢迎光临");
		showStrCn(0x05,0x27,(u8*)"请刷卡");
		len = protocolRes(&cmd);
		if(len == 0)
		{
			if(cmd.uDevId == 0x01)		//舵机控制信息
			{
				if(cmd.uData[0] == 0x01) 
				{
					Servo_ON();
					_delay_sec(2);
					Servo_OFF();
				}
			}
			else if(cmd.uDevId == 0x03)		//RFID确认信息
			{
				if(cmd.uData[0] == 0x01)		//RFID确认成功
				{
					Clear_Screen_IN();
					showStrCn(0x03,0x22,(u8*)"欢迎光临 请进");
					Servo_ON();
					_delay_sec(2);
					Servo_OFF();
					showStrCn(0x01,0x21,(u8*)" forward");
					showStrCn(0x03,0x22,(u8*)"欢迎光临");
				}
				else if(cmd.uData[0] == 0x02)
				{
					Clear_Screen_IN();
					showStrCn(0x03,0x22,(u8*)"识别失败 请重新刷卡");
				}
				flag = 0x00;			//复位读卡标志
			}
		}
		if(!flag)
		{
			Read_cardID();
		}
		else if(flag == 0x01)
		{
			protocolSend(0x01,0x03,0x04,cardID);		//读到卡后发送上行帧
			flag = 0x02;											//保证不会重复发送
		}
			
			
		_delay_ms(1000);
		
	}
	
}

