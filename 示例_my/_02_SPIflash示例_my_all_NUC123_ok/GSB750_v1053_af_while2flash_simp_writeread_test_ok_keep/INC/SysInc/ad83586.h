/****************************************************************************
 * @file:		ad83586.h
 * @version:	v1.0	
 * @breif:		ad83586 header file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __ad83586_h
#define __ad83586_h


#define AD83586_DEVICE_ADDRESS 0X6A
#define AD82586_DEVICE_ADDRESS 0X68


#define AD_MAX_VOLUME	20
// #define AD_RESET_PORT	PB
// #define AD_REST_PIN		9
// #define AD_RESET_PORT_PIN	PB9
// #define AMP_MUTE_PORT		PD
// #define AMP_MUTE_PIN		3
// #define AMP_MUTE_PORT_PIN	PD3
#define AD83586_MUTE			0X0F
#define AD83586_SURROUND_MSK	0X80
#define AD83586_BASS_GAIN_EN	0X40
#define AD83586_CONF_BYTES_COUNT	(0x2c<<1)


typedef enum ad83586_REG{
	AD_SCTL1 = 0, // [7:5] Input Format, [4:0] channel exchange
	AD_SCTL2 = 1, // [5:4] Sampling Frequency, [3:0] PMF : MCLK FS / LR FS
	AD_SCTL3 = 2, // [7] EN_CLK_OUT, [3] Master Mute, [2:0] Channel mute
	AD_MVOL = 3, // Master volume control
	AD_C1VOL = 4, // channel one volume control
	AD_C2VOL = 5, // channel two volume control
	AD_C3VOL = 6, // channel three volume control
	AD_BTONE = 7, // Bass Gain -12db to +12db if BTE in AD_SCTL4 is high
	AD_TTONE = 8,//  Treble Gain -12db to +12db if BTE in AD_SCTL4 is high
	AD_XOF = 9,	// Bass crossover frequency setting. 
				//it is a HPS filter of channel 1/2 in first order
				// and LPS filter of channel 3 in second order
	AD_SCTL4 = 0XA,// DSP setting, 
				    //[7]	SRBP 	surrond enable, 
				    //[6]	BTE 		Bass/Treble enable, 
				    //[5]	TBDRCE 	two Band DRC EN, 
				    //[4]	NGE 		noise gate EN, 
				   // [3]	EQ-LINK
				   // [2]	PSL 		post-scal link,
				   // [1]	DSPB EQ 	bypass
				   // [0]	HPB		DC blocking HPF bypass
	AD_C1CFG = 0XB,// is same as AD_C2CFG
	AD_C2CFG = 0XC, //[4] CxDRCM 	DRC MODE PEAK OR RMS dection
				     //[3] CxPCBP 	channel power clipping bypass
				     //[2] CxDRCBP	channel DRC enable
				     //[1] CxHPFBP	channel HPF bypass 
				     //[0] CxVBP	channel volume master volume bypass		
	AD_C3CFG = 0XD, //[4] C3DRCM 	DRC MODE PEAK OR RMS dection			
				     //[3] C3PCBP 	channel power clipping bypass			
				     //[2] C3DRCBP	channel DRC enable					
				     //[1] C3HPFBP	channel LPF bypass 
				     //[0] C3VBP	channel volume master volume bypass
	AD_LAR = 0XE,//DRC limiter attack/release rate
	AD_SCTL5 = 0X11,	//B[5] SW_RSTB 		Software reset 
					//B[3] SEM 			Single End Mode set 1 is 2.1mode
					//B[2] DIS_MCLK_DET	MCLK detect enable
					//B[1] QT_EN 			Power saving mode enable 
					//B[0] PWM_SEL 		PWM modulation  
	AD_HVUV = 0X12,// pop sound avoid if high voltage supply disappear before low voltage supply
				    // B[7]	DIS_HVUV
				    // B[3:0] 	HV_UV SEL
	AD_NGCFG = 0X13,// Noise gate gain
	AD_CFADDR = 0X14,//
	AD_A1CF1 = 0X15,//
	AD_A1CF2 = 0X16,//
	AD_A1CF3 = 0X17,//
	AD_A2CF1 = 0X18,//
	AD_A2CF2 = 0X19,//
	AD_A2CF3 = 0X1A,//
	AD_B1CF1 = 0X1B,//
	AD_B1CF2 = 0X1C,//
	AD_B1CF3 = 0X1D,//
	AD_B2CF1 = 0X1E,//
	AD_B2CF2 = 0X1F,//
	AD_B2CF3 = 0X20,//
	AD_A0CF1 = 0X21,//
	AD_A0CF2 = 0X22,//
	AD_A0CF3 = 0X23,//
	AD_CFUD = 0X24,//
	AD_QT_SW_LEVEL = 0X1A,//
	AD_VFT = 0X2B//
}AD_REG;


extern u32 ad83586_volume;
extern u32 ad83586_power_down_pin_state;

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


typedef struct {
	AD_REG cmd;
	u8	data;
}ADDATTYPE;

typedef struct{
	enum I2C_STATE state;
	ADDATTYPE ad_data; 
}AD_DATA_CELL;


static __inline void ad83586Reset()
{
// 	GPIO_CLR_PIN(AD_RESET_PORT_PIN);
}

static __inline void ad83586ResetDone()
{
// 	GPIO_SET_PIN(AD_RESET_PORT_PIN);
}


/*-------------------------------------------------------------------------
 * @fucname:	 AMP_MUTE
 * @brief: 	 mute the AD92580
 * @param: 	 NONE
 * @return: 	 NONE
 * @details: 	 mute the AD92580
 *-----------------------------------------------------------------------*/
// static __inline void AMP_MUTE(void)
// {
// 	ad83586_power_down_pin_state = PIN_LOW;
// 	GPIO_CLR_PIN(AMP_MUTE_PORT_PIN);
// }

/*-------------------------------------------------------------------------
 * @fucname:	 AMP_OPEN
 * @brief: 	 unmute the AD92580
 * @param: 	 NONE
 * @return: 	 NONE
 * @details: 	 unmute the AD92580
 *-----------------------------------------------------------------------*/
// static __inline void AMP_OPEN(void)
// {
// 	ad83586_power_down_pin_state = PIN_HIGH;
// 	GPIO_SET_PIN(AMP_MUTE_PORT_PIN);
// }


extern void stopFlashLed(void);
extern void ad83586DecVolume(void);
extern void ad83586AddVolume(void);
extern void ad83586ChangInputVolumeCheck(void);
extern void changeInputVolumeDown(enum AD_STATE state);
extern void changeInputVolumeUp(void);
extern void ad83586Init(void);
extern void ad83586ResumeI2c(void);
extern void ad83586GetAllConf(void);
extern void ad83586SetMute(u8 mute);
extern void ad83586SetDsp(u8 dsp_setting); 
extern void ad83586SetVolume(u8 volume);
extern void ad83586SetTrebleGain(u8 treble_gain);
extern void ad83586SetSubOutGain(u8 bass_gain);
extern void ad83586SetBypass(void);
extern void ad83586EnSurround(void);
extern void ad83586SendCmd(u32 dev_id);
extern void w3dSwitch(void);
extern void testOutputSpeakers(void);
extern void unMute(void);
extern void ad83586ResumeI2c(void);
extern void ad83586MuteSwitch(void);
extern void ad83586SendAllConf(u32 dev_id);
extern void ad83586SendAddr(u8);
extern void ad83586SendEqs(u32 dev_id,U32 start_addr, U32 end_addr);
extern void ad83586SetEqDefault(u32 dev_id,U32 start_addr, U32 end_addr);

#endif

