#include "base_type.h"
#include <stdarg.h>
#include "uart.h"


#define     URMD    0           //0:ʹ�ö�ʱ��2��Ϊ�����ʷ�����
                                //1:ʹ�ö�ʱ��1��ģʽ0(16λ�Զ�����ģʽ)��Ϊ�����ʷ�����
                                //2:ʹ�ö�ʱ��1��ģʽ2(8λ�Զ�����ģʽ)��Ϊ�����ʷ�����
/*----------------------------
��ʼ������
----------------------------*/
void InitUart()
{
    SCON = 0x5a;                //���ô���Ϊ8λ�ɱ䲨����
#if URMD == 0
    T2L = 0xd8;                 //���ò�������װֵ
    T2H = 0xff;                 //115200 bps(65536-18432000/4/115200)
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
#elif URMD == 1
    AUXR = 0x40;                //��ʱ��1Ϊ1Tģʽ
    TMOD = 0x00;                //��ʱ��1Ϊģʽ0(16λ�Զ�����)
    TL1 = 0xd8;                 //���ò�������װֵ
    TH1 = 0xff;                 //115200 bps(65536-18432000/4/115200)
    TR1 = 1;                    //��ʱ��1��ʼ����
#else
    TMOD = 0x20;                //���ö�ʱ��1Ϊ8λ�Զ���װ��ģʽ
    AUXR = 0x40;                //��ʱ��1Ϊ1Tģʽ
    TH1 = TL1 = 0xfb;           //115200 bps(256 - 18432000/32/115200)
    TR1 = 1;
#endif
}


/*----------------------------
���ʹ�������
----------------------------*/
void SendData(BYTE dat)
{
    while (!TI);                    //�ȴ�ǰһ�����ݷ������
    TI = 0;                         //������ͱ�־
    SBUF = dat;                     //���͵�ǰ����
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