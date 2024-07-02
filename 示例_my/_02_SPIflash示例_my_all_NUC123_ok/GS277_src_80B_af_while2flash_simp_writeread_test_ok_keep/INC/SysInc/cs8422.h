/****************************************************************************
 * @file:		cs8422.h
 * @version:	v1.00
 * @breif:	cs8422 header file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __cs8422_h
#define __cs8422_h
#include "sysinc.h"

#define CS8422_ADDR		0X2E
#define CU		0X55AA
#define MU		0XAA55

typedef struct {
	u8 cmd;
	u8	data;
}CS8422_DATA_CELL;


typedef struct{
	volatile enum I2C_STATE state;
	CS8422_DATA_CELL common;
}CS8422_CMDT;


enum CS8422_INPUT_CHANNEL{
	BLUETOOTH = 0,
	USERDEF = 1, 	
	AUX = 2 ,	
	OPTICAL = 3, 	
};

extern enum CS8422_INPUT_CHANNEL cs8422_input_table[4];


extern enum CS8422_INPUT_CHANNEL cs8422_input;

extern enum CS8422_INPUT_CHANNEL getNextInput(void);
extern void changeInputChannel(void);
extern u32 Dwhp83CuMuDetect(void);



extern void cs8422Init(void);     //ma 20150629  declare  cs8422Init
extern void cs8422InitAllConf(void);     //ma 20150629  declare  cs8422InitAllConf

extern void cs8422SRCSourceSDIN(void);    //ma 20150709 

#endif

