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

enum LED_COLOR ioldcolor;
enum LED_COLOR inowcolor;

enum LED_COLOR state_color = BLUETOOTH_COLOR, state_color_back = DARK_COLOR;	// the color of the state
Timer_ID led_task = TASK_FULL;	// led task id
u8 led_flash = 0;
u8 led_battery_flash = 0;
u8 bt_hang_up_signal = FALSE;									//	CN        CM     PRESCALE  DIV	MODE	INV
S_DRVPWM_TIME_DATA_T color_pwm_clc = {1000,    1000,	  12,	           4,		1,		0};

u8 flg_whitebreath=0;

u32 cclsllt = 0;

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
u32 bt_call_hold = 0;
extern  u32 amp_volume_back;
extern u8 pt2314_switch_remap[];
extern u8 amp_volume_tbl[];
//--extern Timer_ID log_id;
//--extern Log_Type event;
s32 breath_flag = 0xff;
extern U32 R_C, B_C, G_C;

/*-------------------------------------------------------------------------
 * @fucname:	 leddisable
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void leddisable()
{
	TIM_DISABLE(TIMER0);
}

/*-------------------------------------------------------------------------
 * @fucname:	 ledInit
 * @brief: 	 init the  pin to pwm function
 * @param: 	 none
 * @return: 	 none
 * @details: 	 init the pin to pwm function
 *-----------------------------------------------------------------------*/
void ledInit() 
{
	
	GPIO_SET_PIN(PA12);
	GPIO_SET_PIN(PA13);
	GPIO_SET_PIN(PA14);
	GPIO_SET_MODE(PA, 12, GPIO_OUTPUT);
	GPIO_SET_MODE(PA, 13, GPIO_OUTPUT);
	GPIO_SET_MODE(PA, 14, GPIO_OUTPUT);

	
	SET_TMR0_CLKSRC(CLK_TMR0_HXT); 	
	DrvTIMER_Init(TIMER0, 21, 10, TIM_MODE_TOGGLE);
	
	NVIC_SetPriority (TMR0_IRQn, ADC_INT_PRIORITY); 
	
	DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);
	TIM_ENABLE(TIMER0); 
	
}

static u32 fade_color;
static u32 ck = 0;
volatile static u32 fade_ = 0;
volatile static u32 fade__ = 0;
extern u32 pwm_ccl;

//static u32 ck2 = 0;

void fadeon( u32 color)
{
	ck = 0;
	fade_color = color;
	breath_flag = 6;
	fade_ = 1; 	
}

void fadeoff( u32 color)
{
	ck = 20;
	fade_color = color;
	breath_flag = 7;
	fade_ = 1;
}

void breathingColor(void)
{
	static u32 cc = 0;
	//--static u32 breath_bak = 0xff;
	u32 b = breath_flag;

	if((++cc & 0x1))
breathing_s:
		switch(b)
	  {
			case 1:	// breath blue
				R_C= 0;  G_C = 0; 
			
			  //ck++;
				ck++;//ck++;  //--zz			
			
				if(ck > 39)
					ck = 1;
				else if(ck > 20)
					B_C = 40-ck;
				else
					B_C = ck;
				break;
				
			case 2:	// breath read
				B_C= 0; G_C = 0; 
				ck++;
				if(ck > 39)
					ck = 1;
				else if(ck > 20)
					R_C = 40-ck;
				else
					R_C = ck;
				break;
				
			case 3: // breath green
				B_C= 0; R_C = 0; 
				ck++;
				if(ck > 20)
					G_C = 40-ck;
				else
					G_C = ck;
				if(ck > 39)
					ck = 1;
				break;
				
			case 4:	// breath yellow
				B_C= 0; 
				ck++;
				if(ck > 39)
					ck = 1;
				else if(ck > 20)
					G_C = 40-ck;
				else
					G_C = ck;
				R_C= G_C;
				break;
				
			case 5:	// breath white				
				ck++;
				if(ck > 65)
					ck = 1;
				else if(ck > 46)
					G_C =  66-ck;
				else if(ck > 20)
					G_C = 20;
				else
					G_C = ck;
				
				
				B_C = R_C= G_C;
				
				flg_whitebreath=1;
				break;
								
			case 6: // fades on
				if(ck != 20)
				{
					b = fade_color;
					goto breathing_s;
				}
				else
				{					
//iputs("breathingColor--test06.\r\n");  //--zz debug					
					breath_flag = 0xff;
					fade_ = 0;
					fade__ = 6;
				}
				break;
				
			case 7: // fades off
				if(ck != 1)
				{
					b = fade_color;
					goto breathing_s;
				}
				else
				{
//iputs("breathingColor--test07.\r\n");  //--zz debug

					breath_flag = 0xff;
					fade_ =  0;
					fade__ = 7;
				}
				break;

			case 8:	// breath white half

				ck++;
				if(ck > 60)
					ck = 1;
				else if(ck > 46)
					G_C =  61-ck;
				else if(ck > 15)
					G_C = 15;
				else
					G_C = ck;				
				
				B_C = R_C= G_C;
				
				flg_whitebreath=1;
				break;
								
			default:
				break;
		}
		
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
							R_C = 20; G_C = 0; B_C = 0;
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
							R_C = 20; G_C = 20; B_C = 20;
							break;
		case BLUETOOTH_COLOR:
							R_C = 0; G_C = 0; B_C = 20;
							break;
		case YELLOW_COLOR:
							//R_C = 20; G_C = 20; B_C = 0;
							R_C = 20; G_C = 10; B_C = 0;
							break;					
		case PURPLE_COLOR:
							R_C = 20; G_C = 0; B_C = 20;
							break;
		case WHITE_COLOR:
							R_C = 20; G_C = 20; B_C = 20;
							break;

		default:
				return;
	}
	
}


extern u8 dc_flag;

u32 battery_low_flag = 0;
U32 pairing_key_flag = 0;
u32 ccf = PAIRING_SECOND;
U32 battery_low_flag2 = 0;

extern u32 battery_f;
void ledTask()
{
	
	if( !GPIO_GET_PIN(PB10) )  
	{     
// iputs("rx int.\r\n");  //--zz debug
		DWHP83DealMsg(); 
	}
	
	if (iselsource==DEV_BLUETOOTH && (flg_poweron2==1) )
	{
		if(GPIO_GET_PIN(PA15))	
		{
			ledColorSet(DARK_COLOR); 
		}
		else
		{
			ledColorSet(BLUE_COLOR); 
		}

		 //breathingColor();
	}
	
// 	if(breath_flag!=0xff)
// 	  breathingColor();
	

/*	//--static u32 bt_match_times = 0, bt_unmatch_times = 0, bt_count = 0;
	//static U32 BT_CALL_FLAG = 0, ccp = 0, ccj = 0, cck = 0,  BFB = 0, cclsl = 0, cclsllt = 0;  //--, cclsllb = 0;
	static U32 BT_CALL_FLAG = 0, ccp = 0, ccj = 0, cck = 0,  BFB = 0, cclsl = 0;  //--, cclsllb = 0;
	
	
	u32 i;

	 pwm_ccl = 30;
	if(dev_state == DEV_TEST || dev_state == DEV_STANDBY)
	{
		if(DECT_NFC())
		{
			outStandby();
			return;
		}
		if(battery_level <= 100)
		{
					//if(battery_level > 30)
					if(battery_state==BATTERY_OFF)  //--ZZ
						//breath_flag = 5;
						breath_flag = 8;
					else if(battery_state==BATTERY_HIGH)   //--ZZ
					{
						breath_flag = 0xff;
					  ledColorSet(WHITE_COLOR);
					}
					else if(battery_level > 10)
					{
						pwm_ccl = 10;
						if(fade_color != 2 && (fade_ == 0 || breath_flag < 6 || breath_flag == 0xff))
						{
							if(fade__ == 6)
								fadeoff(5);
							else
								fadeon(2);
						}
						else if(fade_color != 5 && (fade_ == 0 || breath_flag < 6 || breath_flag == 0xff))
						{
							if(fade__ == 6)
								fadeoff(2);
							else
								fadeon(5);
						}
						goto L3;
					}
					else
						breath_flag = 2;
		}
		else   
		{
			ledColorSet(WHITE_COLOR);
			goto end;
		}     
		goto L3;
	}
	
	if(isBtPaired())
	{
		if(btlinklst() != 4)
		{
			if(BT_MATCH_FLAG == 0 )
			{
				playMusic(PAIRED_SOUND);
				sysDelayAMs(20);
			}
			BT_MATCH_FLAG = 1;
		}
	}
	else   
	{
		if(BT_MATCH_FLAG == 1)
			playMusic(DISCONECTED_SOUND);  
		BT_MATCH_FLAG = 0; 
	}
	
	
	if(isBtStreaming()){
		if(breath_flag < 6)
			breath_flag = 0xff;
	}

	if(twslinklst() == 4 || btlinklst() == 4)
	{
		++cclsl; 
		
		//--if(++cclsllt > 60000)
		//--if(++cclsllt > 6000)
		//if(cclsllt > 6000)
		if(cclsllt >= 30*3240000 )   //--(30*270*200*60)
		{
			time_out =1;
			sleep();
			cclsllt=0;
		}
		if(cclsl < 15)
			ledColorSet(DARK_COLOR);
		else if(cclsl < 30 )
			if(twslinklst() == 4)
				ledColorSet(YELLOW_COLOR);
			else
				ledColorSet(BLUE_COLOR);
		else
			cclsl = 0;
		goto end;
	}
	else
	{
		cclsllt = 0;
	}

	if(!twsPaired())
		I2S_SET_CHANNEL(I2S_CFG_CHANNEL_STEREO);
	
	
	
	
	if(isPairing())
	{
		++ccf;
	}
	else if(ccf > 3)
	{
		ccf = PAIRING_SECOND + 1;
		if(breath_flag == 1)
			breath_flag = 0xff;
	}
	
	if( breath_flag != 7)
	if(isBtStreaming() || callDetect() || (ccf > 0 && ccf < PAIRING_SECOND))
	{
		if(state_color == WHITE_COLOR)
		{
			state_color = BLUETOOTH_COLOR;
			dev_input = BLUETOOTH;
			ampMute();
			//iputs("dd_mute05\r\n");   //--zz debug
			sysDelayMs(30);
			GPIO_SET_PIN(PA10);
		
//iputs("ampSetVol--A08\r\n");    //--zz debug		
			ampSetVolume(0);
			sysDelayMs(10);
			unMute();
			fadeon(1);
			sysDelayMs(30);
//iputs("voladd--02.\r\n");    //--zz debug	
			for(i = 0; i <= amp_volume; i++)
			{
				ampSetVolume(i);
				sysDelayMs(3);
			}
		}
	}
	else
	{
		if(state_color != WHITE_COLOR)
		{
			state_color = WHITE_COLOR;
			dev_input = LINEIN;
			ampMute();
			//iputs("dd_mute06\r\n");   //--zz debug
			sysDelayMs(30);
			GPIO_CLR_PIN(PA10);
		
//iputs("ampSetVol--A09\r\n");    //--zz debug		
			ampSetVolume(0);
			sysDelayMs(10);
			unMute();
			fadeon(5);
			sysDelayMs(30);
//iputs("voladd--03.\r\n");    //--zz debug	
			for(i = 0; i <= amp_volume; i++){
				ampSetVolume(i);
				sysDelayMs(3);
			}
		
		}
	}

	//if(isTwsSearching())
		
	
	if(state_color == LIGHT_BLUE_COLOR && callDetect())
	{

			if(bt_hang_up_signal == TRUE){
				bt_hang_up_signal = FALSE;
				if(isComingCall())
					openCall();
				else{
					closeCall();
				}
				startATask(led_task, LED_FLASH_RATE, 0, ledTask);	
				return;
			}

			if(callair()){
				if(++cck > 30){
					cck = 0;
					led_flash = 15;
				}
			}
	}

	if(volume_key_delay==0)
	{	
			if(isPairing() &&  !isBtStreaming())
			{
				breath_flag = 1;

				if(++ccp > 160 && ccf < PAIRING_SECOND)
				{
					ccp = 0;
//iputs("PAIRING_SOUND--03.\r\n");  //--zz debug
					iputs("pairing sound \r\n");
					playMusic(PAIRING_SOUND);
				}
			}
			else if(isBtPaired())
			{
				if(breath_flag == 1)
					fadeon(5);
				if(BT_MATCH_FLAG == 0)
				{
					playMusic(PAIRED_SOUND);
				}
				BT_MATCH_FLAG = 1;
			}
			else if(isTwsSearching())
			{
				//breath_flag = 1;
				//state_color = YELLOW_COLOR;
			}
		}
		else
			volume_key_delay--;

	if(state_color == BLUETOOTH_COLOR)
	{
		if(callDetect()&&(BT_CALL_FLAG == FALSE))
		{
			sysDelayMs(200);
			//ampEQbypass();
			//ampMuteBass();
			logEvent(call_in);
			state_color = LIGHT_BLUE_COLOR;
			ledColorSet(state_color);
			sysDelayMs(5);
			bt_call_hold = 0;
		

			amp_volume_back = amp_volume;
			BT_CALL_FLAG = CALL_STATE_BLUETOOTH;  // call coming when in bluetooth state
			amp_volume = VOLUME_CALL;
			changeInputVolumeDown(VOLUME_DOWN_UP);
		}
		
		 if(NFC_DECT())
		 {
			 btPairing2();
			 sysDelayMs(200);
		 }
		
	}
	else if( dev_state != DEV_STANDBY )
	{
		if(callDetect() && state_color != LIGHT_BLUE_COLOR)
		{
			sysDelayMs(200);
			//ampEQbypass();
			//ampMuteBass();
			state_color = LIGHT_BLUE_COLOR;
			ledColorSet(state_color);
			if(dev_input == LINEIN)
				BT_CALL_FLAG = CALL_STATE_LINEIN; // call coming when in line in state
			else
				BT_CALL_FLAG = CALL_STATE_24wireless;		

			amp_volume_back = amp_volume; // back the amp_volume
			amp_volume = VOLUME_CALL;
		}
		else if(NFC_DECT())
		{
			//state_color = BLUETOOTH_COLOR;
			//ledColorSet(BLUETOOTH_COLOR);
			//changeInputVolumeDown(INPUT_CHANGE_STATE);
			//led_flash = 0;
			
			btPairing2();
			sysDelayMs(200);
		}
	}
	else if(DECT_NFC())
	{
		;
	}

		
	if((BT_CALL_FLAG != FALSE) && !callDetect() )
	{

		logEvent(call_hangup);
		ampEQon();
		ampUnMuteBass(); 
		state_color = BLUETOOTH_COLOR;
		dev_input = BLUETOOTH;
		ampMute();
		//iputs("dd_mute07\r\n");   //--zz debug
		sysDelayMs(10);
		GPIO_SET_PIN(PA10);
		
//iputs("ampSetVol--A10\r\n");    //--zz debug		
		ampSetVolume(0);
		sysDelayMs(10);
		unMute();
		sysDelayMs(10);
		

		amp_volume = amp_volume_back;
//iputs("voladd--04.\r\n");    //--zz debug	
		for(i = 0; i <= amp_volume; i++){
			ampSetVolume(i);
			sysDelayMs(3);
		}
		BT_CALL_FLAG = FALSE;
	}
		 

	//if(++BAT_STATE_DECT_DELAY_COUNT > 200){
	if(++BAT_STATE_DECT_DELAY_COUNT > 30)  //--zz
	{ 
		BAT_STATE_DECT_DELAY_COUNT = 0;
		led_battery_flash = 1;
	}
 
	if(++ccj == 150)
	{
		ccj = 0;
	//	if(state_color == WHITE_COLOR)
	//		btStatus(); 
	}

	if(breath_flag != 0xff )
	{
	//	led_battery_flash = 0;
	//	BAT_STATE_DECT_DELAY_COUNT = 0;
	}
		
	
	if( DECT_DC() && (!isBtStreaming()) && !(state_color == BLUETOOTH_COLOR && BT_MATCH_FLAG == 0) )   //--zz
	{		
		if(battery_state==BATTERY_OFF)
		{
      breath_flag=5;
			goto L3;				
		}		
		
	}
	
	if(!DECT_DC() && led_battery_flash && !(state_color == BLUETOOTH_COLOR && BT_MATCH_FLAG == 0))
	{
		
		switch(battery_state)
		{
			case BATTERY_LOW:

			case BATTERY_MIDDLE:

					fadeoff(2);
						while(breath_flag != 0xff)
						{
							breathingColor();
							sysDelayMs(20);
							adckeyScan();
						}
					fadeoff(2);
						while(breath_flag != 0xff)
						{
							breathingColor();
							sysDelayMs(20);
							adckeyScan();
						}
				break;
						
			case BATTERY_HIGH:
				if(event != Battery_full)
					logEvent(Battery_full); 
				if(DECT_DC())
				{ 
				  if(BT_MATCH_FLAG && (state_color == BLUETOOTH_COLOR) // is bluetooth and matched
				    ||state_color == WHITE_COLOR )// is line in 			
						BFB = breath_flag;
						//breath_flag = 7;
						//ledColorSet(GREEN_COLOR);
						//sysDelayMs(10);
						breath_flag = BFB;
				}
				else
					goto L2;
				
				break;
				
			case BATTERY_OVER:
						iputs("battery is over!\r\n");
						playMusic(BATTERY_LOW_SOUND);
iputs("BATTERY_LOW_SOUND--03.\r\n");  //--zz debug						
			
						fadeoff(2);
						while(breath_flag != 0xff)
						{
							breathingColor();
							sysDelayMs(18);
						}
						fadeoff(2);
						while(breath_flag != 0xff){
							breathingColor();
							sysDelayMs(18);
						}
						fadeoff(2);
						while(breath_flag != 0xff){
							breathingColor();
							sysDelayMs(18);
						}
						battery_low_flag = 1;
						capIntDisable(); 
						ccf = PAIRING_SECOND;
						btShutDown();
						sysDelayMs(100);
						enterStandby();
				//	}else{
				//  		capIntDisable(); 
				//		ccf = PAIRING_SECOND;
			//			btShutDown();
			//			enterStandby();
			//		}
				break;
						
			default:
				L2:
					if(DECT_DC()&&led_battery_flash == 12)
						if(event != Battery_chargin)
							logEvent(Battery_chargin);
					if(led_battery_flash > 0)
						led_battery_flash--;
				goto L1;
		}
		if(led_battery_flash > 0)
			led_battery_flash--;
		
	}else {
L1:
		if(isTwsSearching() && !callDetect())
			breath_flag = 4;
		else if(isPairing())
			breath_flag = 1;
		else if(pairStatus() == 1){
			breath_flag = 5;
			sysDelayMs(30);
		}else if(isBtStreaming() && breath_flag != 0x7 && !callair()){
			ledColorSet(WHITE_COLOR);
			goto end;
		}else if(twsStatus() == 0){
			breath_flag = 0xff; 
			//if(!isBtStreaming())
			ledColorSet(WHITE_COLOR);	
		}
L3:		
		if(breath_flag != 0xff)
			breathingColor();
		else if(led_flash && dev_state != DEV_STANDBY){
			led_flash--;
			ledColorSet(DARK_COLOR);
		}else
			if(dev_state == DEV_TEST)
				ledColorSet(YELLOW_COLOR); 
			else
				ledColorSet(state_color);	// change led color
	}
	
*/
	
//--end:	
	startATask(led_task, LED_FLASH_RATE, 0, ledTask);		// change led color next 1sec	

}

void ledTaskInit()
{
	
	led_task = getATask();
	//--log_id = getATask();
	
	if(led_task != TASK_FULL)
		startATask(led_task, LED_FLASH_RATE, 0, ledTask);
}
















