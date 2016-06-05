#include "base_type.h"
#include <platform.h>
#include "intrins.h"
#include "adc.h"
#include "tools.h"
#include "uart.h"
#include "timer.h"

#define TIME_15MIN		(15*60*1000)
#define ADC_POWER_MAX		(0x8f)
#define ADC_POWER_MIN		(0x70)

typedef enum _eRunningStatus_{
	_RUNNING_INIT,
	_RUNNING_A_STOP, //�Ӵ���A�㿪�أ�ֹͣ
	_RUNNING_B_STOP, //�Ӵ���B�㿪�أ�ֹͣ
	_RUNNING_ATOB, //A-B�˶�
	_RUNNING_BTOA, //B-A�˶�
	_RUNNING_ATOB_STOP, //A-B�˶���ֹͣ
	_RUNNING_BTOA_STOP, //B-A�˶���ֹͣ
}_eRunningStatus;

static _eRunningStatus g_RunningStatus;

//��Ƭ��ϵͳ��ʼ��
static void __SystemInit(void)
{
		//�ⲿ�ж�0 ����
		IT0 = 0; //�ⲿ�жϵ͵�ƽ����  IT0 = 1 �ⲿ�ж�0�½��ش���
		EX0 = 1; //���ⲿ�ж�0  EX0=0;�ر��ⲿ�ж�0
	 //�ⲿ�ж�1 ����
		IT1 = 0; //�ⲿ�жϵ͵�ƽ����  IT1 = 1 �ⲿ�ж�1�½��ش���
		EX1 = 1; //���ⲿ�ж�1  EX1=0;�ر��ⲿ�ж�1
	
		EA = 1;	//���жϿ���
}

//��ƿ�����ر�
static void __ExternalLED(H_U8 _Status)
{
	if(!_Status)
	{
		//TODO:��ƹر�
		EXTERNAL_LED=0;
	}else
	{
		//TODO:��ƿ���
		EXTERNAL_LED=1;
	}
}

static void __Timer0Callback(void)
{
	//TODO:
	if(!EXTERNAL_LED)
	{
		__ExternalLED(H_TRUE);
	}else 
	{
		__ExternalLED(H_FAUSE);
		_TimerClose(0);//�ر���ƺ�رն�ʱ��
	}
}

static void __MotorStop(void)
{
	//����15���Ӷ�ʱ��
	_TimerCreat(0, TIME_15MIN, __Timer0Callback);
	_TimerStart(0);
	RELAY_C = 0;
}

static void __MotorStart(void)
{
	RELAY_C = 1;
}
 

static H_S8 _SwitchA(void)
{
	if(!(M_SWITCH1&0x01)) //A��1��΢�����أ��͵�ƽ��Ч
	{
		_Delay(10);//��������
		if(!(M_SWITCH1&0x01))//���������պ�
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _SwitchB(void)
{
	if(!(M_SWITCH2&0x01)) //B��2��΢�����أ��͵�ƽ��Ч
	{
		_Delay(10);//��������
		if(!(M_SWITCH2&0x01))//���������պ�
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _SwitchC(void)
{
	if(!(M_SWITCH3&0x01)) //C��3��΢�����أ��͵�ƽ��Ч
	{
		_Delay(10);//��������
		if(!(M_SWITCH3&0x01))//���������պ�
		{
			return 1;
		}
	}
	return 0;
}

static H_S8 _TouchSwitch(void)
{
	if(!(T_SWITCH&0x01)) //�������أ��͵�ƽ��Ч
	{
		hs_printf("TouchSwitch 111 \n\r");
		_Delay(10);//��������
		if(!(T_SWITCH&0x01))//���������պ�
		{
			//�����ε�����

			return 1;
		}
	}
	return 0;
}

static void _EventHandler(void)
{
	H_U16 _AdcData = 0;
	if(_TouchSwitch())//������������
	{
		//1�����״̬��A��ֹͣ������A-B���˶���״̬��Ϊ _RUNNING_ATOB
		//2�����״̬ΪB��ֹͣ������B-A�˶���״̬��Ϊ _RUNNING_BTOA
		//3�����״̬Ϊ _RUNNING_ATOB ����ֹͣ�˶���״̬Ϊ _RUNNING_ATOB_STOP
		//4�����״̬Ϊ _RUNNING_BTOA����ֹͣ�˶���״̬Ϊ _RUNNING_BTOA_STOP
		//5�����״̬Ϊ_RUNNING_ATOB_STOP������B-A�����˶���״̬Ϊ_RUNNING_ATOB
		//6�����״̬Ϊ _RUNNING_BTOA_STOP������A-B�����ƶ���״̬Ϊ _RUNNING_BTOA
		hs_printf("TouchSwitch Enable \n\r");
		if(g_RunningStatus == _RUNNING_A_STOP)
		{
			RELAY_B = 1;
			RELAY_A = 0;
			g_RunningStatus = _RUNNING_ATOB;
			__MotorStart();
		}else if(g_RunningStatus == _RUNNING_B_STOP)
		{
			RELAY_A = 1;
			RELAY_B = 0;
			g_RunningStatus = _RUNNING_BTOA;
			__MotorStart();
		}else if(g_RunningStatus == _RUNNING_ATOB)
		{
			RELAY_A = 0;
			RELAY_B = 0;
			g_RunningStatus = _RUNNING_ATOB_STOP;
			__MotorStop();
		}else if(g_RunningStatus == _RUNNING_BTOA)
		{
			RELAY_A = 0;
			RELAY_B = 0;
			g_RunningStatus = _RUNNING_BTOA_STOP;
			__MotorStop();
		}else if(g_RunningStatus == _RUNNING_ATOB_STOP)
		{
			RELAY_A = 1;
			RELAY_B = 0;
			g_RunningStatus = _RUNNING_ATOB;
			__MotorStart();
		}else if(g_RunningStatus == _RUNNING_BTOA_STOP)
		{
			RELAY_A = 0;
			RELAY_B = 1;
			g_RunningStatus = _RUNNING_BTOA;
			__MotorStart();
		}
		
	}
	
	if(_SwitchA()&&(g_RunningStatus != _RUNNING_A_STOP))//A�㿪�رպ�
	{
		//ֹͣ����˶���״̬Ϊ_RUNNING_A_STOP
		RELAY_A = 1;
		RELAY_B = 0;
		__MotorStop();
		g_RunningStatus = _RUNNING_A_STOP;
		//1�ſ���LEDϨ��
		SWITCH1_LED = 1;
		hs_printf("_SwitchA Enable \n\r");
	}else if(!_SwitchA())
	{
		SWITCH1_LED = 0;
	}
	
	
	if(_SwitchB() && (g_RunningStatus != _RUNNING_B_STOP))
	{
		//ֹͣ����˶���״̬Ϊ_RUNNING_A_STOP
		RELAY_A = 0;
		RELAY_B = 0;
		__MotorStop();
		g_RunningStatus = _RUNNING_B_STOP;
		hs_printf("_SwitchB Enable \n\r");
	}
	
	if(!_SwitchC() && (g_RunningStatus == _RUNNING_ATOB))
	{
		_Delay(100);//�ȴ�һ��ʱ���
		if(_SwitchC())//3�ſ����ֱպ�״̬
		{
			//��תΪA�㣬״̬Ϊ _RUNNING_BTOA
			RELAY_A = 1;
			RELAY_B = 0;
			g_RunningStatus = _RUNNING_BTOA;
			__MotorStart();
			hs_printf("_SwitchC Enable \n\r");
		}
		
	}
	
	_AdcData = _ADCGetResult();
	hs_printf("[main] _ADCGetResult :%x status :%d \n\r",_AdcData,g_RunningStatus);
	if(((_AdcData > ADC_POWER_MAX) ||(_AdcData < ADC_POWER_MIN)) && (g_RunningStatus == _RUNNING_BTOA))
	{
		//��תΪ��B���˶��� ״̬Ϊ _RUNNING_ATOB
		RELAY_A = 0;
		RELAY_B = 1;
		g_RunningStatus = _RUNNING_ATOB;
		__MotorStart();
		hs_printf("_AdcData Enable \n\r");
	}
}

void main()
{
		__SystemInit();
		_ADCInit();
		_UartOpen();
		g_RunningStatus = _RUNNING_INIT;
		hs_printf(RED"[main] Start \n\r");
		while(1)
		{
			_EventHandler();
			
		}
}

