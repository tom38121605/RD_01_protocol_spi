/****************************************************************************
 * @file:		adcKey.c
 * @version:	V1.00	
 * @breif:	adckey source file	
 * @date: 	2013/2/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

// button that press down without relax, adc_key_word will 
// record once untill relax the button
enum KEYWORD key_word = NONE_KEY;

// button that press down without relax,key_word_continue
// will record per configuration interval, e.g record per 100ms
enum KEYWORD key_word_continue = NONE_KEY;	// continue key press

Timer_ID adc_key_task = TASK_FULL;
static u16 key_bak = 0,key_irq_index = 0;		// key value for adc irq
static u16 key_adc_value = 0;
//static u16 key_buf_lrf[MAX_LRF_KEYS] = {NONE_KEY};	// store the keys for LRF
enum KEYWORD key_buf = NONE_KEY;				// the key after LRF
enum KEYWORD key_buf_adc = NONE_KEY;
u16 u16ADCBuf = 0;	// used for pdma destAddr buffer of adc value
u32 auto_standy_delay_minunte = 10;       //10 mins  no sound in  power off
u32 battery_volage = 0;
u32 dc_time_stamp = 0;
static u32 bt_volage_adc, bt_volage, bt_count = 120;
static u32 wl_coco_adc = 0;       //ma 20150424

u8 battery_high_flag = 0;     //ma 20150703   add for  batterry high.
u8 soft_mute_flag = 0;     //ma 20150703  add soft mute flag  when  play flash music 
/*-------------------------------------------------------------------------
 * @fucname:	 AdcInit
 * @brief: 	 adc clk init 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 adc clk init
 *-----------------------------------------------------------------------*/
void adcKeyClkInit()
{
	SYS_RESET_IP2(SYS_RST_ADC);
	SET_ADC_CLKSRC(CLK_ADC_HXT);
	SET_ADC_CLKDIV(ADC_CLK_PRESCALE);
	APBCLK_ENABLE(CLK_ADC);
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 readKeyContinue
 * @brief: 	 get pressed key's value in configuration interval
 * @param: 	 none
 * @return: 	 the key_word_continue value
 * @details: 	 return key_word_continue value, can de-bouncd pulse < 60ms = scan rate 
 *-----------------------------------------------------------------------*/
enum KEYWORD adcReadKeyContinue()
{
	static s32 kwcdc = 0; //key word continue delay counter
	static enum KEYWORD key_c_r = NONE_KEY;
	key_word_continue = NONE_KEY;
	
	if(key_word != NONE_KEY){
		kwcdc = KEY_CONTINUE_START;
	}else if (++kwcdc < KEY_CONTINUE_COUNTER){
		key_c_r = key_buf;
	}else{
		kwcdc = 0;
		if(key_c_r == key_buf)
			key_word_continue = key_buf;	
	}
	
	return key_word_continue;
}

/*-------------------------------------------------------------------------
 * @fucname:	 readKey
 * @brief: 	 return key_word value once in which case the button pressed down without relax
 * @param: 	 none
 * @return: 	 return key_word value
 * @details: 	 return key_word value de-bounce by compared twice if equal
 			 can de-bouncd pulse < 60ms = scan rate 
 *-----------------------------------------------------------------------*/
enum KEYWORD adcReadKey()
{	
	static enum KEYWORD key_word_r;
	static enum KEYWORD key_word_bak = NONE_KEY;	// last key_word

	
		if(key_buf == NONE_KEY){
			key_word = NONE_KEY;
			key_word_bak = NONE_KEY;
			key_word_r = NONE_KEY;
		}else if (key_word_r == key_buf)
			key_word = NONE_KEY;
		else if(key_word_bak == NONE_KEY){
			key_word_bak = key_buf;
		 	key_word_r =key_word = NONE_KEY;
		}else if(key_word_bak == key_buf){
	  		key_word_r = key_word = key_buf;
		}else{
			key_word_bak = key_buf;
		 	key_word_r = key_word = NONE_KEY;
		}
		
		return key_word;
		
}

/*-------------------------------------------------------------------------
 * @fucname:	 devTaskDealKey
 * @brief: 	 do test key 
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 press a key to turn the lcd to corresponding color 
 *-----------------------------------------------------------------------*/
// void devTestDealAdcKey()            //ma 20150707   del useless function   
// {
// 	dealTestKey(key_word);
// 	dealTestKeyContinue(key_word_continue);
// }


/*-------------------------------------------------------------------------
 * @fucname:	 devStandbyDealAdcKey
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void devStandbyDealAdcKey()
{
	delStandbytkey(key_word);
}

/*-------------------------------------------------------------------------
 * @fucname:	 devStartDealKey
 * @brief: 	deal the key event in dev start state
 * @param: 	none
 * @return: 	none
 * @details: 	deal the key event in dev start state
 *-----------------------------------------------------------------------*/
void devStartDealAdcKey()
{
	static u32 k_counter = 0;
	
	delStartkey(key_word);
	if(key_word_continue == INC_KEY || key_word_continue == DEC_KEY){
		if(++k_counter >= INC_KEY_COUNTER){
			delStartkey(key_word_continue);
			k_counter = 0;
		}
	}

}


u32 audioDetect()
{
	if( (audio_L>>10) > AUDIO_THRESHOLD ||(audio_R>>10) > AUDIO_THRESHOLD)
		return TRUE;
	else
		return FALSE;

	
}

/*-------------------------------------------------------------------------
 * @fucname:	 adcKeyHook
 * @brief: 	 audio input detect
 * @param: 	 none
 * @return: 	 none
 * @details: 	 audio input detect
 *-----------------------------------------------------------------------*/
extern u32 BT_MATCH_FLAG;         //ma 20150707
adcKeyHook()
{
	static u32 audio_quiet_time = 0; // audio detect for auto into standby mode if no audio input time upto 15 min

	if(DECT_DC()){
		if(auto_standy_delay_minunte == 10)
			audio_quiet_time = 0;
		auto_standy_delay_minunte = 30;
	}else{
		if(auto_standy_delay_minunte == 30)
			audio_quiet_time = 0;
		auto_standy_delay_minunte = 10;
	}


	if(audioDetect()||play_key_flag){               //ma 20150714
		play_key_flag = 0;                      //ma 20150714
		audio_quiet_time = 0; // audio is inputing
		//AMP_OPEN();
	}else {

		if(++audio_quiet_time > auto_standy_delay_minunte*1000*2) 
		{
		//adcKeyHook is call per 30ms.
		//so when no audio input time is 15min then enter standby mode
			if(!DECT_DC())
				enterStandby();
			else if(dc_auto_down_flag == 5)      //ma 20150508 dis/enable  dc auto instandby        //ma 20150707  change  default 5 to enter standby 
			{
					BT_MATCH_FLAG  =0 ;     //ma 20150707   set  bt match flag 
					setConfig();           //ma 20150706    before power ff ,store 
					sysDelayAMs(100);
					GPIO_SET_PIN(BLUE_PIN);	     //MA 20150706   close led 
					GPIO_SET_PIN(GREEN_PIN);
					GPIO_CLR_PIN(RED_PIN);
// 							DrvTIMER_DisableInt(TIMER0);
				 sleep_flag =1;       // ma 20150706  sleep flag      
				 sleep();
		     audio_quiet_time = 0;
		  }
	  }	
		if(audio_quiet_time > 80*2){
			//audio_quiet_time = 0; // audio is inputing
		//	AMP_MUTE();
		}
	}
	audio_R = audio_L = 0;

	
	bt_volage += bt_volage_adc;	// average the battery_volage
	if(++bt_count >=  (1<<BAT_DEC_COUNT)){
		bt_count = 0;
		bt_volage >>= BAT_DEC_COUNT;
		if(systick_counter < 5000){
			battery_state = BATTERY_MIDDLE;
			return;
		}
		/********add battery lower value*****/
// 		if(bt_volage < BATTERY_LOWER_VALUE)
// 			battery_state = BATTERY_LOWER;
// 		else 
			if(bt_volage < BATTERY_LOW_VALUE)
			battery_state = BATTERY_LOW;
		else if (bt_volage < BATTERY_HIGH_VALUE)
			battery_state = BATTERY_MIDDLE;
		else
		{			
			battery_state = BATTERY_HIGH;
			battery_high_flag++;     //ma 20150703   set  battery_high_flag
			if(battery_high_flag>5)  battery_high_flag = 0;
		}
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 keyScan 
 * @brief: 	 scan the key and get key value if has 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get key_word value with  and key_word_continue value
 *-----------------------------------------------------------------------*/
FUNC_Timer_Task adcKeySubHook = NULL;
void adckeyScan()
{
		
	key_word = adcReadKey();
	key_word_continue = adcReadKeyContinue();
	
	switch(dev_state){		
	// the dev_state is init to DEV_START and if the CHANNEL3_KEY pressed at  the dev is power on
	// qithin DEV_START_TIME then the dev state will change to DEV_TEST
		case DEV_START:
			devStartDealAdcKey();
			break;
		case DEV_TEST:
// 			devTestDealAdcKey();	// if dev is in DEV_TEST STATE do it
			break;
		case DEV_STANDBY:
			devStandbyDealAdcKey();
			break;
		default:
			break;
	}
	
	if(dev_state != DEV_STANDBY)
		adcKeyHook();
	if(adcKeySubHook){
		adcKeySubHook();
		adcKeySubHook = NULL;
	}
	startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
}

/*-------------------------------------------------------------------------
 * @fucname:	 devTest
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// extern void showVersion(void);            //ma 20150706   save  aprom    
// void devTest(void)                               //ma 20150707   del useless function   
// {
// 	static u32 dev_test_flag = 0;
// 	static u32 counter;

// 	if(dc_time_stamp){
// 		dev_test_flag = counter = systick_counter;
// 		dc_time_stamp = dev_test_flag;
// 		while(1){
// 			if(	systick_counter < DEV_TEST_TIMER
// 		   	   &&key_buf_adc == CHANNEL_SWITCH_KEY){
// 				// press the inc_key and power key more than 3 sec when power on
// 				if(systick_counter > counter){
// 					counter = systick_counter;
// 					if(++dev_test_flag > 2000 + dc_time_stamp ){
// // 					showVersion();                 //ma 20150706   save  aprom   
// 						testInit();
// 					}
// 				}
// 				sysDelayMs(1);
// 			}else
// 				if(systick_counter > counter){
// 					counter = systick_counter;
// 					if( (systick_counter - dev_test_flag) > 200 )
// 						break;
// 				}
// 		}
// 	}
// }
/*-------------------------------------------------------------------------
 * @fucname:	 adcKeyInit
 * @brief: 	 open adc function and config adc clock, adc mode, enable adc INT
 * @param: 	 none
 * @return: 	 none
 * @details:	 open adc_key_pin adc function and config adc in single  mode,
 *			 enable adc INT
 *-----------------------------------------------------------------------*/
void adcKeyInit()
{
	ADC_KEY_ENABLE;		 // OPEN KEY PIN ADC FUNCTION
	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN_1, GPIO_INPUT);
	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN, GPIO_INPUT);
	GPIO_SET_MODE(ADC_KEY_PORT,BATTERY_V_DECT_PIN,GPIO_INPUT);
	GPIO_SET_MODE(ADC_KEY_PORT,WL_COCO_KEY_PIN,GPIO_INPUT);   //ma 20150424
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, ADC_KEY_PIN);
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, BATTERY_V_DECT_PIN);
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT,WL_COCO_KEY_PIN);   //ma 20150424
// 	ADC_CH_ENABLE(ADC_KEY_MSK);	                                         //ma 20150917   delete some useless functions
// 	ADC_CONVERTER_ENABLE();


// 	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN_1, GPIO_INPUT);
// 	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN, GPIO_INPUT);
// 	GPIO_SET_MODE(ADC_KEY_PORT,BATTERY_V_DECT_PIN,GPIO_INPUT);
// 	GPIO_SET_MODE(ADC_KEY_PORT,WL_COCO_KEY_PIN,GPIO_INPUT);   //ma 20150424
// 	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, ADC_KEY_PIN);
// 	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, BATTERY_V_DECT_PIN);
// 	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT,WL_COCO_KEY_PIN);   //ma 20150424
	ADC_SET_MODE(ADC_SINGLE_SCAN); // SINGLE SCAN MODE
	ADC_CH_ENABLE(ADC_KEY_MSK);	
	ADC_CONVERTER_ENABLE();
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 keyTaskInit
 * @brief: 	 init adc and start a key scan task
 * @param: 	 none
 * @return: 	 none
 * @details: 	 init adc and start a key scan task
 *-----------------------------------------------------------------------*/
extern Timer_ID appkey_id;
void adckeyTaskInit()
{
	adcKeyInit();	// INIT adckey 
#if(ADC_KEY_PDMA_EN == FALSE)
	NVIC_SetPriority (ADC_IRQn, ADC_INT_PRIORITY); 
	DrvADC_EnableInt();
#endif
	
	adc_key_task = getATask();
	appkey_id = getATask();
 	if(adc_key_task != TASK_FULL);
		startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
}

/*-------------------------------------------------------------------------
 * @fucname:	 adcKeyRemap
 * @brief: 	 remap the key
 * @param: 	 adc key
 * @return: 	 none
 * @details: 	 remap the adc key and store in the key_buf, can de-bounce pulse < 8ms
 *-----------------------------------------------------------------------*/
void adcKeyRemap(u16 adcBuf)
{
	// if has a pulse do lrf 
	if(((adcBuf > key_bak)  ? adcBuf - key_bak :  key_bak - adcBuf) > 115){
		key_adc_value = key_irq_index = 0;
		key_bak = adcBuf;
		return;
	}else 
		key_adc_value += adcBuf;// get adc value
		
	key_bak = adcBuf;
	
	if(++key_irq_index ==  (1<<MAX_LRF_KEYS)){	 //remap the adc key 
		key_irq_index = 0;
		key_adc_value >>= MAX_LRF_KEYS;
		if(key_adc_value > 1000 )
			key_buf = NONE_KEY;
		else if(key_adc_value > 860)	 // the min pulse hight 115 = 1000 - 895 , use in lrf
			key_buf = NONE_KEY;
		else if (key_adc_value > 820)
			key_buf = INC_KEY;	// adc_EN, the last risistance,when press the v is max
		else if(key_adc_value > 750)
			key_buf = NONE_KEY;
		else if(key_adc_value > 710)
			key_buf = DEC_KEY;		// adc_source
		else if(key_adc_value > 640)
			key_buf = NONE_KEY;
		else if(key_adc_value > 600)
			key_buf = CHANNEL_SWITCH_KEY;		// adc_vol+
		else if(key_adc_value > 380)
			key_buf = NONE_KEY;
		else if(key_adc_value > 340)
			key_buf = MULTIROOM_KEY;
		else 
			key_buf = NONE_KEY;
		key_buf_adc = key_buf;
		if(GPIO_GET_PIN(POWER_KEY_PORT_PIN))
			key_buf = POWER_KEY;
		key_adc_value = 0;
		
		if(wl_coco_adc < 0xd8 && wl_coco_adc > 0XB8){
			WL_COCO_KEY = WL_COCO_KEY1;
		}else if(wl_coco_adc < 0x1B0  && wl_coco_adc > 0X188 ){
			WL_COCO_KEY = WL_COCO_KEY2;
		}else if(wl_coco_adc < 0x288  && wl_coco_adc > 0X248){
			WL_COCO_KEY = WL_COCO_KEY3;
		}else if(wl_coco_adc < 0x340  && wl_coco_adc > 0X314){
			WL_COCO_KEY = WL_COCO_KEY4;
		}else WL_COCO_KEY = 0;            //ma 20150424
		
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 ADC_IRQHandler
 * @brief: 	 do LRF of adc key and put the pressed key's value in key_buf
 * @param: 	 none
 * @return: 	 none
 * @details: 	 do lrf to adc key and put the pressed key's value in key_buf which has LRF
 *-----------------------------------------------------------------------*/
void ADC_IRQHandler()
{
	
	u16 key_current;
	static u32 soft_mute_counter=0;
// 	static u32 SB_link_counter= 0;      //ma 20150704  add SB  link count
	ADC_CLR_IF(ADC_CONV_DONE_IF); // clean adc int flag
	key_current = ADC_GET_RSLT(ADC_KEY_PIN);
	
	bt_volage_adc  = ADC_GET_RSLT(BATTERY_V_DECT_PIN);
	
	wl_coco_adc =  ADC_GET_RSLT(WL_COCO_KEY_PIN);
		
	adcKeyRemap(key_current);
	
//	if(key_buf_adc != NONE_KEY)	gpio_int =1;    //ma 20150623   sleep status ADCkey wake up
  if(soft_mute_flag == 1)
	{
		soft_mute_counter++;
		if (soft_mute_counter >= 2000)
		{
			soft_mute_flag = 0;
			soft_mute_counter = 0;			
		}					
	}	
		/*********Add sb link counter handle******/
// 	if(SB_link_flag)
// 	{
// 		SB_link_counter++;
// 		if(SB_link_counter>=5000)                          //ma 20150708   change 2000 to 5000
// 		{
// 			SB_link_counter = 0;
// 			if(SB_flag)
// 			{
// // 				Dwhp83CuMuDetect();
// // 				if(Dwhp83_CuMu == MU)
// // 				{
// // 					RX_SB_alive();
// // 				}
// // 				else if(Dwhp83_CuMu == CU)
// // 				{
// // 					TX_SB_alive();
// // 				}
// 			}
// 			else 
// 			{
// 				
// 			}				
// 		}
// 	}
// 	else
// 	{
// 		SB_link_counter = 0;
// 	}
//  /*********Add sb link counter handle End ******/
}




