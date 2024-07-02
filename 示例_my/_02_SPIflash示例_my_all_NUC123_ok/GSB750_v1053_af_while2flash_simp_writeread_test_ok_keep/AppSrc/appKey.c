/****************************************************************************
 * @file:		appkey.c
 * @version:	V1.00
 * @breif:	do key event which was sent by ir_key or adc_key 
 * @date: 	2013/3/01
 * 	
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

extern u32 BT_MATCH_FLAG;
extern u32 volume_key_delay;
extern FUNC_Timer_Task adcKeySubHook;
extern void playSFMusicEnd(void);

u8 change_source_mute = 0;
u8 change_source_counter = 0;
u8 usb_i2c_enable = 0;
u8 play_demo_sound_mode = 0;
u8 bt_play_music_flag = 0;
u8 bt_play_music_before = 0;
u8 bt_play_music_change_delay = 0;
u8 wireless_mode_open = 0;

u32 led_saving_couter  = 0;
u8 led_saving_power_flag = 0;
u8 led_battery_low_mode = 0;
u8 music_channel_flag = 0;   // 0 for stero ; 1 for left channel; 2 for right channel

u8 call_eq_mode = 0;      //ma 20161123 call eq function 

extern u32 audio_quiet_time;
/*-------------------------------------------------------------------------
 * @fucname:	 delStandbytkey
 * @brief: 	 deal key event when dev is in standby mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in standby mode 
 *-----------------------------------------------------------------------*/
void delStandbytkey(enum KEYWORD keyword)
{
// 	u32 i;

	switch(keyword){
		case POWER_KEY:
			/*
			for(i = 0; i < 75; i++){
				sysDelayMs(20);
				if(!GPIO_GET_PIN(POWER_KEY_PORT_PIN))
					return;
			}
			*/
			outStandby();
			break;
		default:
			break;
	}
	if(DECT_NFC())
	{
		outStandby();
	}	
}


/*-------------------------------------------------------------------------
 * @fucname:	 delStartkey
 * @brief: 	 deal key event when dev is in start mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in start mode 
 *-----------------------------------------------------------------------*/
extern u32 power_delay;
extern volatile u32 Dwhp83_CuMu;
extern enum KEYWORD key_buf;
extern u32 Dwhp83_flag;
extern DWHP83_STATE dwhp83_status;

extern u8 dwhp83_type;       //ma    20150525
void bootLDROM()
{
	UNLOCKREG();
	outpw(&SYS->RSTSRC, 3);//clear bit
	outpw(&FMC->ISPCON, (inpw(&FMC->ISPCON) & 0xFFFFFFFC) |0x2);
	outpw(&SCB->AIRCR, (0x05FA0000UL | 0x00000004UL));//SYSRESETREQ
	while(1);
}

/******************set config******/     //ma 20150522   
void setConfig()
{
// 	u32 devce_config = 0;                //ma  20150513  lower Byte 
// // 	#if(FIXED_COCO_MODE)
// 			devce_config = cumu_flag;      //ma   20150522   add cumu flag
// // 	#endif
// 	SYS_UnlockReg();
// 	FMC_ENABLE_ISP();
// 	DrvFMC_Erase(0x1f000);
// 	devce_config |= 0x50000000;

// 	devce_config |= ((amp_volume&0x00ff)<<8);
// 	devce_config |= ((dev_input&0xff)<<16);
// 	devce_config |= ((dwhp83_type&0xf)<<24);
// 	if(DrvFMC_Write(0x1f000, devce_config) == 0xffff0010){
// 		__nop();
// 		__nop();
// 	}
// 	
// 	
// 	FMC_DISABLE_ISP();
// 	SYS_LockReg();
// 	I2S_DISABLE_INT(I2S_INT_RXTH);
// 	I2S_CLR_RXFIFO();
// 	I2S_CLR_TXFIFO();
// 	i2sStart(); 
	
}


/******************set config******/     //ma 20150522 
void getConfig()
{
// 	u32 k;
//  	
// 	SYS_UnlockReg();
// 	FMC_ENABLE_ISP();
// 	DrvFMC_Read(0x1f000, &k);
// 	if((k&0xF0000000) != 0x50000000){
// 		cumu_flag = 0;       // ma 20150522  0 for MU    1  for CU
// 		dev_input = BLUETOOTH;
// 		amp_volume = 23;              //default 24 level volume
//  		dwhp83_type = VOLUME_C;
// 	}else{
// 		cumu_flag = (k&0xff);
// 		amp_volume = ((k>>8)&0x00ff);
// 		dev_input = (enum PT2314_INPUT_CHANNEL)((k>>16)&0xff);
// 		dwhp83_type = ((k>>24)&0xf);
// 	}

// 	FMC_DISABLE_ISP();
// 	SYS_LockReg();
}
void delStartkey(enum KEYWORD keyword)	
{
// 	u32 i = 0;
	u32 power_key_counter = 0;
// 	u32 inc_key_counter = 0;
	 u8 sharing_key_counter = 0;
	 u32 pause_key_counter = 0;
	if(keyword != NONE_KEY ){
		BAT_STATE_DECT_DELAY_COUNT = 0; // battery power dectect delay
		led_battery_flash = 0; // don't show battery information while a key pressed
	}
	
	switch(keyword){
		case NONE_KEY:
					break;
		case POWER_KEY:
					logEvent(key_power_pressed);
// 					ledColorSet(state_color);
					while(key_buf == POWER_KEY){
							sysDelayMs(1);
							power_key_counter++;
	// // 						if(power_key_counter > 150 && key_buf_adc == NONE_KEY)
// 							if(power_key_counter > 20)
// 								ledColorSet(DARK_COLOR);
							if(power_key_counter > 1000)
							{
								ledColorSet(DARK_COLOR);
								enterStandby();
								power_key_counter = 0;
								break;
							}
						}
// 					devTest();
// 					if(power_delay < 1500)
// 						enterStandby();
					break;	
		case INC_KEY:
					volume_key_delay = BT_MATCHING_DELAY;
					audio_quiet_time = 0;
// 					if(TP_IRQ())
// 					{
// 						while(TP_IRQ())
// 						{
// 							sysDelayMs(20);
// 							inc_key_counter++;
// 							if(inc_key_counter % 25 == 0)
// 							WB_music_speed();
// 						}
// 					}
// 					else
						ampAddVolume();
					led_saving_couter = 0;
					led_saving_power_flag = 0;
					logEvent(key_vol_add_pressed);
					break;
		case DEC_KEY:
					volume_key_delay = BT_MATCHING_DELAY;
					audio_quiet_time = 0;
					ampDecVolume();
					led_saving_couter = 0;
					led_saving_power_flag = 0;
					logEvent(key_vol_dec_pressed);
					break;
		case CHANNEL_SWITCH_KEY:                  //change to play pause key
// 				logEvent(key_source_pressed);
// 				if(state_color == LIGHT_BLUE_COLOR){
// 					bt_hang_up_signal = TRUE;
// 					led_flash = 4;
// 				}else{
// 					while(key_buf == CHANNEL_SWITCH_KEY && dev_input != WIRELESS){
// 						ledColorSet(state_color);
// 						sysDelayMs(20);
// 						if(++i > 100){ 
// 							if(Dwhp83CuMuDetect() == 0){
// 								i = 0;
// 								break;
// 							}

// 							ledColorSet(SOE_COLOR);
// 							if(Dwhp83_CuMu == MU){
// 								Dwhp83GetStatus();
// 								if(dwhp83_status.link == 0 || !isDwhp83Linked()){
// 									Dwhp83Master();
// // #if(FIXED_COCO_MODE)
// 									cumu_flag = 1;   //ma 20150526
// 									setConfig();   //ma 20150526
// // #endif
// 									sysDelayMs(3000);
// 									//Dwhp83EnterPairing();
// 									playMusic(1);
// 								}else{
// 									Dwhp83_flag |= 0x1;
// 									sysDelayMs(1000);
// 								}
// 								
// 							}else{
// 								sysDelayMs(1000);
// 								//Dwhp83EnterPairing();
// 							}
// 							break;
// 							
// 						}
// 					}
// 					if(i < 100){
						audio_quiet_time = 0;
						ledColorSet(state_color);
						led_saving_couter = 0;
						led_saving_power_flag = 0;
						while(key_buf == CHANNEL_SWITCH_KEY){
							sysDelayMs(20);
							pause_key_counter++;
							if(pause_key_counter>900)
							{
								if(0 == usb_i2c_enable)
								{
									ledColorSet(GREEN_COLOR);
									playMusic(PAIRED_SOUND);
									UPGRADE_CLR();
									sysDelayMs(1000);
									ledColorSet(state_color);
									usb_i2c_enable = 1;
								}
								else
								{
									ledColorSet(RED_COLOR);
									playMusic(DISCONECTED_SOUND);
									UPGRADE_SET();
									sysDelayMs(1000);
									ledColorSet(state_color);
									usb_i2c_enable = 0;
								}
								break;
							}
								else if(pause_key_counter == 200)
								{
									if(WB_NO_CALL != WB_call_status && dev_input == BLUETOOTH)
									{
										WB_reject_call();
										break;
									}
									else if((dev_input == BLUETOOTH )&& WB_paired_flag)
									{
										WB_enter_pairing();
										playMusic(PAIRING_SOUND);
										ledColorSet(GREEN_COLOR);
										sysDelayMs(1000);
										ledColorSet(state_color);
										break;
								 }
							 }
						}
						if((pause_key_counter>450)&& (pause_key_counter<=900))
						{
							if(0 == play_demo_sound_mode)
							{
								ledColorSet(GREEN_COLOR);
								sysDelayMs(1000);
								ledColorSet(state_color);	
								play_demo_sound_mode = 1;
								sysDelayMs(100);
								playMusic(DEMO_SOUND);
							}
							else 
							{
								ledColorSet(RED_COLOR);
								sysDelayMs(1000);
								ledColorSet(state_color);	
								play_demo_sound_mode = 0;
							}
							break;
						}
						else if(pause_key_counter == 451)
						{
							ledColorSet(DARK_COLOR);
							sysDelayMs(100);
							ledColorSet(state_color);
						}

						if(pause_key_counter < 100){
							if(play_demo_sound_mode)
							{
								ledColorSet(DARK_COLOR);
								sysDelayMs(100);
								ledColorSet(state_color);	
								playMusic(DEMO_SOUND);
							}
							else if(WB_CALL_COME == WB_call_status  && dev_input == BLUETOOTH)
							{
								WB_open_call();
								if(0 == call_eq_mode)
								{
									ampSendCallEqs_38(0,63);
									call_eq_mode = 1;
								}
								return;
							}
							else if(WB_CALLING == WB_call_status  && dev_input == BLUETOOTH)
							{
								WB_close_call();
								if(1 == call_eq_mode)
								{
									ampSendEqs_38(0,63);
									call_eq_mode = 0;
								}
								return;
							}
							else
							{
// 								if(dev_input == BLUETOOTH)
// 								{
// 									if(WB_paired_flag)
// 									{		
										WB_play_pause();   
										ledColorSet(DARK_COLOR);
										sysDelayMs(100);
										ledColorSet(state_color);	
// 									}
// 								}
	// 							changeInputVolumeDown(INPUT_CHANGE_STATE);
							}
					}
// 				}
				break;
		case SOUND_CLOSE_KEY:
				volume_key_delay = BT_MATCHING_DELAY;
				ampMuteSwitch();
					break;
				
		case GAME_KEY:
			//#if(FILTER_DEBUG == TRUE)
// 				if(filter_bypass == 0){
// 					filter_by        pass = 1;
// 					state_color = YELLOW_COLOR;
// 				//}else if(filter_bypass == 1){
// 				//	filter_bypass = 2;
// 				//	state_color = SOE_COLOR;
// 				//}else if(filter_bypass == 2){
// 				//	filter_bypass = 3;
// 				//	state_color = GREEN_COLOR;
// 				}else if(filter_bypass == 1){
// 					filter_bypass = 0;
// 					state_color = input_channel_color[dev_input];
// 				}
					audio_quiet_time = 0;
					led_saving_couter = 0;
					led_saving_power_flag = 0;
					if(play_demo_sound_mode)
					{
						ledColorSet(DARK_COLOR);
						sysDelayMs(100);
						ledColorSet(state_color);
						return;
					}
					if(!Dwhp83CuMuDetect())
					{
						return;
					}
					ledColorSet(state_color);
					while(key_buf == GAME_KEY)
					{
						sysDelayMs(20);
						sharing_key_counter++;
						if(sharing_key_counter >99)
						{
							if(dev_input != WIRELESS)
							{
									ledColorSet(DARK_COLOR);
									sysDelayMs(100);
									ledColorSet(state_color);              
									if(Dwhp83_CuMu == MU)
									{
										Dwhp83GetStatus();
										if(dwhp83_status.link == 0 || !isDwhp83Linked())
										{
											Dwhp83Master();						
										}
										else
										{
											Dwhp83_flag |= 0x1;       //ma 20151112
										}
									}
									else
									{
										
									}
							 }				 

							return;
						}
					}
					if(sharing_key_counter<100)
					{				
						ledColorSet(DARK_COLOR);
						sysDelayMs(100);
						ledColorSet(state_color);
							if(dev_input != WIRELESS)
							{
								wireless_mode_open = 1;
								ampMute();
								dev_input = WIRELESS;
								state_color = input_channel_color[dev_input];
								ledColorSet(state_color);
								GPIO_CLR_PIN(WIRELESS_BT_SEL_PORT_PIN);
								sysDelayMs(500);
								unMute();          //ma   20160406
								sysDelayMs(20);
								if(Dwhp83_CuMu == CU)
								{
									Dwhp83Slave();
								}
								
							}
							else
							{
								ampMute();
								wireless_mode_open =0;
								change_source_mute = 1;
								change_source_counter = 0;
								sysDelayMs(100);

// 								ampMute();
// 								dev_input = BLUETOOTH;
// 								state_color = input_channel_color[dev_input];
// 								ledColorSet(state_color);
// 								GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
// 								GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
// 								sysDelayMs(150);
// 								unMute();          //ma   20160406
// 								sysDelayMs(20);
							}						
					}	
			//#endif
					break;
			 		case MUSIC_KEY:
						if(2 == music_channel_flag)
							music_channel_flag = 0;
						else music_channel_flag++;
						if(0 == music_channel_flag)
						{
							playMusic(STERO_SOUND);
// 							ledColorSet(GREEN_COLOR);	
							sysDelayMs(500);	 
						}
						else if(1 == music_channel_flag)
						{
							playMusic(LEFT_CHANNEL_SOUND);
// 							ledColorSet(YELLOW_COLOR);	
							sysDelayMs(500);
						}
						else if(2 == music_channel_flag)
						{
							playMusic(RIGHT_CHANNEL_SOUND);
// 							ledColorSet(SOE_COLOR);	
							sysDelayMs(500);
						}
						ledColorSet(state_color); 
						led_saving_couter = 0;	
						led_saving_power_flag = 0;					
						break;
#if(SPIFLASH_ENABLE == TRUE)
		case MOVIE_KEY:
				//if(flash_mode == FLASH_CONFIG){
					playMusic(++flash_music_index);
					state_color = SOE_COLOR;
				//}
				//else{
				//	flash_mode = FLASH_CONFIG;
				//	playSFMusicEnd();
				//}
					break;
// 		case MUSIC_KEY:
			/*
				if(flash_mode == FLASH_RECODE_MUSIC)
					flash_recode_stop = TRUE;
				else
					recodeMusic(0);
					break;
			*/
#endif
		default:
			break;
	}	
}


/*-------------------------------------------------------------------------
 * @fucname:	 delStartkeyContinue
 * @brief: 	 deal continue key event when dev is in start mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal continue key event when dev is in start mode 
 *-----------------------------------------------------------------------*/
void delStartkeyContinue(enum KEYWORD keyword)
{

}



static i2cPoolType upgrade_eeprom_i2c_block;
static volatile u8 upgrade_eeprom_i2c_buf[3];

u8 read_upgrade_eeprom(u32 address)
{
  initI2cDataBlock( &upgrade_eeprom_i2c_block, 
          0xAE, 
          I2C_RD, 
          2,
          1,
          (u8*)&upgrade_eeprom_i2c_buf[0]
        );
  
  upgrade_eeprom_i2c_buf[1] = address&0xff;;
  upgrade_eeprom_i2c_buf[2] = 0x0;
  upgrade_eeprom_i2c_buf[0] = I2C_BUSY;

  addI2cChain(&upgrade_eeprom_i2c_block, 0);
  while(upgrade_eeprom_i2c_buf[0] == I2C_BUSY)
      ;
  return upgrade_eeprom_i2c_buf[0];	
}
