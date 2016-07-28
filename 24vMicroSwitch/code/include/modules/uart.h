#ifndef __UART_H__
#define __UART_H__

extern void InitUart();
extern void SendData(BYTE dat);
extern void SendString(const BYTE *str);
extern void SendDec(WORD dec);
extern void SendHex(WORD hex);
void SendPrintf(const BYTE *fmt, ...);

#endif