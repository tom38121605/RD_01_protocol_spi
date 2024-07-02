/****************************************************************************
 * @file:		uart.h
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef _uart_h_
#define _uart_h_

#include"sysinc.h"


extern void sendInfo(u8* str);
extern void dps(void);
extern u8* itoh(u8 byte, u8* buf);
extern u32 itoa(u32 i, u8* buf);
extern void clr(void);
extern u8 igetc(void);
extern void iungetc(u8 ch);
extern void iputc(u8 ch);
extern void iputs(u8 *buf);
extern void uartInit(void);
extern void sendBack(void);


void uartdecode(u32 x, u8* str);
void uartdecode2(u32 x, u8* str);
void uartdecodeH(u32 x, u8* str);
void uartdecodeH2(u32 x, u8* str);
void uartdecodeH32(u32 x, u8* str);
void iputchar(u8 ch);

#endif


