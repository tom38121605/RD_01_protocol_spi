/****************************************************************************
 * @file:		appKey.h
 * @version:	V1.00
 * @breif:	key application header file 
 * @date: 	2013/03/01
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
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
	NONE_KEY= 0x55,
	POWER_KEY,				// adc_s7
	SOUND_CLOSE_KEY ,		// adc_s0
	INC_KEY ,					// adc_s6
	DEC_KEY,					// adc_s5
	CHANNEL_SWITCH_KEY,	// adc_s4
	BLUETOOTH_KEY,			// adc_s3
	W3D_KEY,				// adc_s2
	MOVIE_KEY,				// adc_s1
	MUSIC_KEY,	
	GAME_KEY,	
	WRONG_KEY ,		// key resistance is out of range 5%  or undefined ir_key
	MULTIROOM_KEY,
	REW_KEY,
	FWD_KEY,
	FACTORY_KEY_1,
	FACTORY_KEY_2,
	FACTORY_KEY_3,
	FACTORY_KEY_4,
	
	/******add  for ir  LAB test MODE ****/  //20150720 
	LAB_BT_MODE,        			//0X30
	LAB_BT_PAIRING_MODE,      //0X31
	LAB_OPTICAL_MODE,         //0X32
	LAB_LINE_IN_MODE,         //0X33
	LAB_NET_WORK_RX_MODE,     //0X34
	LAB_VOL_MAX,         			//0X2F
	LAB_SPEAK_1,        		  //0X21
	LAB_SPEAK_2,        		 	//0X22
	LAB_SPEAK_3,        			//0X23
	LAB_SPEAK_4,         	  	//0X24
	LAB_SPEAK_5,         	 	  //0X25
	LAB_SPEAK_ALL        		  //0X26
};		


extern void delStandbytkey(enum KEYWORD  keyword);
extern void delStartkey(enum KEYWORD keyword);
extern void setConfig(void);      //ma 20150514 declare setconfig
extern u8 SB_flag;
// extern u8 SB_play_flag; //ma 20150515 declare SB playflag
extern u8 bt_disable_reconnect;    //ma 20150520   declare  bereconect
extern u8 bt_power_flag;             //ma 20150526   add bt power flag to enable pairing


extern u8 bt_paring_flag;           //ma 20150527 
extern u8 mic_mute_flag;    //ma 20150528
extern u8 readeeprom(u32 address);   //ma 20150612
extern u32 writeeeprom(u32 data, u32 address);     //ma 20150612
extern void getConfig(void);          //ma 20150612
extern u32 dc_auto_down_flag;         //ma 20150629   declare dc auto down flag 

/********change channal set color  at once  and  after 1sec handle*********/
extern u8 change_channal_flag;     //ma 20150708   add change_channal_flag 


extern enum CS8422_INPUT_CHANNEL dev_input_before_change;     //ma 20150708
// extern u8 set_cu_flag;       //ma 20150714                 //MA 20150813
extern u8 play_key_flag;          //ma 20150714  


 extern u8 soundbar_multiroom_flag;         //ma 20150717

// extern u8 HFP_factory_test_flag;           //ma 20150724
extern  u8  Core_tx_flag;      //ma 20150813

extern u8 TX_sound_flag;     //ma 20150817 add  master transmission sound 
#endif


