/********************************************************
*FileName:     protocol.h
*Destription:  用户协议封装模块
*Author:      
*CreateDate:   2018-7-15
*Copyright     
*Update:       修改人    修改内容
*  2018-7-15     	       添加初始化函数
********************************************************/
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stm32f10x.h>
#include <string.h>
#include <stdlib.h>

//开发板Id号
#define BOARD_ID     0x02


//将用户自定义协议封装成结构体类型
typedef struct
{
	u8 uFrameType;
	u8 uBoardId;
	u8 uDevId;
	u8 uDataLen;
	u8 uData[30];
	u8 uCheck;
	u8 uEnd;
}UserProt;

//协议相关函数接口
// 1. 上传数据
// 2. 处理上层指令
u8 protocolSend(u8 type, u8 DevId, u8 len, u8 * buf);

u8 protocolRes( UserProt * cmd);

u8 protocolCon( UserProt * cmd);




#endif
