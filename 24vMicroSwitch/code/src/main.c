#include "base_type.h"
#include <platform.h>
#include "intrins.h"
#include "adc.h"

static void __Delay(H_U32 ms)
{
	H_U32 i = 0;
	H_U32 j = 0;
	for(i = ms; i> 0; i--)
	{
		for(j = 110; j > 0; j--)
		{
			_nop_();
		}
	}
}

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

//static void 

void main()
{
		__SystemInit();
		_ADCInit();
}