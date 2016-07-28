#ifndef __STC90C52AD_TIMER_H__
#define __STC90C52AD_TIMER_H__

#ifndef FOSC
#define FOSC 18432000L
#endif

#define T1MS (65536-FOSC/1000)      //1Tģʽ
//#define T1MS (65536-FOSC/12/1000) //12Tģʽ


enum {
	_TYPE_TIMER_0 = 0,
	_TYPE_TIMER_1,
	_TYPE_TIMER_2,
};

typedef void (*TimerCallback)(void);
/*
	������ʱ��
	_Number: ��ʱ�����   ֧��0 1 2 ����оƬ�ڲ���ʱ��
	_TimeOut: ��ʱ����ʱ�ж�ʱ�� ms
	_Call:��ʱ����ʱ�жϴ�����
*/
H_U32 _TimerCreat(H_U8 _Number, H_U32 _TimeOut, TimerCallback _Call);

/*
	������ʱ��
	_Number:������ʱ���ı��
*/
H_U32 _TimerStart(H_U8 _Number);

/*
	�رն�ʱ��
*/
H_U32 _TimerClose(H_U8 _Number);

#endif