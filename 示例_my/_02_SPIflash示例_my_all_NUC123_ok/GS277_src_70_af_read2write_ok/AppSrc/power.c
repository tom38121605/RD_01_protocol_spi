/****************************************************************************
 * @file:		power.c
 * @version:	V1.00
 * @breif:		power control source file
 * @date: 	2013/3/13
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/

#include "sysinc.h"
enum WDTHANDLER wdt_handler = WDT_RST;
u32 wdt_timer = 0;
u32 volatile gpio_int = 0;
PWRTYPE *pwrcon = (PWRTYPE*)CLK_BASE;
enum BATTERY_STATE battery_state = BATTERY_OFF;
extern void adcReadKey(void);
extern u32 BAT_STATE_DECT_DELAY_COUNT;
extern u32 dc_time_stamp;
u8 NFC_flag = 0;     // ma 20150528  add NFCflag
u8 enter_standby_flag = 0;      //ma 20150604  add outstanby mode to enable WL
u8 power_first_on_flag  = 0;     // ma 20150530  add power first power to no red color
/******add  factory test mode flag ********/
u8 factory_mode_flag =0;         //ma 20150616      set factory mode flag
u8 volume_store = 24;      //ma 20150617

u8 sleep_flag  = 0;        //ma 20150706  
/*-------------------------------------------------------------------------
 * @fucname:	 sys_resume
 * @brief: 	 sys infomation resume 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys infomation resume 
 *-----------------------------------------------------------------------*/
void sysResume()     //ma 20150624  
{
// 	 /******wait for clk stable****/
// 	SYS_UnlockReg();
// 	*(u32*)0xE000ED10 = 0X4; // WHEN
// 	pwrcon->pd_wu_dly = 0;
// 	pwrcon->pd_wait_cpu = 1; 
// 	pwrcon->pd_wu_int_en = 0;
// 	pwrcon->pwr_down_en = 1; 
// 	pwrcon->osc10k_en = 1;
// 	pwrcon->xtal12m_en = 1;
// 	//pwrcon->xtal12m_en = 0;
// 	SYS_LockReg();
	
	/*****open ADC******/   //ma 20150624
// 	DrvADC_EnableInt();
	
	/*******open uart*****/  //ma 20150624
// 		DrvUART_DisableInt (UART0, UART_IER_BUF_ERR_IEN | UART_IER_RTO_IEN |UART_IER_MODEM_IEN  
// 						| UART_IER_RLS_IEN | UART_IER_THRE_IEN |UART_IER_RDA_IEN);      //ma 20150624
// 	
// 	/******open TIMER 0******/
// 	DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);    //ma 20150624
	 
//  /******ADC high CLK******/         //ma 20150624
// 	SYS_UnlockReg();
// 	SYS_RESET_IP2(SYS_RST_ADC);
// 	SET_ADC_CLKSRC(CLK_ADC_HXT);
// 	SET_ADC_CLKDIV(ADC_CLK_PRESCALE);
// 	APBCLK_ENABLE(CLK_ADC);
// 	SYS_LockReg();
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
	
	GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
	GPIO_SET_MODE(POWER_CTRL_PORT,POWER_CTRL_PIN,GPIO_OUTPUT);
	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
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

	GPIO_SET_MODE(PB, 10, GPIO_INPUT);
	sysDelayUs(100000);
	if(DECT_DC()  || DECT_NFC()){
		if(DECT_NFC())
			NFC_flag = 1;     //ma 20150528  
		GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
		sysDelayUs(100000);
		if(DECT_DC())
			dc_time_stamp = 1;
	}
	else{
		dc_time_stamp = 0;
		if(IS_SYSTEM_RST())
			i = 80;
		
		for(; i < BATTERY_OPEN_DELAY; i++)
			if(GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
				sysDelayMs(5);
				if(DECT_DC()){
					dc_time_stamp = 1;
					GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
					break;
				}
			}
			else{
				//sysDelayMs(100);
				GPIO_CLR_PIN(POWER_CTRL_PORT_PIN);
				i = 0;
				while(GPIO_GET_PIN(POWER_KEY_PORT_PIN) == 0)
					;
			}

	}

	adcReadKey();
	adcReadKey(); // MAKE THE KEY_WORD BE NONE KEY
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
// 	u32 k = 0;      //ma 20150625  add k to dect dc for 1sec
	u32 power_key_release = FALSE;
	
	DrvWDT_DisableInt(); 
	GPIO_CLR_PIN(POWER_CTRL_PORT_PIN);
	
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

		if(DECT_DC()){       //dect dc 1sec enter  standby 
// 			++k;
// 			if(k>5)
// 			{
				GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
				sysDelayMs(50);
				break;
// 			}

		}
			
		if(	power_key_release == TRUE &&   // if user don.t relase the power key , don't restart
			GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
				
				if(++j == 10){
					power_key_release = FALSE; 
					ledColorSet(DARK_COLOR);
					GPIO_SET_PIN(POWER_CTRL_PORT_PIN);
					sysDelayMs(600);
					SYS_UnlockReg();
					NVIC_SystemReset();
				}
		}
		else{
			j = 0;
		}
	}	
}

extern enum KEYWORD last_key;
extern u32 ir_key_bits;
u32 recode_t = 0;

void sleep()
{
	
	MAIN_POWER_OFF();
	standbyPowerCtrlCLR();
	closeMcuCtrlPin();
	
SLEEP_:
	
	GPIO_SET_MODE(POWER_KEY_PORT,POWER_KEY_PIN,GPIO_INPUT);
	GPIO_SET_MODE(DC_DECTECT_PORT,DC_DECTECT_PIN,GPIO_INPUT);
	GPIO_SET_MODE(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INPUT);
	
	while( GPIO_GET_PIN(PB8) )
		sysDelayMs(20);
	
	GPIO_CLR_PIN(RED_PIN);	
	GPIO_SET_MODE(PD,1,GPIO_OUTPUT);
	
	SYS_UnlockReg();
	*(u32*)0xE000ED10 = 0X4; // WHEN
	pwrcon->pd_wu_dly = 0;
	pwrcon->pd_wait_cpu = 1; 
	pwrcon->pd_wu_int_en = 0;
	pwrcon->pwr_down_en = 1; 
	pwrcon->osc10k_en = 1;
	pwrcon->xtal12m_en = 1;
	//pwrcon->xtal12m_en = 0;
	SYS_LockReg();
	
 	DrvADC_DisableInt();       //ma 20150623  do not  disable ADC 
	

	DISABLE_SYSTICK(); 
	WDT_DISABLE_RESET(); 
	last_key = NONE_KEY; 
	
	DrvUART_DisableInt (UART0, UART_IER_BUF_ERR_IEN | UART_IER_RTO_IEN |UART_IER_MODEM_IEN
						| UART_IER_RLS_IEN | UART_IER_THRE_IEN |UART_IER_RDA_IEN);    
						
	GPIO_ENABLE_DEBOUNCE(POWER_KEY_PORT,POWER_KEY_PIN);
	
	//GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_OSC10K, GPIO_DBCLKSEL_4096); 
	
	NVIC_DisableIRQ(I2C0_IRQn); 
	APBCLK_DISABLE(CLK_I2S); 
	DrvTIMER_DisableInt(TIMER0); 	
	
	GPIO_CLR_PIN(RED_PIN);		
	I2S_DISABLE_MCK(); 
	
	GPIO_EnableInt(POWER_KEY_PORT, POWER_KEY_PIN, GPIO_INT_HIGH);  
	GPIO_EnableInt(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INT_HIGH);  
	GPIO_EnableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INT_BOTH_EDGE);  
	NVIC_EnableIRQ(GPAB_IRQn);  
	
	// here shoul set power_key as int source and disable IR int
	//WDTwake();

		
// #if(ADC_KEY_PDMA_EN == FALSE)      //ma 20150623  add   for ADC IRQ   
// 	NVIC_SetPriority (ADC_IRQn, ADC_INT_PRIORITY); 
// 	DrvADC_EnableInt();
// #endif	   

	gpio_int = 0; 
	wdt_timer = 0; 

	//__asm("wfi");
	while(1)
	{
		GPIO_EnableInt(DC_DECTECT_PORT, DC_DECTECT_PIN, GPIO_INT_HIGH); 
		GPIO_EnableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN, GPIO_INT_BOTH_EDGE); 
		GPIO_EnableInt(POWER_KEY_PORT, POWER_KEY_PIN, GPIO_INT_HIGH); 
		
		recode_t = 0; 
		__asm("wfi"); 
		sysDelayUs(100000); 
		
		if((ir_key_bits) == 0Xf40bff00 ||(ir_key_bits<<1) == 0Xf40bff00 ) 
		{
				recode_t = 1; 
				gpio_int = 1;          //ma 20150702   no need to power on when battery mode  
		}
		if(gpio_int == 1) 
			break; 
		
		SYS_UnlockReg(); 
 		*(u32*)0xE000ED10 = 0X4; // WHEN 
		pwrcon->pd_wu_dly = 0; 
		pwrcon->pd_wait_cpu = 1;  
		pwrcon->pd_wu_int_en = 0; 
		pwrcon->pwr_down_en = 1;  
		pwrcon->osc10k_en = 1; 
		pwrcon->xtal12m_en = 1; 
		SYS_LockReg(); 
		NVIC_EnableIRQ(GPAB_IRQn); 

		GPIO_CLR_PIN(PD1); 
	}
	
	DrvWDT_DisableInt(); 
	GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_OSC10K, GPIO_DBCLKSEL_1);  
	
	//NVIC_DisableIRQ(GPAB_IRQn); 
	ENABLE_SYSTICK(); 
	MAIN_POWER_ON(); 
	GPIO_SET_MODE(POWER_CTRL_PORT, POWER_CTRL_PIN, GPIO_OUTPUT); 
	sysDelayMs(100); 
	gpio_int = 0; 
	
	if( DECT_DC()&& (GPIO_GET_PIN(POWER_KEY_PORT_PIN) || DECT_NFC()) || recode_t == 1 )
	{
		/******check nfc start****/
		if(DECT_NFC())
		{
			bt_paring_flag = 1;		   // ma 20150528
			NFC_flag = 1;            //MA 20150528
		}
		
		DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);
		taskFlush();
		
		irKeyTaskInit();
		ledTaskInit(); // flash the led in the borards
		adckeyTaskInit();	// adc key task init
		cs8422Init(); 
		seW58Init();
		DWPH83Init();
		
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
		dc_time_stamp = 0;
		outStandby();
	
	}
	else
	{
		doPowerOff();
		goto SLEEP_;
	}
	
}
/*-------------------------------------------------------------------------
 * @fucname:	 poweroff
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/


void powerOff()
{
// 	U32  i;      //ma 20150629 
	
	//U32 temp;      //ma 20150629  
// 	static u32 bt_state = 0;     //ma 20150629  

	
	AMP_MUTE();
	sysDelayMs(200); 
	BT_OFF();
	dev_state = DEV_STANDBY;
	state_color = RED_COLOR;
	ledColorSet(RED_COLOR);	
	flash_mode = FLASH_CONFIG;	
	stopFlashLed();
	ampPowerDown();
	sysDelayMs(500); 
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
	standbyPowerCtrlCLR();
	sysDelayMs(300); 
	APBCLK_DISABLE(I2C_CLK);    
	closeMcuCtrlPin();
	
//  /******wait for clk stable****/
//  	SYS_UnlockReg();
// 	*(u32*)0xE000ED10 = 0X4; // WHEN
// 	pwrcon->pd_wu_dly = 0;
// 	pwrcon->pd_wait_cpu = 1; 
// 	pwrcon->pd_wu_int_en = 0;
// 	pwrcon->pwr_down_en = 1; 
// 	pwrcon->osc10k_en = 1;
// 	pwrcon->xtal12m_en = 1;
// 	//pwrcon->xtal12m_en = 0;
// 	SYS_LockReg();
// 	

		/*******close uart*****/  //ma 20150624
		DrvUART_DisableInt (UART0, UART_IER_BUF_ERR_IEN | UART_IER_RTO_IEN |UART_IER_MODEM_IEN  
						| UART_IER_RLS_IEN | UART_IER_THRE_IEN |UART_IER_RDA_IEN);      //ma 20150624
		/*****close adc*****/    //ma 20150624
// 	 	DrvADC_DisableInt();       //ma 20150623  do not  disable ADC 
		
// 	/******close TIMER 0******/
// 		DrvTIMER_DisableInt(TIMER0);    //ma 20150624
	 
//  /******ADC LOW CLK******/         //ma 20150624
// 	SYS_UnlockReg();
// 	SYS_RESET_IP2(SYS_RST_ADC);
// 	SET_ADC_CLKSRC(CLK_ADC_HXT);
// 	SET_ADC_CLKDIV((ADC_CLK_PRESCALE)+251);
// 	APBCLK_ENABLE(CLK_ADC);
// 	SYS_LockReg();
// 	sysDelayMs(100);
	
		
	initIRKeyPool();
	while(GPIO_GET_PIN(PB8))
		sysDelayMs(20);
	if(!DECT_DC()){
		doPowerOff();
	}
// 	else if(dc_auto_down_flag == 5)        //ma 20150706       //ma 20150715
// 	{
// 		doPowerOff();
// 	}
		amp_volume = volume_store;    //ma 20150617
	
// 		doPowerOff();    //ma 20150623     set enter sleep
// 		sleep();
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
	sysDelayMs(50); 
	BT_OFF();
	AMP_OPEN();
	MAIN_POWER_ON();
	standbyPowerCtrlSet();
	openMcuCtrlPin();
	sysDelayMs(100);
	MAIN_RST();
	sysDelayMs(100);
	I2SClkInit(48000);	// init i2s
	BTInit();
	//setw58Presel(0);
	sysDelayMs(100); 
	MAIN_RST_DONE();
	i2cSysInit(); //i2c init
	sysDelayMs(100); 
	BT_OPEN();
	i2sStart();
	sysDelayMs(100); 
 	mute();              //ma 20150728   del mute
	ampSendEqs_38(0,63);
	ampSendEqs_38b(0,63);
	ampSendEqs_3A(0,63);
	ampInitAllConf();
	sysDelayMs(5); 
// 	seW58SetVolume(31);
	seW58DefaultVolume();         //ma 20150713  
	seW58sendCmd(); 
	sysDelayMs(5); 
	seW58SetBypass(0); // 3d on
	seW58sendCmd();
	sysDelayMs(5);
	ampSetVolume(amp_volume);
	pcm5121Init();
	sysDelayMs(100); 
	sysDelayMs(1000);       //ma 201509010      add  reset time for wireless module 
}

/*-------------------------------------------------------------------------
 * @fucname:	 outStandby
 * @brief: 	 sys out of standy mode
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys out of standy mode
 *-----------------------------------------------------------------------*/
 extern u32 cc_blue_p;
 extern u32 Dwhp83_flag;     // ma 20150606

void outStandby()
{
// 	u32 k;   //ma 20150629  
// 	u8 coco_first[3] = {0x00,0x00,0x00};   // ma 20150603
	enter_standby_flag = 0;     // ma 20150603
	 
	bt_amp_start_flag  = 1;      //ma 20150707  

	sysResume();       // ma 20150624
	resumeI2c();
	power_first_on_flag = 1;     //ma 20150529    out standby  no red color

 	if(dev_state != DEV_TEST)           //ma 20150707   del useless function   
		dev_state = DEV_START;
	
	// at the opening moment the battery power led don't flash

	cc_blue_p = 45;  //ma 20150907   set 45
	BAT_STATE_DECT_DELAY_COUNT = 120;
// 		getConfig();      //MA 20150713                  //ma 20150715  
// 	if(amp_volume > 24)
// 		amp_volume = 24;   //ma 20150508 change the default volume 25 to 23   //ma 20150609 25 level new ui 

// 	if(amp_volume < 8)   //ma 20150609 9  level new ui 
// 		amp_volume = 8;
// 	if(factory_mode_flag)                           //ma 20150715  
// 			dev_input = AUX; 
// 	state_color = input_channel_color[dev_input];                   //ma 20150715  
// 	ledColorSet(state_color);
	if(!sleep_flag)                             //ma 20150716
	{
		if(NFC_flag)                          //ma 20150715  
			dev_input =BLUETOOTH; 
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);     //ma 20150606
	}
	  sleep_flag  = 0;       //ma 20150706  reset   sleep flag 
	powerOn();
		getConfig();      //MA 20150713                  //ma 20150715 
		if(!SB_flag)      //MA 20150604
	{
			set_local_coco();              //ma 20150728
			sysDelayMs(20);
	}
	else
	{
		set_soundbar_coco();             //ma 20150728
		sysDelayMs(20);
	}
	/****if SB power on CU****/   //20150606
	Dwhp83CuMuDetect();        //ma 20150728
	if(SB_flag)
	{
		Dwhp83_flag = 0;
		if(Dwhp83_CuMu == MU)            //MA 2015728
		{
			Dwhp83Master();
			sysDelayMs(2000);
			SUB_volume_unmute();    //ma 20150608   change channal unmate
// 		DWP83CuPowerOn();
		}
		else sysDelayMs(1000);       //ma 20150731  add  some time for   dwhp83  preparing
	}
	else 
	{
			Dwhp83_flag = 0;
		if(Dwhp83_CuMu == CU)        //MA 2015728
		{
			Dwhp83Slave();
			sysDelayMs(1000);       //ma 20150707    del delay time    //ma 20150713
		}
		else sysDelayMs(1000);       //ma 20150731  add  some time for   dwhp83  preparing
// 			DWP83MuPowerOn();
	}
// 		if(!SB_flag)                        //ma 20150807    check   coco
// 		{
// // 			if(!local_coco_check())
// // 			{
// 				set_local_coco();
// 				sysDelayMs(100);
// // 			}
// 		}
// 		else
// 		{
// // 			if(!soundbar_coco_check())
// 			set_soundbar_coco();
// 			sysDelayMs(100);
// 		}
	if(factory_mode_flag)                           //ma 20150715  
			dev_input = AUX; 
	else if(NFC_flag)                          //ma 20150715  
		dev_input =BLUETOOTH; 
	
#if(SPIFLASH_ENABLE == TRUE)	
	spiFlashInit();
		bt_amp_start_flag  = 0;      //ma 20150707  
	
	/******when no dc  charge play power on sound ******/
 	if(!DECT_DC())      //MA 20150702 
		;//--playMusic(POWER_ON_SOUND);  //--zz test
	else if(factory_mode_flag)
		playMusic(CORE_SOUND);
	
#endif
	
 	unMute();                //ma 20150728  del unmute
	bt_amp_start_flag  = 1;      //ma 20150707  

#if(UART_EN)
	uartInit();
	sendStartInfo();
#endif 

//#if(GESTURE_EN)
//	gsensor_init();
//#endif


// 	SYS_UnlockReg();                    //ma 20150814
// 	WDT_TIMER_RESET();   //Clear WDT timer
// 	SYS_LockReg();
	WDTInit();	// init watch dog timer
	//changeInputVolumeUp(); // bring up volume form 0 to w3d_volume 

	cs8422InitAllConf();
	bt_amp_start_flag  = 0;      //ma 20150707  	
			/*****remember dis HFP****/     
		if(!bt_hfp_flag)         //ma 20150609     default no HFP      //ma 20150724  test HFP
		{
			btDisableHFP();      //ma 20150527
			sysDelayMs(100);     //MA 20150612
// 			sysDelayMs(3000);
		}
	adcReadKey();
	adcReadKey(); // MAKE THE KEY_WORD BE NONE KEY

	amp_volume = readeeprom(0x03); sysDelayMs(5);
	if(amp_volume > 25)         //ma 20150707   
		amp_volume = 25;   //ma 20150508 change the default volume 25 to 23   //ma 20150609 25 level new ui 

	if(amp_volume < 9)   //ma 20150609 9  level new ui 
		amp_volume = 9;
	
// 	if(dev_input!=BLUETOOTH)      //MA 20150709
// 	{
// 			BT_OPEN();            //ma 20150526
// 			btStartUp();        //ma 20150520    BT+ON
// 			sysDelayMs(500);
// 			BT_OFF();            //ma 20150526			
// 			btShutDown();         //ma 20150520    BT+OFF
// 			sysDelayMs(20);
// 			bt_power_flag =1;
// 			BT_MATCH_FLAG = 0;
// 	}

	/*****set volume*****/  // 20150617
// 	amp_volume = volume_store;
// 	Dwhp83CuMuDetect();

// 	if(Dwhp83_CuMu == CU)     ma 20150606
// 		DWP83CuPowerOn();
// 	else if(Dwhp83_CuMu == MU)
// 		DWP83MuPowerOn();
	mute();
	if(!SB_flag)                        //ma 20150806
	{
		if(!local_coco_check())
		{
			{
				set_local_coco();
				sysDelayMs(1000);
			}
		}
// 		else sysDelayMs(1000);      //ma 20150819 add some delay for optical prepare     //ma 20150831   
	}
	else
	{
		if(!soundbar_coco_check())
		{
			set_soundbar_coco();
			sysDelayMs(1000);     //ma 20150819 add some delay for optical prepare 
		}
// 		else sysDelayMs(1000);                                        //ma 20150831  
	}

	/********check cumu******/
	if(Dwhp83CuMuDetect())        //ma 20150728
	{
		if(SB_flag)
		{
			Dwhp83_flag = 0;
			if(Dwhp83_CuMu == MU)             //MA 2015728
			{
				Dwhp83Master();
	// 		DWP83CuPowerOn();
			}
			sysDelayMs(1000);                  //ma 20150831  
		}
		else 
		{
				Dwhp83_flag = 0;
			if(Dwhp83_CuMu == CU)        //MA 2015728
			{
				Dwhp83Slave();

			}
     sysDelayMs(1000);               //ma 20150831  
		}
	}
	unMute();
}
/*-------------------------------------------------------------------------
 * @fucname:	 enterStandby
 * @brief: 	 sys enter standy mode
 * @param: 	 none
 * @return: 	 none
 * @details: 	 sys enter standy mode
 *-----------------------------------------------------------------------*/
extern u32 volume_back;
extern u8 COCO_KEY_BEFORE;   //20150602
void enterStandby()
{
	s32 i;
	u8 coco_last[3] = {0x10,0x10,0x10};       //ma 20150727  change defalut power off coco
	enter_standby_flag = 1;     //ma 20150603
	NFC_flag = 0;    // ma 20150716
	bt_amp_start_flag = 1;   //ma 20150727  avoid power off noise
	if(factory_mode_flag)  //ma 20150617
	{
		test_channel = 5;
		testOutputSpeakers();
	}
// 	if(dev_input != USERDEF&&SB_flag)            //ma 20150603        //MA 20150710       //ma 20150803       //MA 20150825  sub will change his coco automatically
// 	{
// 		Dwhp83CuMuDetect();
// 		if(Dwhp83_CuMu == CU)        //ma 20150514 if  local the-one is MU to tell the current CU to tell SUB  change COCO and enter SB mode 
// 		{
// 			DWHP83toSUB();           //ma 20150512  CU send massage to SUB  0x66
// 			sysDelayMs(100);
// 		}
// 	}
	/****soundbar the-one power off tell sub to sound bar coco***///ma 20150530   
	if(SB_flag&&Dwhp83CuMuDetect())           //ma 20150514  check CUMU                  //ma 20150803
	{
		if(Dwhp83_CuMu == MU)        //ma 20150514 if  local the-one is MU to tell the current CU to tell SUB  change COCO and enter SB mode 
		{
			DWHP83toEXT_MCU();     //ma 20150513  change funcion to AUE   0x55
			sysDelayMs(100);
		}
	}
// 	if(!SB_flag)     //ma 20150604  MCU standby DWHP83 standby         //ma 20150720            
// 	{
// 		coco_last[0] = 0x10;             //ma 20150727   coco 0x00 tx all rx recieve 
// 		coco_last[1] = 0x10;
// 		coco_last[2] = 0x10;
		/****if power off SB_flag == 1 default AUE STATE******/
	if(SB_flag&&dev_input ==USERDEF)
	{	
		dev_input = OPTICAL;       //ma 20150710  change default soundbar power on state OPTICAL
		set_soundbar_coco();          //ma 20150826
		sysDelayMs(100);
	}
  else
	{
		writeDwhp83(coco_last, 0xc6, 3);      //ma 20150727   write default power off coco
		sysDelayMs(100);  
	}		
// 	}
		if(!SB_flag)                       //ma 20150716
		{
			if(Dwhp83CuMuDetect())
			{
				if(Dwhp83_CuMu == CU)
				{
					Dwhp83Slave();            //ma 20150728
// 					sysDelayMs(1000);       //ma 20150728
				}
			}
		}




		/****volume store****/  //ma 20150617 
	volume_store = amp_volume;
	if(volume_store>25)
	{
		volume_store = 25;
	}
	else if(volume_store <9)
	{
		volume_store = 9;
	}
		amp_volume= volume_store ;
// 	amp_volume = 14;
	setConfig();     //ma 20150612    store volume 	
	sysStore();
// 	mute();
// 	volume_back = volume_back&~0x80;
// 	amp_volume = volume_back;

	sysDelayMs(100);
	dev_state = DEV_STANDBY;
	state_color = RED_COLOR;
	ledColorSet(RED_COLOR);
	
	bt_amp_start_flag = 0;   //ma 20150727  avoid power off noise
	DWHP83Mute();              //ma 20150720 
	
	
	
  /******when no dc  charge play power off sound ******/
// 	if(!DECT_DC())      //MA 20150702 
// 		playMusic(POWER_OFF_SOUND);
// 	DWHP83UnMute();          //ma 20150720  

  if(!DECT_DC())    
	{
		sysDelayMs(50); 
	  DWHP83UnMute();         
		sysDelayMs(50); 
		playMusic(POWER_OFF_SOUND);
	}
	
	bt_amp_start_flag = 1;   //ma 20150727  avoid power off noise
	btShutDown();
	sysDelayMs(500);      //ma 20150728
	
	sysDelayMs(1000);   //--zz
	
	for(i=30; i >=  0; i-- ){
		seW58SetVolume(i);
		seW58sendCmd();
		sysDelayMs(5);
	}
	
	mute();
	sysDelayMs(100);
	bt_amp_start_flag = 0;   //ma 20150727  avoid power off noise
	powerOff();
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
// 	WDTwake();                  //ma 20150814 
	SYS_UnlockReg();
	WDT_CLEAR_TIMEOUT_INT_FLAG();
// 		WDT_TIMER_RESET();       
//	WDT_SET_INTERVAL(5);   
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
// 	WDT_TIMER_RESET();              //ma 20150814    
	DrvWDT_Init(5,CLK_WDT_LIRC,WDT_RESET_DELAY_128);
 
	SYS_UnlockReg();
	WDT_ENABLE_COUNTING();    
	WDT_CLEAR_RESET_FLAG();
	WDT_DISABLE_WAKEUP();
	WDT_ENABLE_RESET();
	WDT_TIMER_RESET();
	DrvWDT_EnableInt();//ma 20150814  
	SYS_LockReg();
	//DrvWDT_EnableInt();

}

// void WDTwake()                        //ma 20150814 
// {
// 	DrvWDT_Init(5,CLK_WDT_LIRC,WDT_RESET_DELAY_1024);
// 	DrvWDT_EnableInt();
// 	SYS_UnlockReg();
// 	WDT_TIMER_RESET();
// 	WDT_ENABLE_COUNTING();     
// 	WDT_CLEAR_RESET_FLAG();
// 	WDT_ENABLE_WAKEUP();
// 	WDT_DISABLE_RESET();
// 	SYS_LockReg();
// }



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
	sysDelayMs(2000);;
	SYS_LockReg();
}


void GPAB_IRQHandler()
{
	if(GPIO_GET_PORT_IF(PB) == 0X40)
	{
		IR_IRQHandler();
		return;
	}
	
// 	if(DECT_NFC())                        //ma 20150715 
// 	{
// 		NFC_flag = 1;    //ma 20150609
// 		bt_power_flag = 0;
// 		dev_input = BLUETOOTH;  //MA 20150609
// 		state_color = input_channel_color[dev_input];
// 		ledColorSet(state_color);		
// 	}
	
	GPIO_CLR_PIN_IF(POWER_KEY_PORT, POWER_KEY_PIN);   
	GPIO_CLR_PIN_IF(DC_DECTECT_PORT, DC_DECTECT_PIN);   
	GPIO_CLR_PIN_IF(NFC_DECTECT_PORT,NFC_DECTECT_PIN);
	GPIO_DisableInt(POWER_KEY_PORT, POWER_KEY_PIN); 
	GPIO_DisableInt(DC_DECTECT_PORT, DC_DECTECT_PIN); 
	GPIO_DisableInt(NFC_DECTECT_PORT, NFC_DECTECT_PIN); 
	gpio_int = 1;
	
}

/*
void HardFault_Handler()
{
	SYS_UnlockReg();
	NVIC_SystemReset();
}
*/

