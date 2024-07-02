/****************************************************************************
 * @file:		74hc4096.h
 * @version:	v1.0
 * @breif:		74HC4096 header file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef _74hc4096_h_
#define  _74hx4096_h_


extern void send74data(u16 data);
extern u16 get74data(void);
extern void hc4096Init(void);
extern void flashVolumeLed(void);
extern void showVolumeLed(s16 f);
extern void showPowerLed(s16 f);


#endif





























