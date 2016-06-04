#include "base_type.h"
#include <stdarg.h>
#include "uart.h"

#define DATAZERO(data)\
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


static void __UartPutStr(const H_U8 *str)
{
	while(*str != '\0')
	{
		__UartSend(*str++);
	}
}

static void __UartPutDec(H_U32 dec)
{

	H_U8 buffer[12] = { 0 };
	sprintf(buffer, "%d", dec);
	__UartPutStr(buffer);
}



static void __UartPutBin(H_U32 bin)
{

	H_U8 buffer[12] = { 0 };
	sprintf(buffer, "0x%B", bin);
	__UartPutStr(buffer);
}

static void __UartPutHex(H_U32 hex)
{
	H_U8 buffer[12] = { 0 };
	sprintf(buffer, "0x%x", hex);
	__UartPutStr(buffer);
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
					DATAZERO(vargint);
					__UartPutDec(vargint);
					break;
				case 's':
					vargpch = va_arg(vp, H_U8 *);
					__UartPutStr(vargpch);
					break;
				case 'b':
				case 'B':
					vargint = va_arg(vp, H_U32);
					DATAZERO(vargint);
					__UartPutBin(vargint);
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, H_U32);
					DATAZERO(vargint);
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