#include "base_type.h"
#include "platform.h"
#include "intrins.h"
#include "adc.h"

extern void _Delayms(unsigned int ms);
/*----------------------------
��ʼ��ADC
----------------------------*/
void _ADCInit()
{
    P1ASF = 0xff;                   //����P1��ΪAD��
    ADC_RES = 0;                    //�������Ĵ���
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    _Delayms(2);                       //ADC�ϵ粢��ʱ
}


/*----------------------------
��ȡADC���
----------------------------*/
 BYTE _ADCGetResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //�ȴ�4��NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //����ADC���
}


/*
H_U16 _ADCGetResult(void)
{
		__ADCStart(_ADC_IN0, ADC_SPEEDLL);//����ADCת��

		//�ȴ�
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_Delay(1);
		
		while(!(ADC_CONTER&ADC_FLAG));//ADC_FLAG=1ʱת�����
		//hs_printf("ADC_CONTER:%x  %x\n\r",ADC_DATA_HIGHT, ADC_DATA_LOW);
	
		ADC_CONTER &= ~ADC_FLAG;//ֹͣADCת�������������־
		return (H_U16)(((ADC_DATA_HIGHT&0xFF)<<2)| ADC_DATA_LOW&0x3);
}
*/