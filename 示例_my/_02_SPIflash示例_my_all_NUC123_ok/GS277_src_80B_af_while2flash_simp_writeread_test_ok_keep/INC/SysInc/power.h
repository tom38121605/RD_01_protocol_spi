/****************************************************************************
 * @file:		power.h
 * @version:	V1.00
 * @breif:		power control source file, POWER AND MUTE CONTROL
 * @date: 	2013/3/14
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __power_h__
#define __power_h__
#include "seW58.h"

#define POWER_CTRL_PORT			PA
#define POWER_CTRL_PORT_PIN		PA14
#define POWER_CTRL_PIN			14
#define AMP_MUTE_PORT			PA
#define AMP_MUTE_PORT_PIN		PA13
#define AMP_MUTE_PIN				13
#define POWER_KEY_PORT			PB
#define POWER_KEY_PIN			8
#define POWER_KEY_PORT_PIN		PB8
#define BATT_DECTECT_PORT			PD		// USE AS AD PORT
#define BATT_DECTECT_PIN			4
#define BATT_DECTECT_PORT_PIN		PD4
#define NFC_DECTECT_PORT			PB
#define NFC_DECTECT_PIN			10
#define NFC_DECTECT_PORT_PIN		PB10
#define DC_DECTECT_PORT			PA
#define DC_DECTECT_PIN			15
#define DC_DECTECT_PORT_PIN		PA15
#define MAIN_RST_PORT				PB
#define MAIN_RST_PIN				9
#define MAIN_RST_PORT_PIN			PB9
#define CORE_POWER_PORT			PC
#define CORE_POWER_PIN			13
#define CORE_POWER_PORT_PIN		PC13
#define BT_AUX_CTRL_PORT			PB
#define BT_AUX_CTRL_PIN				5
#define BT_AUX_CTRL_PORT_PIN		PB5
#define BTAUX_UNIFI_CTRL_PORT		PB
#define BTAUX_UNIFI_CTRL_PIN		4
#define BTAUX_UNIFI_CTRL_PORT_PIN	PB4                                             
#define I2S_CTRL_PORT				PB
#define I2S_CTRL_PIN					14
#define I2S_CTRL_PORT_PIN			PB14

#define LED_CTRL_PORT				PD
#define RED_LED             1
#define BLUE_LED             2
#define GREEN_LED             3
#define RED_PIN						PD1
#define BLUE_PIN						PD2
#define GREEN_PIN					PD3

enum WDTHANDLER{
		WDT_RST,
		WDT_CLR
};
extern enum WDTHANDLER wdt_handler;

enum BATTERY_STATE{
		BATTERY_OFF = 0,  // no battery only DC
		BATTERY_MIDDLE = 1,
		BATTERY_LOW = 2,	
		BATTERY_HIGH = 3,     //MA 20150625
	  BATTERY_LOWER = 5     //MA 20150625   ADD 
};
extern enum BATTERY_STATE battery_state;


typedef struct{
	u32 xtal12m_en	: 1;
	u32 reserved1		: 1;	 
	u32 osc22m_en	: 1;
	u32 osc10k_en	: 1;
	u32 pd_wu_dly	: 1; // enable the wakeup delay counte when wakeup form pwoer-down mode to wait for osc stable 
	u32 pd_wu_int_en	: 1; // enable interrupt when wakeup from power-down mode
	u32 pd_wu_sts	:1; // power-down mode wakeup interrupt status,set by GPIO, USB, UART, WDT,CAN, ACMP, BOD
	u32 pwr_down_en	:1; // enable sys into power_down mode
	u32 pd_wait_cpu	:1;  
}PWRTYPE;


extern void outStandby(void);
extern void enterStandby(void);
extern void powerOff(void);
extern void powerOn(void);
extern void WDTInit(void);
extern void dectPowerInfo(void);
extern void dealPowerKey(void);
/*-------------------------------------------------------------------------
 * @fucname:	 powerCtrlCLR
 * @brief: 	 turn off peripheral chip power
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 turn off peripheral chip power
 *-----------------------------------------------------------------------*/
static __inline void standbyPowerCtrlCLR()
{
	GPIO_CLR_PIN(CORE_POWER_PORT_PIN);
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
	GPIO_SET_PIN(CORE_POWER_PORT_PIN);
}

static __inline void MAIN_POWER_ON()
{
	GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
}

static __inline void MAIN_POWER_OFF()
{
	GPIO_CLR_PIN(POWER_CTRL_PORT_PIN);
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
	GPIO_CLR_PIN(MAIN_RST_PORT_PIN);
	GPIO_SET_MODE(MAIN_RST_PORT, MAIN_RST_PIN, GPIO_OUTPUT);
	GPIO_SET_MODE(I2S_CTRL_PORT, I2S_CTRL_PIN, GPIO_OUTPUT);
	GPIO_SET_MODE(POWER_CTRL_PORT, POWER_CTRL_PIN, GPIO_OUTPUT); // enable the power control pin output function
	GPIO_SET_MODE(CORE_POWER_PORT, CORE_POWER_PIN, GPIO_OUTPUT); 
	GPIO_CLR_PIN(AMP_MUTE_PORT_PIN); 
	GPIO_SET_MODE(AMP_MUTE_PORT, AMP_MUTE_PIN, GPIO_OUTPUT); // enable the AMP_MUTE pin output function
	GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
	GPIO_SET_MODE(BT_AUX_CTRL_PORT, BT_AUX_CTRL_PIN, GPIO_OUTPUT); 
	GPIO_SET_MODE(BTAUX_UNIFI_CTRL_PORT, BTAUX_UNIFI_CTRL_PIN, GPIO_OUTPUT); 
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(PA,11,GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(PA,10,GPIO_OPEN_DRAIN);
	GPIO_I2C0();
	GPIO_SET_MODE(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INPUT);
	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
	GPIO_SET_PIN(PD1);
	GPIO_SET_PIN(PD2);
	GPIO_SET_PIN(PD3);
	GPIO_SET_MODE(PD, 1, GPIO_OUTPUT);
	GPIO_SET_MODE(PD, 2, GPIO_OUTPUT);
	GPIO_SET_MODE(PD, 3, GPIO_OUTPUT);
	GPIO_SET_MODE(PB, 10, GPIO_INPUT);
	GPIO_SET_MODE(PB, 7, GPIO_INPUT);
}

static __inline void closeMcuCtrlPin()
{
	GPIO_SET_MODE(I2S_CTRL_PORT, I2S_CTRL_PIN, GPIO_INPUT);
	GPIO_SET_MODE(BT_AUX_CTRL_PORT, BT_AUX_CTRL_PIN, GPIO_INPUT); 
	GPIO_SET_MODE(BTAUX_UNIFI_CTRL_PORT, BTAUX_UNIFI_CTRL_PIN, GPIO_INPUT);  
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_INPUT);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_INPUT);
	
	outpw(&SYS->GPF_MFP, inpw(&SYS->GPF_MFP) & ~(0x3<<2));// I2C CHANGED TO GPIO 
  outpw(&SYS->ALT_MFP1, (inpw(&SYS->ALT_MFP1) & ~(0xF<<24)));
	
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
	//GPIO_SET_PIN(AMP_MUTE_PORT_PIN);
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
	//GPIO_CLR_PIN(AMP_MUTE_PORT_PIN); 
}


static __inline void BT_OPEN()
{
	GPIO_SET_PIN(AMP_MUTE_PORT_PIN);
}


static __inline void BT_OFF()
{
	GPIO_CLR_PIN(AMP_MUTE_PORT_PIN); 
}


/*-------------------------------------------------------------------------
 * @fucname:	 dealCall
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static __inline u32 DECT_DC(void)
{
	return !GPIO_GET_PIN(DC_DECTECT_PORT_PIN);
}

static __inline u32 DECT_NFC(void)
{
	return GPIO_GET_PIN(NFC_DECTECT_PORT_PIN);
}

static __inline void MAIN_RST_DONE(void)
{
	GPIO_SET_PIN(MAIN_RST_PORT_PIN);
}

static __inline void MAIN_RST(void)
{
	GPIO_CLR_PIN(MAIN_RST_PORT_PIN);
}

static __inline void CHOOSE_BT_INPUT(void)
{
	GPIO_CLR_PIN(I2S_CTRL_PORT_PIN);
	GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
	GPIO_SET_PIN(BTAUX_UNIFI_CTRL_PORT_PIN);
}

static __inline void CHOOSE_AUX_INPUT(void)
{
	GPIO_CLR_PIN(I2S_CTRL_PORT_PIN);
	GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN); 
	GPIO_SET_PIN(BTAUX_UNIFI_CTRL_PORT_PIN);
}

static __inline void CHOOSE_UNIFI_INPUT(void)
{
	GPIO_CLR_PIN(BTAUX_UNIFI_CTRL_PORT_PIN);
}

static __inline void CHOOSE_BCALL_INPUT(void)
{
	GPIO_SET_PIN(I2S_CTRL_PORT_PIN);
	GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
	GPIO_SET_PIN(BTAUX_UNIFI_CTRL_PORT_PIN);
}

extern u8 NFC_flag;     // ma 20150528  add NFCflag

extern u8 power_first_on_flag;     // ma 20150530  add power first power to no red color
extern u8 enter_standby_flag;      //ma 20150603  add outstanby mode to enable WL

extern u8 factory_mode_flag;         //ma 20150616      set factory mode flag   

extern u8 volume_store;      //ma 20150617 

//extern u32 volatile gpio_int;     //ma 20150623  declare gpio_int for  ADCkey  change 
extern void sleep(void);               //ma 20150623
extern void doPowerOff(void);       //ma20150624


extern void WDTwake(void);         //ma 20150629


extern u8 sleep_flag;        //ma 20150706 
#endif




