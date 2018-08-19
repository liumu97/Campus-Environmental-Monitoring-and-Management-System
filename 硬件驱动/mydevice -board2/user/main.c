#include <stm32f10x.h>
#include <stdio.h>
#include "briupGPIO.h"
#include "basePeripheral.h"
#include "briupDelay.h"
#include "briupUSART.h"
#include "briupI2C.h"
#include "briupUsart3.h"
#include "sht20.h"
#include "veml6070.h"
#include "CO2.h"
#include "BH1750.h"
#include "briupZigbee.h"
#include "protocol.h"

//board2可用硬件
//基础硬件
//二氧化碳传感器		缺失
//温湿度传感器
//紫外线传感器
//光照强度传感器		缺失

//协议相关 需测试float转换是否成功。

int fputc(int ch, FILE* stream)
{
    putChar((uint8_t)ch);
    return ch;
}

 int main(void)
{
	float Tem,Hum,Li;
	u16 tem,hum,li;
	u16 Result;
	u8 VH,VL;
	u8 buf[30];
	UserProt cmd;
	
	//NVIC中断分组配置
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	//基础初始化
	briupGPIOInit();
	usart1Init( 72, 57600);
	zigbeeInit( 36, 57600);
	_delay_init(72);
	//hardware初始化
	IIC_Init();
	usart3Init(36,9600);
	//device初始化
	VEML6070Init();
	BH1750_Init();
	
//	baseLedSet(LED_CYAN);
//	while(protocolCon(&cmd)!=6)
//	{
//		_delay_ms(115);
//	}
//	_delay_ms(50);
	baseLedSet(LED_G);
	
	
	while(1)
	{
			//protocolRes(&cmd);	//接收控制帧和心跳帧
			baseLedSet(LED_G);
			_delay_sec(9);
			CO2_Measure(&VH, &VL);		//CO2 整数
			_delay_ms(50);
			buf[0] = VH;
			buf[1] = VL;
			SHT20_Measure(&Tem, &Hum);	//温湿度	浮点数 温度保留三位小数，湿度保留两位小数
			_delay_ms(50);
			tem = (u16)(Tem*1000);
			hum = (u16)(Hum*100);
			VH = (u8)(tem>>8);
			VL = (u8)tem;
			buf[2] = VH;
			buf[3] = VL;
			VH = (u8)(hum>>8);
			VL = (u8)hum;
			buf[4] = VH;
			buf[5] = VL;
			BH1750_Measure(&Li);			//光强	浮点数 保留一位小数
			_delay_ms(50);
			li = (u16)(Li*10);
			VH = (u8)(li>>8);
			VL = (u8)li;
			buf[6] = VH;
			buf[7] = VL;	
			VEML6070Measure(&Result);		//紫外线		整形 强度等级
			_delay_ms(50);
			VH = Result>>8;
			VL = Result;
			buf[8] = VH;
			buf[9] = VL;
			protocolSend(0x01,0x05,0x0a,buf);	//发送数据，顺序为上面调用顺序
			baseLedSet(LED_B);
			_delay_ms(500);
	}
	
}

