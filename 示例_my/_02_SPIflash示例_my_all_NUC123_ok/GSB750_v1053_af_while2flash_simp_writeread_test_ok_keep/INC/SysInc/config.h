/****************************************************************************
 * @file:		config.h
 * @version:	v1.00
 * @breif:	config header file	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __config_h
#define __config_h

#include "sysinc.h"

#define WRITEFLASHSTART 0xd0000  //--zz

#define VERSION	"v10.53"

#define TRUE 1

#define FALSE 0

#define NULL 0

#define PIN_HIGH	1	
#define PIN_LOW	0

#define Release 					FALSE

#define DMAI2S					FALSE

#define SPIFLASH_ENABLE			TRUE

#define UART_EN					TRUE 

#define GESTURE_EN				FALSE

#define ESD						FALSE
 
#if(Release)
#define SYS_ERROR_OUTPUT_EN	FALSE
#define INT_TRACE_EN			FALSE 
#define IR_DEBUG_EN				FALSE 
#define PT2314_DEBUG_EN			FALSE
#define FILTER_DEBUG				FALSE
#else
#define SYS_ERROR_OUTPUT_EN	FALSE
#define INT_TRACE_EN			FALSE
#define IR_DEBUG_EN				FALSE
#define FILTER_DEBUG				TRUE
#endif

#define IR_CODE_ERROR_REPAIRE_EN		FALSE
#define ADC_KEY_PDMA_EN				FALSE	// adc use PDMA channel 0


#define VOLUMECONTOL			1	// 0 USE PT2314, 1 USE SE_W58_II
#define VOLUME_CALL			18
#define VOLUME_MUSIC			24
 
#define BATTERY_LOW_VALUE		0X240     //ma 7.1v       39k and 100k 
#define BATTERY_HIGH_VALUE		0X2a0   //ma 8.4v

#define BATTERY_OPEN_DELAY		100  // delay time = BATTERY_OPEN_DELAY * 10ms

#define BT_HANGUP_PULSE_WIDRH 200 //600MS 
#define BT_MATCHING_DELAY		30  // 50*30 = 1500ms

#if(VOLUMECONTOL)
#define DecVolume()	seW58DecVolume()
#define AddVolume()	seW58AddVolume()
#else
#define DecVolume()	pt2314DecVolume()
#define AddVolume()	pt2314AddVolume()
#endif

// the test key press should within this time than dev state change to DEV_TEST
#define DEV_TEST_TIMER 		5000 	// 3000 ms

enum DEVSTATE{
	DEV_TEST,
	DEV_START,
	DEV_STANDBY,
	DEV_STANDBYP,

};

extern enum DEVSTATE dev_state; 
extern enum PT2314_INPUT_CHANNEL	dev_input;
extern enum LED_COLOR input_channel_color[];
enum SYSERROR{
	NONE_SYS_ERROR = 0,
	SYS_IR_ERROR = 1,
	SYS_DAC_ERROR = 2,
	SYS_I2C_ERROR = 3
};

extern enum SYSERROR sys_error; 
extern U32 	total_input_channel;

#define SYS_ERROR_SIGNAL 0xff	//output the sys error signal in max voltage, also use 0 when sys is busy


#endif

