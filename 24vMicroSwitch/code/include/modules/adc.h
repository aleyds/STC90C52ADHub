#ifndef __90C52AD_ADC_H__
#define __90C52AD_ADC_H__

#define ADC_FLAG			(0x10)		//ADC转换结束标志
#define ADC_START			(0x08)		//ADC转换开始标志
#define ADC_SPEEDHH		(0x00)		//89 clocks
#define ADC_SPEEDH		(0x20)		//178 clocks
#define ADC_SPEEDL		(0x40)		//356 clocks
#define ADC_SPEEDLL		(0x60)		//534 clocks
void _ADCInit(void);
H_U16 _ADCGetResult(void);

#endif