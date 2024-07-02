/****************************************************************************
 * @file:		led.h
 * @version:	V1.00
 * @breif:	Led header file
 * @date: 	2012/2/22
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __led_h
#define __led_h

#include "sysinc.h"

void fadeon( u32 color);


extern u32 cclsllt;
extern enum LED_COLOR ioldcolor;
extern enum LED_COLOR inowcolor;


#define LED_PORT	PA
#define RLED_PIN		12
#define LED_PORT_PIN	PA12
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

#define BT_LED_DET_PORT			PC
#define BT_LED_DET_PIN			11
#define BT_LED_DET_PORT_PIN	PC11

#define PAIRING_SECOND	(33*180)

void breathingColor(void);

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
	BLUETOOTH_COLOR = 9
};
extern enum LED_COLOR state_color;

extern Timer_ID led_task;
extern u8 led_flash;
extern u8 led_battery_flash;
extern u8 bt_hang_up_signal;

typedef struct {
	U8 R;
	U8 G;
	U8 B;
}LED_COLOR_TYPE;

extern void ledTaskInit(void);
//--extern void ledClkInit(void);
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
/*--static __inline void ledClkInit()
{
	
}
*/


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


#endif








