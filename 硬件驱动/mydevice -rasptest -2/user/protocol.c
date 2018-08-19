/********************************************************
*FileName:     protocol.c
*Destription:  用户协议封装模块实现
*Author:      
*CreateDate:   2018-7-15
*Copyright     
*Update:       修改人    修改内容
*  2018-7-15     	       实现初始化函数
********************************************************/
#include "protocol.h"
#include "briupZigbee.h"
#include "briupUsart.h"   //调试用

#define FRAME_HEAD1   0xaa
#define FRAME_HEAD2   0xbb

//定义内部校验函数
static u8 cmdCheck( UserProt * cmd);

//协议相关函数接口
// 1. 上传数据
// 2. 处理上层指令
u8 protocolSend(u8 type, u8 DevId, u8 len, u8 * buf)
{
	//上行数据需要通过Zigbee协议封装
	//并通过Zigbee接口发送
	//效率最高，最节省内存
	u8 frame[50], i, check;
	check = BOARD_ID;
	check ^= DevId ^ type ^ len;
	frame[0] = FRAME_HEAD2;     //用户自定义
	frame[1] = FRAME_HEAD1;
	frame[2] = 0;        //zigbee帧长度
	frame[3] = 0xff;     //短地址H
	frame[4] = 0xff;     //短地址L
	frame[5] = type;
	frame[6] = BOARD_ID;
	frame[7] = DevId;
	frame[8] = len;
	for(i = 0; i < len;i ++)
	{
		frame[9 + i] = *buf;
		check ^= *buf;
		buf ++;
	}
	frame[9 + i] = check;
	frame[10 + i] = 0xee;
	frame[11 + i] = 0xff;
	zigbeeSendBuf( frame, 12 + i);
	return 0;
}

//接收并解析一帧数据
// return 0 正常接收并解析一帧数据
u8 protocolRes( UserProt * cmd)
{
	//判断是否接收到一帧数据
	u8 len, buf[50];
	len = zigbeeGetData( buf, 1);
	if( len == 0)
		return 1;           //表示未接收到有效数据
	putBuffer(buf,len);
	
//	//判断数据合法性
//	if( buf[0] != FRAME_HEAD2 || buf[1] != FRAME_HEAD1)
//		return 2;           //zigbee帧头校验失败
//	
//	if( buf[5] != BOARD_ID)
//		return 3;           //数据不是给该功能板
//	
//	
//	memset( cmd, 0, sizeof( UserProt));
//	memcpy( cmd, (buf + 5), len - 3);
//	cmd->uCheck = buf[len - 3];
//	cmd->uEnd   = buf[len - 2];
//	
//	if( cmdCheck( cmd))
//		return 4;           //校验未通过
	
	return 0;
}

//-----------------------------------内部处理函数-----------------------
static u8 cmdCheck( UserProt * cmd)
{
	u8 check = cmd->uFrameType;
	u8 i, len, *cp = (u8 *)cmd;
	len = cmd->uDataLen + 4;
	cp++;
	for(i = 1;i < len;i ++)
	{
		check ^= *cp;
		cp ++;
	}
	if( check == cmd->uCheck)
		return 0;        //校验成功
 	else 
		return 1;        //校验失败
}



