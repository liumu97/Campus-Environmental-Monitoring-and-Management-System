/********************
* FileName: 	briupTim.c
* Description: 	定时器初始化接口实现
* Author:		Dennis Chen
* CreateDate:	2018-7-6
* Copyright:	www.briup.com
* Update:	更改日期  更改人  更改内容
*	2018-7-6	陈哲	添加初始化接口声明
*********************/

#include "briupTim.h"


//定时器初始化函数接口
// pclk 分频系数	arr 自动装载值
void timInit(u8 pclk, u16 arr)
{
	//PA6 PB0 PB1 PB6
	//TIM3 CH1 CH3 CH4  TIM4 CH1
	//开启GPIOA/GPIOB/TIM3/TIM4的RCC时钟
	RCC->APB2ENR |= 1<<2 | 1<<3;
	RCC->APB1ENR |= 1<<1 | 1<<2;
	
	GPIOA->CRL &= ~0x0F000000;
	GPIOA->CRL |=  0x0B000000;
	
	GPIOB->CRL &= ~0x0F0000FF;
	GPIOB->CRL |=  0x0B0000BB;
	
	TIM3->PSC = pclk - 1;	//分频系数
	TIM4->PSC = pclk - 1;
	TIM3->ARR = arr;		//自动装载值
	TIM4->ARR = arr;
	
	TIM3->CCMR1 |= 0x68;	//CH1
	TIM3->CCMR2 |= 0x6868;	//CH4 CH3
	TIM4->CCMR1 |= 0x68;
	
	TIM3->CCR1 = 0;		//比较寄存器归0
	TIM3->CCR3 = 0;		//表示占空比 0%
	TIM3->CCR4 = 0;
	TIM4->CCR1 = 0;
	
	//使能输出，高电平有效
	TIM3->CCER |= 0x1101;
	TIM4->CCER |= 0x01;
	
	//使能自动装载功能和使能定时器
	TIM3->CR1 |= 1<<7 | 1<<0;
	TIM4->CR1 |= 1<<7 | 1<<0;
}
