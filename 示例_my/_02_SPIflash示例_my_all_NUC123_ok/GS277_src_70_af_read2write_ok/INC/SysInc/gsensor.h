// /****************************************************************************
//  * @file:		gsensor.h
//  * @version:	v1.00
//  * @breif:		gesture sensor header file
//  * @date: 		
//  * 
//  * @note 		
//  *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
//  * 
//  ****************************************************************************/
// #ifndef __GSENSOR_H__
// #define __GSENSOR_H__


// #define GS_CE_PORT			PC
// #define GS_CE_PIN			13
// #define GS_CE_PORT_PIN		PC13

// #define GS_IRQ_PORT			PB 
// #define GS_IRQ_PIN			7
// #define GS_IRQ_PORT_PIN		PB7

// #define GS_I2C_ADDR	0xD2

// typedef enum {
// 	N_OP = 0,
// 	TEST,
// 	READ_STATUS,
// 	SET_STATUS,
// 	READ_DATA,
// 	FIFO_SHFIT,
// 	SLEEP,
// 	TEMP_READ,
// 	BROAD_ADDR,
// 	FIFO_CLEAR
// }GSCMD;

// typedef struct _gstauts{
// 		volatile enum I2C_STATE state;
// 		GSCMD cmd;
// 		u8 buf[15];
// }GStatusbuf;

// typedef struct {
// 	u16 temp;
// 	S16 ch1;
// 	S16 ch2;
// 	S16 ch3;
// 	S16 ch4;
// }GsData;

// extern void gsensor_init(void);
// extern void gsReadData(void);
// extern void gs_event(void);
// #endif





