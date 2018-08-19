/********************
* FileName: 	a4988.h
* Description: 	
		步进电机的初始化和驱动函数接口
		以及引脚的定义
* Author:		Dennis Chen
* CreateDate:	2018-7-10
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-10	陈哲	添加初始化接口声明
*********************/
#ifndef __A4988_H__
#define __A4988_H__

#include <stm32f10x.h>

//设置功能引脚为高电平
#define A4988_EN_H()	GPIOB->ODR |= (1<<0)
#define A4988_MS1_H()   GPIOB->ODR |= (1<<1)
#define A4988_MS2_H()	GPIOB->ODR |= (1<<5)
#define A4988_MS3_H()   GPIOB->ODR |= (1<<6)
#define A4988_RESET_H()	GPIOB->ODR |= (1<<7)
#define A4988_SLEEP_H()	GPIOB->ODR |= (1<<8)
#define A4988_DIR_H()	GPIOB->ODR |= (1<<13)
//设置功能引脚为低电平
#define A4988_EN_L()	GPIOB->ODR &= ~(1<<0)
#define A4988_MS1_L()   GPIOB->ODR &= ~(1<<1)
#define A4988_MS2_L()	GPIOB->ODR &= ~(1<<5)
#define A4988_MS3_L()   GPIOB->ODR &= ~(1<<6)
#define A4988_RESET_L()	GPIOB->ODR &= ~(1<<7)
#define A4988_SLEEP_L()	GPIOB->ODR &= ~(1<<8)
#define A4988_DIR_L()	GPIOB->ODR &= ~(1<<13)


/******************************************
函数名：TIM4_TIM3_Init
函数功能：通过配置TIM4 TIM3为主从模式实现
					准确脉冲个数输出
					此方式多用于步进电机与伺服电机驱动
*******************************************/
void  TIM4_TIM3_Init(void);

/*********************************************************
名称：Motor_Control(u16 cycle, u16 pulse_num)
说明：生成指定个数脉冲，每个脉冲周期为cycle微秒，脉冲个数生成的个数
      和单脉冲高电平时间有关系，脉冲个数就由高电平时间来确定
参数cycle：为TIM3一个脉冲周期,单位(us)
参数pulse_num：为脉冲个数，决定步进电机步数
返回值：无
*********************************************************/
void Motor_Control(u16 cycle, u16 pulse_num, u8 dir);

/******************************************
函数名：A4988_Init
函数功能：初始化A4988步进电机驱动芯片
*******************************************/
void  A4988_Init(void);

#endif
