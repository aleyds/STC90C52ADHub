#include "base_type.h"
#include "platform.h"
#include "intrins.h"
#include "adc.h"

extern void _Delayms(unsigned int ms);
/*----------------------------
初始化ADC
----------------------------*/
void _ADCInit()
{
    P1ASF = 0xff;                   //设置P1口为AD口
    ADC_RES = 0;                    //清除结果寄存器
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
    _Delayms(2);                       //ADC上电并延时
}


/*----------------------------
读取ADC结果
----------------------------*/
 BYTE _ADCGetResult(BYTE ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_();                        //等待4个NOP
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //返回ADC结果
}


/*
H_U16 _ADCGetResult(void)
{
		__ADCStart(_ADC_IN0, ADC_SPEEDLL);//启动ADC转换

		//等待
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_Delay(1);
		
		while(!(ADC_CONTER&ADC_FLAG));//ADC_FLAG=1时转换完成
		//hs_printf("ADC_CONTER:%x  %x\n\r",ADC_DATA_HIGHT, ADC_DATA_LOW);
	
		ADC_CONTER &= ~ADC_FLAG;//停止ADC转换，清楚结束标志
		return (H_U16)(((ADC_DATA_HIGHT&0xFF)<<2)| ADC_DATA_LOW&0x3);
}
*/