/****************************************************************************
 * @file:		config.h
 * @version:	v3.47
 * @breif:	config header file	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __config_h
#define __config_h

#include "sysinc.h"

#define version	"v5.30"
#define TRUE 1

#define FALSE 0

#define NULL 0

#define Release 					FALSE 

#define DMAI2S					FALSE

#define SPIFLASH_ENABLE			TRUE

#define GESTURE_EN				FALSE 

#define UART_EN					FALSE 

#if(Release)
#define SYS_ERROR_OUTPUT_EN	FALSE 
#define INT_TRACE_EN			FALSE 
#define IR_DEBUG_EN				FALSE 
#define PT2314_DEBUG_EN		FALSE
#else
#define SYS_ERROR_OUTPUT_EN	FALSE
#define INT_TRACE_EN			FALSE
#define IR_DEBUG_EN				TRUE
#endif

#define IR_CODE_ERROR_REPAIRE_EN		FALSE
#define ADC_KEY_PDMA_EN				FALSE	// adc use PDMA channel 0


#define VOLUMECONTOL			1	// 0 USE PT2314, 1 USE SE_W58_II
#define VOLUME_CALL				20
#define VOLUME_MUSIC			20

#define BATTERY_LOW_VALUE		0X3A0             //MA 20150626    CHANGE 335 TO 3A9  14.05 v     //MA CHANGE 3A9 TO 3A0
#define BATTERY_HIGH_VALUE		0X3b2           //MA 20150626    CHANGE 335 TO 3A9  14.20 v

// #define BATTERY_LOW_VALUE		0X33B             //MA 20150629    CHANGE 335 TO 33B  14.38v    5%
// #define BATTERY_HIGH_VALUE		0X346           //MA 20150626    CHANGE 335 TO 3A9  14.6 v     10%
// #define BATTERY_HIGHER_VALUE		0X37b           //MA 20150626    CHANGE 335 TO 17b 15.6 v     50%
// #define BATTERY_LOW_VALUE		0X3A9             //MA 20150626    CHANGE 335 TO 3A9  14.05 v
// #define BATTERY_HIGH_VALUE		0X3b2           //MA 20150626    CHANGE 335 TO 3A9  14.20 v

// #define BATTERY_LOW_VALUE		0X335         //MA 20150625    14.2 v
// #define BATTERY_HIGH_VALUE		0X3b2       //MA 20150625    16.46 v
// #define BATTERY_LOWER_VALUE   0X32C       //MA 20150625    14.05 v   5%  
// #define BATTERY_EMPTY_VALUE   0X323       // MA 20150625  13.9 v == 0%

#define BATTERY_OPEN_DELAY		100  // delay time = BATTERY_OPEN_DELAY * 10ms  //ma 20150703200 to 100  change 2  sec to 1 sec when battery mode power on 

#define BT_HANGUP_PULSE_WIDRH 600 //600MS
#define BT_MATCHING_DELAY		50  // 50*30 = 1500ms

#define AUDIO_THRESHOLD		15

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
extern enum CS8422_INPUT_CHANNEL dev_input;
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

