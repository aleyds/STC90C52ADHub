#include "base_type.h"
#include <stdarg.h>
#include "uart.h"


#define     URMD    0           //0:使用定时器2作为波特率发生器
                                //1:使用定时器1的模式0(16位自动重载模式)作为波特率发生器
                                //2:使用定时器1的模式2(8位自动重载模式)作为波特率发生器
/*----------------------------
初始化串口
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //设置串口为8位可变波特率
#if URMD == 0
    T2L = 0xd8;                 //设置波特率重装值
    T2H = 0xff;                 //115200 bps(65536-18432000/4/115200)
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
#elif URMD == 1
    AUXR = 0x40;                //定时器1为1T模式
    TMOD = 0x00;                //定时器1为模式0(16位自动重载)
    TL1 = 0xd8;                 //设置波特率重装值
    TH1 = 0xff;                 //115200 bps(65536-18432000/4/115200)
    TR1 = 1;                    //定时器1开始启动
#else
    TMOD = 0x20;                //设置定时器1为8位自动重装载模式
    AUXR = 0x40;                //定时器1为1T模式
    TH1 = TL1 = 0xfb;           //115200 bps(256 - 18432000/32/115200)
    TR1 = 1;
#endif
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(BYTE dat)
{
    while (!TI);                    //等待前一个数据发送完成
    TI = 0;                         //清除发送标志
    SBUF = dat;                     //发送当前数据
}


 void SendString(const BYTE *str)
{
	while(*str != '\0')
	{
		SendData(*str++);
	}
}

 void SendDec(WORD dec)
{

	BYTE buffer[12] = { 0 };
	sprintf(buffer, "%d", dec);
	SendString(buffer);
}

 void SendHex(WORD hex)
{
	BYTE buffer[12] = { 0 };
	sprintf(buffer, "0x%x", hex);
	SendString(buffer);
}

void SendPrintf(const BYTE *fmt, ...)
{

	WORD vargint = 0;
	BYTE *vargpch = H_NULL;
	BYTE vargch = 0;
	BYTE *pfmt = H_NULL;

	va_list vp;
	pfmt = fmt;
	va_start(vp, fmt);
	while(*pfmt)
	{
		if(*pfmt == '%')
		{
			switch(*(++pfmt))
			{
				case 'c':
					vargch = va_arg(vp,H_U32);
					SendData((BYTE)vargch);
					break;
				case 'd':
					vargint = va_arg(vp, H_U32);
					SendDec(vargint);
					break;
				case 's':
					vargpch = va_arg(vp, BYTE *);
					SendString(vargpch);
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, H_U32);
					SendHex(vargint);
					break;
				case '%':
					SendData('%');
					break;
			}
			pfmt++;
		}
		else
		{
			SendData(*pfmt++);
		}
	}
	va_end(vp);

}