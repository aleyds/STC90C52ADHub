#include "base_type.h"
#include <platform.h>
#include "intrins.h"
#include "uart.h"
#include "timer.h"
#include "adc.h"

#define STARTALLIRQ()		{EA=1;}

void Delay1us()		//@18.432MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay1ms()		//@18.432MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}


void _Delayms(unsigned int ms)
{
	while(ms--)
	{
		Delay1ms();
	}
}

#define TIME_15MIN		(15*60*1000)

#define TOUCH_RESTORE_TIME		(3)
static xdata H_U8 g_TouchEnable = 0;
static xdata H_U32 g_TouchRestore = 0;
static xdata H_U32 g_CurrrentData = 0;
static xdata H_U8 g_TimerStop = 1;
static xdata H_U8 g_Switch3 = 0;
#define ADC_POWER_MAX		(0x8f)
#define ADC_POWER_MIN		(0x70)

typedef enum _eRunningStatus_{
	_RUNNING_INIT = 0,
	_RUNNING_A_STOP, //接触到A点开关，停止
	_RUNNING_B_STOP, //接触到B点开关，停止
	_RUNNING_ATOB, //A-B运动
	_RUNNING_BTOA, //B-A运动
	_RUNNING_ATOB_STOP, //A-B运动中停止
	_RUNNING_BTOA_STOP, //B-A运动中停止
}_eRunningStatus;

static xdata _eRunningStatus g_RunningStatus;

//开关A LED灯显示关闭
static void LEDSwitchA(BYTE display)
{
	if(display)
	{
		SWITCH1_LED = 0;
	}else
	{
		SWITCH1_LED = 1;
	}
}

//外灯开启或关闭
static void __ExternalLED(H_U8 _Status)
{
	if(!_Status)
	{
		//TODO:外灯关闭
		RELAY_C=0;
	}else
	{
		//TODO:外灯开启
		RELAY_C=1;
	}
}

static void __Timer0Callback(void)
{
	//TODO:
	//hs_printf("Timer0 Call \n\r");

	if(!RELAY_C)
	{
		//SWITCH2_LED=0;
		__ExternalLED(H_TRUE);
	}else 
	{
		//SWITCH2_LED=1;
		__ExternalLED(H_FAUSE);
		_TimerClose(0);//关闭外灯后关闭定时器
		g_TimerStop = 1;
	}
}

//读取10次求平均值
static H_U16 _ADCResultAverage(void)
{
	H_U8 i = 0;
	H_U32 sum = 0;
	for(i = 0; i < 20;i++)
	{
		sum += _ADCGetResult(0);
	}
	return (sum/20);
}

static void __MotorStop(void)
{
	//开启15分钟定时器
	if(g_TimerStop)
	{
		
		_TimerCreat(0, TIME_15MIN, __Timer0Callback);
		_TimerStart(0);
		g_TimerStop = 0;
	}
	
	RELAY_A = 0;
	RELAY_B = 0;
	//RELAY_C = 0;
	g_CurrrentData = 0;
	//hs_printf(" Motor Stop \n\r");
}

static void __MotorStart(H_U8 _Turn)
{
	if(_Turn == 0)
	{
		RELAY_B = 0;
		RELAY_A = 1;
		
	}else
	{
		RELAY_A = 0;
		RELAY_B = 1;
	}
	//hs_printf(" Motor Start \n\r");
	g_CurrrentData = _ADCResultAverage();
	_TimerClose(0);//关闭外灯后关闭定时器
	g_TimerStop = 1;
}
 

static H_S8 _SwitchA(void)
{
	if(!(M_SWITCH1&0x01)) //A点1号微动开关，低电平有效
	{
		_Delayms(10);//按键抖动
		if(!(M_SWITCH1&0x01))//开关真正闭合
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _SwitchB(void)
{
	if(!(M_SWITCH2&0x01)) //B点2号微动开关，低电平有效
	{
		_Delayms(10);//按键抖动
		if(!(M_SWITCH2&0x01))//开关真正闭合
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _SwitchC(void)
{
	if(!(M_SWITCH3&0x01)) //C点3号微动开关，低电平有效
	{
		_Delayms(10);//按键抖动
		if(!(M_SWITCH3&0x01))//开关真正闭合
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _TouchSwitch(void)
{
	if(!(T_SWITCH&0x01)) //触摸开关，低电平有效
	{
		//hs_printf("TouchSwitch 111 \n\r");
		_Delayms(10);//按键抖动
		if(!(T_SWITCH&0x01))//开关真正闭合
		{
			//发出滴的声音

			return 1;
		}
	}
	return 0;
}

static H_U16 _ADCGetPoint(H_U16 current)
{
	if(current == 0)
	{
		return 0;
	}
	 return ((current - 511)>>3);
}


static void _EventHandler(void)
{
	BYTE _AdcData = 0;
	g_TouchRestore = (g_TouchRestore > TOUCH_RESTORE_TIME)?TOUCH_RESTORE_TIME:g_TouchRestore;
	if(g_TouchRestore == 0)
	{
		g_TouchEnable = 0;
	}else 
	{
		g_TouchRestore--;
	}
	
	if(_TouchSwitch()&&g_TouchEnable==0)//触发触摸开关
	{
		SendString("Touch PullDown \n\r");
		g_TouchEnable = 1;
		g_TouchRestore = TOUCH_RESTORE_TIME;
		//1、如果状态在A点停止，则由A-B点运动，状态改为 _RUNNING_ATOB
		//2、如果状态为B点停止，则由B-A运动，状态改为 _RUNNING_BTOA
		//3、如果状态为 _RUNNING_ATOB ，则停止运动，状态为 _RUNNING_ATOB_STOP
		//4、如果状态为 _RUNNING_BTOA，则停止运动，状态为 _RUNNING_BTOA_STOP
		//5、如果状态为_RUNNING_ATOB_STOP，则由B-A返回运动，状态为_RUNNING_ATOB
		//6、如果状态为 _RUNNING_BTOA_STOP，则由A-B返回云顶，状态为 _RUNNING_BTOA
		//hs_printf("TouchSwitch Enable \n\r");
		if(g_RunningStatus == _RUNNING_A_STOP)
		{
			//RELAY_B = 1;
			//RELAY_A = 0;
			g_RunningStatus = _RUNNING_ATOB;
			__MotorStart(1);
		}else if(g_RunningStatus == _RUNNING_B_STOP)
		{
			//RELAY_A = 1;
			//RELAY_B = 0;
			g_RunningStatus = _RUNNING_BTOA;
			__MotorStart(0);
		}else if(g_RunningStatus == _RUNNING_ATOB)
		{
			g_RunningStatus = _RUNNING_ATOB_STOP;
			__MotorStop();
		}else if(g_RunningStatus == _RUNNING_BTOA)
		{
			g_RunningStatus = _RUNNING_BTOA_STOP;
			__MotorStop();
		}else if(g_RunningStatus == _RUNNING_ATOB_STOP)
		{
			//RELAY_A = 1;
			//RELAY_B = 0;
			g_RunningStatus = _RUNNING_ATOB;
			__MotorStart(0);
		}else if(g_RunningStatus == _RUNNING_BTOA_STOP)
		{
			//RELAY_A = 0;
			//RELAY_B = 1;
			g_RunningStatus = _RUNNING_BTOA;
			__MotorStart(1);
		}
		
		//开启LED灯
		ALARM_SWITCH=0;
		_Delayms(500);
		EXTERNAL_LED=1;
	}
	
	if(_SwitchA())//A点开关闭合
	{
		//停止电机运动，状态为_RUNNING_A_STOP
		//RELAY_A = 1;
		//RELAY_B = 0;
		__MotorStop();
		g_RunningStatus = _RUNNING_A_STOP;
		//1号开关LED熄灭
		LEDSwitchA(1);
		SendString("Switch A PullDown \n\r");
	}else if(!_SwitchA())
	{
		LEDSwitchA(0);
	}
	
	
	if(_SwitchB() )
	{
		//停止电机运动，状态为_RUNNING_A_STOP
		//RELAY_A = 0;
		//RELAY_B = 0;
		__MotorStop();
		g_RunningStatus = _RUNNING_B_STOP;
		SendString("Switch B PullDown \n\r");
	
	}
	
	if ((!_SwitchC() && (g_RunningStatus == _RUNNING_ATOB)))
	{
		g_Switch3 = 1;
		//SWITCH2_LED=0;
		g_RunningStatus = _RUNNING_BTOA;
		__MotorStart(0);
		SendString("Switch C PullDown \n\r");
	}else if((g_Switch3==1) && (_SwitchC()))
	{
		//SWITCH2_LED=1;
		g_Switch3 = 0;
	}
	
	
	_AdcData = _ADCGetResult();
	
	//hs_printf("[main] _ADCGetResult :%d current :%d  status:%d \n\r",_AdcData,g_CurrrentData,_ADCGetPoint(g_CurrrentData));
	SendPrintf("ADC Data:%d\n\r",_AdcData);
	if((g_CurrrentData != 0) &&(_AdcData > (g_CurrrentData+_ADCGetPoint(g_CurrrentData)) ) && (g_RunningStatus == _RUNNING_BTOA))
	{
		//反转为向B点运动， 状态为 _RUNNING_ATOB
		//RELAY_A = 0;
		//RELAY_B = 1;
		g_RunningStatus = _RUNNING_ATOB;
		__MotorStart(1);
		//SWITCH2_LED=0;
		//_Delayms(500);
		//SWITCH2_LED=1;
		//hs_printf("_AdcData Enable \n\r");
	}
}

 
static void _Sleep(void)
{
	PCON = 0x02;
}

static void _SleepInit(void)
{
	WKTCL = 49;//设置唤醒周期为488us*(49+1)=24.4ms
	WKTCH = 0x80;//使能掉电唤醒定时器
}


void main()
{
	
		//__SystemInit();
		//_ADCInit();
		//_SleepInit();
		//_UartOpen();
		//g_RunningStatus = _RUNNING_INIT;
		//_MotorStart(0);//启动向A点运动
		//hs_printf(RED"[main] Start \n\r");
		InitUart();//串口初始化
		_ADCInit();//ADC初始化
		_SleepInit();//低功耗模式，暂时不使用
		SendString("WillCome to 24V\n\r");
		
		g_RunningStatus = _RUNNING_INIT;
		_MotorStart(0);//启动向A点运动
		 RELAY_C = 0;
		ALARM_SWITCH = 0;
		EXTERNAL_LED = 0;
		SWITCH1_LED = 0;
		SWITCH2_LED = 0; 
		P1 = 0xff;
		STARTALLIRQ();
		
		
		while(1)
		{
			_Sleep();//低功耗模式，暂时不使用
			_EventHandler();
			
			//全部GPIO测试代码
			 /* P0 = 0xFF;
			P1 = 0xFF;
			P2 = 0xFF;
			P3 = 0xFF;
			P4 = 0xFF;
			P5 = 0xFF;
			_Delayms(3000);
			P0 = 0x00;
			P1 = 0x00;
			P2 = 0x00;
			P3 = 0x00;
			P4 = 0x00;
			P5 = 0x00;
			_Delayms(3000);  */
			
			//_EventHandler();
		}
}

