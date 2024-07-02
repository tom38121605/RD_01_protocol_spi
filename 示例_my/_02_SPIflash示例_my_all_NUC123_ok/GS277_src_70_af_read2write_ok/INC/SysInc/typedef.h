/****************************************************************************
 * @file:		typedef.h
 * @version:	V1.00	
 * @breif:	typedef header file	
 * @date: 	2013/02/21		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __typedef_h
#define __typedef_h

typedef   signed          	char s8;
typedef   signed short	int	s16;
typedef   signed           	int	s32;
typedef   signed       __int64	s64;

    /* exact-width unsigned integer types */
typedef unsigned          	char u8;
typedef unsigned short	int 	u16;
typedef unsigned           	int 	u32;
typedef unsigned       	__int64 u64;

typedef	signed          	char S8;
typedef   	signed short     int 	S16;
typedef   	signed           	int   	S32;
typedef   	signed       __int64	S64;

    /* exact-width unsigned integer types */
typedef unsigned          	char U8;
typedef unsigned short	int	U16;
typedef unsigned		int    U32;
typedef unsigned       __int64 	U64;

typedef u8		bool;
typedef u8 		BOOL;
typedef u8		Timer_ID;


typedef struct {
	u8 byte0;
	u8 byte1;
	u8 byte2;
	u8 byte3;
}U32_BYTE;

typedef struct {
	u16 word0;
	u8 byte2;
	u8 byte3;
}U32_WB;

typedef  void (*FUNC_Timer_Task)(void); 

extern void __nop(void);      //ma 20150629  add nop declare 
#endif


