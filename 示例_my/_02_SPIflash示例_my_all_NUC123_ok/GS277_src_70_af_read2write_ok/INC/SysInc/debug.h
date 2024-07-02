/*-------------------------------------------------------------------------
 * @file:		debug.h
 * @version:	V1.00	
 * @breif:	debug header file	
 * @date: 	2013/02/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
	
#ifndef __debug_h
#define __debug_h

#define DAC_A0_PIN		0

#if (DAC_A0_PIN == 1)
#define DEBUG_DAC_ADDRESS 	0x9a 	// DAC5571 A0 pin is high
#else
#define DEBUG_DAC_ADDRESS 	0x98 	// DAC5571 A0 pin is low
#endif

#define DEBUG_SIGNAL	DNULL


#define DEBUG_TASK_TIME	5// DEBUG TASK DELAY	 work
#define DEBUG_MODE_1K		0X10	// power-down with 1k to GND
#define DEBUG_MODE_100K		0X20 // power-down with 100k to GND
#define DEBUG_MODE_HIGH		0X30 //power-down with high impedance to GND


enum DEBUG{
	DNULL = 0,
	DMIPS,
	DADC
};
extern enum DEBUG debug; 	// indicate the debug type 

extern u32 i2c_tx_c;	//i2c send data count
extern volatile u32 DMipsCounter;	
extern volatile u32 DDacCounter;
#define DEBUG_MODE_OP		0X00	// DAC5571 normal


extern void debugInit(void);
void OutPutDebugSignal(u8 signal);
extern void i2cDacHandler(void);
extern void debugResumeI2c(void);

#endif



