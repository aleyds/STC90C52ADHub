#include "base_type.h"
#include <stdarg.h>
#include "uart.h"

#define check_zero(data)\
do{\
	if(data == 0)\
	{\
		__UartSend((H_U8)('0'));\
		break;\
	}\
}while(0)

void _UartOpen(void)
{
	SCON=0x50;
	TMOD = TMOD | 0x20;
	PCON=0x00;//波特率不倍增
	TH1=0xFD; //波特率为9600
	TL1=0xFD;
	ES=1;//允许串口中断
	TR1=1;
}

static void __UartSend(H_U8 _ch)
{
	SBUF = _ch;
	while(!TI);
	TI=0;
}


static void __UartPutDec(H_U32 dec)
{
	if(dec == 0)
	{
		return;
	}
	__UartPutDec(dec/10);
	__UartSend((H_U8)(dec%10 + '0'));
}

static void __UartPutStr(const H_U8 *str)
{
	while(*str != '\0')
	{
		__UartSend(*str++);
	}
}

static void __UartPutBin(H_U32 bin)
{
	if(bin == 0)
	{
		__UartPutStr("0b");
		return;
	}
	__UartPutBin(bin/2);
	__UartSend((H_U8 )(bin%2 + '0'));
}

static void __UartPutHex(H_U32 hex)
{
	if(hex == 0)
	{
		__UartPutStr("0x");
		return;
	}
	__UartPutHex(hex/16);
	if(hex%16 < 10)
	{
		__UartSend((H_U8)(hex%16 + '0'));
	}
	else
	{
		__UartSend((H_U8)((hex%16 - 10) + 'A'));
	}
}

void _UartPrintf(H_U8 *fmt, ...)
{

	H_U32 vargint = 0;
	H_U8 *vargpch = H_NULL;
	H_U8 vargch = 0;
	H_U8 *pfmt = H_NULL;

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
					__UartSend((H_U8)vargch);
					break;
				case 'd':
				case 'i':
					vargint = va_arg(vp, H_U32);
					//check_zero(vargint);
					__UartPutDec(11);
					break;
				case 's':
					vargpch = va_arg(vp, H_U8 *);
					__UartPutStr(vargpch);
					break;
				case 'b':
				case 'B':
					vargint = va_arg(vp, H_U32);
					check_zero(vargint);
					__UartPutBin(vargint);
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, H_U32);
					//check_zero(vargint);
					__UartPutHex(vargint);
					break;
				case '%':
					__UartSend('%');
					break;
			}
			pfmt++;
		}
		else
		{
			__UartSend(*pfmt++);
		}
	}
	va_end(vp);

}