#include "base_type.h"
#include "platform.h"
#include "intrins.h"
#include "adc.h"

void _ADCInit(void)
{
		P1_ADC_EN = 0xFF;
		ADC_DATA_HIGHT = 0x0;
		ADC_DATA_LOW = ADC_DATA_LOW&0xFC;
		ADC_CONTER = ADC_SPEEDLL;
}

static void __ADCStart(H_U8 _Channal , H_U8 _Speed)
{
		ADC_CONTER = (_Speed | ADC_START| _Channal);
		return;
}

H_U16 _ADCGetResult(void)
{
		__ADCStart(0, ADC_SPEEDLL);//启动ADC转换
	
		//等待
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		
		while(!(ADC_CONTER&ADC_FLAG));//ADC_FLAG=1时转换完成
	
		ADC_CONTER &= ~ADC_FLAG;//停止ADC转换，清楚结束标志
		return (((ADC_DATA_HIGHT)<<2)| ADC_DATA_LOW&0x3);
}