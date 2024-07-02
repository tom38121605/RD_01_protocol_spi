/****************************************************************************
 * @file:		sta339.h
 * @version:		
 * @breif:		STA339 AMP header file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __sta339_h
#define __sta339_h
#include "sysinc.h"
#define Limiter1_rate		0x54
#define Limter1_att		 0xBc 
#define Limter1_rel		 0xC4 
#define STA339_I2C_ADDR38		0X38
#define STA339_I2C_ADDR3A		0X3A 
#define MAX_VOLUME	32
// #define AMP_MUTE_PORT		PD
// #define AMP_MUTE_PIN		3
// #define AMP_MUTE_PORT_PIN	PD3
#define AMP_CONF_BYTES_COUNT		(44<<1)


typedef enum AMP_REG{
	CONFA = 0, // [7:5] Input Format, [4:0] channel exchange
	CONFB = 1, // [5:4] Sampling Frequency, [3:0] PMF : MCLK FS / LR FS
	CONFC = 2,
	CONFD = 3,
	CONFE = 4,
	CONFF = 5,
	MUTELOC = 6,  // [7,6] LOC, [3:1] Channel mute
	MVOL = 7, // Master volume control
	C1VOL = 8, // channel one volume control
	C2VOL = 9, // channel two volume control
	C3VOL = 0xA, // channel three volume control
	AUTO1 = 0XB, //DRC, clipping
	AUTO2 = 0XC,//  
	AUTO3 = 0xD,	//
	C1CFG = 0XE,// 
	C2CFG = 0XF, 		
				//[4] 
				//[3]
				//[2] 
			 	//[1] 
				//[0] 		
	C3CFG = 0X10,
				//[4]		
				//[3]		
				//[2] 				
				//[1] 
				//[0] 
	TONE = 0X11,//
	L1AR = 0X12,	
				//B[5]
				//B[3] SEM 		
				//B[2] DIS_MCLK_DET	
				//B[1] QT_EN 			
				//B[0] PWM_SEL 		
	L1ATRT = 0X13,
	L2AR = 0X14,// 
	L2ATRT = 0X15,// 
	CFADDR = 0X16,//
	B1CF1 = 0X17,//
	B1CF2 = 0X18,//
	B1CF3 = 0X19,//
	B2CF1 = 0X1A,//
	B2CF2 = 0X1B,//
	B2CF3 = 0X1C,//
	A1CF1 = 0X1D,//
	A1CF2 = 0X1E,//
	A1CF3 = 0X1F,//
	A2CF1 = 0X20,//
	A2CF2 = 0X21,//
	A2CF3 = 0X22,//
	B0CF1 = 0X23,//
	B0CF2 = 0X24,//
	B0CF3 = 0X25,//
	CFUD = 0X26,//
	MPCC1 = 0X27,//
	MPCC2 = 0X28,
	DCC1 = 0X29,
	DCC2 = 0X2A,
	FDRC1 = 0X2B,
	FDRC2 = 0X2C,
	STATUS = 0X2D,
	EQCFG = 0X31,
	EATH1 = 0X32,
	ERTH1 = 0X33,
	EATH2 = 0X34,
	ERTH2 = 0X35,
	CONFX = 0X36,
	SVCA = 0X37,
	SVCB = 0X38,
	RMS0A = 0X39,
	RMS0B = 0X3A,
	RMS0C = 0X3B,
	RMS1A = 0X3C,
	RMS1B = 0X4D,
	RMS1C = 0X3E
}AMP_REG;


extern u32 amp_volume;


typedef struct {
	AMP_REG cmd;
	u8	data;
}AMPDATTYPE;

typedef struct{
	enum I2C_STATE state;
	AMP_REG ad_data; 
}AD_DATA_CELL;

enum AD_STATE{
	AD_IDLE_STATE = 0, // W3D IS IDLE, NOT IN BELOW STATES
	INPUT_CHANGE_STATE = 1, // W3D is  turning down/up volume in change input state
	VOLUME_UP_STATE = 2,	 // w3d is turning up volume
	POWER_OFF_STATE = 3,	// w3d is turning down volume before power off
	MUTE_STATE = 4,		// W3D is turning down volume before mute 
	UN_MUTE_STATE = 5,
	AD_SWITCH_STATE = 6,
	VOLUME_RECOVER = 7,
	VOLUME_DOWN_STATE = 8,
	VOLUME_DOWN_UP = 9,
	AUTO_MUTE = 10
};


extern void ampDecVolume(void);
extern void ampAddVolume(void);
extern void ampInitAllConf(void);
extern u8 ampGetStatus(void);
extern void ampSetVolume(u32 v);
extern void ampSendCmd_38(AMP_REG reg, u8 data);
extern void ampSendCmd_3A(AMP_REG reg, u8 data);
extern void ampResumeI2c(void);
extern void ampSendEqs_38(U32 start_addr, U32 end_addr);

extern void ampSendCallEqs_38(U32 start_addr, U32 end_addr);

extern void ampSendEqs_3A(U32 start_addr, U32 end_addr);
extern void ChangInputVolumeCheck(void);
extern void changeInputVolumeDown(enum AD_STATE state);
extern U32 changeInputVolumeUp(void);
extern void ampEQbypass(void);
extern void ampEQon(void);

extern void ampMuteSwitch(void);
extern void ampMuteBass(void);
extern void ampUnMuteBass(void);
extern void ampInit(void);
extern void stopFlashLed(void);
extern void ampPowerDown(void);
extern void unMute(void);
extern void ampMute(void);
extern u8 amp_i2c_status_check(void);

extern u8 ampGetCmd_38(AMP_REG reg);
#endif


