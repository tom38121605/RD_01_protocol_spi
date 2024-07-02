/****************************************************************************
 * @file:		wb6084.c
 * @version:		
 * @breif:		wb6084 source file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/

#include "sysinc.h"
u8 WB_paired_flag = 0;        //ma 20151103 add BT paired flag  0:pairing 1:paired
u8 WB_paired_back_flag = 0;  //ma 20151103 add BT paired last time flag  0:pairing 1:paired
u8 WB_paried_msg[3] = {0x48,0x58,0x31};   //ma 20151103
u8 WB_cmd[WB_CMD_COUNTER] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// u8 WB_cmd[] = "<  AA 00 03 02 00 5D AB";
u8 WB_call_status = WB_NO_CALL;             //ma 20160315  
void WB_MMI_commend(u8 mmi_cmd)
{
	WB_cmd[0] = WB_HEAD;
	WB_cmd[1] = MMI_LENGTH_H;
	WB_cmd[2] = MMI_LENGTH_L;
	WB_cmd[3] = WB_MMI_ACTION;
	WB_cmd[4] = HF_STATUS;
	WB_cmd[5] = mmi_cmd;
	WB_cmd[6] = 0x00-WB_cmd[5]-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
	uartSendCmd(MMI_LENGTH_L+4, WB_cmd);
}

// void WB_MMI_commend(u8 music_cmd)
// {
// 	WB_cmd[0] = MMI_HEAD;
// 	WB_cmd[1] = MMI_LENGTH_H;
// 	WB_cmd[2] = MMI_LENGTH_L;
// 	WB_cmd[3] = WB_MMI_ACTION;
// 	WB_cmd[4] = HF_STATUS;
// 	WB_cmd[5] = mmi_cmd;
// 	WB_cmd[6] = 0x00-WB_cmd[5]-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
// 	uartSendCmd(MMI_LENGTH_L+4, WB_cmd);
// }

void WB_enter_pairing()
{
	WB_MMI_commend(WB_PAIRING);
}
void WB_nolink_pairing()           //ma 20151111
{
	WB_MMI_commend(WB_NOLINK_PAIRING);
}
void WB_get_version()
{
	WB_cmd[0] = WB_HEAD;
	WB_cmd[1] = MMI_LENGTH_H;
	WB_cmd[2] = 0x01;
	WB_cmd[3] = WB_GET_VERSION;
	WB_cmd[4] = 0x00-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
	uartSendCmd(5, WB_cmd);
}

void WB_open_call()
{
	WB_MMI_commend(WB_ANSWER);
}
void WB_close_call()
{
	WB_MMI_commend(WB_HUNG_UP);
}
void WB_reject_call()
{
	WB_MMI_commend(WB_REJECT);
}
void WB_HF_connect()
{
	WB_MMI_commend(WB_HF_CONNECT);
}
void WB_play_pause()
{
	WB_MMI_commend(WB_PLAY_PAUSE);
}

void WB_music_speed()
{
	WB_cmd[0] = WB_HEAD;      //aa 00 01 04 00 01 fa
	WB_cmd[1] = MMI_LENGTH_H;
	WB_cmd[2] = 0x03;
	WB_cmd[3] = WB_MUSIC_CONTROL;
	WB_cmd[4] = HF_STATUS;
	WB_cmd[5] = 0x01;
	WB_cmd[6] = 0x00-WB_cmd[5]-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
	uartSendCmd(MMI_LENGTH_L+4, WB_cmd);
}

void WB_music_back()
{
	WB_cmd[0] = WB_HEAD;
	WB_cmd[1] = MMI_LENGTH_H;
	WB_cmd[2] = 0x03;
	WB_cmd[3] = WB_MUSIC_CONTROL;
	WB_cmd[4] = HF_STATUS;
	WB_cmd[5] = 0x03;
	WB_cmd[6] = 0x00-WB_cmd[5]-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
	uartSendCmd(MMI_LENGTH_L+4, WB_cmd);
}

void WB_next_song()
{
	WB_MMI_commend(WB_NEXT_SONG);
}
void WB_last_song()
{
	WB_MMI_commend(WB_LAST_SONG);
}

void WB_stop_speed()
{
	WB_cmd[0] = WB_HEAD;
	WB_cmd[1] = MMI_LENGTH_H;
	WB_cmd[2] = 0x03;
	WB_cmd[3] = WB_MUSIC_CONTROL;
	WB_cmd[4] = HF_STATUS;
	WB_cmd[5] = 0x00;
	WB_cmd[6] = 0x00-WB_cmd[5]-WB_cmd[4]-WB_cmd[3]-WB_cmd[2]-WB_cmd[1];
	uartSendCmd(MMI_LENGTH_L+4, WB_cmd);
}

