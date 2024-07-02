/****************************************************************************
 * @file:		appKey.h
 * @version:	V1.00
 * @breif:	key application header file 
 * @date: 	2013/03/01
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __appKey_h
#define __appKey_h

#include "sysinc.h"

/*
  * define key_word values ,  
  * key remap should change ADC_IRQHandler() in adcKey.c 
  * AND IrKeyRemap() in irKey.c
  */
enum KEYWORD{	
	NONE_KEY = 0xff,
	POWER_KEY = IR_K1,				// adc_s7
	SOUND_CLOSE_KEY = IR_K2,		// adc_s0
	INC_KEY = IR_K3,					// adc_s6
	DEC_KEY = IR_K4,					// adc_s5
	CHANNEL_SWITCH_KEY = IR_K5,	// adc_s4
	BLUETOOTH_KEY = IR_K6,			// adc_s3
	W3D_KEY = IR_K7,				// adc_s2
	MOVIE_KEY = IR_K8,				// adc_s1
	MUSIC_KEY = IR_K9,				
	GAME_KEY = IR_K10,
	W3D_MODE_KEY,					
	DEBUG_EN_KEY,
	WRONG_KEY		// key resistance is out of range 5%  or undefined ir_key
};

#define LED_SAVING_TIME 1
#define BT_LED_PARING_TIME  5
extern void delStandbytkey(enum KEYWORD  keyword);
extern void delStartkey(enum KEYWORD keyword);

extern u8 change_source_mute;
extern u8 change_source_counter;

extern void setConfig(void);        //ma 20150526
extern void getConfig(void);        //ma 20150526  
extern u8 read_upgrade_eeprom(u32 address);
extern void bootLDROM(void);
extern u8 play_demo_sound_mode;

extern u8 usb_i2c_enable;
extern u8 bt_play_music_flag;
extern u8 bt_play_music_before;
extern u8 bt_play_music_change_delay;
extern u8 wireless_mode_open;

extern u32 led_saving_couter;
extern u8 led_saving_power_flag;

extern u8 led_battery_low_mode;
extern u8 music_channel_flag;   // 0 for stero ; 1 for left channel; 2 for right channel

extern u8 call_eq_mode;      //ma 20161123 call eq function 
#endif


