/****************************************************************************
 * @file:		adcKey.c
 * @version:	V1.00	
 * @breif:	adckey source file	
 * @date: 	2013/2/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define AUDIO_THRESHOLD		0x40

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
u32 auto_standy_delay_minunte = 15;
u32 battery_volage = 0;
u32 dc_time_stamp = 1;
static u32 bt_volage_adc, bt_volage, bt_count = 0;
u32 audio_quiet_time = 0; // audio detect for auto into standby mode if no audio input time upto 15 min
static  u32 audio_open_time = 1;
extern u32 soft_mute;
extern u8 ad83586_volume_tbl[];
extern u8 w3d_sub_tbl[];
extern u32 amp_open;
FUNC_Timer_Task adcKeySubHook = NULL;
/*-------------------------------------------------------------------------
 * @fucname:	 AdcInit
 * @brief: 	 adc clk init 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 adc clk init
 *-----------------------------------------------------------------------*/
void adcKeyClkInit()
{
	SET_ADC_CLKSRC(CLK_ADC_HXT);
	SET_ADC_CLKDIV(ADC_CLK_PRESCALE);
	APBCLK_ENABLE(CLK_ADC);
	SYS_RESET_IP2(SYS_RST_ADC);
	
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

	if(key_buf == CHANNEL_SWITCH_KEY)
		key_word_continue = CHANNEL_SWITCH_KEY;
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
// 	static u32 key_flag_count = 0;
	
		
// 		if(!GPIO_GET_PIN(POWER_KEY_PORT_PIN))
// 			key_buf = POWER_KEY;
	
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
		if(key_buf == POWER_KEY)
			key_word =  key_buf;
		return key_word;
}

/*-------------------------------------------------------------------------
 * @fucname:	 devTaskDealKey
 * @brief: 	 do test key 
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 press a key to turn the lcd to corresponding color 
 *-----------------------------------------------------------------------*/
// void devTestDealAdcKey()
// {
// 	static u32 k_counter0 = 0;
// 		
// 	dealTestKey(key_word);
// 	if(key_word_continue == INC_KEY || key_word_continue == DEC_KEY){
// 		if(++k_counter0 >= INC_KEY_COUNTER){
// 			delStartkey(key_word_continue);
// 			k_counter0 = 0;
// 		}
// 	}
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
extern u32 BT_MATCH_FLAG;
extern u32 bt_check_flag;
void devStartDealAdcKey()
{
	static u32 k_counter0 = 0;

	delStartkey(key_word);
	if(key_word_continue == INC_KEY || key_word_continue == DEC_KEY){
		if(++k_counter0 >= INC_KEY_COUNTER){
			delStartkey(key_word_continue);
			k_counter0 = 0;
		}
	}

}


u32 audioDetect()
{
	u32 audio_threshold;

	if(amp_volume < 26)
		audio_threshold = AUDIO_THRESHOLD;
	else if(amp_volume < 29)
		audio_threshold = AUDIO_THRESHOLD+(AUDIO_THRESHOLD>>1);
	else
		audio_threshold = (AUDIO_THRESHOLD<<1);

	if(dev_input == BLUETOOTH && BT_MATCH_FLAG == 0)
		audio_threshold = AUDIO_THRESHOLD*2;
	
	if( (audio_L>>10) > audio_threshold ||(audio_R>>10) > audio_threshold)
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
adcKeyHook()
{

	
// 	if(DECT_DC()){
// 		if(auto_standy_delay_minunte == 10)
// 			audio_quiet_time = 0;
// 		auto_standy_delay_minunte = 30;
// 	}else{
// 		if(auto_standy_delay_minunte == 30)
// 			audio_quiet_time = 0;
// 		auto_standy_delay_minunte = 10;
// 	}

	
	if(audioDetect() || dev_state == DEV_TEST){  
		if(--audio_open_time == 0){
			audio_open_time = 1;
			if(soft_mute == FALSE && amp_open == 0 ){
	//			ad83586SetMute(0);
	//			ad83586SendCmd();               
	//			sysDelayMs(1);
			}
			audio_quiet_time = 0; // audio is inputing
		} 
	}else { 
		
		if(++audio_quiet_time > auto_standy_delay_minunte*1000*2) {
		//adcKeyHook is call per 30ms.
		//so when no audio input time is 15min then enter standby mode
			if(auto_standy_delay_minunte == 10)
				logEvent(No_audio_present_for10_mins);
			else
				logEvent(No_audio_present_for30_mins);
			
			enterStandby();    //ma  20160727  add 10 mins no sound enter power saving mode
			audio_quiet_time = 0;
		}
				
		if( audio_quiet_time > 33*3 ){ 
			audio_open_time = 1;
			if(amp_open == 1){
	//			dspInit();
	//			ad83586SetMute(1);
	//			ad83586SendCmd(); 
	//			sysDelayMs(1);
			}
		}
	}
	if((0 == WB_paired_flag) && (BLUETOOTH == dev_input))
	{
		if(++led_saving_couter > BT_LED_PARING_TIME*1000*2)
		{
			led_saving_power_flag = 1;
			led_saving_couter--;
		}
		else 
		{
			led_saving_power_flag = 0;
		}
	}
	else
	{
		if(++led_saving_couter > LED_SAVING_TIME*1000*2)
		{
			led_saving_power_flag = 1;
			led_saving_couter--;
		}
		else 
		{
			led_saving_power_flag = 0;
		}
	}
	audio_R = audio_L = 0;
	audio_detect_L = audio_detect_R = 0;
	
	bt_volage += bt_volage_adc;	// average the battery_volage
	if(++bt_count >=  (1<<BAT_DEC_COUNT)){
		bt_count = 0;
		bt_volage >>= BAT_DEC_COUNT;
		if(bt_volage < BATTERY_LOW_VALUE)
			battery_state = BATTERY_LOW;
		else if (!GPIO_GET_PIN(CHARGE_DONE_PORT_PIN))                //ma 20160224 diffrence IC
			battery_state = BATTERY_MIDDLE;
		else 
			battery_state = BATTERY_HIGH;
	}

}

/*-------------------------------------------------------------------------
 * @fucname:	 keyScan 
 * @brief: 	 scan the key and get key value if has 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get key_word value with  and key_word_continue value
 *-----------------------------------------------------------------------*/
u32 device_subt = 0;
void adckeyScan()
{
	static u32 device_subt_b;
//   u8 tp_touch_counter = 0;
// 	u8 tp_touch_value = 0;
	key_word = adcReadKey();
// 	if(TP_IRQ())
// 	{
// 		writeCAP8298(0x00,0x0);	
// 		while(++tp_touch_counter<20)
// 		{
// 			sysDelayMs(20);
// 			if(TP_IRQ())
// 			{
// 				tp_touch_value = readCAP8298(0x03);
// 				switch(tp_touch_value)
// 				{
// 					case 0:
// 					key_word = NONE_KEY;					
// 					break;
// 					case 1:
// 					key_word = DEC_KEY;					
// 					break;
// 					case 2:
// 					key_word = INC_KEY;					
// 					break;
// 					case 4:
// 					key_word = CHANNEL_SWITCH_KEY;					
// 					break; 
// 					case 8:
// 					key_word = GAME_KEY;					
// 					break;
// 					default:
// 					key_word = NONE_KEY;					
// 					break;
// 				}
// 				writeCAP8298(0x00,0x0);	
// 				break;
// 			}
// 		}
// 		if(tp_touch_counter > 0 && key_word == CHANNEL_SWITCH_KEY)
// 		{
// 				sysDelayMs(350);
// 				tp_touch_value = readCAP8298(0x03);
// 				switch(tp_touch_value)
// 				{
// 					case 0:				
// 					break;
// 					case 1:
// 					sysDelayMs(20);
// 					while(tp_touch_counter<18)
// 					{	
// 						writeCAP8298(0x00,0x0);	
// 						sysDelayMs(350);						
// 						if(TP_IRQ())
// 						{
// 							tp_touch_counter++;
// 						}
// 						else
// 						{
// 							break; 
// 						}
// 					}	
// 					if(18 == tp_touch_counter && dev_input == BLUETOOTH)		
// 					{
// // 						if(WB_paired_back_flag)
// 								WB_enter_pairing();
// 							writeCAP8298(0x00,0x0);	
//               playMusic(DISCONECTED_SOUND);
// 							WB_paired_back_flag = 0;
// 							WB_paired_flag = 0;
// 							sysDelayMs(2000);
// // 						else WB_nolink_pairing();
// 							key_word = NONE_KEY;	
// 						tp_touch_counter = 0;
// 					}
// 				
// 					break;
// 					case 2:			
// 					break;
// 					case 4:					
// 					break; 
// 					case 8:			
// 					break;
// 					default:				
// 					break;
// 				}
// // 				key_word = NONE_KEY;
// // 				writeCAP8298(0x00,0x0);	
// 		}
// 	}
	key_word_continue = adcReadKeyContinue();

// 	if(GPIO_GET_PIN(POWER_KEY_PORT_PIN))
// 			key_word = POWER_KEY;

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

	if(device_subt_b != device_subt && systick_counter > 5000){
		device_subt_b = device_subt;
		

		if(device_subt == VOLUME_R)
			ampSetR();
		else if(device_subt == VOLUME_L)
			ampSetL();
		else
			ampSetS();
	}
	
	startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
}

/*-------------------------------------------------------------------------
 * @fucname:	 DEVtest
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
 extern void showVersion(void);
void DEVtest()
{
// 	static u32 dev_test_flag = 0;
// 	static u32 counter;
// 	u32 ioi = 0;

// 	if(dc_time_stamp){
// 		dev_test_flag = counter = systick_counter;
// 		dc_time_stamp = dev_test_flag;
// 		while(1){
// 			if(	systick_counter < DEV_TEST_TIMER &&
// 		   		key_buf_adc == DEC_KEY &&
// 		  		GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
// 		  		ioi = 1;
// 				// press the inc_key and power key more than 3 sec when power on
// 				if(systick_counter > counter){
// 					counter = systick_counter;
// 					if(++dev_test_flag > 3000 + dc_time_stamp )
// 						showVersion();
// 					
// 				}
// 			}else
// 				if(systick_counter > counter){
// 					counter = systick_counter;
// 					if( (systick_counter - dev_test_flag) > 800 || ioi == 1)
// 						break;
// 				}
// 		}
// 	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 devTest
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u32 power_delay = 0;
extern void bootLDROM(void);
void devTest(void)
{
// 	static u32 dev_test_flag = 0;
// 	static u32 counter;
// 	u32 stamp;

// 		dev_test_flag = counter = systick_counter;
// 		stamp = dev_test_flag;
// 		power_delay = 0;
// 		while(1){
// 			sysDelayMs(1);
// 			if(GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
// 				// press the inc_key and power key more than 3 sec when power on
// 				if(key_buf != DEC_KEY && key_buf != INC_KEY ){
// 					if(systick_counter > counter){
// 						counter = systick_counter;
// 						if(++dev_test_flag > 1500 + stamp ){
// 							Dwhp83EnterPairing();
// 							led_flash = 4;
// 							break;
// 						}
// 					}
// 				}else if(key_buf == DEC_KEY){
// 					if(systick_counter > counter){
// 						counter = systick_counter;
// 						if(++dev_test_flag > 1550 + stamp ){
// 							testInit();
// 							break;
// 						}
// 					}
// 				}else if(key_buf == INC_KEY){
// 					if(systick_counter > counter){
// 						counter = systick_counter;
// 						if(++dev_test_flag > 1550 + stamp ){
// 							bootLDROM();
// 							break;
// 						}
// 					}
// 				}
// 			}else
// 				if(systick_counter > counter){
// 					counter = systick_counter;
// 					if( (systick_counter - dev_test_flag) > 200 )
// 						break;
// 				}
// 		}

// 		power_delay = systick_counter - stamp;
		
}
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
	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN, GPIO_INPUT);
	GPIO_SET_MODE(ADC_KEY_PORT,BATTERY_V_DECT_PIN,GPIO_INPUT);
// 	GPIO_SET_MODE(ADC_KEY_PORT,1,GPIO_INPUT);
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, ADC_KEY_PIN);
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, BATTERY_V_DECT_PIN);
// 	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, 1);
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
void adckeyTaskInit()
{
	adcKeyInit();
#if(ADC_KEY_PDMA_EN == FALSE)
	NVIC_SetPriority (ADC_IRQn, ADC_INT_PRIORITY); 
	DrvADC_EnableInt();
#endif
	
	adc_key_task = getATask();
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
u16 device_type_adc1 = 0;
u16 device_type_adc2 = 0;
u16 device_type_adc = 0;
extern u8 dwhp83_type;
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
	device_type_adc2 += device_type_adc1;
	
	if(++key_irq_index ==  (1<<MAX_LRF_KEYS)){	 //remap the adc key 
		key_irq_index = 0;
		key_adc_value >>= MAX_LRF_KEYS;
		device_type_adc = device_type_adc2>>MAX_LRF_KEYS;
		device_type_adc2 = 0;
		if(device_type_adc < 0xd8 && device_type_adc > 0XB8){
			dwhp83_type = VOLUME_C;
			device_subt = VOLUME_L;
		}else if(device_type_adc < 0x1B0  && device_type_adc > 0X188 ){
			dwhp83_type = VOLUME_C;
			device_subt = VOLUME_C;
		}else if(device_type_adc < 0x288  && device_type_adc > 0X248){
			dwhp83_type = VOLUME_C;
			device_subt = VOLUME_R;
		}else if(device_type_adc < 0x340  && device_type_adc > 0X314){
			dwhp83_type = VOLUME_STREO;	                //ma 20150526 no adc 
			device_subt = VOLUME_C;
		}
			if(key_adc_value > 1000 )
			key_buf = WRONG_KEY;
			
		else if(key_adc_value > 865)
			key_buf = WRONG_KEY;
		else if(key_adc_value > 825)
		{
			key_buf = MUSIC_KEY;
		}
		else if(key_adc_value > 760)	 // the min pulse hight 115 = 1000 - 895 , use in lrf
			key_buf = WRONG_KEY;
		else if (key_adc_value > 720)
		{
			key_buf = INC_KEY;     

		}
		else if(key_adc_value > 660)
			key_buf = WRONG_KEY;
		else if(key_adc_value > 620)
		{
			key_buf = DEC_KEY;
		}

		else if(key_adc_value > 555)
			key_buf = WRONG_KEY;
		else if(key_adc_value > 515)
		{
			key_buf = GAME_KEY;
		}

		else if(key_adc_value > 380)
			key_buf = WRONG_KEY;
		else if(key_adc_value > 340)
		{ 
			key_buf = CHANNEL_SWITCH_KEY;         // change key map 
		}
		else if(key_adc_value == 0)
		{ 
			key_buf = POWER_KEY;         // change key map 
		}
		else 
			key_buf = WRONG_KEY;
		key_adc_value = 0;
		key_buf_adc = key_buf;
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
	
	ADC_CLR_IF(ADC_CONV_DONE_IF); // clean adc int flag
	key_current = ADC_GET_RSLT(ADC_KEY_PIN);
	
	bt_volage_adc  = ADC_GET_RSLT(BATTERY_V_DECT_PIN);

	device_type_adc1 = 0;
		
	adcKeyRemap(key_current);
}




