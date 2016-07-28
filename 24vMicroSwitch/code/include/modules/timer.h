#ifndef __STC90C52AD_TIMER_H__
#define __STC90C52AD_TIMER_H__

#ifndef FOSC
#define FOSC 18432000L
#endif

#define T1MS (65536-FOSC/1000)      //1T模式
//#define T1MS (65536-FOSC/12/1000) //12T模式


enum {
	_TYPE_TIMER_0 = 0,
	_TYPE_TIMER_1,
	_TYPE_TIMER_2,
};

typedef void (*TimerCallback)(void);
/*
	创建定时器
	_Number: 定时器编号   支持0 1 2 三个芯片内部定时器
	_TimeOut: 定时器超时中断时间 ms
	_Call:定时器超时中断处理函数
*/
H_U32 _TimerCreat(H_U8 _Number, H_U32 _TimeOut, TimerCallback _Call);

/*
	启动定时器
	_Number:启动定时器的编号
*/
H_U32 _TimerStart(H_U8 _Number);

/*
	关闭定时器
*/
H_U32 _TimerClose(H_U8 _Number);

#endif