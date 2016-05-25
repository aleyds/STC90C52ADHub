#ifndef __PLATFOEM_H__
#define __PLATFOEM_H__
#include "REG51.H"
#define STC90C52AD

#ifdef STC90C52AD
sfr WDG_REGISTER=0xe1;//看门狗
sfr ISP_CONTR=0xe7; //ISP/IAP功能及软件复位

sfr P1_ADC_EN = 0x97; // P1口模拟配置寄存器，配置对应的端口为ADC输入口
sfr ADC_CONTER = 0xC5; //ADC 控制寄存器
sfr ADC_DATA_HIGHT = 0xC6; //ADC 转换结果，10位高8位数据
sfr ADC_DATA_LOW = 0xC7; //ADC 转换结果，10位数据的低2位
#endif

#endif