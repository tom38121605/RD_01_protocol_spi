/****************************************************************************
 * @file:		test.c
 * @version:	v1.00
 * @breif:		test source file
 * @date: 	2013..4.1	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
						

u8 test_channel = 0;	
u8 channelspeaker[6] = {0x82, 0x84, 0x88, 0x90, 0xc0, 0xff, };
extern u32 volume_key_delay;




/*-------------------------------------------------------------------------
 * @fucname:	 dealTestKeyContinue
 * @brief: 	 deal key event when dev is in test mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in test mode
 *-----------------------------------------------------------------------*/
// void dealTestKeyContinue(enum KEYWORD keyword_c)             //ma 20150707   del useless function   
// {

// 	switch(keyword_c){
// 		
// 		case NONE_KEY:
// 			break;
// 			
// 		case INC_KEY:
// 			volume_key_delay = 50;
// 			seW58AddVolume();
// 				break;
// 			
// 		case DEC_KEY:
// 			volume_key_delay = 50;
// 			seW58DecVolume();
// 				break;
// 		default:
// 				break;
// 	}

/*
	static u32 k_counter = 0;	// for delay in key_word_continue event 

	switch(keyword_c){
		case NONE_KEY:
						break;
		case INC_KEY:
					if(k_counter > INC_KEY_COUNTER){
						k_counter = 0;
						return;
					}else {
						if(++k_counter >= INC_KEY_COUNTER){
							(state_color == DARK_COLOR)	? ledColorSet(state_color = RED_COLOR)
														: ledColorSet(state_color = DARK_COLOR);
							k_counter = 0;
						}	
					}
					break;
		case DEC_KEY:
					if(k_counter > INC_KEY_COUNTER+DEC_KEY_COUNTER+1
						||k_counter  <  INC_KEY_COUNTER+1){
						k_counter = INC_KEY_COUNTER+1;
						return;
					}else {
						if(++k_counter >= INC_KEY_COUNTER+DEC_KEY_COUNTER+1){
							(state_color == DARK_COLOR)	? ledColorSet(state_color = GREEN_COLOR)
														: ledColorSet(state_color = DARK_COLOR);
							k_counter = INC_KEY_COUNTER+1;
						}
					}
					break;
		case CHANNEL_SWITCH_KEY:
					if(k_counter > INC_KEY_COUNTER+DEC_KEY_COUNTER+CHANNEL0_KEY_COUNTER+2
						||k_counter  <  INC_KEY_COUNTER+DEC_KEY_COUNTER+2){
						k_counter = INC_KEY_COUNTER+DEC_KEY_COUNTER+2;
						return;
					}else {
						if(++k_counter >= CHANNEL0_KEY_COUNTER+INC_KEY_COUNTER+DEC_KEY_COUNTER+2){
							(state_color == DARK_COLOR)	? ledColorSet(state_color = WHITE_COLOR)
														: ledColorSet(state_color = DARK_COLOR);
							k_counter = INC_KEY_COUNTER+DEC_KEY_COUNTER+2;
						}
					}
					break;


		default:
			break;
	}
*/
// }



