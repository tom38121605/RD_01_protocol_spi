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

void capIntEnable(void);
void getDevCofig(void);
void setConfig(void);

extern u8 SB_flag;

/*
  * define key_word values ,  
  * key remap should change ADC_IRQHandler() in adcKey.c 
  * AND IrKeyRemap() in irKey.c
  */
enum KEYWORD{	
	//NONE_KEY = 0x0,
	TOUCH_KEY0 =0X10,  //MULTIROOM
	TOUCH_KEY1,        //V+
	TOUCH_KEY2,        //V-
	TOUCH_KEY3,        //SOURCE
	TOUCH_KEY4,        //NC
	TOUCH_KEY5,        //POWER
	TOUCH_KEY0_BOTH,   //IR_MUTE
	
	TOUCH_KEY0_LONG =0X20,
	TOUCH_KEY1_LONG,
	TOUCH_KEY2_LONG,
	TOUCH_KEY3_LONG,  
	TOUCH_KEY4_LONG,   //--NC
	TOUCH_KEY5_LONG,
	
	NONE_KEY = 0x55,
	BATTERY_STATE_KEY,
	TEST_MODE_KEY,
	POWER_KEY, 				 
	MULTIROOM_KEY,
	STEREO_KEY,
	SOUND_CLOSE_KEY,		 
	INC_KEY,					 
	DEC_KEY,					 
	CHANNEL_SWITCH_KEY,	 
	BLUETOOTH_KEY,		 
	W3D_KEY,				 
	MOVIE_KEY,			 
	MUSIC_KEY,	
	GAME_KEY,	
	WRONG_KEY,	 
	REW_KEY,
	FWD_KEY,
	PAIR_KEY,
	HAND_FREE_KEY,
	BT_RST_KEY,
	SIRI_KEY,
};

extern void delStandbytkey(enum KEYWORD  keyword);
extern void delStartkey(enum KEYWORD keyword);

#endif
    


       
  
  
   
  
   
