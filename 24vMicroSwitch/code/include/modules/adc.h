#ifndef __90C52AD_ADC_H__
#define __90C52AD_ADC_H__

#define ADC_FLAG			(0x10)		//ADC转换结束标志
#define ADC_START			(0x08)		//ADC转换开始标志
#define ADC_SPEEDHH		(0x00)		//89 clocks
#define ADC_SPEEDH		(0x20)		//178 clocks
#define ADC_SPEEDL		(0x40)		//356 clocks
#define ADC_SPEEDLL		(0x60)		//534 clocks

enum {
	_ADC_IN0 = 0,	//P1^0
	_ADC_IN1,		//P1^1
	_ADC_IN2,		//P1^2
	_ADC_IN3,		//P1^3
	_ADC_IN4,		//P1^4
	_ADC_IN5,		//P1^5
	_ADC_IN6,		//P1^6
	_ADC_IN7,		//P1^7
};
void _ADCInit(void);
H_U16 _ADCGetResult(void);

#endif