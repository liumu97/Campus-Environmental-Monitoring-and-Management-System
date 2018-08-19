
#ifndef __FIGURE_H__
#define __FIGURE_H__

#include <stm32f10x.h>

void figureInit(void);

u8 addFigure(void);

u8 checkFigure(void);

void usart3Init( u8 pclk, u32 baud);
void usart3DmaPut( u8 * buf, u16 len);
u16 usart3GetBuffer(u8 * buf);

#endif
