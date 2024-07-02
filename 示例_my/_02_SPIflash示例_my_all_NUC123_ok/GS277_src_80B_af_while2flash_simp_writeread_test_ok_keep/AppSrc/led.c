/****************************************************************************
 * @file:		led.c
 * @version:	V1.00
 * @breif:	Led C source file. 
 * @date: 	2013/2/23
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define CALL_STATE_BLUETOOTH	0x1
#define CALL_STATE_LINEIN		0x2



enum LED_COLOR state_color = BLUETOOTH_COLOR;	// the color of the state
Timer_ID led_task = TASK_FULL;	// led task id
u32 led_flash = 0;
// u32 led_battery_flash = 0;                  //ma 20150825  dummy useless value 
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
u32 volume_key_delay = 30; 
u32 bt_call_hold = 0; // whether call is hold on
extern  u32 w3d_volume_back;
extern u8 pt2314_switch_remap[];
// extern u8 bt_disable_reconnect;         //ma 20150520  cut repeated claration
U32  pwtable[21] = {1,2,3,4,6,9,12,15,18,24,30,39,48,62,78,95,116, 131,150, 170,200};
U32 R_C = 0, B_C = 0, G_C = 0;
// extern u8 mute_back_volume;      //ma 20150606      //ma 20150825 cut useless value     
	
 u8 NFC_decting_flag = 0;         //ma 20150709   
/*-------------------------------------------------------------------------
 * @fucname:	 ledInit
 * @brief: 	 init the  pin to pwm function
 * @param: 	 none
 * @return: 	 none
 * @details: 	 init the pin to pwm function
 *-----------------------------------------------------------------------*/
void ledInit() 
{
	SET_TMR0_CLKSRC(CLK_TMR0_HXT);  
	DrvTIMER_Init(TIMER0, 21, 10, TIM_MODE_TOGGLE);
	NVIC_SetPriority (TMR0_IRQn, ADC_INT_PRIORITY); 
    	DrvTIMER_EnableInt(TIMER0, TIM_INT_EN);
    	TIM_ENABLE(TIMER0);
	R_C = B_C = G_C = 0;
	GPIO_SET_MODE(LED_CTRL_PORT, RED_LED, GPIO_OUTPUT);
	GPIO_SET_MODE(LED_CTRL_PORT, BLUE_LED, GPIO_OUTPUT);
	GPIO_SET_MODE(LED_CTRL_PORT, GREEN_LED, GPIO_OUTPUT);
}


u32 ptimer = 0;
void TMR0_IRQHandler(void)
{
//   static u8 icount =0;
	
//	u32 temp_c;//ma 20150629 
	
	 TIM_CLEAR_INTERRUPT_FLAG(TIMER0);
	
	/*****100ms wake adc*****/
// 	if(ptimer == 5)
// 		ADC_CONVERTER_DISABLE();
// 	if(ptimer == 195)
// 		ADC_CONVERTER_ENABLE();
	
// 	if(icount++==4)
// 	{
// 		icount=0;
// 		ptimer++;
// 	}
// 	if(ptimer > 200)
// 		ptimer = 0;
		
	
	if(++ptimer > 200)
		ptimer = 0;

	if(ptimer < pwtable[R_C])
		GPIO_CLR_PIN(RED_PIN);	
	else
		GPIO_SET_PIN(RED_PIN);

	if(ptimer < pwtable[B_C])
	{
		if(!sleep_flag)              //ma 20150706    sleep   no   blue 
			GPIO_CLR_PIN(BLUE_PIN);	
	}
	else
		GPIO_SET_PIN(BLUE_PIN);

	if(ptimer < pwtable[G_C])
	{
		if(!sleep_flag)          //ma 20150706    sleep   no   blue 
			GPIO_CLR_PIN(GREEN_PIN);	
	}
	else
		GPIO_SET_PIN(GREEN_PIN);
}


static u32 fade_color;
static u32 ck = 0;
u32 breath_flag = 0xff;

void fadeon( u32 color)
{
	ck = 0;
	fade_color = color;
	breath_flag = 0XAA;
}

void fadeoff( u32 color)
{
	ck = 20;
	fade_color = color;
	breath_flag = 0XBB;
}

void breathingColor()
{
	static u32 cc = 0;
//	static u32 breath_bak = 0xff;   //ma 20150629 
	u32 b = breath_flag;

	if((++cc & 0x1))
breathing_s:
		switch(b){
			case BLUETOOTH_COLOR:	// breath blue
				R_C= 0; G_C = 0; 
				ck++;
				if(ck > 19)
					B_C = 40-ck;
				else
					B_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case RED_COLOR:	// breath read
				B_C= 0; G_C = 0; 
				ck++;
				ck++;
				if(ck > 19)
					R_C = 40-ck;
				else
					R_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case GREEN_COLOR: // breath green
				B_C= 0; R_C = 0; 
				ck++;
				if(ck > 19)
					G_C = 40-ck;
				else
					G_C = ck;
				if(ck > 39)
					ck = 0;
				break;
			case YELLOW_COLOR:	// breath yellow
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
			case PURPLE_COLOR:
				G_C = 0;
				ck++;
				if(ck > 19)
					B_C = 40-ck;
				else
					B_C = ck;
				if(ck > 39)
					ck = 0;
				R_C= B_C;
				break;
			case WHITE_COLOR:	// breath white
				ck++;               //MA 20150612
				if(ck > 19)
					B_C = 40-ck;
				else
					B_C = ck;
				if(ck > 37)
					ck = 0;
				R_C= G_C = B_C;
				R_C= G_C *2/3;
				break;
			case 0xAA: // fades on
				if(ck != 20){
					b = fade_color;
					goto breathing_s;
				}else{
					breath_flag = 0xff;
				}
				break;
			case 0XBB: // fades off
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
							R_C = 15; G_C = 0; B_C = 0;
							break;
		case BLUE_COLOR:
							R_C = 0; G_C = 0; B_C = 15;
							break;
		case GREEN_COLOR:
							R_C = 0; G_C = 15; B_C = 0;
							break;
		case SOE_COLOR:
							R_C = 15; G_C = 10; B_C = 15;
							break;
		case LIGHT_BLUE_COLOR:
							R_C = 0; G_C = 15; B_C = 0;
							break;
		case BLUETOOTH_COLOR:
							R_C = 0; G_C = 0; B_C = 15;
							break;
		case YELLOW_COLOR:
							R_C = 15; G_C = 15; B_C = 0;
							break;					
		case PURPLE_COLOR:
							R_C = 15; G_C = 0; B_C = 15;
							break;
		case WHITE_COLOR:
							R_C = 12; G_C = 18; B_C = 18;
							break;
		case PAIRING_COLOR:
							R_C = 15; G_C = 18; B_C = 9;
							break; 
		case SWITCH_CU_COLOR:
							R_C = 6; G_C = 12; B_C = 4;
							break;
		case SWITCH_MU_COLOR:
							R_C = 12; G_C = 6; B_C = 6;
							break;
		default:
				return;
	}
	
}



/*-------------------------------------------------------------------------
 * @fucname:	 ledTask
 * @brief: 	 led timed task
 * @param: 	 none
 * @return: 	 none
 * @details: 	 flash led per 1sec
 *-----------------------------------------------------------------------*/
extern enum W3D_STATE change_input_state;
extern DWHP83_STATE dwhp83_status;

extern u8 COCO_KEY_BEFORE; 	  // ma 20150507    //ma 20150707 declare   at led .c

u32 cc_blue_p = 45 ;     //ma   20150907  change to 45
u32 ccp = 0;      // ma 20150525  
// extern u8 Dwhp83_flag;     //ma 20150603      //for CUMU  COMMEND
u8 key_time_count  =0;       //ma 20150617
extern u16 mu_addr;               //ma 20150818    add declare mu addr 
u32 change_channal_counter =0;      //ma 20150708 add change channale counter  to handle change channal  especially in soundbar mode
// u8  source_change_delay = 0;        //ma 20150810    add sb source change delay    
u8 sb_change_source_mute_flag = 0;   //ma 20150819  add soft mute i2s flag when sb change source 
u8 tx_linked_flag = 0;   //ma 20150906  add tx  linked  flag 
void ledTask()
{
// 	static u32 soundbar_multiroom_counter  = 0;     //ma 20150717  
// 	static u8 set_cu_counter = 0;     //ma 20150714   
// 	u8 coco_sb[3] = {0x05,0x05,0x05};       //ma 20150512 SB   mode  COCO change     //ma 20150708   move to led.c
  static u32 soundbar_alive_counter = 0;  //ma 20150710 
	static u32 bt_match_times = 0;
	static U32 BT_CALL_FLAG = 0; 
	static u32 bt_amp_start_time = 0;          //MA 20150630  add for  bt  pause  i2s  mute 
	static u32 cct =0;       //ma 20150716  for soundbar multiroom 
//	static u32 battery_flash=0;    //ma 20150629 
//	static u32 ccw = 0;       //ma 20150629 
	static u32 cck = 0, ccr = 0, ccm = 0,	ccp = 0;          //ma 20150520  //ma 20150603  add cct to if there is any TX
// 	static u32   ccd = 0;    //ma 20150805 
   // ma 20150606  ccm for muting 
  //ma 20150615 add ccp for music play
		/******add play batterry state play flag******/
	static u8 batterry_middle_play_flag = 0; 
	static u8 batterry_low_play_flag = 0;
	/*********************************************/
		static u8 Core_tx_counter = 0;      //ma 20150813   1.5seconds
	
		 static  u8 TX_sounder_counter =0;       //ma 20150817    add  TX sounder counter  to play  transmission  sound 
 	if(dev_state == DEV_TEST)                 //ma 20150707   del useless function   
 		goto L3;
// 	if(ccp < MUSIC_FLASH_SIZE)    //   20150615  for music testing 
// 	{
// 		playMusic(ccp);
// 		sysDelayMs(3000);
// 		ccp++;
// 	}

// 	if(++ccd > 30/*!GPIO_GET_PIN(PB7)*/){
		if(!GPIO_GET_PIN(PB7)){      // ma  20150525  WL irq   //ma 20150604    change 30 to 5   //ma 20150606  change 5 to 3    //ma 20150805
		DWHP83DealMsg();
// 		ccd = 0;
	}
	/******battery high reset flag*****/
	if(battery_high_flag == 2)     //ma 20150703   set  battery_high_flag
	{
			batterry_middle_play_flag = 0;
			batterry_low_play_flag = 0;
			battery_high_flag = 0;
	}
	/******battery high reset flag end *****/
	
	/****Core tx mute****/    //ma 20150813  1.5seconds
	if(Core_tx_flag)
		Core_tx_counter++;
	if(Core_tx_counter==50)
	{
		DWHP83Mute();        //ma 20150813
	}
 if(Core_tx_counter>150)
	{
		Core_tx_counter = 0;
		Core_tx_flag  =0;
		DWHP83UnMute();        //ma 20150813
	}
	
	/********add change channal handle *******/
	if(change_channal_flag)                    //ma 20150708
	{
			change_channal_counter++;
				
	}
  if(change_channal_counter == 1&&SB_flag)  sb_change_source_mute_flag = 1;  //ma 20150818  add mute when sb change source 
	if(change_channal_counter==50)          //ma 20150708   1sec         //ma 20150708
	{

		if(SB_flag)		
		{
			if(soundbar_multiroom_flag){
				DWHP83Mute();
			}
			else
				DWHP83UnMute();
			if(dev_input != USERDEF)
			{

				if(!soundbar_coco_check())  //ma 20150710  
				{
					DWHP83toEXT_MCU();
					sysDelayMs(100);
					set_soundbar_coco();
					sysDelayMs(100);
				}
				Dwhp83CuMuDetect();
				Dwhp83_flag  = 0;                            //MA 20150714
				if(Dwhp83_CuMu == MU)
				{
					DWHP83toEXT_MCU();
					sysDelayMs(100);
					Dwhp83Master();
				}
			}
			else 
			{
        
				Dwhp83CuMuDetect();
				if(Dwhp83_CuMu == CU)
				{
					change_to_local_coco();
					sysDelayMs(100);

				}
				if(soundbar_coco_check())
				{
						set_local_coco();
				}
				Dwhp83_flag  = 0;                   //MA 20150714
				if(Dwhp83_CuMu == CU)
					Dwhp83Slave();
			}
		}
				sb_change_source_mute_flag = 0;   //ma 20150818  add mute when sb change source 
	}


	
			 if(change_channal_counter>220)       //ma 20150810 
			{
// 				led_flash = 4;       //ma 20150902  del  useless function 
				change_channal_counter =0;
				change_channal_flag = 0;
				if(soundbar_multiroom_flag){
						TX_sound_flag = 1;     //ma 20150901     
// 					delStartkey(MULTIROOM_KEY);
						if(soundbar_coco_check()||(!isDwhp83Linked()))        //ma 20150703  if  sounbar coco link multiroom to 
							{
											change_to_local_coco();    //ma 20150515   SB mode  tell SUB  change to local  COCO    0x22
											sysDelayMs(100);
											DWHP83Mute();
											set_local_coco();              //ma 20150807
											sysDelayMs(100);        //ma 20150514  wait for the current CU change to MU
                      Dwhp83Slave();
// 								      sysDelayMs(1000);

											DWP83SpiritTask();											
// 											DWHP83UnMute();                     //ma 20150902    play music will unmute dwhp83
											TX_flag = 1;
							}
							soundbar_multiroom_flag = 0;
				}
				
			}
/*******bt pause for 10sec then play handle****/
 	/******add  tx sound  play function  when tx transmission state******/
		if(TX_sound_flag > 0)                                       //ma	  20150817
		{
			TX_sounder_counter++;
			sb_change_source_mute_flag = 1;
		}
		else  
		{
			sb_change_source_mute_flag = 0;        
			TX_sounder_counter = 0;                     //ma 20150906
		}
	 if(TX_sound_flag&&TX_sounder_counter == 88 )       //ma	  20150901         //ma 20150907   change to 88
	 {
		 TX_sounder_counter = 0;
		 TX_sound_flag++;
// 		 if(SB_flag == 0)
// 		 {
// 			if(TX_sound_flag>1)
// 				TX_sound_flag =  0;
// 		 }
// 		 else 
// 		 {
			 if(TX_sound_flag>42)       //MA 20150902   play tx sound for 2 mins     //ma 20150906     3 to 40
				TX_sound_flag = 0;
// 		 }
		 playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
	 }
  /*******transmission sound play end ********/	
	if(bt_amp_start_flag)     //ma 20150630
	{
		bt_amp_start_time++;
	}

	if(bt_amp_start_time>=20)     //ma 20150630   set I2s mute for 0.6sec when  bt  pause 10sec  then play 
	{		
		bt_amp_start_time = 0;
		bt_amp_start_flag = 0;
	}
// 	/*********end*******/
	if(play_music_flag > 0&&play_music_flag <40)
	{
		key_time_count++;
		if(key_time_count>20)          //ma 20150928   change 10 to 40 
		{
				play_music_flag++;
			if(play_music_flag ==2)
			{
				playMusic(MCU_SOUND);
// 				sysDelayMs(1400);
// 				sysDelayMs(200);            //ma 20150928  test  no delay  when play  wersion number 
			}
			else if(play_music_flag == 3)
			{
				playMusic(VERSION_SOUND);
// 				sysDelayMs(200);              //ma 20150928  test  no delay  when play  wersion number
// 				sysDelayMs(1000);
			}
			else if(play_music_flag==4)
			{
				playMusic(MCU_VERSION_NUMBER[0] - '0'+NUMBER_0);

// 				sysDelayMs(1200);
			}
			else if(play_music_flag ==5)
			{
				playMusic(POINT_SOUND);

// 				sysDelayMs(1000);
			}
			else if(play_music_flag ==6)
			{
				playMusic(MCU_VERSION_NUMBER[2] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag ==7)
			{
				playMusic(MCU_VERSION_NUMBER[3] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			
			else if(play_music_flag == 12)
			{
				playMusic(BLUETOOTH_SOUND);
// 				sysDelayMs(1400);
// 				sysDelayMs(200);
			}
			else if(play_music_flag == 13)
			{
				playMusic(VERSION_SOUND);
// 				sysDelayMs(200);
// 				sysDelayMs(1400);
			}
			else if(play_music_flag == 14)
			{
				playMusic(BT_VERSION_NUMBER[0] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag== 15)
			{
				playMusic(POINT_SOUND);
// 				sysDelayMs(1200);
			}
			else if(play_music_flag == 16)
			{
				playMusic(BT_VERSION_NUMBER[2] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag == 17)
			{
				playMusic(BT_VERSION_NUMBER[3] - '0'+NUMBER_0);
	

// 				sysDelayMs(1000);
			}
// 				/*****play irkey *****/		
// 			else if(play_music_flag == 18)
// 			{
// 				if(ir_key_volue<100)
// 				playMusic(NUMBER_0);
// 				else playMusic(NUMBER_1);
// // 				sysDelayMs(1400);
// 			}
// 			else if(play_music_flag == 19)
// 			{
// 				playMusic(ir_key_volue/10%10+NUMBER_0);
// // 				sysDelayMs(1000);
// 			}
// 			else if(play_music_flag== 20)
// 			{
// 				playMusic(ir_key_volue%10+NUMBER_0);
// // 				sysDelayMs(1000);
// 			}
			else if(play_music_flag == 22)
			{
				playMusic(WIRELESS_SOUND);
// 				sysDelayMs(200);           //ma 20150928  test  no delay  when play  wersion number
// 				sysDelayMs(1400);
			}
			else if(play_music_flag == 23)
			{
				playMusic(VERSION_SOUND);
// 				sysDelayMs(200);               //ma 20150928  test  no delay  when play  wersion number
// 				sysDelayMs(1400);
			}
			else if(play_music_flag == 24)
			{
				playMusic(WL_VERSION_NUMBER[0] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag== 25)
			{
				playMusic(POINT_SOUND);
// 				sysDelayMs(1200);
			}
			else if(play_music_flag == 26)
			{
				playMusic(WL_VERSION_NUMBER[2] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag == 27)
			{
				playMusic(WL_VERSION_NUMBER[3] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}

			else if(play_music_flag == 32)
			{
				playMusic(SOUND_SOUND);
// 				sysDelayMs(200);             //ma 20150928  test  no delay  when play  wersion number
// 				sysDelayMs(1400);
			}
			else if(play_music_flag == 33)
			{
				playMusic(VERSION_SOUND);
// 				sysDelayMs(200);             //ma 20150928  test  no delay  when play  wersion number
// 				sysDelayMs(1000);
			}
			else if(play_music_flag == 34)
			{
				playMusic(FLASH_VERSION_NUMBER[0] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag==	35)
			{
				playMusic(POINT_SOUND);
// 				sysDelayMs(1200);
			}
			else if(play_music_flag == 36)
			{
				playMusic(FLASH_VERSION_NUMBER[2] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			else if(play_music_flag == 37)
			{
				playMusic(FLASH_VERSION_NUMBER[3] - '0'+NUMBER_0);
// 				sysDelayMs(1000);
			}
			
			else play_music_flag =0;
// 			sysDelayMs(500);                //ma 20150928  test  no delay  when play  wersion number
			key_time_count = 0;
		}
// 			goto L1;
	}
	
	if(play_music_flag >40)
	{
		key_time_count++;
		if(key_time_count>30)
		{
				if(play_music_flag == 0x82)
					playMusic(NUMBER_1);
				else if(play_music_flag == 0x84)
					playMusic(NUMBER_2);
				else if(play_music_flag == 0x88)
					playMusic(NUMBER_3);
				else if(play_music_flag == 0x90)
					playMusic(NUMBER_4);
				else if(play_music_flag == 0xc0)
					playMusic(NUMBER_5);
		// 	 play_music_flag =0;
// 				sysDelayMs(500);
				key_time_count = 0;
		}
	}
	
	if(TX_flag == 1)
	{
		cct++;

	}
	if(cct == 2)      //ma 20150605 if no link  ,it no need to trans CU with MU
	{
// 		Dwhp83GetStatus();                                  
// 		if(dwhp83_status.link == 0 || !isDwhp83Linked());
// 		else 	Dwhp83_flag |= 0X01; 
		MU_tell_CU_change();
		DWP83SpiritTask();
	}
	if(cct ==160)           //ma 20150901
	{
		MU_tell_CU_change();        //ma 20150813 
	}
	if(cct>220)                         //ma 20150807
	{
			tx_linked_flag = 0;       //ma 20150906   clr  tx linked flag 
			cct = 0;
			MU_tell_CU_change();
			if(TX_flag == 1)
			{
					if(Dwhp83CuMuDetect())           //ma 20150514  check CUMU
					{
						if(Dwhp83_CuMu == MU){
							Dwhp83GetStatus();
							if(dwhp83_status.link == 0 || !isDwhp83Linked()){
								DWP83SpiritTask();
								MU_tell_CU_change();
// 								MU_tell_CU_change();     //ma 20150514  local the-one want to be CU ,he tell the current CU change to MU 
// 								sysDelayMs(1000);        //ma 20150514  wait for the current CU change to MU
								Dwhp83Master();          
								sysDelayMs(2000);              //ma 20150921     add some delay  for prepare tx 
								playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								TX_sounder_counter = 0;				     //ma 20150901 				
								//Dwhp83EnterPairing();
// 								playMusic(PAIRED_SOUND);         //ma 20150606  no sound  to indicate 
							}else{
								MU_tell_CU_change();
								Dwhp83_flag |= 0x1;
								DWP83SpiritTask();                           //ma 20150717
// 								sysDelayMs(1000);                          //ma 20150715     //ma 20150810
// 							DWHP83SendCmd(0x02,0,mu_addr);                    //ma 20150716
// 								sysDelayMs(1000);
								Dwhp83Master();
							  sysDelayMs(2000);                             //ma 20150921     add some delay  for prepare tx
								playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								TX_sounder_counter = 0;				     //ma 20150901								
								Dwhp83_flag = 0;
// 								sysDelayMs(2000);                          // ma  20150715 
								DWP83SpiritTask();  
							}
									
						}
					}
// 					if(!soundbar_multiroom_flag){                       //ma 20150902  play music will unmute dwhp83 
// 						DWHP83UnMute();     //ma 20150713
// 					}
					TX_flag = 0;
// 					sysDelayMs(1000);      //ma 20150821     add some delay  for prepare tx 
			}
	}


	if(ccr>200)    //ma 20150530            //ma 20150803    change 500 to 200 
	{
// 			if(SB_flag)                            //ma  20150714
// 			{
// 				if(Dwhp83CuMuDetect())           //ma 20150514  check CUMU
// 				{
// 					if(Dwhp83_CuMu != CU)        //ma 20150515 if  sb play mode the one (SB) tell  SUB change to local  COCO 
// 					{
// 						Dwhp83Master();			
// 					}
// 				}
// 			}
			ccr = 0;
			power_first_on_flag = 0;   //ma 20150529   outstandby no RED COLOR
			
	// 		bt_paring_flag = 0;
	}
	if(power_first_on_flag)
			ccr++;	
	
// 	if(bt_paring_flag)
// 		ccr++;
// 	else ccr = 0;
// 	if(ccr>5000)    //ma 20150527
// 	{
// 		ccr = 0;
// 		bt_paring_flag = 0;
// 	}
// 	if(!bt_power_flag)       //ma 20150526
// 	{
// 		ccr++;
// 		if(ccr > 5000)
// 		{
// 			ccr = 0;
// 			bt_power_flag = 1;
// 		}
// 	}
// 	else ccr = 0;
	
	if(state_color == LIGHT_BLUE_COLOR & callDetect()){

			if(bt_hang_up_signal == TRUE){
				bt_hang_up_signal = FALSE;
				if(isCallIn()){
					openCall();
					sysDelayMs(20);
				}else
					closeCall();
				
				startATask(led_task, LED_FLASH_RATE, 0, ledTask);	
				return;
			}
	}
	
	if(state_color == PURPLE_COLOR){
		if(++ccp > 30){ 
			Dwhp83GetStatus();
			ccp = 0;
		}
		if(dwhp83_status.link == 0)
		{
			if(cck > 0){
				--cck;
			}else{
				cck = 10;
				led_flash = 5;

			}
				
// 			SB_link_flag = 0;     //ma 20150707  clr SB_link_flag 
		}
		else                      //ma 20150707   add sb link  counter 
		{
// 				if(local_coco_check()&&TX_sound_flag)        //ma   20150817         // ma 20150902  if local linked ,do not play tx sound 
// 				{
// 					Dwhp83CuMuDetect();
// 					if(Dwhp83_CuMu == CU)
// 					{
// 						 TX_sounder_counter = 0;     //ma 20150901
// 						 TX_sound_flag = 0;
// 					}
// 				}
		}

	}
	 if(soundbar_alive_counter == 15)                //ma 20150819 add check sb volume every second
	 {
		 		Dwhp83CuMuDetect();
				if(Dwhp83_CuMu == MU)
				{
					RX_SB_alive();
					if((isMute() || amp_volume==0));
					else
					{
						if((isMute() || amp_volume==0));
						else
						{	
							sysDelayMs(20);
							SB_rx_volume_2_SUB();    //ma 20150818
						}						
					}		
				}
		}
		if(SB_flag&&(++soundbar_alive_counter>30)&&dev_state!=DEV_STANDBY)    //ma 20150731  standby mode  do not send this message 
		{
			soundbar_alive_counter =0;
			Dwhp83CuMuDetect();
			if(Dwhp83_CuMu == MU)
			{
				RX_SB_alive();         
// 				if((isMute() || amp_volume==0));        //ma  20150819    del repeat function 
// 				else
// 				{
// 					if((isMute() || amp_volume==0));
// 					else
// 					{	
// 						sysDelayMs(20);
// 						SB_rx_volume_2_SUB();    //ma 20150818
// 					}						
// 				}					
			}
			else if(Dwhp83_CuMu == CU)
			{
				TX_SB_alive();
			}
		}
	if(state_color == BLUETOOTH_COLOR){

		if(callDetect()&&(BT_CALL_FLAG == FALSE)){
			state_color = LIGHT_BLUE_COLOR;
			ledColorSet(state_color);
			mute();
			sysDelayMs(200);
			CHOOSE_BCALL_INPUT();
			callOnMode();
			sysDelayMs(200);
			unMute();
			bt_call_hold = 0;
			BT_CALL_FLAG = CALL_STATE_BLUETOOTH;  // call coming when in bluetooth state
			w3d_volume_back = amp_volume; // back the w3d_volume
			amp_volume = VOLUME_CALL;
			changeInputVolumeDown(VOLUME_DOWN_UP);
		}		

		if(volume_key_delay==0)
		{
			if(!isBtPaired()){
				if(bt_match_times > 0){
					bt_match_times --;
				}
				else{
					bt_match_times = 8;
					//bt_match_times = 16;
					
					led_flash = 5;
					//led_flash = 1;
				}
				if(BT_MATCH_FLAG){
					playMusic(DISCONECTED_SOUND);
// 					cc_blue_p = 25;         //ma 20150608 
				}else
					//if(++cck > 85 && cc_blue_p > 0){          //ma 20150902  change 150 to 100
					if(++cck > 125 && cc_blue_p > 0){          //ma 20150902  change 150 to 100
						cc_blue_p--;
						//Dwhp83CuMuDetect();
						//if(Dwhp83_CuMu != 0x55aa)
						if(!bt_change_name_mode&&!isBtPaired())      //ma 20150806
						{		
							if(0 == TX_sound_flag)							         //ma 20150907
								;//--playMusic(NEW_BT_PAIRING_SOUND);        //ma 20150901  add new bt pairing sound      
							  //;//playMusic(0);           
// 							sysDelayMs(500);                   //ma 20150817      add some delay  for play paried  sound  
						}
						cck = 0;
				}
				BT_MATCH_FLAG = 0;
// 				if(bt_disable_reconnect == 0)
// 					btReconnect(); 
				
				if(ccp == 300)      //ma  20150521 be reconnect for 5 sec 
				{
// 					ledColorSet(GREEN_COLOR);
					btPairing();
					sysDelayMs(20);
					bt_reconnect_flag = 0;
// 					bt_paring_flag =1;
				}
				/******bt faraway reconnect ****/
				if(ccp > 550)	       //ma 20150609
				{
					if(bt_lost_flag == 1 &&bt_paring_flag == 0)   //ma 20150610  add bt lost flag
						btReconnect();
// 					btShutDown();         //ma 20150520    BT+OFF
// 					sysDelayMs(100);  
// 					btStartUp();        //ma 20150520    BT+ON
					ccp = 0;
				}
				if(ccr ==0)
				{
					ccp = ccp + 1;
				}

			}else{
				ccp = 0;
				if(BT_MATCH_FLAG == 0)
				{
					playMusic(PAIRED_SOUND);
					sysDelayMs(500);                   //ma 20150817      add some delay  for play paried  sound  
// 					bt_reconnect_flag = 0;
// 					bt_paring_flag = 0;
// 					NFC_flag = 0;    // ma 20150528
					ccr = 0;
					cc_blue_p = 45;         //ma 20150907    set 45
					bt_paring_flag = 0;
					bt_lost_flag =0;     //ma 20150610
				}
				BT_MATCH_FLAG = 1;
			} 
		}
		else
		{
			volume_key_delay--;
			if(isBtPaired())
			{
				if(BT_MATCH_FLAG == 0)
					{
						playMusic(PAIRED_SOUND);
						sysDelayMs(500);                   //ma 20150817      add some delay  for play paried  sound  
						cc_blue_p = 45;                   //ma 20150907     set to 45 
						bt_reconnect_flag = 0;
						bt_paring_flag = 0;
						BT_MATCH_FLAG = 1;
						NFC_flag = 0;    // ma 20150528
						ccr = 0;
						bt_lost_flag =0;     //ma 20150610
					}
			}
		}
		if(DECT_NFC()&&(!NFC_decting_flag)){
                    //MA 20150709 
			
			cc_blue_p = 45;                   //ma 20150907        set 45
			NFC_decting_flag = 1;
			bt_reconnect_flag = 1;
			bt_power_flag = 0;                  // ma 20150813 NFC change source set bt power on flag
// 			NFC_flag = 1;     //ma 20150528          //ma 200150702    when NFC if  bt restart  bt reconnect    
			bt_dis_last_connect();// ma 20150527
			sysDelayMs(20);
			btPairing();                                 //MA 20150709
			if(SB_flag)
			{
					change_channal_flag = 1;          // ma 20150813  set  channal flag
					TX_sounder_counter = 0;           //ma 20150902 
					TX_sound_flag = 0;                //ma 20150902 
			}
			change_channal_counter = 0;       // ma 20150813  clr  change channal counter 
			sysDelayMs(20);
// 			bt_paring_flag = 1;		   // ma 20150527		
		}
		
	}else if( dev_state != DEV_STANDBY ){
		if(callDetect() && state_color != LIGHT_BLUE_COLOR){
			
			bt_call_hold = 0;
			state_color = LIGHT_BLUE_COLOR;
			ledColorSet(state_color);
			seW58SetBypass(1); // 3D off
			seW58sendCmd();
			sysDelayMs(5);
			dev_input = BLUETOOTH;
			BT_CALL_FLAG = CALL_STATE_LINEIN; // call coming when in line in state
			w3d_volume_back = amp_volume; // back the w3d_volume
			amp_volume = VOLUME_CALL;
			changeInputVolumeDown(INPUT_CHANGE_STATE);
			
		}else if(DECT_NFC()){
// 			NFC_decting_flag = 1;
			NFC_flag = 1;     //ma 20150528  
			dev_input = BLUETOOTH;			
			cc_blue_p = 45;                   //ma 20150907       set 45 
			bt_power_flag = 0;                  // ma 20150813 NFC change source set bt power on flag
			sysDelayMs(30);
			BT_OPEN();            //ma 20150526
			btStartUp();        //ma 20150520    BT+ON
			CHOOSE_BT_INPUT();
			cs8422SRCSourceSDIN();
			sysDelayMs(200);	
			state_color = input_channel_color[dev_input];
			if(SB_flag)
			{
					change_channal_flag = 1;          // ma 20150708  set  channal flag 
					TX_sounder_counter = 0;           //ma 20150902 
					TX_sound_flag = 0;                //ma 20150902 
			}
			change_channal_counter = 0;       // ma 20150708  clr  change channal counter 
//			changeInputVolumeDown(INPUT_CHANGE_STATE);
		}
	}else if(DECT_NFC()){	
			NFC_flag =1;    // ma 20150528
			sysDelayMs(30);
			dev_input = BLUETOOTH;
			outStandby();
	}
	
	if(!DECT_NFC())
	{
		NFC_decting_flag =0;
	}

		
	if((BT_CALL_FLAG != FALSE) && !callDetect() ){

		if(BT_CALL_FLAG == CALL_STATE_LINEIN){
			dev_input = AUX;
			amp_volume = w3d_volume_back;
			changeInputVolumeDown(INPUT_CHANGE_STATE);
		}
		else{
			state_color = BLUETOOTH_COLOR;
			CHOOSE_BT_INPUT();
			callOffMode();
			ledColorSet(state_color);
			amp_volume = w3d_volume_back;
			changeInputVolumeDown(VOLUME_RECOVER);
		}

		BT_CALL_FLAG = FALSE;
	}
		

	//if(++BAT_STATE_DECT_DELAY_COUNT > BATTERY_SHOW_INFO_TIME){
	//	BAT_STATE_DECT_DELAY_COUNT = 0;
//		led_battery_flash = 12;
//	}

	
	if(++BAT_STATE_DECT_DELAY_COUNT > BATTERY_SHOW_INFO_TIME){
		switch(battery_state){
			case BATTERY_LOW:
				if(systick_counter < 5000)
					break;
				 // DC IN AND BLUE TOOTH IS MATCHING THEN DON'T SHOW RED
				if(!DECT_DC()){

					/****first out stanby no red flash*****/
					if(!power_first_on_flag)
					{
						/*******play batterry low music*****/   //ma 20150625

					if(batterry_low_play_flag == 0)
					{
						playMusic(BATTERY_LOW_SOUND);
						sysDelayMs(300);
						batterry_low_play_flag = 1;
						
					}
					else if(batterry_low_play_flag == 1)
					{
						playMusic(BATTERY_LOW_SOUND);
						sysDelayMs(300);
						batterry_low_play_flag = 2;						
					}
					/******add 4 times battery low sound *****/
					else if(batterry_low_play_flag == 2)      //ma 20150702  
					{
						playMusic(BATTERY_LOW_SOUND);
						sysDelayMs(300);
						batterry_low_play_flag = 3;						
					}
					else if(batterry_low_play_flag == 3)
					{
						playMusic(BATTERY_LOW_SOUND);
						setConfig();               //ma 20150731   add store core state when %5 batterry
						sysDelayMs(300);
						batterry_low_play_flag = 4;		

					}
					breath_flag = RED_COLOR;      //ma 20150530   close red flash
					breathingColor();
					}
									goto L1;              //ma 20150716
				}
				else                             //ma 20150715  add dc battery low mode 
				{
						state_color = input_channel_color[dev_input];
						ledColorSet(state_color);     //ma 20150715 
						batterry_low_play_flag =0;		 //ma 20150731 				
				}
				  break;                   //ma 20150716

				case BATTERY_MIDDLE:
				if(systick_counter < 5000)
					break;
				 // DC IN AND BLUE TOOTH IS MATCHING THEN DON'T SHOW RED
				if(!DECT_DC()){

					/****first out stanby no red flash*****/
					if(!power_first_on_flag)
					{
						/*******play batterry low music*****/   //ma 20150625
						if(batterry_middle_play_flag == 0)
						{
							playMusic(BATTERY_LOW_SOUND);
							sysDelayMs(300);
							batterry_middle_play_flag = 1;						
						}
						/********add 2 battery sound *********/
						if(batterry_middle_play_flag == 1)     //ma 20150702 
						{
							playMusic(BATTERY_LOW_SOUND);
							sysDelayMs(300);
							batterry_middle_play_flag = 2;						
						}
						breath_flag = RED_COLOR;      //ma 20150527   close red flash
						breathingColor();
					}
									goto L1;           //ma 20150716
				}
				else                             //ma 20150715  add dc battery middle  mode 
				{
						state_color = input_channel_color[dev_input];
						ledColorSet(state_color);     //ma 20150715  
						batterry_middle_play_flag	=0;    //ma 20150731 			
				}    
        break;                      //ma 20150716
// 				/*****add battery  high handle****/
// 				case BATTERY_HIGH:     //ma 20150629  
// 				if(systick_counter < 5000)
// 					break;
// 					/****first out stanby no red flash*****/
// 					if(!power_first_on_flag)
// 					{				
// 						batterry_low_play_flag  = 0;  
// 						batterry_middle_play_flag = 0;
// 					}
// 				goto L1;
			default:
				break;
		}
		
	}
		
		
			
 L3:
		/****no call default enable mic****/
		if(!callDetect())      //ma 20150529
		{
			mic_mute_flag = 0;
		}
		if(callDetect()&&mic_mute_flag == 1&&dev_state != DEV_STANDBY)   //ma 20150525 
		{
			breath_flag = GREEN_COLOR;
			breathingColor();
			goto L1;
		}
		if((isMute() || amp_volume==0) && dev_state != DEV_STANDBY){
			breath_flag = state_color;
			breathingColor();
			if(!ccm)
			{
				if(SB_flag&&dev_input == USERDEF)
				{
					SB_volume_mute();    //ma 20150608   SB_volume_mute

						ccm = 1;
	// 				}
				}
				else ccm =0;
			}
			goto L1;
		}
		else 
		{
			if(ccm)
			{
				if(SB_flag&&dev_input == USERDEF)
				{
					SB_volume_unmute();
					ccm = 0;
				}
				else ccm =0;
			}
// 			goto L1;
		}
		if(led_flash && dev_state != DEV_STANDBY){
			ledColorSet(DARK_COLOR);                
			led_flash--;
		}
// 		else                        
// 			if(dev_state == DEV_TEST)               //ma 20150707   del useless function                  
// 				ledColorSet(YELLOW_COLOR); 
			else
				if(!factory_mode_flag)
				ledColorSet(state_color);	// change led color

		
L1:
	if(dev_state == DEV_STANDBY && !DECT_DC()){
		// if didn't dect DC and it is in DEV_STANDBY, then turn off the power
		doPowerOff();
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

	if(led_task != TASK_FULL)
		startATask(led_task, LED_FLASH_RATE, 0, ledTask);
}





