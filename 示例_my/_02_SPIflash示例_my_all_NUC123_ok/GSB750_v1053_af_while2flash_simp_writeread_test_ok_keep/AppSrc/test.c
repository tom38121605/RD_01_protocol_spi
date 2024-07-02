// /****************************************************************************
//  * @file:		test.c
//  * @version:	v1.00
//  * @breif:		test source file
//  * @date: 	2013..4.1	
//  * 
//  * @note 		
//  *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
//  * 
//  ****************************************************************************/
// #include "sysinc.h"
// 						
// u32 test_mode = 0;
// u8 test_channel = 0;	
// u8 channelspeaker[6] = {0x82, 0x84, 0x88, 0x90, 0xc0, 0xff, };
// extern u32 drc_bypass;
// extern u32 volume_key_delay;
// extern FUNC_Timer_Task adcKeySubHook;


// void testInit()
// {
// 	dev_state = DEV_TEST;
// 	ledColorSet(SOE_COLOR);
// 	BT_POWER_OFF();
// 	sysDelayMs(200);
// 	BT_POWER_ON();
// 	sysDelayMs(200);
// 	BT_POWER_OFF();
// 	test_mode = 1;
// 	test_channel = 0;
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 dealTestkey
//  * @brief: 	 deal key event when dev is in test mode
//  * @param: 	 keyword
//  * @return: 	 none
//  * @details: 	 deal key event when dev is in test mode
//  *-----------------------------------------------------------------------*/
// void dealTestKey(enum KEYWORD keyword)
// {
// // 	u32 i;
// 	
// 	if(keyword != NONE_KEY ){
// 		BAT_STATE_DECT_DELAY_COUNT = 0; // battery power dectect delay
// 		led_battery_flash = 0; // don't show battery information while a key pressed
// 	}
// 		
// 	switch(keyword){
// 		case NONE_KEY:
// 			break;

// 		case SOUND_CLOSE_KEY:
// 				ampMuteSwitch();
// 					break;
// 					
// 		case POWER_KEY:
// 			/*
// 			if(DECT_DC()){
// 				volume_key_delay = 50;
// 			 	if(test_channel == 6)
// 			 		test_channel = 0;
// 			 	testOutputSpeakers();	
// 			 	++test_channel;
// 			 	led_flash = 4;
// 			}
// 			else */

// 				enterStandby(); 
// 			break;
// 			
// 		case INC_KEY:
// 			volume_key_delay = 50;
// 			ampAddVolume();
// 					break;
// 			
// 		case DEC_KEY:
// 			volume_key_delay = 50;
// 			ampDecVolume();
// 					break;
// 		
// 		case MOVIE_KEY:
// 		case CHANNEL_SWITCH_KEY:
// 				led_flash = 4;
// 				if(flash_mode == FLASH_CONFIG){
// 					playMusic(DEMO_SOUND);
// 					state_color = YELLOW_COLOR;
// 				}
// 				else{
// 					//flash_mode = FLASH_CONFIG;
// 					flash_mode = FLASH_CONFIG;
// 					SPI_CLR_SS0(SPI2);
// 				}
// 					
// 				break;
// 	
// 		case GAME_KEY:
// 				led_flash = 4;
// 				if(drc_bypass == FALSE)
// 					drc_bypass = TRUE;
// 				else
// 					drc_bypass = FALSE;
// 				break;
// 		default:
// 			break;
// 	}

// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 dealTestKeyContinue
//  * @brief: 	 deal key event when dev is in test mode
//  * @param: 	 keyword
//  * @return: 	 none
//  * @details: 	 deal key event when dev is in test mode
//  *-----------------------------------------------------------------------*/
// void dealTestKeyContinue(enum KEYWORD keyword_c)
// {

// 	switch(keyword_c){
// 		
// 		case NONE_KEY:
// 			break;
// 			
// 		case INC_KEY:
// 			volume_key_delay = 50;
// 			ampAddVolume();
// 				break;
// 			
// 		case DEC_KEY:
// 			volume_key_delay = 50;
// 			ampDecVolume();
// 				break;
// 		default:
// 				break;
// 	}

// /*
// 	static u32 k_counter = 0;	// for delay in key_word_continue event 

// 	switch(keyword_c){
// 		case NONE_KEY:
// 						break;
// 		case INC_KEY:
// 					if(k_counter > INC_KEY_COUNTER){
// 						k_counter = 0;
// 						return;
// 					}else {
// 						if(++k_counter >= INC_KEY_COUNTER){
// 							(state_color == DARK_COLOR)	? ledColorSet(state_color = RED_COLOR)
// 														: ledColorSet(state_color = DARK_COLOR);
// 							k_counter = 0;
// 						}	
// 					}
// 					break;
// 		case DEC_KEY:
// 					if(k_counter > INC_KEY_COUNTER+DEC_KEY_COUNTER+1
// 						||k_counter  <  INC_KEY_COUNTER+1){
// 						k_counter = INC_KEY_COUNTER+1;
// 						return;
// 					}else {
// 						if(++k_counter >= INC_KEY_COUNTER+DEC_KEY_COUNTER+1){
// 							(state_color == DARK_COLOR)	? ledColorSet(state_color = GREEN_COLOR)
// 														: ledColorSet(state_color = DARK_COLOR);
// 							k_counter = INC_KEY_COUNTER+1;
// 						}
// 					}
// 					break;
// 		case CHANNEL_SWITCH_KEY:
// 					if(k_counter > INC_KEY_COUNTER+DEC_KEY_COUNTER+CHANNEL0_KEY_COUNTER+2
// 						||k_counter  <  INC_KEY_COUNTER+DEC_KEY_COUNTER+2){
// 						k_counter = INC_KEY_COUNTER+DEC_KEY_COUNTER+2;
// 						return;
// 					}else {
// 						if(++k_counter >= CHANNEL0_KEY_COUNTER+INC_KEY_COUNTER+DEC_KEY_COUNTER+2){
// 							(state_color == DARK_COLOR)	? ledColorSet(state_color = WHITE_COLOR)
// 														: ledColorSet(state_color = DARK_COLOR);
// 							k_counter = INC_KEY_COUNTER+DEC_KEY_COUNTER+2;
// 						}
// 					}
// 					break;


// 		default:
// 			break;
// 	}
// */
// }



