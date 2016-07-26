#ifndef __GPIO_H__
#define __GPIO_H__
#include "STC15W404AS.h"


sfr WDG_REGISTER=0xe1;//看门狗
sfr ISP_CONTR=0xe7; //ISP/IAP功能及软件复位

//微动开关
sbit M_SWITCH1=P3^3;//A点1号微动开关
sbit M_SWITCH2=P3^6;//B点2号微动开关
sbit M_SWITCH3=P3^7;//C点3号微动开关
sbit T_SWITCH =P3^2;//触摸开关

//LED	
sbit EXTERNAL_LED=P1^5;
sbit SWITCH1_LED=P1^7;//1号微动开关显示灯，离开开关时LED点亮  接触1号开关时LED熄灭
sbit SWITCH2_LED=P1^6;

sbit ALARM_SWITCH=P3^4;

//继电器开关
sbit RELAY_C = P3^5;  //电机开关
sbit RELAY_A = P1^3;  //控制运动方向  1
sbit RELAY_B = P1^4;  //控制运动方向  2

//ADC寄存器
sfr P1_ADC_EN = 0x97; // P1口模拟配置寄存器，配置对应的端口为ADC输入口
sfr ADC_CONTER = 0xC5; //ADC 控制寄存器
sfr ADC_DATA_HIGHT = 0xC6; //ADC 转换结果，10位高8位数据
sfr ADC_DATA_LOW = 0xC7; //ADC 转换结果，10位数据的低2位


#endif