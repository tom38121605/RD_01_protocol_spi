/****************************************************************************
 * @file:		wb6084.h
 * @version:		
 * @breif:		wb6084 bt header file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __wb6084_h
#define __2b6084_h


// WB-6084 CMD 
#define WB_HEAD		 0xAA
#define WB_MAKE_CALL    				0X00
#define WB_MMI_ACTION           0X02
			#define MMI_LENGTH_H 0X00
			#define MMI_LENGTH_L 0X03
			#define HF_STATUS    0X00
									#define WB_HUNG_UP  		0x02
									#define WB_ANSWER       0X04
									#define WB_REJECT       0x05
									#define WB_MIC_MUTE     0x08
									#define WB_MIC_UNMUTE   0x09
									#define WB_HF_CONNECT   0x16
									#define WB_PLAY_PAUSE   0x32
									#define WB_NEXT_SONG   0x34
									#define WB_LAST_SONG   0x35
									#define WB_NOLINK_PAIRING 0x50
									#define WB_PAIRING      0x5D
									#define WB_CLEAR_ALL    0X56
// #define WB_MUSIC_CONTROL     		0x04
#define WB_CHANGE_DEVICE_NAME 	0X05
#define WB_PAIRING_TIMEOUT   		0x07
#define WB_GET_VERSION      		0x08

#define WB_MUSIC_CONTROL        0x04
//WB-6084 EVENT
#define WB_ACK 									0x00           //with 1 byte     SPK moudule shut down state 
#define WB_DEVICE_STATE         0X01           //with 2 bytes    byte 1: SPK module state   byte 2: special state
							    #define WB_POWER_OFF    0x00
									#define WB_UNDER_SEARCH 0x01
									#define WB_POWER_ON     0x02
									#define WB_PAIRED       0x03
									#define WB_UNPAIRED     0x04
#define WB_CALL_STATUS          0X02           //with 2 bytes    byte 1: sate base index    byte 2: call status
																								#define WB_IN_CALL      0X04
						
#define WB_CMD_COUNTER  14

#define WB_NO_CALL         0
#define WB_CALL_COME       2
#define WB_CALLING         4


extern void WB_play_pause(void);
extern u8 WB_call_status;             //ma 20160315
extern void WB_enter_pairing(void);
extern void WB_open_call(void);
extern void WB_close_call(void);
extern void WB_get_version(void);
extern u8 WB_paired_flag;        //ma 20151103 add BT paired flag  0:pairing 1:paired
extern u8 WB_paried_msg[3];   //ma 20151103
extern u8 WB_paired_back_flag;  //ma 20151103 add BT paired last time flag  0:pairing 1:paired
extern void WB_nolink_pairing(void);           //ma 20151111
extern void WB_music_speed(void);
extern void WB_next_song(void);
extern void WB_last_song(void);
extern void WB_stop_speed(void);
extern void WB_reject_call(void);
#endif


