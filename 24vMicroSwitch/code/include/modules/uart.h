#ifndef __UART_H__
#define __UART_H__

#define DEBUG_SWITCH
#define LEVEL_DEBUG 			(1)
#define LEVEL_WARNING			(2)
#define LEVEL_ERROR				(3)
#define LEVEL_INFO				(4)
#define DEBUG_LEVEL_ENABLE		(0) /*0:ȫ��  �������ڸ�����ֵ��Ϊ����*/

#define NONE         			"\033[m" 
#define RED          			"\033[0;32;31m" 
#define LIGHT_RED    			"\033[1;31m" 
#define GREEN        			"\033[0;32;32m" 
#define LIGHT_GREEN  			"\033[1;32m" 
#define BLUE         			"\033[0;32;34m" 
#define LIGHT_BLUE   			"\033[1;34m" 
#define DARY_GRAY    			"\033[1;30m" 
#define CYAN         			"\033[0;36m" 
#define LIGHT_CYAN   			"\033[1;36m" 
#define PURPLE       			"\033[0;35m" 
#define LIGHT_PURPLE 			"\033[1;35m" 
#define BROWN        			"\033[0;33m" 
#define YELLOW       			"\033[1;33m" 
#define LIGHT_GRAY   			"\033[0;37m" 
#define WHITE        			"\033[1;37m"

void _UartOpen(void);
void _UartPrintf(H_U8 *fmt, ...);

/*
#if defined(DEBUG_SWITCH)

#define hs_printf(moulde,level,fmt,args...)\
do{\
	if(level >= DEBUG_LEVEL_ENABLE)\
	{\
		if(level == LEVEL_DEBUG)\
		{\
			_UartPrintf("["moulde"][%d]"fmt"\n\r",level,##args);\
		}\
		else if(level == LEVEL_WARNING)\
		{\
			_UartPrintf(BLUE"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
		else if(level == LEVEL_ERROR)\
		{\
			_UartPrintf(RED"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
		else \
		{\
			_UartPrintf(GREEN"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
	}\
}while(0);

#define hs_printf_array(array,len)\
do{\
	H_U32 i;\
	for(i = 0; i < len; i++)\
	{\
		if(i%16 == 0)\
		{\
			_UartPrintf("\n\r");\
		}\
		_UartPrintf("%x ",array[i]);\
	}\
	_UartPrintf("\n\r len:%d \n\r",i);\
}while(0);
#else
#define hs_printf(moulde,level,fmt,args...)			//do{}while(0);
#define hs_printf_array(array,len)					//do{}while(0);
#endif
*/

#endif