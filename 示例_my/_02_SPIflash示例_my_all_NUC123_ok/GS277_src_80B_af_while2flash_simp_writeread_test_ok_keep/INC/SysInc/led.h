/****************************************************************************
 * @file:		led.h
 * @version:	V1.00
 * @breif:	Led header file
 * @date: 	2012/2/22
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __led_h
#define __led_h

#include "sysinc.h"

#define LED_PORT	PA
#define RLED_PIN		12
#define GLED_PIN	13
#define BLED_PIN		14		
#define LED_FLASH_RATE	30
#define RLED_POE		PWM_POE0
#define GLED_POE		PWM_POE1
#define BLED_POE		PWM_POE2

#define RLED_PWM_EN	PWM_CH0_EN
#define GLED_PWM_EN	PWM_CH1_EN
#define BLED_PWM_EN	PWM_CH2_EN

#define RLED_PIN_PWM	SYS_MFPA12_PWM0
#define GLED_PIN_PWM	SYS_MFPA13_PWM1
#define BLED_PIN_PWM	SYS_MFPA14_PWM2



//       RED -- PWM0     GREEN -- PWM1   BLUE -- PWM2
//        |                       |                          |
enum {RED_PWM = 0, GREEN_PWM = 1, BLUE_PWM = 2}; // define  PWM CHANNEL corresponding colors

enum LED_COLOR{
	DARK_COLOR = 0,
	RED_COLOR = 1,
	GREEN_COLOR = 2,
	BLUE_COLOR = 3,
	YELLOW_COLOR = 4,
	PURPLE_COLOR = 5,
	WHITE_COLOR = 6,
	SOE_COLOR = 7,
	LIGHT_BLUE_COLOR = 8,
	BLUETOOTH_COLOR = 9,
	PAIRING_COLOR,
	SWITCH_CU_COLOR,
	SWITCH_MU_COLOR,
};
extern enum LED_COLOR state_color;

extern Timer_ID led_task;
extern u32 led_flash;
// extern u32 led_battery_flash;        //ma 20150825  dummy useless value 
extern u32 bt_hang_up_signal;

typedef struct {
	U8 R;
	U8 G;
	U8 B;
}LED_COLOR_TYPE;

extern void ledTaskInit(void);
extern void ledClkInit(void);
extern void ledInit(void);
extern void ledColorSet(enum LED_COLOR color);
extern void ledTask(void);
extern void leddisable(void);

/*-------------------------------------------------------------------------
 * @fucname:	 ledClkInit
 * @brief: 	 led clk init
 * @param: 	 none
 * @return: 	 none
 * @details: 	led clk init
 *-----------------------------------------------------------------------*/
static __inline void ledClkInit()
{
	SET_PWM01_CLKSRC(CLK_PWM01_HXT_L, CLK_PWM01_HXT_H);
	SET_PWM23_CLKSRC(CLK_PWM01_HXT_L, CLK_PWM01_HXT_H);
	APBCLK_ENABLE(CLK_PWM01 | CLK_PWM23);
}


/*-------------------------------------------------------------------------
 * @fucname:	 setRedLedBright
 * @brief: 	 set Red Led brightness
 * @param: 	 none
 * @return: 	 none
 * @details: 	 set Red Led brightness
 *-----------------------------------------------------------------------*/
static __inline void setRedLedBright(u16 x)
{
	PWMA_SET_CMR0(x);
}


/*-------------------------------------------------------------------------
 * @fucname:	 setGreenLedBright
 * @brief: 	 set GREEN Led brightness
 * @param: 	 none
 * @return: 	 none
 * @details: 	 set GREEN Led brightness
 *-----------------------------------------------------------------------*/
static __inline void setGreenLedBright(u16 x)
{
	PWMA_SET_CMR1(x);
}


/*-------------------------------------------------------------------------
 * @fucname:	 setBlueLedBright
 * @brief: 	 set blue Led brightness
 * @param: 	 none
 * @return: 	 none
 * @details: 	 set blue Led brightness
 *-----------------------------------------------------------------------*/
static __inline void setBlueLedBright(u16 x)
{
	PWMA_SET_CMR2(x);
}

extern u32 ccp;     // ma 20150525  
extern u8 key_time_count;       //ma 20150617

extern u32 change_channal_counter;      //ma 20150708  add change channale counter  to handle change channal  especially in soundbar mode
extern u8 NFC_decting_flag;         //ma 20150709  

// extern u8  source_change_delay;        //ma 20150810    add sb source change delay   
extern u8 sb_change_source_mute_flag;   //ma 20150819  add soft mute i2s flag when sb change source 
extern u8 tx_linked_flag;   //ma 20150906  add tx  linked  flag 
#endif



	




