/****************************************************************************
 * @file:		seW58.h
 * @version:	v1.00	
 * @breif:		SE_W58_II control header file
 * @date: 	2013/3/15
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/

#ifndef __seW58_h__
#define __seW58_h__

#define SE_W58_II_ADDR	0x90
#define SE_MAX_VOLUME	32
#define SE_W58_PRESEL_PIN1 9
#define SE_W58_PRESEL_PIN2 10
#define SE_W58_DEFAULT_VOLUME 0xcd

enum W3D_CMD{
	w3d_cmd_Volume =  0X0,
	w3d_cmd_Ambiance = 0X1,
	w3d_cmd_Balance = 0X2,
	w3d_cmd_Clarity = 0X3,
	w3d_cmd_SubOutGain = 0X4,
	w3d_cmd_Bypass = 0X5,
	w3d_cmd_BypassGain = 0X6,
	w3d_cmd_Mute = 0X7,
	w3d_cmd_ChannelCheck = 0X8,
	w3d_cmd_Status1func = 0X9,
	w3d_cmd_SurroundGain = 0XA,
	w3d_cmd_CenterGain = 0XB
};


enum W3D_STATE{
	W3D_IDLE_STATE = 0, // W3D IS IDLE, NOT IN BELOW STATES
	INPUT_CHANGE_STATE = 1, // W3D is  turning down/up volume in change input state
	VOLUME_UP_STATE = 2,	 // w3d is turning up volume
	POWER_OFF_STATE = 3,	// w3d is turning down volume before power off
	MUTE_STATE = 4,		// W3D is turning down volume before mute 
	UN_MUTE_STATE = 5,
	W3D_SWITCH_STATE = 6,
	VOLUME_RECOVER = 7,
	VOLUME_DOWN_STATE = 8,
	VOLUME_DOWN_UP = 9
};

typedef struct {
	enum W3D_CMD cmd;
	u8	data;
}W3DDATTYPE;

typedef struct{
	enum I2C_STATE state;
	W3DDATTYPE w3d_data; 
}W3D_DATA_CELL;



extern W3D_DATA_CELL w3d_volume_change;

extern u32 w3d_volume;
extern u8 w3d_lock;

extern void stopFlashLed(void);
extern void seW58DecVolume(void);
extern void seW58AddVolume(void);
extern void seW58ChangInputVolumeCheck(void);
extern void changeInputVolumeDown(enum W3D_STATE state);
extern void changeInputVolumeUp(void);
extern void seW58Init(void);
extern void seW58Getstatus(void);
extern void seW58ResumeI2c(void);
extern void seW58sendCmd(void);
extern void seW5SendAllStatus(void);
extern void seW58SetAmbiance(u8 amblance);
extern void seW58SetBalance(u8 balance);
extern void seW58SetClarity(u8 clarity);
extern void seW58SetSubOutGain(u8 sub_out_gain);
extern void seW58SetBypass(u8 bypass);
extern void seW58SetBypassGain(u8 by_pass_gain);
extern void seW58SetMute(u8 mute);
extern void seW58SetChannelCheck(u8 channel_check) ;
extern void seW58SetStatus1func(u8 status1_func);
extern void seW58SetSurroundGain(u8 surround_gain); 
extern void seW58SetVolume(u8 volume);
extern void w3dSwitch(void);
extern void testOutputSpeakers(void);
extern void w3d_mute(void);
extern void unMute(void);
extern void turnVolumeZero(void);

extern u32  ismute(void);       //ma  20150624

extern void mute(void);     //ma 20150624
extern u32 isMute(void);     //ma 20150629  declare  isMute
extern void seW58DefaultVolume(void);                     //ma 20150713
#endif


