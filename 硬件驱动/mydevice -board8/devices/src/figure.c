
#include "figure.h"
#include "briupUsart.h"
#include "briupDelay.h"
#include "mini12864.h"
#include "basePeripheral.h"
#include "briupDelay.h"
//===================================以下是不需要研究的东西=====================================
//声明接收数据的缓冲区及标记
static volatile u8 usart3Buffer[256];
//数据头和数据尾
static volatile u16 buf3Head, buf3Tail;
static volatile u16 buf3Count = 0;
u32 SaveNumber=0; 
u32 searchnum=0;
u32 SearchNumber=0;	


//验证设备握手口令 ，发送16个，回传12个字节
unsigned char  VPWD[16]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};	

//探测手指口令并从传感器上读入图像   发送12个字节，回传12个
unsigned char  GIMG[12]={0xef,0x01,0xff,0xff,0xff,0xff, 0x01,0x00,0x03,0x01,0x00,0x05};

//根据原始图像生成指纹特征  发送13 ，回传12
unsigned char  GENT1[13]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x04,0x02,0x01,0x00,0x08};

//根据原始图像生成指纹特征2： 发送13 回传12
unsigned char  GENT2[13]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 

//以charbuffera或charbufferb中的特征文件搜索整个或部分指纹库  发17 回 16
unsigned char  SEAT[17]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xe7,0x00,0xf8};

//将charbuffera与charbufferb中的指纹特征文件合并生成模板，存于ModelBuffer中  发12回12
unsigned char  MERG[12]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x05,0x00,0x09};

//将ModelBuffer2中的文件存到flash指纹库中  发 15 回 12
unsigned char  STOR[15]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x06,0x06,0x02,0x00,0x00,0x00,0x0f};

//清除指纹库    发 12  回  12
unsigned char  DELE_all[12]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x0d,0x00,0x11};

u8 VefPSW(void);
u8 ImgProcess(u8 BUFID);
u8 search(void);
u8 savefingure(u32 ID);
u8 enroll(void);

//===================================以上是不需要研究的东西=====================================




void figureInit(void)
{
	usart3Init( 36, 9600);
	_delay_ms(100);
	
}

u8 addFigure(void)
{
	Clear_Screen_IN();//全情屏
	
	if(VefPSW()==1)
	{
		showStrCn(1,0,(u8*)" 握手成功");
		baseBeepSet(1);
		_delay_ms(500);
		baseBeepSet(0);
	//其他操作
	}
	if(SaveNumber<162)
	{
		_delay_ms(50);
		baseBeepSet(1);
		_delay_ms(500);
		baseBeepSet(0);
		Clear_Screen_IN();
		showStrCn(3,0,(u8*)" 请按下手指");
		_delay_sec(4);

		if(enroll()==1)
		{
			_delay_ms(500);
			Clear_Screen_IN();
			showStrCn(3,0,(u8*)" 录入完毕");
			showStrCn(3,0,(u8*)" 请松开手指");
			_delay_ms(2000);
			if(savefingure(SaveNumber+1)==1)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 存储成功");
				SaveNumber++;
				_delay_ms(100);
				baseBeepSet(1);
				_delay_ms(100);
				baseBeepSet(0);
				_delay_sec(3);	
			}
		}
	}
	return 0;
}

u8 checkFigure(void)
{
	Clear_Screen_IN();
	_delay_ms(500);
				baseBeepSet(1);
	_delay_ms(500);
				baseBeepSet(0);
	Clear_Screen_IN();
	showStrCn(3,0," 请按下手指");
	_delay_ms(2000);			
	searchnum = search();
	if(searchnum>=1&&searchnum<=1000)
	{
				baseBeepSet(1);
		_delay_ms(500);
				baseBeepSet(0);
		Clear_Screen_IN();
		showStrCn(1,0," 识别成功");
		showStrCn(3,0," 欢迎回来");
		//LOCK_CONTROL=1;
		_delay_ms(3000);
		return 1;
	}
	else if(searchnum==0)
	{
		Clear_Screen_IN();
		showStrCn(1,0," 识别失败");
				baseBeepSet(1);
		_delay_ms(100);
				baseBeepSet(0);
		_delay_ms(100);
				baseBeepSet(1);
		_delay_ms(100);
				baseBeepSet(0);
		_delay_ms(3000);
		//LOCK_CONTROL=0;
		return 0;
	}
}


//=====================================以下又是不需要研究的东西=========================================

u8 VefPSW(void)
{
	u8 len, buf[100];
	usart3DmaPut(VPWD, 16);
	do
	{
		len = usart3GetBuffer( buf);
	}while(len == 0);
	
	if(buf[9]==0x00){
		return 0;//表示口令验证正确
	}else if(buf[9]==0x01){
		return 2;//表示收包有错
	}else {
		return 3;//表示口令不正确
	}
}

u8 ImgProcess(u8 BUFID){
	u8 len, buf[100];
	usart3DmaPut(GIMG, 12);
	do
	{
		len = usart3GetBuffer( buf);
	}while(len == 0);
	if(buf[9]==0x00)
	{
			if(BUFID==1)
			{
				usart3DmaPut(GENT1, 13);
				do
				{
					len = usart3GetBuffer( buf);
				}while(len == 0);
				if(buf[9]==0x00){
					return 1;//成功
				}
				else
				{
					return 0;//失败
				}
			}
			else if(BUFID==2)
			{
				usart3DmaPut(GENT2, 13);
				do
				{
					len = usart3GetBuffer( buf);
				}while(len == 0);
				if(buf[9]==0x00){
					return 1;//成功
				}
				else
				{
					return 0;//失败
				}
			}
		
	}else if(buf[9]==0x01)
	{
		return 2;//表示收包有错
	}else if(buf[9]==0x02)
	{
		return 3;//表示传感器上无手指
	}else if(buf[9]==0x03)
	{
		return 4;
	}
	return 0;
}

u8 search(void)
{
	u8 i=0;
	u8 len, buf[100];
	while(i<20)
	{
		usart3DmaPut(GIMG, 12);
		do
		{
			len = usart3GetBuffer( buf);
		}while(len == 0);
		if(buf[9] == 0x00)
		{
			usart3DmaPut(GENT1, 13);
			do
			{
				len = usart3GetBuffer( buf);
			}while(len == 0);
			if(buf[9] == 0x00)
			{
				usart3DmaPut(SEAT, 17);
				do
				{
					len = usart3GetBuffer( buf);
				}while(len == 0);
				if(buf[9] == 0x00)
					return (buf[10]<<8)|buf[11];
				else if(buf[9] == 0x09)
				{
					return 0;
				}
			}
		}
		i++;
	}
	return 0;
}

u8 savefingure(u32 ID)
{
	u8 i=0;
	u32 sum=0;
	u8 FIFO[80]={0};
	u8 len, buf[100];
	
	for(i=0;i<15;i++)
	{
		FIFO[i]=STOR[i];
	}
	
	FIFO[11]=ID/256;
	FIFO[12]=ID%256;
	
	for(i=6;i<13;i++)
	{
		sum= sum + FIFO[i];
	}
	
	FIFO[13]=sum/256;
	FIFO[14]=sum%256;
	
	usart3DmaPut( FIFO, 15);
	do
	{
		len = usart3GetBuffer( buf);
	}while(len == 0);
	if(buf[9]==0x00)
	{
		return 1;//存储成功
	}
	else
	{
		return 0;
	}
}

u8 enroll(void)
{
	u8 temp = 0, count = 0;
	u8 len, buf[100];
	usart3DmaPut((u8*)"开始采集指纹...", 15);
	while(1)
	{
		Clear_Screen_IN();
		showStrCn(3,0,(u8*)" 开始录入");
		_delay_ms(1000);
		temp=ImgProcess(1);
		if(temp==1)
		{
			Clear_Screen_IN();
			showStrCn(3,0,(u8*)" 第一次采集成功");
			count=0;
			//采集第一个特征成功
			baseBeepSet( 1);
			_delay_ms(100);
			baseBeepSet( 0);
			_delay_ms(2000);
			break;
		}
		else
		{
			if(temp==2)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)"采集失败");
				showStrCn(5,0,(u8*)"传包错误");
				_delay_ms(1000);
				count++;
				if(count>=40)
					return 0;
			}
			else if(temp ==3)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 采集失败");
				showStrCn(5,0,(u8*)"传感器上无手指");
				_delay_ms(1000);
				count++;
				if(count>=40)
					return 0;
			}
			else if(temp==4)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 采集失败");
				showStrCn(5,0,(u8*)"指纹特征不明显");
				_delay_ms(1000);
				count++;
				if(count>=40)
					return 0;
			}
		}
	}
	_delay_ms(1000);
	while(1)
	{
		Clear_Screen_IN();
		showStrCn(3,0,(u8*)" 开始录入");
		_delay_ms(1000);
		temp = ImgProcess(2);
		if(temp==1)
		{
			Clear_Screen_IN();
			showStrCn(3,0,(u8*)" 第二次采集成功");
			_delay_ms(2000);
			usart3DmaPut( MERG, 12);
			do
			{
				len = usart3GetBuffer( buf);
			}while( len == 0);
			if(buf[9]==0x00)
			{	
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 合成模板成功");
				baseBeepSet(1);
				_delay_ms(100);
				baseBeepSet(0);
				_delay_ms(100);
				baseBeepSet(1);
				_delay_ms(100);
				baseBeepSet(0);
				_delay_ms(2000);
				return 1;
			}
			else
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 合成失败");
				_delay_ms(1000);
				return 0;
			}
		}
		else
		{
			if(temp==2)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 采集失败");
				showStrCn(5,0,(u8*)"传包错误");
				_delay_ms(1000);
				count++;
				if(count>=25)
					return 0;
			}
			else if(temp ==3)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 采集失败");
				showStrCn(5,0,(u8*)"传感器上无手指");
				_delay_ms(1000);
				count++;
				if(count>=25)
					return 0;
			}
			else if(temp==4)
			{
				Clear_Screen_IN();
				showStrCn(3,0,(u8*)" 采集失败");
				showStrCn(5,0,(u8*)"指纹特征不明显");
				_delay_ms(1000);
				count++;
				if(count>=25)
					return 0;
			}
			else 
				return 0;
		}
	}
}

//串口3初始化函数接口
//	pclk:输入的时钟频率	MHz
//	baud:目标波特率
void usart3Init( u8 pclk, u32 baud)
{
	NVIC_InitTypeDef nvic_init;
	//计算DIV定点数的值
	double usartdiv = (pclk * 1000000.0)/(16 * baud);
	u16 mant = (u16)usartdiv;
	u16 frac = (usartdiv - mant) * 16;
	mant = mant<<4 | (frac & 0x0f);
	
	//开启USART3和GPIOB的时钟
	//	Tx -> PB10
	//	Rx -> PB11
	RCC->APB2ENR |= 1<<3;	//GPIOB
	RCC->APB1ENR |= 1<<18;	//USART3
	
	//配置GPIO的复用功能模式
	GPIOB->CRH &= ~0x0000FF00;
	GPIOB->CRH |=  0x00008B00;
	GPIOB->ODR |= 1<<11;	//PB11上拉输入
	
	//配置USART波特率
	USART3->BRR = mant;
	//13->串口EN 3->TxEN 2->RxEN  5->接收中断EN
	USART3->CR1 |= 1<<13 | 1<<3 | 1<<2 | 1<<5;
	USART3->CR3 |= 1<<7;
	
	DMA1_Channel2->CPAR = (u32)&USART3->DR;
	DMA1_Channel2->CMAR = 0;	//使用时设置
	DMA1_Channel2->CNDTR = 0; //传输数量0
	DMA1_Channel2->CCR = 0;
	DMA1_Channel2->CCR |= 1<<13 | 1<<7 | 1<<4;
	
	nvic_init.NVIC_IRQChannel = USART3_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_init.NVIC_IRQChannelSubPriority = 3;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &nvic_init);
}

u16 usart3GetBuffer(u8 * buf)
{
	//将最近一次获取到的缓冲区数据
	//转移到buf中，并移动缓冲区头部
	u16 temp, i, head = buf3Head;
	u16 lenH = (head + 7)%256;
	u16 lenL = (lenH + 1)%256;
	vu16 length = 0;
	while(1)
	{
		if( length == 0)
			length = usart3Buffer[lenH] << 8 | usart3Buffer[lenL];
		if( length != 0)
		{
			temp = buf3Count;
			if( temp < length + 9)
				continue;
			else
				break;
		}
		_delay_ms(1);
	}
	buf3Count = 0;
	for( i = 0;i < temp;i ++)
	{
		*buf = usart3Buffer[buf3Head];
		buf ++;
		buf3Head ++;
		buf3Head %= 256;
	}
	return temp;
}

//通过DMA发送buf中指定长度(len)的数据
//	buf -> 源地址
//	len -> 长度
void usart3DmaPut(u8 * buf, u16 len)
{
	DMA1_Channel2->CCR &= ~0x01;
	DMA1_Channel2->CMAR = (u32)buf;
	DMA1_Channel2->CNDTR = len;
	DMA1_Channel2->CCR |= 0x01;
}


//==============中断的分割线==================
//USART接收中断处理程序
void USART3_IRQHandler(void)
{
	u8 temp = 0;
	if( USART3->SR & (1<<5))
	{
		temp = USART3->DR;
		usart3Buffer[buf3Tail] = temp;
		buf3Tail ++;
		buf3Tail %= 256;
		buf3Count ++;
	}
}
