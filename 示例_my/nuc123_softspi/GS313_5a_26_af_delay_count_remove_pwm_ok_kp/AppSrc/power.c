/****************************************************************************
 * @file:		power.c
 * @version:	V1.00
 * @breif:		power control source file
 * @date: 	2013/3/13
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "NUC123Series.h"
#define SOC_TOUCH		1
#define SOC_POWKEY		2
#define SOC_BT			3

u8 flg_poweronsound=0;
u32 ipoweroncount=0;

u8 flg_poweron2=0;

u8 NFC_flag = 0;     // ma 20150528  add NFCflag

u8 dc_flag = 0;
enum WDTHANDLER wdt_handler = WDT_RST;
u32 wdt_timer = 0;
u8 gpio_int = 0;
PWRTYPE *pwrcon = (PWRTYPE*)CLK_BASE;
enum BATTERY_STATE battery_state = BATTERY_OFF;
//--extern void adcReadKey(void);
extern u32 BAT_STATE_DECT_DELAY_COUNT;
extern u32 dc_time_stamp;
extern u32 dc_eq;
u32 pww = 130;
U32 pwm_tbl[11] = {105,105,105,110,115,120,130,135,145,199,180};

u32 pww2;
u8 flg_playpoweron;

extern u8 flg_whitebreath;

extern void CLK_Init(void);

u8 flg_poweron;
u8 flg_motorup;


/*-------------------------------------------------------------------------
 * @fucname:	 sys_resume
 * @brief: 	 sys infomation resume 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys infomation resume 
 *-----------------------------------------------------------------------*/
void sysResume()
{
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 sysStore
  * @brief: 	 sys infomation store
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys infomation store
 *-----------------------------------------------------------------------*/
void sysStore()
{
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 resetHook
 * @brief: 	 clear reset flag
 * @param: 
 * @return: 
 * @details: 	 when the watch dog timer reset the cpu, the WDT IF didn't clear. So clear it here
 			 this funcation is called by resetHandler in startup.s
 *-----------------------------------------------------------------------*/
void resetHook()
{
	
	/*--if(WDT_GET_RESET_FLAG()) 
		WDT_CLEAR_RESET_FLAG();
	BT_POWER_OFF();
	GPIO_SET_MODE(BT_POWER_EN_PORT, BT_POWER_EN_PIN, GPIO_OUTPUT);
	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
	GPIO_SET_MODE(NFC_DECTECT_PORT,NFC_DECTECT_PIN,GPIO_INPUT);



	GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
	GPIO_SET_MODE(MAIN_POWER_CTRL_PORT,MAIN_POWER_CTRL_PIN,GPIO_OUTPUT);
  */
}


/*-------------------------------------------------------------------------
 * @fucname:	 dealPowerKey
 * @brief: 	 power key on if dc open immediately, 
 			 else if battery wait for 2 sec to open the main power.
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern u8 readCAP8298(u32);
volatile static u32 source_p = 0xff;
void dealPowerKey(void)
{
/*--	u32 i = 0;	

	GPIO_SET_MODE(PA, 11, GPIO_INPUT);
	GPIO_SET_MODE(PA, 15, GPIO_INPUT);
	GPIO_SET_PIN(PA12);
	GPIO_SET_PIN(PA13);
	GPIO_SET_PIN(PA14);
	GPIO_SET_MODE(PA, 12, GPIO_OUTPUT);
	GPIO_SET_MODE(PA, 13, GPIO_OUTPUT);
	GPIO_SET_MODE(PA, 14, GPIO_OUTPUT);
	sysDelayMs(200);
	
	if(DECT_NFC() || DECT_DC() || !GPIO_GET_PIN(PB10))
	{

		GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
		source_p = 0xaa;
		if(DECT_DC())
			dc_time_stamp = 1;
	}
	else 
	{
		if(GPIO_GET_PIN(POWER_KEY_PORT_PIN))
		{
			dc_time_stamp = 0;
			source_p = 0x55;
			if(IS_SYSTEM_RST())
				i = 0;
		
			for(; i < BATTERY_OPEN_DELAY; i++) 
				if(GPIO_GET_PIN(POWER_KEY_PORT_PIN))
				{
					sysDelayMs(15);
					if(DECT_DC())
					{

						GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
						break;
					}
				}
				else
				{
					//sysDelayMs(100);
					GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
					i = 0;
					while(GPIO_GET_PIN(POWER_KEY_PORT_PIN) == 0)
					;
				}
		
		}
		else
		{
			GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
			i = 0;
			while(GPIO_GET_PIN(POWER_KEY_PORT_PIN) == 0)
			;
		}
		
	}
	*/

}

/*-------------------------------------------------------------------------
 * @fucname:	 doPowerOff
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void doPowerOff(void)
{
/*--	u32 i = 0;
	U32 j = 0;
	u32 power_key_release = FALSE;
	
	DrvWDT_DisableInt(); 
	

	GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
	
	while(1)
	{
		iputs("power off\r\n");
		sysDelayMs(20);

		if(power_key_release == FALSE 
		&&	GPIO_GET_PIN(POWER_KEY_PORT_PIN))
		{
			power_key_release = FALSE;
			iputs("power_key_on\r\n");
		}
		else
		{
			if(++i == 3)
			{
				power_key_release = TRUE;
				iputs("power_key_release\r\n");
			}	
		}

		if(DECT_DC() || GPIO_GET_PIN(NFC_DECTECT_PORT_PIN))
		{

			GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
			sysDelayMs(50);
			iputs("dc_nfc_detect\r\n");
			break;
		} 
			
		if(	power_key_release == TRUE &&   // if user don.t relase the power key , don't restart
			GPIO_GET_PIN(POWER_KEY_PORT_PIN))
		{
				
				if(++j == 10)
				{
					power_key_release = FALSE; 
					iputs("mcu_reset\r\n");
					ledColorSet(DARK_COLOR);

					GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
					GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
					sysDelayMs(600);
					SYS_UnlockReg();
					NVIC_SystemReset();
				}
		}
		else
		{
			j = 0;
		}
		
	}	
	*/
	
}
 


/*-------------------------------------------------------------------------
 * @fucname:	 poweroff
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern u32 volume_back;
u32 nfc_start;

extern u32 breath_flag;
//--void WDTwake();
extern  capPinEnable(u8 m);
void powerOff()
{
  ;/*--	//--U32 temp, i;
	//--static u32 bt_state = 0;

	AMP_MUTE();
	dev_state = DEV_STANDBY;
	flash_mode = FLASH_CONFIG;
	//amp_volume = volume_back&~0x80;
	SPI_CLR_SS0(SPI2); 
	flash_mode = FLASH_CONFIG; 
	stopFlashLed(); 
	sysDelayMs(200); 
	//capPinEnable(0x04);  
	//resetCap();
	sysDelayMs(5);  
	sysDelayMs(200); 
	//--level_open = 0; 
	BT_POWER_OFF(); 
	ampPowerDown(); 
	sysDelayMs(200); 
	iputs("standby1\r\n");
	NVIC_DisableIRQ(I2S_IRQn);	
	APBCLK_DISABLE(CLK_I2S);
	
	outpw(&SYS->GPC_MFP, inpw(&SYS->GPC_MFP) & ~(0xF<<0)); // gpio i2s disable
	outpw(&SYS->ALT_MFP, inpw(&SYS->ALT_MFP) & ~(0xF<<5));
	outpw(&SYS->GPA_MFP, inpw(&SYS->GPA_MFP) & ~(0x1<<15)); // MCLK TO GPIO
	outpw(&SYS->ALT_MFP, inpw(&SYS->ALT_MFP) & ~(0x1<<9));
	GPIO_SET_MODE(PC,12,GPIO_INPUT); // SET I2S MCLK GPIO_INPUT
	GPIO_SET_MODE(PC,0,GPIO_INPUT);  // SET I2S LR GPIO_INPUT
	GPIO_SET_MODE(PC,1,GPIO_INPUT);  // SET I2S BCLK GPIO_INPUT
	GPIO_SET_MODE(PC,2,GPIO_INPUT);  // SET I2S DI GPIO_INPUT
	GPIO_SET_MODE(PC,3,GPIO_INPUT);  // SET I2S DO GPIO_INPUT
	
	//touchKeyRemap();
	sysDelayMs(100); 
	logEvent(System_standby);
	logClose(); 
	//while(GPIO_GET_PIN(PB8))
	//	sysDelayMs(20);
	if(!DECT_DC()){
		logClose();
		doPowerOff();
	}

	MAIN_POWER_OFF();
	
	sysDelayMs(200);
	ledTask();
	iputs("standby2\r\n");
	*/
}


/*--u32 volatile motor_cc = 0;
u32 motor_direction = 3;
void motorRise(void)
{
	motor_cc = 78;//78;
	motor_direction = 1;
	
}

void motorDown()
{
	motor_cc = 72;
	motor_direction = 2;
	while(motor_cc) sysDelayMs(2);
	motor_direction = 3;
	MOTOR_STANDBY();
}
*/

void timerInit() 
{
	SET_TMR0_CLKSRC(CLK_TMR0_HXT);  
	DrvTIMER_Init(TIMER0, 21, 10, TIM_MODE_TOGGLE);
	DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);
	TIM_ENABLE(TIMER0);
}


void set_p(void)
{
	source_p = 0x55;
}

void ampInit1(void)
{
//	I2SClkInit(48000);	// init i2s
//	i2cSysInit(); 	
//	sysDelayMs(50);

//  GPIO_SET_MODE(PB, 9, GPIO_OUTPUT); 	
//	ampReset();	
//	sysDelayMs(200);
//	ampResetDone();		
//	sysDelayMs(50);
//	
//// 	AMP_OPEN();
//// 	sysDelayMs(10);
//	
//	ampInitAllConf(); 
//	ampSendEqs_38(0,63); 
//	sysDelayMs(20);
}


/*--void ampInit1(void)
{
	I2SClkInit(48000);	// init i2s
	i2cSysInit(); 
	sysDelayMs(50);
	ampReset();	// reset the ad83586
	sysDelayMs(200);
	ampResetDone();	// reset the ad83586 done 
	sysDelayMs(50);
	AMP_OPEN();
	sysDelayMs(10);
	ampInitAllConf(); 
	ampSendEqs_38(0,63); 
	sysDelayMs(20);
}
*/

/*-------------------------------------------------------------------------
 * @fucname:	 powerOn
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u32 bt_ck = 0;
extern enum BATTERY_STATE battery_state;
extern u32 bt_volage_adc;
u32 bt_cycle = 6;
//--u32 loevl;
//--extern volatile u32 bt_avmx;
extern u8 battery_f;

extern u8 touchClr(void); 
//--extern void breathingColor(); 
extern u32 bat_vtbl_0[]; 
extern U32 battery_low_flag2; 
extern u32 f_test; 
volatile u32 pvo = 0; 


void powerOn()  
{
  ;
}



/*-------------------------------------------------------------------------
 * @fucname:	 outStandby
 * @brief: 	 sys out of standy mode
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys out of standy mode
 *-----------------------------------------------------------------------*/
extern u32 BT_MATCH_FLAG;
extern u32 ccount;
extern u32 test_mode;
//--extern enum KEYWORD key_word;

extern u32 battery_low_flag;
void outStandby()
{
	
 	if(dev_state != DEV_TEST)             
		 dev_state = DEV_START;
	
	
	//...
	;
}

/*-------------------------------------------------------------------------
 * @fucname:	 enterStandby
 * @brief: 	 sys enter standy mode
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys enter standy mode
 *-----------------------------------------------------------------------*/

void enterStandby()
{
	/*--u32 i = 0;//--, j = 0;
	sysStore();
	logEvent(Power_down_sequence_started);
	dev_state = DEV_STANDBY;
	stopATask(led_task);
	pww = pwm_tbl[10-battery_level/10];
	motor_direction = 2;
	battery_f = battery_level; 
	setConfig();
	sysDelayMs(200);
	if(time_out == 0)
	if(battery_state !=  BATTERY_OVER)
	{
		playMusic(POWER_OFF_SOUND);		
//iputs("POWER_OFF_SOUND\r\n");	  //--zz debug		
		
	}
	
	while(GPIO_GET_PIN(PD1))
	{
		sysDelayMs(100);
		motor_direction = 2;
		if(++i > 60)
			break;
	}
//--end_1:
	
	sysDelayMs(500);
	
	motor_direction = 3;
	MOTOR_STANDBY(); 
	sysDelayMs(300);
	
//iputs("voladd--01.\r\n");    //--zz debug	
	for(i = amp_volume; i > 0; i-- )
	{
		ampSetVolume(i);
		sysDelayMs(5);
	}
	ampMute();
	//iputs("dd_mute04\r\n");   //--zz debug
	sysDelayMs(20);	

	playSFMusicEnd();
	
	sysDelayMs(20);
		
//iputs("ampSetVol--A06\r\n");    //--zz debug		
	ampSetVolume(amp_volume);
	bt_cycle = 6;
	iputs("moter stop\r\n");
	powerOff();
	*/
	
} 


/*void WDT_IRQHandler()
{
	
iputs("WDT--IN.\r\n");  //--zz debug         
	
	//WDTwake();
	SYS_UnlockReg();
	WDT_CLEAR_TIMEOUT_INT_FLAG();
	WDT_TIMER_RESET();       
	WDT_SET_INTERVAL(6);   
	SYS_LockReg();	
	++wdt_timer;
	
}

void WDTInit()
{
	WDT_TIMER_RESET();          
	DrvWDT_Init(6,CLK_WDT_LIRC,WDT_RESET_DELAY_128); 
	
	SYS_UnlockReg(); 
	(WDT->WTCR |= WDT_WTCR_WTR);//WDT_TIMER_RESET(); 
	WDT_ENABLE_COUNTING();     
	WDT_CLEAR_RESET_FLAG();  
	WDT_DISABLE_WAKEUP(); 
	SYS_LockReg(); 
	
	//DrvWDT_EnableInt();
	WDT_ENABLE_RESET();
}
*/

//watchdog irq
void WDT_IRQHandler()
{	

iputs("WDT--01.\r\n");  //--zz debug   
	
  if( WDT->WTCR&(1<<3)  ) 
  {
    SYS_UnlockReg();   
    WDT->WTCR |= (1<<3); 
    SYS_LockReg();
		
    //flg_WDTTimeoutINT = 1;
  }

}

//watchdog ini 
void WDTInit(void)
{
	//WDT_TIMER_RESET();          
	DrvWDT_Init(5,CLK_WDT_LIRC,WDT_RESET_DELAY_128);  //1.6384s
	
	SYS_UnlockReg(); 	
	
	WDT_TIMER_RESET(); 
	WDT_ENABLE_COUNTING();     
	WDT_CLEAR_RESET_FLAG();   
	WDT_DISABLE_WAKEUP(); 
	
	WDT_ENABLE_RESET();	 //--need to unlock before do it
	
	SYS_LockReg(); 
	
	DrvWDT_EnableInt(); 

}


void WDTwake(void)
{
	DrvWDT_Init(6,CLK_WDT_LIRC,WDT_RESET_DELAY_1024);
	DrvWDT_EnableInt();
	
	SYS_UnlockReg();
	WDT_TIMER_RESET();
	WDT_ENABLE_COUNTING();     
	WDT_CLEAR_RESET_FLAG();
	WDT_ENABLE_WAKEUP();
	WDT_DISABLE_RESET();
	SYS_LockReg();
}


/*-------------------------------------------------------------------------
 * @fucname:	 PWRWU_IRQHandler ISR
 * @brief: 	 wake from power down mode will come here
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void PWRWU_IRQHandler()
{
	SYS_UnlockReg();
	ledColorSet(GREEN_COLOR);
	sysDelayMs(2000);
	SYS_LockReg();
}

/*--void GPAB_IRQHandler()
{ 
	if(GPIO_GET_PIN_IF(PB,10)){
		gpio_int = SOC_TOUCH;
		GPIO_CLR_PIN_IF(PB, 10); 
		GPIO_DisableInt(PB, 10); 
	}
	

	if(GPIO_GET_PIN_IF(NFC_DECTECT_PORT,NFC_DECTECT_PIN))
	{
		gpio_int = SOC_TOUCH;
		GPIO_CLR_PIN_IF(NFC_DECTECT_PORT, NFC_DECTECT_PIN);   
		GPIO_DisableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN); 		
	}
	
}
*/

void GPAB_IRQHandler()
{ 

	if(GPIO_GET_PIN_IF(PB,10))
	{
		gpio_int = SOC_TOUCH;
		GPIO_CLR_PIN_IF(PB, 10); 
		GPIO_DisableInt(PB, 10); 
	
	  //iputs("PB.10 INT occurred.\n");    //--zz debug
	}
	
	if(GPIO_GET_PIN_IF(PA,15))
	{
		gpio_int = SOC_TOUCH;
		GPIO_CLR_PIN_IF(PA, 15); 
		GPIO_DisableInt(PA, 15); 
	
	  //iputs("PA15 INT occurred.\n");    //--zz debug
	}
	
	if(GPIO_GET_PIN_IF(PA,11))
	{
		gpio_int = SOC_TOUCH;

		NVIC->ISER[0]&= ~(1 << 4);   //Disable	
		
		//iputs("PA.11 INT occurred.\n");   //--zz debug
		
		flg_touchin=1;
		
		//gpio_int = SOC_TOUCH
		GPIO_CLR_PIN_IF(PA,11);   //Clear
		
		NVIC->ISER[0] |= (1 << 4);   //Enable
	}
	
}


/*void GPCDF_IRQHandler()
{
	if(GPIO_GET_PIN_IF(POWER_KEY_PORT,POWER_KEY_PIN))
	{
		gpio_int = SOC_POWKEY;
		GPIO_CLR_PIN_IF(POWER_KEY_PORT, POWER_KEY_PIN);   
		GPIO_DisableInt(POWER_KEY_PORT, POWER_KEY_PIN); 
	}
}
*/


U32 R_C =  0, B_C =  0, G_C =  0; 
U32 pwtable[21] = {0,2,3,4,6,9,12,15,18,24,30,39,48,62,78,95,116, 131,150, 170,200};
void TMR0_IRQHandler(void)
{
	static u32 ptimer = 0;
	//--u32 i;
	
	 TIM_CLEAR_INTERRUPT_FLAG(TIMER0);
	
	
//-----------------------------------------
	
	/*if(twslinklst() == 4 || btlinklst() == 4)
		++cclsllt; 
	else
		cclsllt = 0;
		*/
	
	//if(!audioDetect())
	//  audio_quiet_time++;
	//else
	//	audio_quiet_time = 0;
	
		if(++ptimer > 200)
			ptimer = 0;
	
	if (flg_whitebreath==1)
	{
		flg_whitebreath=0;
		if(ptimer < pwtable[R_C])
		{
	    //PA->PMD &= ~((1<<12)|(1<<13)|(1<<14));
	    PA->DOUT &= ~((1<<12)|(1<<13)|(1<<14));
		}
		else
		{
	    //PA->PMD |= (1<<12)|(1<<13)|(1<<14);
	    PA->DOUT |= (1<<12)|(1<<13)|(1<<14);
		}		
		
	}
	else
	{
		if(ptimer < pwtable[R_C])
			GPIO_CLR_PIN(PA12);	
		else
			GPIO_SET_PIN(PA12);

		if(ptimer < pwtable[B_C])
			GPIO_CLR_PIN(PA14);	
		else
			GPIO_SET_PIN(PA14);

		if(ptimer < pwtable[G_C])
			GPIO_CLR_PIN(PA13);	
		else
			GPIO_SET_PIN(PA13);		
		
	}
	
}

//void HardFault_Handler()
//{	
//	NVIC_SystemReset();
//}


void openMcuCtrlPin(void)    //--2015.07.02
{
		//--GPIO_SET_MODE(I2S_CTRL_PORT, I2S_CTRL_PIN, GPIO_OUTPUT);
	
/*--
	GPIO_SET_MODE(AMP_MUTE_PORT, AMP_MUTE_PIN, GPIO_OUTPUT); // enable the AMP_MUTE pin output function
	GPIO_CLR_PIN(BT_POWER_EN_PORT_PIN);
	GPIO_SET_MODE(BT_POWER_EN_PORT, BT_POWER_EN_PIN, GPIO_OUTPUT); // BT_POWER_CTRL open
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(BT_HANG_UP_PORT, BT_HANG_UP_PIN, GPIO_OUTPUT);
	GPIO_CLR_PIN(AD_RESET_PORT_PIN);
	GPIO_SET_MODE(AD_RESET_PORT, AD_REST_PIN, GPIO_OUTPUT);
	GPIO_SET_MODE(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INPUT);
	GPIO_SET_MODE(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INPUT);
	GPIO_SET_MODE(BT_LED_DET_PORT, BT_LED_DET_PIN, GPIO_INPUT);
	GPIO_SET_MODE(PD,1,GPIO_INPUT);
	GPIO_CLR_PIN(PC8);
	GPIO_SET_MODE(PC, 8, GPIO_OUTPUT);
	GPIO_CLR_PIN(PC9);
	GPIO_SET_MODE(PC, 9, GPIO_OUTPUT);
	GPIO_SET_MODE(PC, 13, GPIO_INPUT);
	GPIO_SET_MODE(PA, 10, GPIO_OUTPUT);
	GPIO_SET_MODE(PC,13,GPIO_INPUT);
	GPIO_CLR_PIN(PB7);
	GPIO_SET_MODE(PB, 7, GPIO_OUTPUT);
	GPIO_CLR_PIN(PB6);
	GPIO_SET_MODE(PB, 6, GPIO_OUTPUT);
	GPIO_SET_MODE(PB, 8, GPIO_INPUT);
	GPIO_SET_MODE(PB, 10, GPIO_INPUT);
	outpw(&SYS->GPB_MFP, inpw(&SYS->GPB_MFP) & ~(0x1<<14));
	*/
}

/*--void BT_POWER_ON(void)
{
	BT_FIRST_START = 1;
	GPIO_SET_PIN(BT_POWER_EN_PORT_PIN);
}

void BT_POWER_OFF(void)
{
	BT_FIRST_START = 0;
	GPIO_CLR_PIN(BT_POWER_EN_PORT_PIN);

	//GPIO_SET_PIN(PC13);
	//sysDelayMs(600);
	//GPIO_CLR_PIN(PC13);
}
*/

/*--void MOTOR_STANDBY(void)
{
	GPIO_CLR_PIN(MOTOR_C2_PORT_PIN);
	GPIO_CLR_PIN(MOTOR_C1_PORT_PIN);
}


void MOTOR_RVS(void)
{
	GPIO_CLR_PIN(MOTOR_C2_PORT_PIN);
	GPIO_SET_PIN(MOTOR_C1_PORT_PIN);
}

void MOTOR_FWD(void)
{
	GPIO_SET_PIN(MOTOR_C2_PORT_PIN);
	GPIO_CLR_PIN(MOTOR_C1_PORT_PIN);
}
*/

