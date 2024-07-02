/****************************************************************************
 * @file:		uart.h
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef _uart_h_
#define _uart_h_

#include"sysinc.h"
#define ATCMD_COUNT		5
#define INDCMD_COUNT	6
#define SYSCMD_COUNT	1

enum BTSTATE{
	SYS_INFO,
	SYS_READ_VAR,
	SYS_SET_VAR,
	BT_PAIR,
	BT_PAIRING,
	BT_UNPAIRING,
	BT_PAIRED,
	BT_CALLCOME,
	BT_CLRLOG,
	BT_CALLING,
	PRINT_LOG,
	DUMP_FLASH,
	BT_HANGUPED,
	BT_REST,
	BT_UNDEF
};

extern enum BTSTATE bt_state;

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
extern void sendStartInfo(void);
extern void uartSendCmd(u32 count, u8* buf);               //add uart send commend 
extern u8 WB_paired_flag;
extern u8 WB_paired_back_flag;

void uartdecodeH(u32 x, u8* str);
void uartdecodeH32(u32 x, u8* str);
#endif


