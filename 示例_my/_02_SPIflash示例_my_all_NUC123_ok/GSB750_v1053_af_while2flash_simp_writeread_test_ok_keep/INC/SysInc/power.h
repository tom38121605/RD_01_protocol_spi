/****************************************************************************
 * @file:		power.h
 * @version:	V1.00
 * @breif:		power control source file, POWER AND MUTE CONTROL
 * @date: 	2013/3/14
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __power_h__
#define __power_h__
#include "sysinc.h"


#define RED_LED_PORT          PA
#define RED_LED_PIN           12 
#define RED_LED_PORT_PIN      PA12

#define GREEN_LED_PORT        PA
#define GREEN_LED_PIN         13 
#define GREEN_LED_PORT_PIN    PA13

#define BLUE_LED_PORT         PA
#define BLUE_LED_PIN          14 
#define BLUE_LED_PORT_PIN     PA14
 

#define POWER_CTRL_PORT	PD
#define POWER_CTRL_PIN		5
#define POWER_CTRL_PORT_PIN	PD5

#define BT_POWER_EN_PORT		PB
#define BT_POWER_EN_PIN		14
#define BT_POWER_EN_PORT_PIN	PB14
// #define BT_HANG_UP_PORT		PD
// #define BT_HANG_UP_PIN			3
// #define BT_HANG_UP_PORT_PIN	PD3

#define POWER_KEY_PORT			PB
#define POWER_KEY_PIN			8
#define POWER_KEY_PORT_PIN		PB8

#define MAIN_POWER_CTRL_PORT	PC
#define MAIN_POWER_CTRL_PIN	8
#define MAIN_POWER_CTRL_PORT_PIN	PC8

// #define STANDBY_POWER_CTRL_PORT	PD
// #define STANDBY_POWER_CTRL_PIN	5
// #define STANDBY_POWER_CTRL_PORT_PIN	PD5
#define BATT_DECTECT_PORT		PD		// USE AS AD PORT
#define BATT_DECTECT_PIN		4
#define BATT_DECTECT_PORT_PIN		PD4
// #define NFC_DECTECT_PORT		PA
// #define NFC_DECTECT_PIN			11
// #define NFC_DECTECT_PORT_PIN		PA11
#define DC_DECTECT_PORT		PC
#define DC_DECTECT_PIN			10
#define DC_DECTECT_PORT_PIN		PC10
// #define CALL_DECTECT_PORT		PC
// #define CALL_DECTECT_PIN		8
// #define CALL_DECTECT_PORT_PIN		PC8
#define AD_RESET_PORT	PB
#define AD_REST_PIN		9
#define AD_RESET_PORT_PIN	PB9
// #define AMP_MUTE_PORT		PB
// #define AMP_MUTE_PIN		14
// #define AMP_MUTE_PORT_PIN	PB14


#define WIRELESS_BT_SEL_PORT	PC
#define WIRELESS_BT_SEL_PIN	13
#define WIRELESS_BT_SEL_PORT_PIN	PC13

#define AMP_MUTE_PORT		PD
#define AMP_MUTE_PIN		3
#define AMP_MUTE_PORT_PIN	PD3

#define WIRELESS_RST_PORT 	PA
#define WIRELESS_RST_PIN 	15
#define WIRELESS_RST_PORT_PIN 	PA15

// #define NFC_DECTECT_PORT		PB
// #define NFC_DECTECT_PIN			8
// #define NFC_DECTECT_PORT_PIN PB8


// #define TP_ALERT_PORT		PD
// #define TP_ALERT_PIN			1
// #define TP_ALERT_PORT_PIN PD1

#define BT_AUX_CTRL_PORT      PC
#define BT_AUX_CTRL_PIN       11
#define BT_AUX_CTRL_PORT_PIN  PC11

#define CHARGE_DONE_PORT      PC
#define CHARGE_DONE_PIN       9
#define CHARGE_DONE_PORT_PIN  PC9

// #define POWER_OFF_DET_PORT      PC
// #define POWER_OFF_DET_PIN       8
// #define POWER_OFF_DET_PORT_PIN  PC8

#define WL_IRQ_PORT         PD
#define WL_IRQ_PIN          1
#define WL_IRQ_PORT_PIN     PD1

#define WL_I2C_SCL_PORT      PA
#define WL_I2C_SCL_PIN       11
#define WL_I2C_SCL_PORT_PIN  PA11

#define WL_I2C_SDA_PORT      PA
#define WL_I2C_SDA_PIN       10
#define WL_I2C_SDA_PORT_PIN  PA10

#define UPGRADE_SEL_PORT       PD
#define UPGRADE_SEL_PIN        2
#define UPGRADE_SEL_PORT_PIN   PD2

#define AUX_DETECT_PORT       PB
#define AUX_DETECT_PIN        10
#define AUX_DETECT_PORT_PIN   PB10

enum WDTHANDLER{
		WDT_RST,
		WDT_CLR
};
extern enum WDTHANDLER wdt_handler;

enum BATTERY_STATE{
		BATTERY_OFF = 0,  // no battery only DC
		BATTERY_MIDDLE = 1,
		BATTERY_LOW = 2,	
		BATTERY_HIGH = 3
};
extern enum BATTERY_STATE battery_state;


typedef struct{
	u32 xtal12m_en	: 1;
	u32 reserved1	: 1;	 
	u32 osc22m_en	: 1;
	u32 osc10k_en	: 1;
	u32 pd_wu_dly	: 1; // enable the wakeup delay counte when wakeup form pwoer-down mode to wait for osc stable 
	u32 pd_wu_int_en	: 1; // enable interrupt when wakeup from power-down mode
	u32 pd_wu_sts	:1; // power-down mode wakeup interrupt status,set by GPIO, USB, UART, WDT,CAN, ACMP, BOD
	u32 pwr_down_en	:1; // enable sys into power_down mode
	u32 pd_wait_cpu	:1;  
}PWRTYPE;

extern u32 BT_FIRST_START;
extern u32 nfc_dec;
extern U32 BT_FIRST_START;
extern void outStandby(void);
extern void enterStandby(void);
extern void powerOff(void);
extern void powerOn(void);
extern void WDTInit(void);
extern void dectPowerInfo(void);
extern void dealPowerKey(void);
extern void doPowerOff(void);
/*-------------------------------------------------------------------------
 * @fucname:	 powerCtrlCLR
 * @brief: 	 turn off peripheral chip power
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 turn off peripheral chip power
 *-----------------------------------------------------------------------*/
static __inline void standbyPowerCtrlCLR()
{
	GPIO_CLR_PIN(POWER_CTRL_PORT_PIN);
}

/*-------------------------------------------------------------------------
 * @fucname:	 powerCtrlSet
 * @brief: 	 turn on peripheral chip power
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 turn on peripheral chip power
 *-----------------------------------------------------------------------*/
static __inline void standbyPowerCtrlSet()
{
	GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
}

/*-------------------------------------------------------------------------
 * @fucname:	 powerInit
  * @brief: 	 set mcu control pin is output mode
 * @param: 	 none
 * @return: 	 none 
 * @details: 	set mcu control pin is output mode
 *-----------------------------------------------------------------------*/
static __inline void openMcuCtrlPin()
{
	GPIO_SET_MODE(MAIN_POWER_CTRL_PORT, MAIN_POWER_CTRL_PIN, GPIO_OUTPUT);
	GPIO_CLR_PIN(POWER_CTRL_PORT_PIN);
	GPIO_SET_MODE(POWER_CTRL_PORT, POWER_CTRL_PIN, GPIO_OUTPUT); // enable the power control pin output function
	GPIO_CLR_PIN(AMP_MUTE_PORT_PIN);
	GPIO_SET_MODE(AMP_MUTE_PORT, AMP_MUTE_PIN, GPIO_OUTPUT); // enable the AMP_MUTE pin output function
	GPIO_CLR_PIN(BT_POWER_EN_PORT_PIN);
	GPIO_SET_MODE(BT_POWER_EN_PORT, BT_POWER_EN_PIN, GPIO_OUTPUT); // BT_POWER_CTRL open
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_OPEN_DRAIN);
// 	GPIO_SET_MODE(BT_HANG_UP_PORT, BT_HANG_UP_PIN, GPIO_OUTPUT);
	GPIO_CLR_PIN(AD_RESET_PORT_PIN);
	GPIO_SET_MODE(AD_RESET_PORT, AD_REST_PIN, GPIO_OUTPUT);
	
	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
// 	GPIO_SET_MODE(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INPUT);
	GPIO_SET_MODE(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(POWER_OFF_DET_PORT, POWER_OFF_DET_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(CALL_DECTECT_PORT, CALL_DECTECT_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(BT_LED_DET_PORT, BT_LED_DET_PIN, GPIO_INPUT);
	GPIO_CLR_PIN(WIRELESS_RST_PORT_PIN);
	GPIO_SET_MODE(WIRELESS_RST_PORT, WIRELESS_RST_PIN, GPIO_OUTPUT);
	GPIO_CLR_PIN(WIRELESS_BT_SEL_PORT_PIN);
	GPIO_SET_MODE(WIRELESS_BT_SEL_PORT, WIRELESS_BT_SEL_PIN, GPIO_OUTPUT);
	
	GPIO_CLR_PIN(UPGRADE_SEL_PORT_PIN);
	GPIO_SET_MODE(UPGRADE_SEL_PORT, UPGRADE_SEL_PIN, GPIO_OUTPUT);
	
	GPIO_CLR_PIN(CHARGE_DONE_PORT_PIN);
	GPIO_SET_MODE(CHARGE_DONE_PORT, CHARGE_DONE_PIN, GPIO_INPUT);
	
	
	GPIO_CLR_PIN(AUX_DETECT_PORT_PIN);
	GPIO_SET_MODE(AUX_DETECT_PORT, AUX_DETECT_PIN, GPIO_INPUT);
	
	GPIO_CLR_PIN(WIRELESS_RST_PORT_PIN);
	GPIO_SET_MODE(WIRELESS_RST_PORT, WIRELESS_RST_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(PB, 10, GPIO_INPUT);
// 	GPIO_SET_MODE(PC, 13, GPIO_OUTPUT);
	GPIO_SET_MODE(BT_AUX_CTRL_PORT, BT_AUX_CTRL_PIN, GPIO_OUTPUT);
	outpw(&SYS->GPB_MFP, inpw(&SYS->GPB_MFP) & ~(0x1<<14));
}

static __inline void closeMcuCtrlPin()
{
	GPIO_SET_MODE(POWER_CTRL_PORT, POWER_CTRL_PIN, GPIO_INPUT); // enable the power control pin output function
	GPIO_SET_MODE(AMP_MUTE_PORT, AMP_MUTE_PIN, GPIO_INPUT); 
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_INPUT);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_INPUT);
	GPIO_SET_MODE(AD_RESET_PORT, AD_REST_PIN, GPIO_INPUT);
	//outpw(&SYS->GPF_MFP, inpw(&SYS->GPF_MFP) & ~(0x3<<2));// I2C CHANGED TO GPIO 
  	//outpw(&SYS->ALT_MFP1, (inpw(&SYS->ALT_MFP1) & ~(0xF<<24)));

		
	//GPIO_SET_MODE(BT_HANG_UP_PORT, BT_HANG_UP_PIN, GPIO_INPUT); 
	//GPIO_SET_MODE(STANDBY_POWER_CTRL_PORT,STANDBY_POWER_CTRL_PIN,GPIO_INPUT);
	//GPIO_SET_MODE(MAIN_POWER_CTRL_PORT, MAIN_POWER_CTRL_PIN, GPIO_INPUT); 
}


static __inline void BT_POWER_ON(void)
{
	BT_FIRST_START = 1;
	GPIO_SET_PIN(BT_POWER_EN_PORT_PIN);
}

static __inline void BT_POWER_OFF(void)
{
	BT_FIRST_START = 0;
// 	GPIO_CLR_PIN(BT_POWER_EN_PORT_PIN);

}



static __inline u32 DECT_CALL(void)
{
// 	return !GPIO_GET_PIN(CALL_DECTECT_PORT_PIN);
	return 0;
}

static __inline u32 DECT_DC(void)
{ 
	return !GPIO_GET_PIN(DC_DECTECT_PORT_PIN);
// 	return 0;
}

static __inline u32 DECT_NFC(void)
{
// 	return GPIO_GET_PIN(NFC_DECTECT_PORT_PIN);
	return 0;
}

static __inline u32 AUX_DETECT(void)
{
	return !GPIO_GET_PIN(AUX_DETECT_PORT_PIN);
// 	return 0;
}
// static __inline u32 POWRER_OFF_DET(void) 
// {
// // 	return GPIO_GET_PIN(POWER_OFF_DET_PORT_PIN);

// }

static __inline void MAIN_POWER_ON(void)
{
	GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
}

static __inline void MAIN_POWER_OFF(void)
{
	GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
}

static __inline void ampReset()
{
	GPIO_CLR_PIN(AD_RESET_PORT_PIN);
}

static __inline void ampResetDone()
{
	GPIO_SET_PIN(AD_RESET_PORT_PIN);
}

static __inline void dwhp83Rst()
{
	GPIO_CLR_PIN(WIRELESS_RST_PORT_PIN);
}

static __inline void dwhp83RstDone()
{
	GPIO_SET_PIN(WIRELESS_RST_PORT_PIN);
}


static __inline void UPGRADE_SET()
{
	GPIO_SET_PIN(UPGRADE_SEL_PORT_PIN);
}
static __inline void UPGRADE_CLR()
{
	GPIO_CLR_PIN(UPGRADE_SEL_PORT_PIN);
}
/*-------------------------------------------------------------------------
 * @fucname:	 AMP_MUTE
 * @brief: 	 mute the AD92580
 * @param: 	 NONE
 * @return: 	 NONE
 * @details: 	 mute the AD92580
 *-----------------------------------------------------------------------*/
static __inline void AMP_MUTE(void)
{
	GPIO_CLR_PIN(AMP_MUTE_PORT_PIN);
}

/*-------------------------------------------------------------------------
 * @fucname:	 AMP_OPEN
 * @brief: 	 unmute the AD92580
 * @param: 	 NONE
 * @return: 	 NONE
 * @details: 	 unmute the AD92580
 *-----------------------------------------------------------------------*/
static __inline void AMP_OPEN(void)
{
	GPIO_SET_PIN(AMP_MUTE_PORT_PIN);
}


static __inline u32 TP_IRQ()
{
// 	if(DECT_NFC())
		return 0;
// 	else
// 	 return !GPIO_GET_PIN(TP_ALERT_PORT_PIN);
}

extern u8 reset_en;
extern u8 enter_standby_mode;
extern u8 enter_Standby_stop;
#endif




