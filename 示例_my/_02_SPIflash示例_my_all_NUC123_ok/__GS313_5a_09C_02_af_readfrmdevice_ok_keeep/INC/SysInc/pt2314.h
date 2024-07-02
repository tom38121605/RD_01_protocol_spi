/****************************************************************************
 * @file:		pt2314.h
 * @version:	V1.00
 * @breif:		the header file for pt2314
 * @date: 	2013/3/13
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef	__pt2314_h__
#define	__pt2314_h__

#define PT2314_ADDR	0X88
#define PT2314_IN1_CODE	0X5c
#define PT2314_IN2_CODE	0X5d
#define PT2314_IN3_CODE	0X5e
#define PT2314_IN4_CODE	0X5f 
#define AUDIO_DET_PORT_PIN	PD1
#define AUDIO_DET_PORT	PD
#define AUDIO_DET_PIN	1
#define SPDIF_SEL_PORT	PD
#define SPDIF_SEL_PORT_PIN 	PD4
#define SPDIF_SEl_PIN 	4

enum PT2314_FUNC_MODE{
	PT2314_VOLUME_CTRL,	
	PT2314_SPEAK_ATTL_CTRL, 
	PT2314_SPEAK_ATTR_CTRL ,
	PT2314_AUDIO_SWITCH_CTRL,	
	PT2314_BASS_CTRL,
	PT2314_TREBLE_CTRL
};
extern enum PT2314_FUNC_MODE pt2314_mode;

enum PT2314_INPUT_CHANNEL{ //--zz
	BLUETOOTH = 0,
	LINEIN = 1, 	
	FLASHIN = 2,
	MUTEIN =3,
	OPTICAL = 4, 	
	COAXIAL = 5 	
	
};


extern enum PT2314_INPUT_CHANNEL pt2314_input_table[];
extern enum PT2314_INPUT_CHANNEL pt2314_input;


typedef struct{
	enum I2C_STATE state;
	u8 volume;
	u8 speaker_ATT_L;
	u8 speaker_ATT_R;
	u8 audio_switch;
	u8 bass;	// bass voice control
	u8 treble;	//high voice control
}PT2314_STATU;

extern PT2314_STATU pt2314_status;

//--extern void changeInputChannel(void);   //--2015.07.22
//--extern void pt2314DataResume(void);     //--2015.07.22  
extern void pt2314Init(void);
extern void pt2314ResumeI2c(void);
extern void getChangeVolume(void);

/*-------------------------------------------------------------------------
 * @fucname:	 audioDetect
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static __inline U32 _audioDetect()
{
	return GPIO_GET_PIN(AUDIO_DET_PORT_PIN);
}


/*-------------------------------------------------------------------------
 * @fucname:	 selOptical
 * @brief: 	 select optical input
 * @param: 	 none
 * @return: 	 none
 * @details: 	 select optical input
 *-----------------------------------------------------------------------*/
static __inline void selOptical()
{
	GPIO_SET_PIN(SPDIF_SEL_PORT_PIN);
}

/*-------------------------------------------------------------------------
 * @fucname:	 selCoaxial
 * @brief: 	 select coaxial input
 * @param: 	 none
 * @return: 	 none
 * @details: 	 select coaxial input
 *-----------------------------------------------------------------------*/
static __inline void selCoaxial()
{
	GPIO_CLR_PIN(SPDIF_SEL_PORT_PIN);
}


#endif



