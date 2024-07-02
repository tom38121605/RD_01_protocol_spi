/****************************************************************************
 * @file:		csr_module.h
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __csr_module_h
#define __csr_module_h
#include "sysinc.h"

void BTSendCmd(u8* buf);

enum BTMSG{
//------------------=   IND MESSAGE   =------------------//
	GEN_INIT_SUCESS_IND,
	GEN_POWER_IND,
	GEN_PAIRING_STATUS_IND,
	GEN_A2DP_IND,
	GEN_AVRCP_IND,
	GEN_SW_VERSION_IND,
	GEN_AMP_IND,           //MA 20150630  
	HF_STATUS_IND,
	GEN_HFP_LINK_LOST_IND,
	HF_INBAND_RING_IND,
	HF_RING_IND,
	HF_VOLUME_IND,
	GEN_A2DP_LINK_LOST_IND,
	BT_LOST,                      //MA 20150610
	A2DP_VOLUME_IND,
	AVRCP_STATUS_IND,
	AVRCP_AUDIO_IND,
	SYS_STATUS_IND,
	TWS_STATUS_IND,
	BLE_STATUS_IND,

//------------------=    RSP MESSAGE   =------------------//
	GEN_STARTUP_RSP,
	GEN_SHUTDOWN_RSP,
	GEN_ENTER_PAIRING_RSP,
	GEN_RESET_PAIRED_LIST_RSP,
	GEN_ENTER_TEST_MODE_RSP,
	GEN_SET_LOCAL_DEVICE_NAME_RSP,
	GEN_PLAY_TONE_RSP,
	GEN_SET_PIO_RSP,
	GEN_SEND_MFB_RSP,
	GEN_PLAY_FILE_RSP,
	GEN_SET_TONE_VOL_RSP,
	GEN_CONNECT_ADDR_RSP,
	GEN_STOP_FILE_RSP,
	GEN_DISCONNECT_RSP,
	GEN_GET_SW_VERSION_RSP,
	HF_SLC_CONNECT_RSP,
	HF_SLC_DISCONNECT_RSP,
	HF_VOICE_DIAL_RSP,
	HF_LAST_NUMBER_REDIAL_RSP,
	HF_ANSWER_CALL_RSP,
	HF_REJECT_CALL_RSP,
	HF_HANGUP_CALL_RSP,
	HF_AUDIO_TRANSFER_RSP,
	HF_AUDIO_MUTE_UNMUTE_RSP,
	HF_CHANGE_VOLUME_RSP,
	HF_GET_VOLUME_RSP,
	HF_GET_STATUS_RSP,
	A2DP_CONNECT_RSP,
	A2DP_DISCONNECT_RSP,
	A2DP_CHANGE_VOLUME_RSP,
	A2DP_GET_VOLUME_RSP,
	A2DP_GET_STATUS_RSP,
	A2DP_SET_VOLUME_RSP,
	AVRCP_SEND_CMD_RSP,
	
//------------------=    REQ MESSAGE   =------------------//
	GEN_STARTUP_REQ,
	GEN_SHUTDOWN_REQ,
	GEN_ENTER_PAIRING_REQ,
	GEN_RESET_PAIRED_LIST_REQ,
	GEN_ENTER_TEST_MODE_REQ,
	GEN_SET_LOCAL_DEVICE_NAME_REQ,
	GEN_PLAY_TONE_REQ,
	GEN_SET_PIO_REQ,
	GEN_SEND_MFB_REQ,
	GEN_PLAY_FILE_REQ,
	GEN_SET_TONE_VOL_REQ,
	GEN_CONNECT_ADDR_REQ,
	GEN_STOP_FILE_REQ,
	GEN_DISCONNECT_REQ,
	GEN_GET_SW_VERSION_REQ,
	HF_SLC_CONNECT_REQ,
	HF_SLC_DISCONNECT_REQ,
	HF_VOICE_DIAL_REQ,
	HF_LAST_NUMBER_REDIAL_REQ,
	HF_ANSWER_CALL_REQ,
	HF_REJECT_CALL_REQ,
	HF_HANGUP_CALL_REQ,
	HF_AUDIO_TRANSFER_REQ,
	HF_AUDIO_MUTE_UNMUTE_REQ,
	HF_CHANGE_VOLUME_REQ,
	HF_GET_VOLUME_REQ,
	HF_GET_STATUS_REQ,
	A2DP_CONNECT_REQ,
	A2DP_DISCONNECT_REQ,
	A2DP_CHANGE_VOLUME_REQ,
	A2DP_GET_VOLUME_REQ,
	A2DP_GET_STATUS_REQ,
	A2DP_SET_VOLUME_REQ,
	AVRCP_SEND_CMD_REQ,


	//----------------------------------------------//
	BT_UNEND,
	BT_UNDEF,
};


extern U32 bt_state;
extern  void closeCall(void);
extern void openCall(void);
extern void btStartUp(void);
extern void btShutDown(void);
extern void btPairing(void);
extern u32 callDetect(void);
extern u32 isBtPaired(void);
extern void btGetMsg(void);
extern void btChangeName(void);
extern u32 isBtStreaming(void);
extern u32 isPairing(void);
extern  u32 isTwsSearching(void);
extern u32 twsPaired(void);

extern void btReconnect(void);        // ma  20150519   declare  brReconnect 
extern void btStopPairing(void);      //ma 20150520  declare  btstoppairing


extern u8 bt_hfp_flag;              //ma 20150527  declare bt hfp flag 
extern u8 bt_reconnect_flag;   // ma 20150521 add bt reconnect flag 

extern void btEnaleHFP(void);        //ma 20150527
extern void btDisableHFP(void);    			 //ma 20150527

extern void bt_dis_last_connect(void);   //ma 20150527  dis last connect 
extern u8 bt_lost_flag;    //ma 20150610  add bt lost flag
extern void bt_name_read(void);     //ma 20150616 
extern void bt_name_send(u8 *bt_name);     //ma 20150616 
extern u8 bt_change_name_mode; //ma 20150616   add  bt_change_name_flag
/******read  bt name *****/
extern void bt_version_read(void);   //ma 20150618

// extern void BTSendCmd(u8* buf);       //ma 20150624

extern void btEnableMic(void);           //ma 20150629  declare   btEnableMic
extern void btDisableMic(void);           //ma 20150629  declare   btDisableMic

extern u32 isCallIn(void);                //ma 20150629  declare   isCallIn
extern void BTInit(void);                //ma 20150629  declare   BTInit




extern u8 bt_amp_start_flag;     //ma 20150630    add  bt_amp_start_flag to pause  10sec then play 
extern void btForgetAll(void);       //MA 20150805   add  bluetooth forget function 
#endif

