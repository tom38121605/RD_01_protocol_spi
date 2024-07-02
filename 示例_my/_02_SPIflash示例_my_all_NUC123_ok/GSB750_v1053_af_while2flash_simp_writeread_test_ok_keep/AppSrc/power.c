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
u32 dc_flag = 0;
enum WDTHANDLER wdt_handler = WDT_RST;
u32 wdt_timer = 0;
u32 gpio_int = 0;
PWRTYPE *pwrcon = (PWRTYPE*)CLK_BASE;
enum BATTERY_STATE battery_state = BATTERY_OFF;
extern void adcReadKey(void);
extern u32 BAT_STATE_DECT_DELAY_COUNT;
extern u32 dc_time_stamp;
extern u32 audio_quiet_time;
extern u32 dc_eq;


u8 enter_standby_mode = 0;
u8 enter_Standby_stop = 0;
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
	
	if(WDT_GET_RESET_FLAG()) 
		WDT_CLEAR_RESET_FLAG();
	
// 	GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
// 	GPIO_SET_MODE(BT_HANG_UP_PORT, BT_HANG_UP_PIN, GPIO_OUTPUT);
// 	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
// 	GPIO_SET_MODE(NFC_DECTECT_PORT,NFC_DECTECT_PIN,GPIO_INPUT);
// 	GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
// 	GPIO_SET_MODE(MAIN_POWER_CTRL_PORT,MAIN_POWER_CTRL_PIN,GPIO_OUTPUT);

}

/*-------------------------------------------------------------------------
 * @fucname:	 dealPowerKey
 * @brief: 	 power key on if dc open immediately, 
 			 else if battery wait for 2 sec to open the main power.
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void dealPowerKey(void)
{
	u32 i = 0;

// 	if(DECT_NFC() || DECT_DC()){
		if(0){                        //ma 20160728
		GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
		dc_time_stamp = 1;
		sysDelayUs(100000);
	}
	else{
		dc_time_stamp = 0;
		
		if(IS_SYSTEM_RST())
			i = 80;

		for(; i < BATTERY_OPEN_DELAY; i++)
			if(GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
				sysDelayMs(5); 
// 				if(DECT_DC()){             //ma 20160728
// 					dc_time_stamp = 1;
// 					GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
// 					break;
// 				}
			}
			else{
				//sysDelayMs(100);
				GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
				i = 0;
				while(GPIO_GET_PIN(POWER_KEY_PORT_PIN) == 0)
					;
			}

	}


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
	u32 i = 0;
	U32 j = 0;
	u32 power_key_release = FALSE;
	
	DrvWDT_DisableInt(); 
// 	GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
	
	while(1){
			
		sysDelayMs(20);

		if(power_key_release == FALSE 
		&&	GPIO_GET_PIN(POWER_KEY_PORT_PIN))
			power_key_release = FALSE;
		else{
			if(++i == 3){
				power_key_release = TRUE;
			}	
		}

// 		if(DECT_DC()){
// 			GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
// 			sysDelayMs(50);
// 			break;
// 		}
			
		if(	power_key_release == TRUE 
// 			&&   // if user don.t relase the power key , don't restart
// 			GPIO_GET_PIN(POWER_KEY_PORT_PIN)
		)
		{
				
				if(++j == 10){
					power_key_release = FALSE; 
					ledColorSet(DARK_COLOR);
					GPIO_CLR_PIN(MAIN_POWER_CTRL_PORT_PIN);
// 					GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
// 					sysDelayMs(600);
					SYS_UnlockReg();
					NVIC_SystemReset();
				}
		}
		else{
			j = 0;
		}
	}	
}

/*-------------------------------------------------------------------------
 * @fucname:	 poweroff
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
 extern u32 volume_back;
 extern u32 nfc_start;
 void WDTwake(void);

void sleep()
{

// SLEEP_:
// 	//if(systick_counter < 1000)
// 	//	return;
// 	//NVIC_SetPriority (PWRWU_IRQn, 0);
// 	//leddisable();
// 	//DISABLE_GPIO_IR();
// 	while(GPIO_GET_PIN(POWER_KEY_PORT_PIN))
// 		sysDelayMs(20);
// 	leddisable();
// 	GPIO_CLR_PIN(PA12);
// 	SYS_UnlockReg();
// 	*(u32*)0xE000ED10 = 0X4; // WHEN
// 	pwrcon->pd_wu_dly = 1;
// 	pwrcon->pd_wait_cpu = 1; 
// 	pwrcon->pd_wu_int_en = 0;
// 	pwrcon->pwr_down_en = 1; 
// 	pwrcon->xtal12m_en = 1;
// 	//pwrcon->xtal12m_en = 0;
// 	SYS_LockReg();
// 	DrvADC_DisableInt();
// 	DISABLE_SYSTICK();
// 	WDT_DISABLE_RESET();

// 	DrvUART_DisableInt (UART0, UART_IER_BUF_ERR_IEN | UART_IER_RTO_IEN |UART_IER_MODEM_IEN
// 						| UART_IER_RLS_IEN | UART_IER_THRE_IEN |UART_IER_RDA_IEN);    
// 	GPIO_ENABLE_DEBOUNCE(POWER_KEY_PORT,POWER_KEY_PIN);
// 	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_OSC10K, GPIO_DBCLKSEL_4096); 
// 	NVIC_DisableIRQ(I2C0_IRQn);
// 	APBCLK_DISABLE(CLK_I2S);
// 	I2S_DISABLE_MCK();
// 	GPIO_CLR_PIN_IF(NFC_DECTECT_PORT,NFC_DECTECT_PIN);
// 	GPIO_CLR_PIN_IF(POWER_KEY_PORT,POWER_KEY_PIN);
// 	GPIO_CLR_PIN_IF(DC_DECTECT_PORT,DC_DECTECT_PIN);
// 	GPIO_EnableInt(POWER_KEY_PORT, POWER_KEY_PIN, GPIO_INT_HIGH); 
// 	GPIO_EnableInt(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INT_HIGH); 
// 	GPIO_EnableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INT_BOTH_EDGE); 
// 	NVIC_EnableIRQ(GPAB_IRQn);  
// 	NVIC_EnableIRQ(GPCDF_IRQn); 
// 	// here shoul set power_key as int source and disable IR int
// 	WDTwake();
// 	gpio_int = 0;
// 	wdt_timer = 0;
// 	//__asm("wfi");
// 	while(1){
// 		__asm("wfi");
// 		if(gpio_int == 1)
// 			break;
// 		if(++wdt_timer < 3){
// 			//if(dc_time_stamp == 1)
// 			//	GPIO_SET_PIN(PA12);
// 			//else
// 				GPIO_CLR_PIN(PA12);
// 			GPIO_SET_PIN(PA13);
// 			GPIO_SET_PIN(PA14);
// 		}else if(wdt_timer < 4){
// 			SYS_UnlockReg();
// 			WDT_SET_INTERVAL(4);   
// 			SYS_LockReg();	
// 			if(GPIO_GET_PIN(PC9)){
// 				GPIO_CLR_PIN(PA12);
// 				GPIO_CLR_PIN(PA13);
// 				GPIO_SET_PIN(PA14);
// 			}else{
// 				GPIO_SET_PIN(PA12);
// 				GPIO_CLR_PIN(PA13);
// 				GPIO_SET_PIN(PA14);
// 			}
// 				
// 		}else{
// 			SYS_UnlockReg();
// 			WDT_SET_INTERVAL(5);   
// 			SYS_LockReg();	
// 			//if(dc_time_stamp == 1)
// 			//	GPIO_SET_PIN(PA12);
// 			//else
// 				GPIO_CLR_PIN(PA12);
// 			GPIO_SET_PIN(PA13);
// 			GPIO_SET_PIN(PA14);
// 			wdt_timer = 0;
// 		}
// 		
// 		SYS_UnlockReg();
// 		*(u32*)0xE000ED10 = 0X4; // WHEN
// 		pwrcon->pd_wu_dly = 1;
// 		pwrcon->pd_wait_cpu = 1; 
// 		pwrcon->pd_wu_int_en = 0;
// 		pwrcon->pwr_down_en = 1; 
// 		pwrcon->xtal12m_en = 1;
// 		SYS_LockReg();
// 		GPIO_EnableInt(POWER_KEY_PORT, POWER_KEY_PIN, GPIO_INT_HIGH); 
// 		GPIO_EnableInt(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INT_HIGH); 
// 		GPIO_EnableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INT_BOTH_EDGE); 
// 		NVIC_EnableIRQ(GPAB_IRQn); 
// 		NVIC_EnableIRQ(GPCDF_IRQn); 
// 	}

// 	DrvWDT_DisableInt();
// 	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_OSC10K, GPIO_DBCLKSEL_1); 
// 	NVIC_DisableIRQ(GPAB_IRQn);
// 	ENABLE_SYSTICK();
// 	GPIO_SET_MODE(MAIN_POWER_CTRL_PORT, MAIN_POWER_CTRL_PIN, GPIO_OUTPUT);
// 	GPIO_SET_PIN(MAIN_POWER_CTRL_PORT_PIN);
// 	sysDelayMs(200);
// 	gpio_int = 0;
// 	if(DECT_DC()&&(GPIO_GET_PIN(POWER_KEY_PORT_PIN) || DECT_NFC())){
// 		if(DECT_NFC())
// 			nfc_start = 1;
// 		taskFlush();
// 		ledClkInit();	
// 		ledInit();
// 		ledTaskInit(); // flash the led in the borards
// 		adckeyTaskInit();	// adc key task init
// 		pt2314Init(); 
// 		ampInit();
// 		DWPH83Init();
// 		state_color = input_channel_color[dev_input];
// 		ledColorSet(state_color);
// 		dc_time_stamp = 0;
// 		outStandby();
// 		/*
// 		GPIO_CLR_PIN(PA12);
// 		GPIO_SET_PIN(PA13);
// 		GPIO_SET_PIN(PA14);
// 		for(i = 0; i < 75; i++){
// 			sysDelayMs(20);
// 			if(!GPIO_GET_PIN(POWER_KEY_PORT_PIN))
// 				goto SLEEP_;
// 		}
// 		outStandby();
// 		*/
// 	}else{
// 		doPowerOff();
// 		goto SLEEP_;
// 	}
	
}
extern enum KEYWORD key_buf;
void powerOff()
{
// 	U32 temp, i;
// 	static u32 bt_state = 0;

	AMP_MUTE();
	breath_flag = 0xff;
	BT_POWER_OFF();
	flash_mode = FLASH_CONFIG;
	amp_volume = volume_back&~0x80;
	SPI_CLR_SS0(SPI2);
	dev_state = DEV_STANDBY;
// 	state_color = RED_COLOR;
// 	ledColorSet(RED_COLOR);
	flash_mode = FLASH_CONFIG;
	stopFlashLed();
	sysDelayMs(200);
	ampPowerDown();
	sysDelayMs(200);
	APBCLK_DISABLE(CLK_I2S);
	I2S_DISABLE_MCK();
	outpw(&SYS->GPC_MFP, inpw(&SYS->GPC_MFP) & ~(0xF<<0)); // gpio i2s disable
	outpw(&SYS->ALT_MFP, inpw(&SYS->ALT_MFP) & ~(0xF<<5));
	outpw(&SYS->GPA_MFP, inpw(&SYS->GPA_MFP) & ~(0x1<<15)); // MCLK TO GPIO
	outpw(&SYS->ALT_MFP, inpw(&SYS->ALT_MFP) & ~(0x1<<9));
	GPIO_SET_MODE(PC,12,GPIO_INPUT); // SET I2S MCLK GPIO_INPUT
	GPIO_SET_MODE(PC,0,GPIO_INPUT);  // SET I2S LR GPIO_INPUT
	GPIO_SET_MODE(PC,1,GPIO_INPUT);  // SET I2S BCLK GPIO_INPUT
	GPIO_SET_MODE(PC,2,GPIO_INPUT);  // SET I2S DI GPIO_INPUT
	GPIO_SET_MODE(PC,3,GPIO_INPUT);  // SET I2S DO GPIO_INPUT
	sysDelayMs(100); 
// 	initIRKeyPool();
	logEvent(System_standby);
	logClose(); 
	
// 	if(!DECT_DC()){
if(enter_Standby_stop)
{
	outStandby();
}
else
{
		logClose();
		standbyPowerCtrlCLR();
		doPowerOff();
}
// 	}
	
//  standbyPowerCtrlCLR();     //ma 20160223  close core power control 
	//closeMcuCtrlPin();
// 	GPIO_SET_MODE(BT_HANG_UP_PORT, BT_HANG_UP_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(BT_POWER_EN_PORT, BT_POWER_EN_PIN, GPIO_INPUT); // BT_POWER_CTRL close
	// here shoul set power_key as int source and disable IR int
	
	//sleep();
}


/*-------------------------------------------------------------------------
 * @fucname:	 powerOn
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void powerOn() 
{
// 	U32 i = 0;
	
	AMP_MUTE();
// 	MAIN_POWER_OFF();
// 	sysDelayMs(100);
	openMcuCtrlPin();
	MAIN_POWER_ON();
	//DmaInit();
	AMP_OPEN();
	ampReset();	// reset the ad83586
	sysDelayMs(200);
	standbyPowerCtrlSet(); 
	dwhp83Rst();
	sysDelayMs(200); //delay 200 ms for pt2314 i2c module and adc  prepaired
	dwhp83RstDone();
	BT_POWER_ON();
	I2SClkInit(48000);	// init i2s
	sysDelayMs(200);
	i2cSysInit(); //i2c init
// 	logInit();
// 	logEvent(Power_up_sequence_started);
	sysDelayMs(100); // wait the ad83586 reset
	ampResetDone();	// reset the ad83586 done 
	sysDelayMs(50);

	sysDelayMs(10);
	ampInitAllConf(); 
	ampSendEqs_38(0,63);
	ampSetVolume(0);        	
// 	BT_POWER_OFF();
	unMute();  
	sysDelayMs(200);
	i2sStart();
	UPGRADE_CLR();
	sysDelayMs(200);
	getMuAddr();
	pt2314DataResume();	 // pt2314 init

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
// extern u32 test_mode;
extern u32 f_test;
extern u32 Dwhp83_CuMu;
void outStandby()
{
// 	static u32 default_input_volume;

	sysResume();
	resumeI2c();
/*
	if(test_mode == 0)
		dev_state = DEV_START;
	else
		dev_state = DEV_TEST;
*/
	enter_standby_mode = 0;
	enter_Standby_stop = 0;
	if(dev_state != DEV_TEST)
		dev_state = DEV_START;
		
	// at the opening moment the battery power led don't flash
	BAT_STATE_DECT_DELAY_COUNT = 0;
	BT_MATCH_FLAG = 0;
	led_battery_flash = 0; 
	ccount = 0;
	
	if(AUX_DETECT())
	{
		dev_input = 	LINEIN;
	}
	else 
	{
		dev_input = 	BLUETOOTH;
	}
	
	if(DECT_NFC()){
		//logEvent(NFC_touch_detected);
		dev_input = BLUETOOTH;
	}
	
// 	if(DECT_DC() && amp_volume > 24)
	if(amp_volume > 24)
		amp_volume = 24;
		if(amp_volume<9)
		{
			amp_volume= 9;
		}
	if(dev_state == DEV_TEST){
		amp_volume = VOLUME_MUSIC + 5;
		dev_input = LINEIN;
		ledColorSet(SOE_COLOR);
	}else{
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
	}

	
	change_source_mute = 1;
	spiFlashInit();
	powerOn();
	if(!DECT_DC())
	{
		if(read_upgrade_eeprom(0) != I2C_ERROR)
		{
			bootLDROM();
		}
	}
	UPGRADE_SET();
	if(f_test){
		ampSetVolume(VOLUME_MUSIC);
		ampSendCmd_38(MUTELOC, 0x16);
		ledColorSet(LIGHT_BLUE_COLOR);
		playMusic(POWER_ON_SOUND);
		sysDelayMs(300);
		playMusic(POWER_ON_SOUND);
		sysDelayMs(300);
		flash_mode = FLASH_CONFIG;
		ledColorSet(WHITE_COLOR);
		ampSendCmd_38(MUTELOC, 0x1a);
		sysDelayMs(200);
		for(f_test = 15; f_test>0; f_test--){
			playMusic(DD_SOUND);
			sysDelayMs(200);
		}
		flash_mode = FLASH_CONFIG;
		ampSendCmd_38(MUTELOC, 0x1C);
		ledColorSet(PURPLE_COLOR);
		sysDelayMs(200);
		for(f_test = 15; f_test>0; f_test--){
			playMusic(DD_SOUND);
			sysDelayMs(200);
		}
		flash_mode = FLASH_CONFIG;
		ledColorSet(state_color);
		ampSetVolume(0);
		ampSendCmd_38(MUTELOC, 0x10);
		sysDelayMs(1000);
		f_test = 0;
	}
	
// 	if(DECT_DC())
// 		logEvent(DC_inserted);
	
	
	
	audio_quiet_time = 0;
	dc_flag = 0;
	
// 	amp_volume=24;

#if(SPIFLASH_ENABLE == TRUE)	
	//spiFlashInit();
	if(dev_state != DEV_TEST)
			if(reset_en == 0){
					playMusic(POWER_ON_SOUND);
				}
				else reset_en = 0;
#endif
#if(UART_EN)
	uartInit();
	sendStartInfo();
#endif	
	
	
#if(GESTURE_EN)
	gsensor_init();
#endif
	ledTask();
	ledColorSet(state_color);
	WDTInit();	// init watch dog timer
	sysDelayMs(200);
// 	ampSetVolume(VOLUME_MUSIC); 
	//if(dev_state == DEV_TEST)
	//	ampSetVolume(amp_volume);
	//else
	//	changeInputVolumeUp(); // bring up volume form 0 to w3d_volume 

	adcReadKey();
	adcReadKey();
	sysDelayMs(1200);
// 	ampSetVolume(15); 
	if(check_coco())          //ma 20150521  function error    //ma 20151130   //MA 20151201
	{

		Dwhp83FixedCoCo();         //ma  20150521  set fixed  coco
		sysDelayMs(100);        //ma 20150525 add 2sec to dwhp83
	}
	Dwhp83CuMuDetect();
	if(Dwhp83_CuMu == CU)
	{	
		DWP83MuPowerOn();
		Dwhp83Slave();
	}
// 	else if(Dwhp83_CuMu == MU)
// 		DWP83MuPowerOn();
	change_source_mute = 0;
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
	u32 i;
// 	static u32 stamp = 0;
	enter_standby_mode = 1;
	sysStore();
// #if(FIXED_COCO_MODE)
	setConfig();   //ma 20150525
// #endif
	change_source_mute = 1;
	//logEvent(Power_down_sequence_started);
	sysDelayMs(100);
// 	state_color = RED_COLOR;
	dev_state = DEV_STANDBYP;
// 	stopATask(led_task);
// 	leddisable();
// 	ledColorSet(RED_COLOR);
	playMusic(POWER_OFF_SOUND);
	BT_POWER_OFF();
	
	//sysDelayMs(1000);
	sysDelayMs(1500);		
	
// 	BT_POWER_ON();
// 	sysDelayMs(150);
// 	BT_POWER_OFF();
// 	sysDelayMs(1000);	
	for(i=15; i > 0; i--){ 
		ampSetVolume(i);
		sysDelayMs(3);

	}
	ampSetVolume(0);
	ampMute();
	change_source_mute = 0;
	if(enter_Standby_stop)
	{
		outStandby();
	}
	else powerOff();
} 


/*-------------------------------------------------------------------------
 * @fucname:	 WDT_IRQHandler
 * @brief: 	 watch dog ISR
 * @param: 
 * @return: 
 * @details: 	now just reset the chip
 *-----------------------------------------------------------------------*/
void WDT_IRQHandler()
{
	WDTwake();
	SYS_UnlockReg();
	WDT_CLEAR_TIMEOUT_INT_FLAG();
	WDT_TIMER_RESET();       
	WDT_SET_INTERVAL(5);   
	SYS_LockReg();	
}

/*-------------------------------------------------------------------------
 * @fucname:	 WDTInit
 * @brief: 	 init the watch dog timer
 * @param: 	 
 * @return: 
 * @details: 	 init the watch dog timer to  reset cpu 
 *-----------------------------------------------------------------------*/
void WDTInit()
{
	WDT_TIMER_RESET();          
	DrvWDT_Init(5,CLK_WDT_LIRC,WDT_RESET_DELAY_128);
	SYS_UnlockReg();
	WDT_TIMER_RESET();
	WDT_ENABLE_COUNTING();    
	WDT_CLEAR_RESET_FLAG();
	WDT_DISABLE_WAKEUP();
	SYS_LockReg();
	//DrvWDT_EnableInt();
	WDT_ENABLE_RESET();
}

void WDTwake()
{
	DrvWDT_Init(5,CLK_WDT_LIRC,WDT_RESET_DELAY_1024);
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


void GPAB_IRQHandler()
{
// 	GPIO_CLR_PIN_IF(POWER_KEY_PORT, POWER_KEY_PIN);   
// 	GPIO_CLR_PIN_IF(NFC_DECTECT_PORT,NFC_DECTECT_PIN);
// 	GPIO_DisableInt(POWER_KEY_PORT, POWER_KEY_PIN); 
// 	GPIO_DisableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN); 
	gpio_int = 1;
}

void GPCDF_IRQHandler()
{
	GPIO_CLR_PIN_IF(DC_DECTECT_PORT, DC_DECTECT_PIN);
	GPIO_DisableInt(DC_DECTECT_PORT, DC_DECTECT_PIN); 
	gpio_int = 1;
}

void HardFault_Handler()
{	
	NVIC_SystemReset();
}



