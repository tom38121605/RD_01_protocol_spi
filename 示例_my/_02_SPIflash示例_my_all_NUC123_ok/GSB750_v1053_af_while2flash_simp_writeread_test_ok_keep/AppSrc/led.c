/****************************************************************************
 * @file:		led.c
 * @version:	V1.00
 * @breif:	Led C source file. 
 * @date: 	2013/2/23
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define CALL_STATE_BLUETOOTH	0x1
#define CALL_STATE_LINEIN		0x2
#define CALL_STATE_24wireless	0x3

enum LED_COLOR state_color = BLUETOOTH_COLOR, state_color_back = DARK_COLOR;	// the color of the state
Timer_ID led_task = TASK_FULL;	// led task id
u32 led_flash = 0;
u32 led_battery_flash = 0;
u32 bt_hang_up_signal = FALSE;									//	CN        CM     PRESCALE  DIV	MODE	INV
S_DRVPWM_TIME_DATA_T color_pwm_clc = {1000,    1000,	  12,	           4,		1,		0};
u16 RGB_brightness[] = {
     			    1000,	//  index = 0
     			    900, //   index = 1
     			    800, //   index = 2
     			    700, //   index = 3
     			    600, //   index = 4
     			    500, //   index = 5
     			    400, //   index = 6
     			    300, //   index = 7
     			    200, //   index = 8
     			    100, //   index = 9
     			    0, //   index = 10
};
u32 BAT_STATE_DECT_DELAY_COUNT = 0;
u32 BT_MATCH_FLAG = 0;
u32 volume_key_delay = 0; 
u32 BT_FIRST_START = 0, ccount = 0;
extern  u32 amp_volume_back;
extern u8 pt2314_switch_remap[];
extern u8 amp_volume_tbl[];
extern Timer_ID log_id;
extern Log_Type event;
u8 NFC_flag = 0;      //ma 20160223  add NFC function 
/*-------------------------------------------------------------------------
 * @fucname:	 ledInit
 * @brief: 	 init the  pin to pwm function
 * @param: 	 none
 * @return: 	 none
 * @details: 	 init the pin to pwm function
 *-----------------------------------------------------------------------*/
void timerInit()
{
	SET_TMR0_CLKSRC(CLK_TMR0_HXT);  
	DrvTIMER_Init(TIMER0, 21, 10, TIM_MODE_TOGGLE);
	NVIC_SetPriority (TMR0_IRQn, ADC_INT_PRIORITY); 
    	DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);
    	TIM_ENABLE(TIMER0);
}

void ledInit() 
{
/*
	// PA12/13/14  function is PWM
	SYS->GPA_MFP |= RLED_PIN_PWM | GLED_PIN_PWM |BLED_PIN_PWM; 
	DrvPWM_SetClk(RED_PWM, &color_pwm_clc);	// SET PWM0 CLC
	DrvPWM_SetClk(GREEN_PWM, &color_pwm_clc);	// SET PWM1 CLC
	DrvPWM_SetClk(BLUE_PWM, &color_pwm_clc);	// SET PWM2 CLC
	PWMA_POE_ENABLE(RLED_POE |GLED_POE |BLED_POE) ;  // PWM OUT ENABLE
	PWMA->PCR |= RLED_PWM_EN |GLED_PWM_EN | BLED_PWM_EN;  // PWM TIMER START UP
	//ledColorSet(state_color);    //Led  init to default color
*/
	GPIO_CLR_PIN(RED_LED_PORT_PIN);
	GPIO_CLR_PIN(GREEN_LED_PORT_PIN);
	GPIO_CLR_PIN(BLUE_LED_PORT_PIN);
	GPIO_SET_MODE(RED_LED_PORT,    RED_LED_PIN,    GPIO_OUTPUT);
	GPIO_SET_MODE(GREEN_LED_PORT,  GREEN_LED_PIN,  GPIO_OUTPUT);
	GPIO_SET_MODE(BLUE_LED_PORT,   BLUE_LED_PIN,   GPIO_OUTPUT);
	ledColorSet(DARK_COLOR);
	timerInit();
	
}


U32 R_C = 0, B_C = 0, G_C = 0; 
U32 pwtable[21] = {0,2,3,4,6,9,12,15,18,24,30,39,48,62,78,95,116, 131,150, 170,200};
void TMR0_IRQHandler(void)
{
	static u32 ptimer = 0;
// 	u32 i;
	
	 TIM_CLEAR_INTERRUPT_FLAG(TIMER0);
	 

	if(++ptimer > 200)
		ptimer = 0;
	

	if(ptimer < pwtable[R_C])
		GPIO_SET_PIN(RED_LED_PORT_PIN);	
	else
		GPIO_CLR_PIN(RED_LED_PORT_PIN);

	if(ptimer < pwtable[B_C])
		GPIO_SET_PIN(BLUE_LED_PORT_PIN);	
	else
		GPIO_CLR_PIN(BLUE_LED_PORT_PIN);

	if(ptimer < pwtable[G_C])
		GPIO_SET_PIN(GREEN_LED_PORT_PIN);	
	else
		GPIO_CLR_PIN(GREEN_LED_PORT_PIN);
	
}

static u32 fade_color;
static u32 ck = 0;
u32 breath_flag = 0xff;

void fadeon( u32 color)
{
	ck = 0;
	fade_color = color;
	breath_flag = 6;
}

void fadeoff( u32 color)
{
	ck = 20;
	fade_color = color;
	breath_flag = 7;
}

void breathingColor()
{
	static u32 cc = 0;
// 	static u32 breath_bak = 0xff;
	u32 b = breath_flag;

	if((++cc & 0x1))
breathing_s:
		switch(b){
			case 1:	// breath blue
				R_C= 0; G_C = 0; 
				ck++;
				if(ck > 19)
					B_C = 40-ck;
				else
					B_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case 2:	// breath read
				B_C= 0; G_C = 0; 
				ck++;
				if(ck > 19)
					R_C = 40-ck;
				else
					R_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case 3: // breath green
				B_C= 0; R_C = 0; 
				ck++;
				if(ck > 19)
					G_C = 40-ck;
				else
					G_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case 4:	// breath yellow
				B_C = 0;
				ck++;
				if(ck > 19)
					G_C = 40-ck;
				else
					G_C = ck;
				if(ck > 39)
					ck = 0;
				R_C= G_C;
				break;
			case 5:	// breath white
				ck++;
				if(ck > 19)
					B_C = 40-ck;
				else
					B_C = ck;
				if(ck > 39)
					ck = 0;
				R_C= G_C = B_C;
				break;
			case 6: // fades on
				if(ck != 20){
					b = fade_color;
					goto breathing_s;
				}else{
					breath_flag = 0xff;
				}
				break;
			case 7: // fades off
				if(ck != 0){
					b = fade_color;
					goto breathing_s;
				}else
					breath_flag = 0xff;
				break;
			default:
				break;
		}
}
/*-------------------------------------------------------------------------
 * @fucname:	 leddisable
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void leddisable()
{
	SYS->GPA_MFP &= ~(GLED_PIN_PWM |BLED_PIN_PWM |RLED_PIN_PWM);
	PWMA_POE_DISABLE(GLED_POE |BLED_POE |RLED_POE) ;  // PWM OUT ENABLE
	GPIO_CLR_PIN(RED_LED_PORT_PIN);
	GPIO_CLR_PIN(GREEN_LED_PORT_PIN);
	GPIO_CLR_PIN(BLUE_LED_PORT_PIN);
	GPIO_SET_MODE(RED_LED_PORT,   RED_LED_PIN,   GPIO_OUTPUT);
	GPIO_SET_MODE(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_OUTPUT);
	GPIO_SET_MODE(BLUE_LED_PORT,  BLUE_LED_PIN,  GPIO_OUTPUT);
}

/*-------------------------------------------------------------------------
 * @fucname:	 ledColorSet
 * @brief: 	set led color
 * @param:    led color 
 * @return: 	none
 * @details: 	ser led color
 *-----------------------------------------------------------------------*/
void ledColorSet(enum LED_COLOR color)
{
	
	switch(color){
		case DARK_COLOR:
							R_C= 0; G_C = 0; B_C = 0;	
							break;
		case RED_COLOR:	
							R_C = 16; G_C = 0; B_C = 0;
							break;
		case BLUE_COLOR:
							R_C = 0; G_C = 0; B_C = 20;
							break;
		case GREEN_COLOR:
							R_C = 0; G_C = 10; B_C = 0;
							break;
		case SOE_COLOR:
							R_C = 10; G_C = 5; B_C = 10;
							break;
		case LIGHT_BLUE_COLOR:
							R_C = 0; G_C = 10; B_C = 10;
							break;
		case BLUETOOTH_COLOR:
							R_C = 0; G_C = 0; B_C = 20;
							break;
		case YELLOW_COLOR:
							R_C = 16; G_C = 18; B_C = 0;
							break;					
		case PURPLE_COLOR:
							R_C = 20; G_C = 0; B_C = 20;
							break;
		case WHITE_COLOR:
							R_C = 13; G_C = 18; B_C = 20;
							break;

		default:
				return;
	}
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 btHangUpSignalProcess
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static void btHangUpSignalProcess(void)
{
// 	GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
	ledTask();
}

/*-------------------------------------------------------------------------
 * @fucname:	 ledTask
 * @brief: 	 led timed task
 * @param: 	 none
 * @return: 	 none
 * @details: 	 flash led per 1sec
 *-----------------------------------------------------------------------*/
extern u32 dc_flag;
extern u32 bt_check_flag;
extern Timer_ID pt2314_id;
extern DWHP83_STATE dwhp83_status;
extern void btCheckDone(void);
void ledTask()
{
	static u32 amp_i2c_error_counter = 0;
	static u32 amp_i2c_check_counter = 0;
	static u32 bt_match_times = 0;
	static U32 BT_CALL_FLAG = 0, led_cc = 1, ccj = 0,ccv = 0;  //ma 20150522  add ccv  to sync volume
// 	u32 i;

/*	
	if(dev_state != DEV_STANDBY){
		if(DECT_DC()){
			if(dc_flag == 0){
				ampSendCmd_38(0x12,0x54);
				ampSendCmd_38(0x13,0x15);
				ampSendCmd_38(0x14,0x54);
				ampSendCmd_38(0x15,0x15);
				ampSetL1ATRT(0X26);
				dc_flag = 1;
				ampSetVolume(amp_volume);
			}
		}else{
			if(dc_flag){
				ampSendCmd_38(0x12,0x54);
				ampSendCmd_38(0x13,0x15);
				ampSendCmd_38(0x14,0x54);
				ampSendCmd_38(0x15,0x15);
				ampSetVolume(amp_volume);
				dc_flag = 0;
			}
		}
	}
*/

	if(dev_state == DEV_TEST)
		goto L1;
	
	amp_i2c_check_counter ++;
	if(amp_i2c_check_counter == 100)
	{
		if(0 == usb_i2c_enable)
		{
			if(dev_state == DEV_START)
			{
				if(ampGetStatus())
					reset_en = 1;
			}
		}
		amp_i2c_check_counter = 0;
	}

	if(amp_i2c_status_check() >0)
	{
		amp_i2c_error_counter++;
	}
	else
	{
		amp_i2c_error_counter = 0;
	}
	if(amp_i2c_error_counter >35)
	{
		amp_i2c_error_counter = 0;
		reset_en = 1;
	}
	
	if(WB_paired_flag == 1)
	{
		if(WB_paired_back_flag != WB_paired_flag)
		{
// 				iputs("BT ok!");
			playMusic(PAIRED_SOUND);
			WB_paired_back_flag = WB_paired_flag;
			BAT_STATE_DECT_DELAY_COUNT = 0;
		}
	}
	else 
	{
		if(WB_paired_back_flag != WB_paired_flag)
		{
// 				iputs("BT not ok!");
			playMusic(DISCONECTED_SOUND);
			WB_paired_back_flag = WB_paired_flag;
		}
	}
	
  if(bt_play_music_before != bt_play_music_flag)
	{
		bt_play_music_change_delay++;
		if(60 == bt_play_music_change_delay)
		{
			bt_play_music_change_delay = 0;
			bt_play_music_before = bt_play_music_flag;
		}
	}		
	if(bt_play_music_before && WB_paired_flag)      //add source change 
	{
		if(dev_input == LINEIN  || (dev_input == WIRELESS && wireless_mode_open == 0))
		{
			ampMute();
			change_source_mute = 1;
			change_source_counter = 0;
			dev_input = BLUETOOTH;
			state_color = input_channel_color[dev_input];
			ledColorSet(state_color);
			GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
			GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
			sysDelayMs(500);
			unMute();          //ma   20160406
			sysDelayMs(20);
			led_saving_couter = 0;
			led_saving_power_flag = 0;
			BAT_STATE_DECT_DELAY_COUNT = 0;
		}
		
	}
	else 
	{
		if(dev_input == BLUETOOTH|| (dev_input == WIRELESS && wireless_mode_open == 0))
		{
			if(AUX_DETECT()&& (WB_NO_CALL == WB_call_status))
			{
				ampMute();
				change_source_mute = 1;
				change_source_counter = 0;
				dev_input = LINEIN;
				state_color = input_channel_color[dev_input];
				ledColorSet(state_color);
				GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
				GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
				sysDelayMs(500);
				unMute();          //ma   20160406
				sysDelayMs(20);
				led_saving_couter = 0;
				led_saving_power_flag = 0;
			}
			else if(dev_input == WIRELESS && wireless_mode_open == 0)
			{
				ampMute();
				change_source_mute = 1;
				change_source_counter = 0;
				dev_input = BLUETOOTH;
				state_color = input_channel_color[dev_input];
				ledColorSet(state_color);
				GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
				GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
				sysDelayMs(500);
				unMute();          //ma   20160406
				sysDelayMs(20);
				led_saving_couter = 0;
				led_saving_power_flag = 0;
			}
		}
		if(dev_input == LINEIN)
		{
			if((!AUX_DETECT()) || (WB_CALLING == WB_call_status || WB_CALL_COME == WB_call_status))
			{
				ampMute();
				change_source_mute = 1;
				change_source_counter = 0;
				dev_input = BLUETOOTH;
				state_color = input_channel_color[dev_input];
				ledColorSet(state_color);
				GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
				GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
				sysDelayMs(500);
				unMute();          //ma   20160406
				sysDelayMs(20);
				led_saving_couter = 0;
				led_saving_power_flag = 0;
			}
		}
	}	
	
	if(change_source_mute == 1)
	{
		change_source_counter++;
	}
	if(change_source_counter> 10)
	{
		change_source_mute = 0;
	}
// 	if(state_color == PURPLE_COLOR)
// 	{
// 		if(DECT_NFC())
// 		{
// 			if(NFC_flag == 0)
// 			{
// 				changeInputChannel();
// 				NFC_flag = 1;
// 			}
// 		}
// 	}
// 	else if(state_color == BLUETOOTH_COLOR)
// 	{
// 		if(DECT_NFC())
// 		{
// 			if(NFC_flag == 0)
// 			{
// 				WB_enter_pairing();
// 				NFC_flag = 1;
// 			}

// 		}

// 	}
// 	else
// 	{
// 		if(DECT_NFC())
// 		{
// 			if(NFC_flag == 0)
// 			{
// 				changeInputChannel();
// 				NFC_flag = 1;
// 			}
// 		}
// 	}
	if(!DECT_NFC())
	{
		NFC_flag = 0;
	}
	else NFC_flag = 1;
	
			
	if(Dwhp83CuMuDetect())     // ma 20150522
	{
		if(ccv == 0)
		{
				DWP83CuCV();
				ccv = 1;
		}
	}
	else 
	{
			ccv = 0;
	}	
	
	if(state_color == LIGHT_BLUE_COLOR & DECT_CALL()){

			if(bt_hang_up_signal == TRUE){
				bt_hang_up_signal = FALSE;
			bt_match_times = 10;
// 				GPIO_SET_PIN(BT_HANG_UP_PORT_PIN);
				playMusic(PAIRED_SOUND);
				logEvent(call_pickup);
				startATask(led_task, BT_HANGUP_PULSE_WIDRH, 0, btHangUpSignalProcess);
				return;
			}
	}

	
	
	if(dev_input == WIRELESS){
		
		if(volume_key_delay !=0)
			volume_key_delay--;
		else{
			if(!isDwhp83Linked() && ++ccj> 5){
				led_flash = 3;
				ccj = 0;
			}
		}	
		if(DECT_NFC()){
			state_color = BLUETOOTH_COLOR;
			ledColorSet(BLUETOOTH_COLOR);
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			led_flash = 0;
		}
		if(1 == call_eq_mode)         
		{
			ampSendEqs_38(0,63);
			call_eq_mode = 0;
		}
	}else if(dev_input == BLUETOOTH)
	{
	 		if(DECT_NFC())
			{
				if(WB_paired_flag == 1)
				{
					if(NFC_flag == 0)
					{
						WB_enter_pairing();
						NFC_flag = 1;
					}
				}
			}
// 		ccp = cck = 0;
// 		if(DECT_CALL()){
// 			//ampEQbypass();
// 			DWHP83Mute();
// 			ampMuteBass();
// 			logEvent(call_in);
// 			state_color = LIGHT_BLUE_COLOR;
// 			ledColorSet(state_color);
// 			BT_CALL_FLAG = CALL_STATE_BLUETOOTH;  // call coming when in bluetooth state
// 			amp_volume_back = amp_volume; // back the amp_volume
// 			amp_volume = VOLUME_CALL;
// 			changeInputVolumeUp();
// 		}

// 		
// 		if(volume_key_delay==0){	
// 			if(0){
// 				led_flash = 0; // IF BT_LED DEC IS HIGH DARK THE LED
// 				bt_match_times = 0;
// 				if(bt_unmatch_times < 20)
// 					bt_unmatch_times++;
// 				else{
// 					if(BT_MATCH_FLAG){
// 						logEvent(bt_disconnect);
// // 						playMusic(DISCONECTED_SOUND);
// 					}else
// 						if(++ccount > 100){
// // 							playMusic(PAIRING_SOUND);
// 							ccount = 0;
// 					}
// 										
// 					BT_MATCH_FLAG = 0;
// 					if(systick_counter > 2000 && ++bt_cc > 60){
// 						bt_cc = 0;
// // 						GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
// // 						BT_POWER_OFF();
// // 						sysDelayMs(200);
// // 						BT_POWER_ON();
// // 						sysDelayMs(300);
// 						bt_check_flag = 1;
// // 						GPIO_SET_PIN(BT_HANG_UP_PORT_PIN);
// 						startATask(pt2314_id, 1500, 0, btCheckDone);
// 					}
// 				}
// 			}
// 			else {
// 				bt_cc = 0;
// 				if(bt_match_times < 20){
// 					bt_match_times++;
// // 					led_flash = 1; // IF BT_LED DEC IS LOW LIGHT THE LED
// 				}
// 				else if(BT_MATCH_FLAG == 0){
// 					BT_MATCH_FLAG = 1;
// 					logEvent(bt_paired);
// // 					playMusic(PAIRED_SOUND);
// 					bt_unmatch_times = 0;
// 					ccount = 0;
// 					if(BT_FIRST_START){
// 						BT_FIRST_START = 0;
// 						//GPIO_CLR_PIN(PC13);
// 						//sysDelayMs(600);
// 						//GPIO_SET_PIN(PC13);
// 					}
// 				}
// 			}
// 		}
// 		else
// 			volume_key_delay--;
			if((!WB_paired_flag) || (WB_CALL_COME == WB_call_status)){
				if(bt_match_times > 0){
					bt_match_times --;
				}else{
					if(!WB_paired_flag)
					{						
						bt_match_times = 8;
						led_flash = 5;
					}
					else 
					{
						bt_match_times = 6;
						led_flash = 3;
					}
				}
			}
			if(WB_call_status != WB_NO_CALL)
			{
				led_saving_couter = 0;
				led_saving_power_flag = 0;
				if(1 == call_eq_mode)
				{
					ampSendEqs_38(0,63);
					call_eq_mode = 0;
				}
			}
			else if(WB_call_status  == WB_CALLING)
			{
				if(0 == call_eq_mode)
				{
					ampSendCallEqs_38(0,63);
					call_eq_mode = 1;
				}
			}
	}else if( dev_state != DEV_STANDBY ){
		if(DECT_CALL() && state_color != LIGHT_BLUE_COLOR){
			//ampEQbypass();
			ampMuteBass();
			
			state_color = LIGHT_BLUE_COLOR;
			ledColorSet(state_color);
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			if(dev_input == LINEIN)
				BT_CALL_FLAG = CALL_STATE_LINEIN; // call coming when in line in state
			else
				BT_CALL_FLAG = CALL_STATE_24wireless;
			amp_volume_back = amp_volume; // back the amp_volume
			amp_volume = VOLUME_CALL;
		}else if(DECT_NFC()){
			state_color = BLUETOOTH_COLOR;
			ledColorSet(BLUETOOTH_COLOR);
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			led_flash = 0;
		}
		if(1 == call_eq_mode)         
		{
			ampSendEqs_38(0,63);
			call_eq_mode = 0;
		}
	}else if(DECT_NFC()){
// 		if(dev_input == LINEIN){
// 			dev_input = BLUETOOTH;
// 			pt2314_status.audio_switch = pt2314_switch_remap[(u8)pt2314_input_table[dev_input]];
// 		}
// 		outStandby();
	}

		
	if((BT_CALL_FLAG != FALSE) && !DECT_CALL() ){

		logEvent(call_hangup);
		//ampEQon();
		DWHP83UnMute();
		ampUnMuteBass();
		
		if(BT_CALL_FLAG == CALL_STATE_LINEIN){
			dev_input = BLUETOOTH;
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			amp_volume = amp_volume_back;
		}
		else if(BT_CALL_FLAG == CALL_STATE_24wireless){
			dev_input = LINEIN;
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			amp_volume = amp_volume_back;
		}
		else{
			state_color = BLUETOOTH_COLOR;
			ledColorSet(state_color);
			changeInputVolumeDown(VOLUME_RECOVER);
		}

		BT_CALL_FLAG = FALSE;
	}
		
// 	if(DECT_NFC() || (state_color == BLUETOOTH_COLOR && BT_MATCH_FLAG == 0))
// 		led_battery_flash = BAT_STATE_DECT_DELAY_COUNT = 0;
	
// 	if(DECT_DC())
// 		BAT_STATE_DECT_DELAY_COUNT++;
	if(++BAT_STATE_DECT_DELAY_COUNT > BATTERY_SHOW_INFO_TIME){
		BAT_STATE_DECT_DELAY_COUNT = 0;
		led_battery_flash = 28;
		if(++led_cc>1)
			led_cc = 0;
	}
	
	if(led_battery_flash && (!DECT_DC()) ){
			switch(battery_state){
				case BATTERY_LOW:
					 // DC IN AND BLUE TOOTH IS MATCHING THEN DON'T SHOW RED
					 if(event != Battery_low)
						logEvent(Battery_low);
					 if(DECT_DC()){
						if(led_cc==0)
						{
// 								ledColorSet(YELLOW_COLOR);
						}
						break;
					 }
						else
						{
							ledColorSet(RED_COLOR);
							led_battery_low_mode = 0;
						}
				 
					break;
				case BATTERY_MIDDLE:
					if(DECT_DC()){
							if(WB_paired_flag && (state_color == BLUETOOTH_COLOR) && led_cc==0 // is bluetooth and matched
									||state_color != BLUETOOTH_COLOR ){// is line in 
					 
// 							ledColorSet(YELLOW_COLOR);
							}
					}else
						goto L2;
					break;
				case BATTERY_HIGH:
					if(DECT_DC()){ 
						if(WB_paired_flag && (state_color == BLUETOOTH_COLOR) &&led_cc==0// is bluetooth and matched
							||state_color != BLUETOOTH_COLOR 
						){// is line in 							
// 						ledColorSet(GREEN_COLOR);
							if(event != Battery_full)
							logEvent(Battery_full);
						}
					}else
						goto L2;
					break;
				default:
					L2:
						if(DECT_DC()&&led_battery_flash == 12)
							if(event != Battery_chargin)
								logEvent(Battery_chargin);
			
						led_battery_flash--;
					goto L1;
			}
			if(!DECT_DC())
			led_battery_flash--;
			
		}else {
	L1:
// 			if((WB_CALLING == WB_call_status || WB_CALL_COME == WB_call_status) && dev_input == BLUETOOTH)
// 			{
// 				ledColorSet(GREEN_COLOR);
// 			}
// 			else
			if(0 == led_saving_power_flag )
				{
				
			if(breath_flag != 0xff)
					breathingColor();
			else if(led_flash && dev_state != DEV_STANDBY){
				ledColorSet(DARK_COLOR);
				led_flash--;
			}else
				if(dev_state == DEV_TEST)
					ledColorSet(SOE_COLOR); 
				else
					ledColorSet(state_color);	// change led color
		}

	else
	{
		ledColorSet(DARK_COLOR);
	}
// 	if(bt_play_music_flag)
// 	{
// 		ledColorSet(GREEN_COLOR);
// 	}
// 	else
// 	{
// 		ledColorSet(PURPLE_COLOR); 
// 	}
	}
	startATask(led_task, LED_FLASH_RATE, 0, ledTask);		// change led color next 1sec
}


/*-------------------------------------------------------------------------
 * @fucname:	LedTaskInit
 * @brief: 	led task init 
 * @param: 	none
 * @return: 	none
 * @details: 	led task init
 *-----------------------------------------------------------------------*/
void ledTaskInit()
{
	
	led_task = getATask();
	log_id = getATask();
	
	if(led_task != TASK_FULL)
		startATask(led_task, LED_FLASH_RATE, 0, ledTask);
}





