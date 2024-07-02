/****************************************************************************
 * @file:		csr_module.c
 * @version:		
 * @breif:	csr module source file.
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

#define REQCMD_COUNT		34
#define INDCMD_COUNT		14
#define RSPCMD_COUNT		34

#define BT_UR	UART0
#define BT_RX_BUF_SIZE	(1<<6)
#define BT_RX_SIZE_MASK (BT_RX_BUF_SIZE -1)
#define BT_TX_BUF_SIZE	(1<<7)
#define BT_TX_BUF_MASK	(BT_TX_BUF_SIZE-1)



static u8 BT_RX_buf[BT_RX_BUF_SIZE];
static u8 lex_buf[BT_RX_BUF_SIZE];
static u32 BT_in_index = 0, BT_out_index = 0;
static u8  BT_TX_buf[BT_TX_BUF_SIZE];
static u32 BT_utx_in = 0, BT_utx_out = 0;
static u32 BT_txidle = TRUE;
static s16 gen_status = 0xffff;
static s16 hf_status = 0Xffff;
static s16 a2dp_status = 0Xffff;
static s16 soc_status = 0xffff;

struct BT_cmd_tbl{
	enum BTMSG cmd;
	const u16* str; 
};


//------------------=   ind message   =------------------//
static const u16 gen_init_sucess_ind[2] = {0x0400, 0x0000};
static const u16 gen_shutdown_complete_ind[2] = {0x0401, 0x0002};
static const u16 gen_pairing_status_ind[2] = {0x0402, 0x0002};
static const u16 gen_scan_enable_ind[2] = {0x0403, 0x0002};
static const u16 gen_connection_status_ind[2] = {0x0406, 0x0002};
static const u16 gen_sw_version_ind[2] = {0x0407, 0x0004};
static const u16 hf_status_ind[2] = {0x0420, 0x0002};
static const u16 hf_sco_status_ind[2] = {0x0421, 0x0002};
static const u16 hf_inband_ring_ind[2] = {0x0422, 0x0002};
static const u16 hf_ring_ind[2] = {0x0423, 0x0000};
static const u16 hf_volume_ind[2] = {0x0427, 0x0002};
static const u16 a2dp_status_ind[2] = {0x0440, 0x0002};
static const u16 a2dp_volume_ind[2] = {0x0441, 0x0002};
static const u16 avrcp_status_ind[2] = {0x0460, 0x0002};

//------------------=    req message   =------------------//
static const u16 gen_startup_req[2] = {0x0000, 0x0000};
static const u16 gen_shutdown_req[2] = {0x0001, 0x0000};
static const u16 gen_enter_pairing_req[2] = {0x0002, 0x0002};
static const u16 gen_reset_paired_list_req[2] = {0x0003,0x0000};
static const u16 gen_enter_test_mode_req[2] = {0x0004, 0x0002};
static const u16 gen_set_local_device_name_req[2] = {0x0006, 0x0012};
static const u16 gen_play_tone_req[2] = {0x0007,0x0002};
static const u16 gen_set_pio_req[2] = {0x0008,0x0004};
static const u16 gen_send_mfb_req[2] = {0x0009, 0x0002};
static const u16 gen_play_file_req[2] = {0x000a,0x0002};
static const u16 gen_set_tone_vol_req[2] = {0x000b, 0x0002}; 
static const u16 gen_connect_addr_req[2] = {0x000c, 0x0006};
static const u16 gen_stop_file_req[2] = {0x000d, 0x0000};
static const u16 gen_disconnect_req[2] = {0x000e,0x0000};
static const u16 gen_get_sw_version_req[2] = {0x000f,0x0000};
static const u16 hf_slc_connect_req[2] = {0x0020,0x0000};
static const u16 hf_slc_disconnect_req[2] = {0x0021,0x0000};
static const u16 hf_voice_dial_req[2] = {0x0022,0x0000};
static const u16 hf_last_number_redial_req[2] = {0x0023,0x0000};
static const u16 hf_answer_call_req[2] = {0x0024,0x0000};
static const u16 hf_reject_call_req[2] = {0x0025,0x0000};
static const u16 hf_hangup_call_req[2] = {0x0026,0x0000};
static const u16 hf_audio_transfer_req[2] = {0x0027,0x0000};
static const u16 hf_audio_mute_unmute_req[2] = {0x0028,0x0002};
static const u16 hf_change_volume_req[2] = {0x0029,0x0002};
static const u16 hf_get_volume_req[2] = {0x002a,0x0000};
static const u16 hf_get_status_req[2] = {0x002b,0x0000};
static const u16 a2dp_connect_req[2] = {0x0040,0x0002};
static const u16 a2dp_disconnect_req[2] = {0x0041,0x0000};
static const u16 a2dp_change_volume_req[2] = {0x0042, 0x0002};
static const u16 a2dp_get_volume_req[2] = {0x0043, 0x0000};
static const u16 a2dp_get_status_req[2] = {0x0044, 0x0000};
static const u16 a2dp_set_volume_req[2] = {0x0045, 0x0002};
static const u16 avrcp_send_cmd_req[2] = {0x0060, 0x0002};

//------------------=    rsp message   =------------------//
static const u16 gen_startup_rsp[2] = {0x0200, 0x0002};
static const u16 gen_shutdown_rsp[2] = {0x0201,0x0002};
static const u16 gen_enter_pairing_rsp[2] = {0x0202,0x0002};
static const u16 gen_reset_paired_list_rsp[2] = {0x0203,0x0002};
static const u16 gen_enter_test_mode_rsp[2] = {0x0204,0x0002};
static const u16 gen_set_local_device_name_rsp[2] = {0x0206,0x0002};
static const u16 gen_play_tone_rsp[2] = {0x0207, 0x0002};
static const u16 gen_set_pio_rsp[2] = {0x0208, 0x0002};
static const u16 gen_send_mfb_rsp[2] = {0x0209, 0x0002};
static const u16 gen_play_file_rsp[2] = {0x020a,0x0002};
static const u16 gen_set_tone_vol_rsp[2] = {0x020b, 0x0002};
static const u16 gen_connect_addr_rsp[2] = {0x020c, 0x0002};
static const u16 gen_stop_file_rsp[2] = {0x020d, 0x0002};
static const u16 gen_disconnect_rsp[2] = {0x020e,0x0002};
static const u16 gen_get_sw_version_rsp[2] = {0x020f,0x0002};
static const u16 hf_slc_connect_rsp[2] = {0x0220,0x0002};
static const u16 hf_slc_disconnect_rsp[2] = {0x0221,0x0002};
static const u16 hf_voice_dial_rsp[2] = {0x0222,0x0002};
static const u16 hf_last_number_redial_rsp[2] = {0x0223,0x0002};
static const u16 hf_answer_call_rsp[2] = {0x0224,0x0002};
static const u16 hf_reject_call_rsp[2] = {0x0225,0x0002};
static const u16 hf_hangup_call_rsp[2] = {0x0226,0x0002};
static const u16 hf_audio_transfer_rsp[2] = {0x0227,0x0002};
static const u16 hf_audio_mute_unmute_rsp[2] = {0x0228,0x0002};
static const u16 hf_change_volume_rsp[2] = {0x0229,0x0002};
static const u16 hf_get_volume_rsp[2] = {0x022a,0x0002};
static const u16 hf_get_status_rsp[2] = {0x022b,0x0002};
static const u16 a2dp_connect_rsp[2] = {0x0240,0x0002};
static const u16 a2dp_disconnect_rsp[2] = {0x0241,0x0002};
static const u16 a2dp_change_volume_rsp[2] = {0x0242, 0x0002};
static const u16 a2dp_get_volume_rsp[2] = {0x0243, 0x0002};
static const u16 a2dp_get_status_rsp[2] = {0x0244, 0x0002};
static const u16 a2dp_set_volume_rsp[2] = {0x0245, 0x0002};
static const u16 avrcp_send_cmd_rsp[2] = {0x0260, 0x0002};



static struct BT_cmd_tbl INDcmdtbl[INDCMD_COUNT] = {
		{GEN_INIT_SUCESS_IND,gen_init_sucess_ind},
		{GEN_SHUTDOWN_COMPLETE_IND,gen_shutdown_complete_ind},
		{GEN_PAIRING_STATUS_IND,gen_pairing_status_ind},
		{GEN_SCAN_ENABLE_IND,gen_scan_enable_ind},
		{GEN_CONNECTION_STATUS_IND,gen_connection_status_ind},
		{GEN_SW_VERSION_IND,gen_sw_version_ind},
		{HF_STATUS_IND,hf_status_ind},
		{HF_SCO_STATUS_IND,hf_sco_status_ind},
		{HF_INBAND_RING_IND,hf_inband_ring_ind},
		{HF_RING_IND,hf_ring_ind},
		{HF_VOLUME_IND,hf_volume_ind},
		{A2DP_STATUS_IND,a2dp_status_ind},
		{A2DP_VOLUME_IND,a2dp_volume_ind},
		{AVRCP_STATUS_IND,avrcp_status_ind},
};


static struct BT_cmd_tbl REQcmdtbl[REQCMD_COUNT] = {
		{GEN_STARTUP_REQ,gen_startup_req},
		{GEN_SHUTDOWN_REQ, gen_shutdown_req},
		{GEN_ENTER_PAIRING_REQ, gen_enter_pairing_req},
		{GEN_RESET_PAIRED_LIST_REQ,gen_reset_paired_list_req},
		{GEN_ENTER_TEST_MODE_REQ,gen_enter_test_mode_req},
		{GEN_SET_LOCAL_DEVICE_NAME_REQ,gen_set_local_device_name_req},
		{GEN_PLAY_TONE_REQ,gen_play_tone_req},
		{GEN_SET_PIO_REQ,gen_set_pio_req},
		{GEN_SEND_MFB_REQ,gen_send_mfb_req},
		{GEN_PLAY_FILE_REQ,gen_play_file_req},
		{GEN_SET_TONE_VOL_REQ,gen_set_tone_vol_req},
		{GEN_CONNECT_ADDR_REQ,gen_connect_addr_req},
		{GEN_STOP_FILE_REQ,gen_stop_file_req},
		{GEN_DISCONNECT_REQ,gen_disconnect_req},
		{GEN_GET_SW_VERSION_REQ,gen_get_sw_version_req},
		{HF_SLC_CONNECT_REQ,hf_slc_connect_req},
		{HF_SLC_DISCONNECT_REQ,hf_slc_disconnect_req},
		{HF_VOICE_DIAL_REQ,hf_voice_dial_req},
		{HF_LAST_NUMBER_REDIAL_REQ,hf_last_number_redial_req},
		{HF_ANSWER_CALL_REQ,hf_answer_call_req},
		{HF_REJECT_CALL_REQ,hf_reject_call_req},
		{HF_HANGUP_CALL_REQ,hf_hangup_call_req},
		{HF_AUDIO_TRANSFER_REQ,hf_audio_transfer_req},
		{HF_AUDIO_MUTE_UNMUTE_REQ,hf_audio_mute_unmute_req},
		{HF_CHANGE_VOLUME_REQ,hf_change_volume_req},
		{HF_GET_VOLUME_REQ,hf_get_volume_req},
		{HF_GET_STATUS_REQ,hf_get_status_req},
		{A2DP_CONNECT_REQ,a2dp_connect_req},
		{A2DP_DISCONNECT_REQ,a2dp_disconnect_req},
		{A2DP_CHANGE_VOLUME_REQ,a2dp_change_volume_req},
		{A2DP_GET_VOLUME_REQ,a2dp_get_volume_req},
		{A2DP_GET_STATUS_REQ,a2dp_get_status_req},
		{A2DP_SET_VOLUME_REQ,a2dp_set_volume_req},
		{AVRCP_SEND_CMD_REQ, avrcp_send_cmd_req},
};


static struct BT_cmd_tbl RSPcmdtbl[RSPCMD_COUNT] = {
		{GEN_STARTUP_RSP,gen_startup_rsp},
		{GEN_SHUTDOWN_RSP, gen_shutdown_rsp},
		{GEN_ENTER_PAIRING_RSP, gen_enter_pairing_rsp},
		{GEN_RESET_PAIRED_LIST_RSP,gen_reset_paired_list_rsp},
		{GEN_ENTER_TEST_MODE_RSP,gen_enter_test_mode_rsp},
		{GEN_SET_LOCAL_DEVICE_NAME_RSP,gen_set_local_device_name_rsp},
		{GEN_PLAY_TONE_RSP,gen_play_tone_rsp},
		{GEN_SET_PIO_RSP,gen_set_pio_rsp},
		{GEN_SEND_MFB_RSP,gen_send_mfb_rsp},
		{GEN_PLAY_FILE_RSP,gen_play_file_rsp},
		{GEN_SET_TONE_VOL_RSP,gen_set_tone_vol_rsp},
		{GEN_CONNECT_ADDR_RSP,gen_connect_addr_rsp},
		{GEN_STOP_FILE_RSP,gen_stop_file_rsp},
		{GEN_DISCONNECT_RSP,gen_disconnect_rsp},
		{GEN_GET_SW_VERSION_RSP,gen_get_sw_version_rsp},
		{HF_SLC_CONNECT_RSP,hf_slc_connect_rsp},
		{HF_SLC_DISCONNECT_RSP,hf_slc_disconnect_rsp},
		{HF_VOICE_DIAL_RSP,hf_voice_dial_rsp},
		{HF_LAST_NUMBER_REDIAL_RSP,hf_last_number_redial_rsp},
		{HF_ANSWER_CALL_RSP,hf_answer_call_rsp},
		{HF_REJECT_CALL_RSP,hf_reject_call_rsp},
		{HF_HANGUP_CALL_RSP,hf_hangup_call_rsp},
		{HF_AUDIO_TRANSFER_RSP,hf_audio_transfer_rsp},
		{HF_AUDIO_MUTE_UNMUTE_RSP,hf_audio_mute_unmute_rsp},
		{HF_CHANGE_VOLUME_RSP,hf_change_volume_rsp},
		{HF_GET_VOLUME_RSP,hf_get_volume_rsp},
		{HF_GET_STATUS_RSP,hf_get_status_rsp},
		{A2DP_CONNECT_RSP,a2dp_connect_rsp},
		{A2DP_DISCONNECT_RSP,a2dp_disconnect_rsp},
		{A2DP_CHANGE_VOLUME_RSP,a2dp_change_volume_rsp},
		{A2DP_GET_VOLUME_RSP,a2dp_get_volume_rsp},
		{A2DP_GET_STATUS_RSP,a2dp_get_status_rsp},
		{A2DP_SET_VOLUME_RSP,a2dp_set_volume_rsp},
		{AVRCP_SEND_CMD_RSP, avrcp_send_cmd_rsp},
};



static u32 column = 0;
/*-------------------------------------------------------------------------
 * @fucname:	 BTSendCmd
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void BTSendCmd(u32 count, u8* buf)
{
	u32 i;	
	for(i = 0; i < count; i++){
		BT_TX_buf[BT_utx_in&BT_TX_BUF_MASK] = buf[i];
		BT_utx_in++;
	}
	
	if(BT_txidle == TRUE){
		BT_txidle = FALSE;
		UART_ENABLE_INT(BT_UR, UART_IER_THRE_IEN);
	}
}


u8 BtGetc(void)
{
	return BT_RX_buf[BT_RX_SIZE_MASK&BT_out_index++];
}


void BtUngetc(u8 ch)
{
	;
}

static bool strncmp(u8 *src, const u8 *des, u32 n)
{
	while(n--)
		if(src[n] != des[n])
			return 0;
	return 1;
}

static bool strcmp(u8 *src, const u8 *des)
{
	s32 i = 0;

	while(src[i] || des[i]){
		if(src[i] != des[i])
			return 0;
		i++;
	}

	return 1;
}

static u8* strncpy(u8 *src, u8 *des, u32 n)
{
	while(n--)
		src[n] = des[n];
	
	return des;
}


static enum BTMSG findCmd(u8* ptr, u32 cmd_count, struct BT_cmd_tbl *tbl)
{
	U32 i;
	
	for(i = 0; i < cmd_count; i++){
		if(strncmp(ptr, (u8*)tbl[i].str,2)) 
			return tbl[i].cmd;
	}
	
	return BT_UNDEF;
}

static enum BTMSG lookUp( u8 *ptr)
{	
	enum BTMSG cmd;
	
	if((cmd = findCmd(ptr, INDCMD_COUNT, INDcmdtbl))== BT_UNDEF)
		cmd =  findCmd(ptr, RSPCMD_COUNT, RSPcmdtbl);
		
	return cmd;

}

/*------------------------------------------------------------------------
 * @fucname:	 analyse
 * @brief: 	 analyse the data in BT_buf
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static struct{
	enum BTMSG cmd;
	u16 cmd16;
	u16 length;
	u16 payload[32];
}bt_rx_msg;

static struct{
	enum BTMSG cmd;
	u16 length;
	u16 payload[32];
}bt_tx_msg;

static u32 cmd_index = 0;

static void dumpMsg()
{
	u16 *ptr16;
	u16 i;
	u8 buf[4];
	u8 *ptr8;
	
	i = bt_rx_msg.length;

	i = 2 + i/2;

	ptr16 = &bt_rx_msg.cmd16;

	iputs(" ");
	for(; i > 0; i--){
		ptr8 = (u8*)ptr16;
		iputs("0x");
		itoh(*(ptr8+1),buf);
		iputs(buf);
		itoh(*ptr8,buf);
		iputs(buf);
		iputc(' ');
		ptr16++;
	}
	sendInfo("\r\n");
}

static void BT_analyse()
{
	u8 cc; 
	u32 i = 0;

	// load cmd into cmd_list and analyse

	iputs("BTget: ");
	switch(bt_rx_msg.cmd){
		case GEN_INIT_SUCESS_IND:
			iputs("GEN_INIT_SUCESS_IND");
			dumpMsg();
			BTSendCmd(4,(u8*)gen_startup_req);
			sysDelayMs(10);
			break;	
		case GEN_SHUTDOWN_COMPLETE_IND:
			hf_status = 0;
			iputs("GEN_SHUTDOWN_COMPLETE_IND");
			dumpMsg();
			break;
		case GEN_PAIRING_STATUS_IND:
			iputs("GEN_PAIRING_STATUS_IND");
			dumpMsg();
			break;
		case GEN_SCAN_ENABLE_IND:
			iputs("GEN_SCAN_ENABLE_IND");
			dumpMsg();
			break;
		case GEN_CONNECTION_STATUS_IND:
			gen_status = bt_rx_msg.payload[0];
			iputs("GEN_CONNECTION_STATUS_IND");
			dumpMsg();
			if(gen_status == 2){
				sysDelayMs(100);
				btPairing();
			}
			break;
		case GEN_SW_VERSION_IND:
			iputs("GEN_SW_VERSION_IND");
			dumpMsg();
			break;
		case HF_STATUS_IND:
			hf_status = bt_rx_msg.payload[0];
			iputs("HF_STATUS_IND");
			dumpMsg();
			break;
		case HF_SCO_STATUS_IND:
			soc_status =  bt_rx_msg.payload[0];
			iputs("HF_SCO_STATUS_IND");
			dumpMsg();
			break;
		case HF_INBAND_RING_IND:
			iputs("HF_INBAND_RING_IND");
			dumpMsg();
			break;
		case HF_RING_IND:
			iputs("HF_RING_IND");
			dumpMsg();
			break;
		case HF_VOLUME_IND:
			iputs("HF_VOLUME_IND");
			dumpMsg();
			break;
		case A2DP_STATUS_IND:
			a2dp_status = bt_rx_msg.payload[0];
			iputs("A2DP_STATUS_IND");
			dumpMsg();
			break;
		case A2DP_VOLUME_IND:
			iputs("A2DP_VOLUME_IND");
			dumpMsg();
			break;
		case AVRCP_STATUS_IND:
			iputs("AVRCP_STATUS_IND");
			dumpMsg();
			break;
		case GEN_STARTUP_RSP:
			iputs("GEN_STARTUP_RSP");
			dumpMsg();
			btPairing();
			break;
		case GEN_SHUTDOWN_RSP:
			iputs("GEN_SHUTDOWN_RSP");
			dumpMsg();
			break;
		case GEN_ENTER_PAIRING_RSP:
			iputs("GEN_ENTER_PAIRING_RSP");
			dumpMsg();
			break;
		case GEN_RESET_PAIRED_LIST_RSP:
			iputs("GEN_RESET_PAIRED_LIST_RSP");
			dumpMsg();
			break;
		case GEN_ENTER_TEST_MODE_RSP:
			iputs("GEN_ENTER_TEST_MODE_RSP");
			dumpMsg();
			break;
		case GEN_SET_LOCAL_DEVICE_NAME_RSP:
			iputs("GEN_SET_LOCAL_DEVICE_NAME_RSP");
			dumpMsg();
			break;
		case GEN_PLAY_TONE_RSP:
			iputs("GEN_PLAY_TONE_RSP");
			dumpMsg();
			break;
		case GEN_SET_PIO_RSP:
			iputs("GEN_SET_PIO_RSP");
			dumpMsg();
			break;
		case GEN_SEND_MFB_RSP:
			iputs("GEN_SEND_MFB_RSP");
			dumpMsg();
			break;
		case GEN_PLAY_FILE_RSP:
			iputs("GEN_PLAY_FILE_RSP");
			dumpMsg();
			break;
		case GEN_SET_TONE_VOL_RSP:
			iputs("GEN_SET_TONE_VOL_RSP");
			dumpMsg();
			break;
		case GEN_CONNECT_ADDR_RSP:
			iputs("GEN_CONNECT_ADDR_RSP");
			dumpMsg();
			break;
		case GEN_STOP_FILE_RSP:
			iputs("GEN_STOP_FILE_RSP");
			dumpMsg();
			break;
		case GEN_DISCONNECT_RSP:
			iputs("GEN_DISCONNECT_RSP");
			dumpMsg();
			break;
		case GEN_GET_SW_VERSION_RSP:
			iputs("GEN_GET_SW_VERSION_RSP");
			dumpMsg();
			break;
		case HF_SLC_CONNECT_RSP:
			iputs("HF_SLC_CONNECT_RSP");
			dumpMsg();
			break;
		case HF_SLC_DISCONNECT_RSP:
			iputs("HF_SLC_DISCONNECT_RSP");
			dumpMsg();
			break;
		case HF_VOICE_DIAL_RSP:
			iputs("HF_VOICE_DIAL_RSP");
			dumpMsg();
			break;
		case HF_LAST_NUMBER_REDIAL_RSP:
			iputs("HF_LAST_NUMBER_REDIAL_RSP");
			dumpMsg();
			break;
		case HF_ANSWER_CALL_RSP:
			iputs("HF_ANSWER_CALL_RSP");
			dumpMsg();
			break;
		case HF_REJECT_CALL_RSP:
			iputs("HF_REJECT_CALL_RSP");
			dumpMsg();
			break;
		case HF_HANGUP_CALL_RSP:
			iputs("HF_HANGUP_CALL_RSP");
			dumpMsg();
			break;
		case HF_AUDIO_TRANSFER_RSP:
			iputs("HF_AUDIO_TRANSFER_RSP");
			dumpMsg();
			break;
		case HF_AUDIO_MUTE_UNMUTE_RSP:
			iputs("HF_AUDIO_MUTE_UNMUTE_RSP");
			dumpMsg();
			break;
		case HF_CHANGE_VOLUME_RSP:
			iputs("HF_CHANGE_VOLUME_RSP");
			dumpMsg();
			break;
		case HF_GET_VOLUME_RSP:
			iputs("HF_GET_VOLUME_RSP");
			dumpMsg();
			break;
		case HF_GET_STATUS_RSP:
			hf_status = bt_rx_msg.payload[0];
			iputs("HF_GET_STATUS_RSP");
			dumpMsg();
			break;
		case A2DP_CONNECT_RSP:
			iputs("A2DP_CONNECT_RSP");
			dumpMsg();
			break;
		case A2DP_DISCONNECT_RSP:
			iputs("A2DP_DISCONNECT_RSP");
			dumpMsg();
			break;
		case A2DP_CHANGE_VOLUME_RSP:
			iputs("A2DP_CHANGE_VOLUME_RSP");
			dumpMsg();
			break;
		case A2DP_GET_VOLUME_RSP:
			iputs("A2DP_GET_VOLUME_RSP");
			dumpMsg();
			break;
		case A2DP_GET_STATUS_RSP:
			a2dp_status = bt_rx_msg.payload[0];
			iputs("A2DP_GET_STATUS_RSP");
			dumpMsg();
			break;
		case A2DP_SET_VOLUME_RSP:
			iputs("A2DP_SET_VOLUME_RSP");
			dumpMsg();
			break;
		case AVRCP_SEND_CMD_RSP:
			iputs("AVRCP_SEND_CMD_RSP");
			dumpMsg();
			break;
		default:
			iputs("NON_DEF_CMD");
			dumpMsg();
			break;
	}

}

/*-------------------------------------------------------------------------
 * @fucname:	 dealBtError
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
dealBtError()
{
	BT_out_index = BT_in_index = 0;
}

/*------------------------------------------------------------------------
 * @fucname:	 getCmd
 * @brief: 	
 * @param: 	
 * @return: 	
 * @details: 	
 *-----------------------------------------------------------------------*/
void btGetMsg()
{
	u16 buf[2] = {0x0400, 0x0000};
	volatile u32 index;
	static u32 cmd_time = 0;
	static u32 time_clean = 1;
	struct S_msg{
		u16 cmd;
		u16 length;
	}cmd_msg;
	
	/* give whole cmd to BT_analyse, change u16 cmd to BTMSG cmd. */
	
	/* make sure BT_out_index is even */
	
	/*
		a whole message is not received whitin 10ms, means error happen!

		if error happen, should ask BT status(HF_status & GEN_status) to
		know what happened.
	*/

	//---------------------- code ----------------------------
	
	if(BT_in_index > BT_out_index)
		if(time_clean){
			cmd_time = systick_counter;
			time_clean = 0;
		}else{
			if(systick_counter > cmd_time + 10){
				dealBtError();	// dealwith bt uart error.
				time_clean = 1;
				sendInfo("BT uart time out\r\n");
				return;
			}
		}
	
	if(BT_in_index < BT_out_index +4)
		return;

	if((BT_out_index&0x1) == 0){
		cmd_msg.cmd = *(u16*)&BT_RX_buf[BT_out_index&BT_RX_SIZE_MASK];
		cmd_msg.length = *(u16*)&BT_RX_buf[(BT_out_index+2)&BT_RX_SIZE_MASK];
	}else{
		dealBtError();
		time_clean = 1;
		sendInfo("BT_out_index is not even\r\n");
		return;
	}

	if(BT_in_index < BT_out_index+4+cmd_msg.length)
		return;
	time_clean = 1;

	bt_rx_msg.cmd = lookUp((u8*)&cmd_msg.cmd);
	bt_rx_msg.cmd16 = cmd_msg.cmd;
	bt_rx_msg.length = cmd_msg.length;
	for(index = 0; index < cmd_msg.length/2; index++)
		bt_rx_msg.payload[index] = BT_RX_buf[(BT_out_index+4+index)&BT_RX_SIZE_MASK];
	
	BT_out_index += 4 + cmd_msg.length;
	
	if(BT_out_index > BT_RX_SIZE_MASK){
		UART_DISABLE_INT(BT_UR, UART_IER_RDA_IEN);
		BT_out_index &= BT_RX_SIZE_MASK;
		BT_in_index &= BT_RX_SIZE_MASK;
		UART_ENABLE_INT(BT_UR, UART_IER_RDA_IEN);
	}

	BT_analyse();
}


/*-------------------------------------------------------------------------
 * @fucname:	 BT_UR_IRQHandler
 * @brief: 	 deal the communication with blue teeth module
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
	u8 cc;
	
	if(UART_GET_RDA_INT(BT_UR) || UART_GET_TOUT_INT(BT_UR)){

		// BT receive data
		while(UART_GET_RX_EMPTY(BT_UR) == 0){
			
			cc = BT_UR->RBR;
			
			BT_RX_buf[(BT_in_index++)&BT_RX_SIZE_MASK] = cc;
			BT_in_index &= (BT_RX_BUF_SIZE<<1) -1;
		}

	}else if(UART_GET_THRE_INT(BT_UR)){
	
		while((BT_utx_out < BT_utx_in) && (UART_GET_TX_FULL(BT_UR)==0)){
			BT_UR->THR = BT_TX_buf[BT_utx_out&BT_TX_BUF_MASK];
			BT_utx_out++;
		}

		if(BT_utx_out == BT_utx_in){
			UART_DISABLE_INT(BT_UR, UART_IER_THRE_IEN);
			BT_txidle = TRUE;
			BT_utx_out = BT_utx_in = BT_utx_in&BT_TX_BUF_MASK;
		}
	}
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 BTInit
 * @brief: 	 	 init BT baund rate and length of data bits, length of stop bit
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void BTInit(void)
{    
	STR_UART_T sParam;
	u32 i;
	
	GPIO_UART0_RXTX_PC();//set PC.4 = RX, PC.5 =TX
	
	/* BT Setting */
	sParam.u32ClkSrc	= CLK_UART_HXT;
	sParam.u32ClkDiv	= 1;
	sParam.u32ClkFreq	= __XTAL;			//HXT/1 == __XTAL
	sParam.u32BaudRate = 115200;
	sParam.u32cDataBits = UART_WORD_LEN_8;
	sParam.u32cStopBits = UART_STOP_BIT_1;
	sParam.u32cParity 	= UART_PARITY_NONE;
	sParam.u32cRxTriggerLevel	= UART_FCR_RFITL_4BYTES;
	sParam.u32BaudMode    = UART_BAUD_MODE2;
	sParam.u8RxTimeOut	= 100;

    /* Set BT Configuration */
	DrvUART_Init(BT_UR,&sParam);

	NVIC_SetPriority (UART0_IRQn, 2);
    /* Enable BT Interrupt */
	DrvUART_EnableInt(BT_UR, UART_IER_TIMEOUT_COUNTER_EN|
    							UART_IER_RDA_IEN|UART_IER_RTO_IEN);

	for(i = 0; i<BT_RX_BUF_SIZE; i++){
		BT_RX_buf[i] = 0;
	}

	for(i = 0; i<BT_TX_BUF_SIZE; i++){
		BT_TX_buf[i] = 0;
	}

	BT_in_index = BT_out_index = 0;
	BT_utx_in = BT_utx_out = 0;
	hf_status = a2dp_status = 0xffff;
	BT_txidle = TRUE;
}

void closeCall(void)
{
	u16 buf[] = {0x0009,0x0002,0x0000};

	sendInfo("BTsend: MFB END CALL\r\n");
	BTSendCmd(6,(u8*)&buf[0]);
}
void openCall(void)
{
	u16 buf[] = {0x0009,0x0002,0x0000};

	sendInfo("BTsend: ANSWER CALL\r\n");
	BTSendCmd(6,(u8*)&buf[0]);
}

void btPairing(void)
{
	u16 buf[] = {0x0002,0x0002,0x0258};
	if(DECT_DC()) 
		buf[2] = 0xFFF;
	else
		buf[2] = 0x7FF;

	sendInfo("BTsend: Enter pairing\r\n");
	BTSendCmd(6,(u8*)&buf[0]);
}

void btStartUp(void)
{
	BTSendCmd(4,(u8*)&gen_startup_req[0]);	
}

void btShutDown(void)
{
	if(hf_status > 2){
		BTSendCmd(4,(u8*)&gen_disconnect_req[0]);
		sysDelayMs(200);
	}
	BTSendCmd(4,(u8*)&gen_shutdown_req[0]);
}

void btDiscontect(void)
{
	BTSendCmd(4,(u8*)&gen_disconnect_req[0]);
}


u32 callDetect(void)
{
	if(soc_status == 1)
		return TRUE;
	else
		return FALSE;
	
}

u32 isBtPaired(void)
{
	static u32 count = 0;
	
	if(gen_status > 2 || a2dp_status > 0)
		return TRUE;
	else{
		
		if(++count == 30){ 
			BTSendCmd(4,(u8*)&hf_get_status_req[0]);
			count = 0;
		}
		
		return FALSE;
	}
}

void avrcpSendCmd(u32 cmd)
{
	bt_tx_msg.cmd = 0x0060;
	bt_tx_msg.length = 0x0002;
	bt_tx_msg.payload[0] = cmd;
	
	switch(bt_tx_msg.payload[0]){
				case 0:
					iputs("A2DP_PLAY");
					break;
				case 1:
					iputs("A2DP_PAUSE");
					break;
				case 2:
					iputs("A2DP_STOP");
					break;
				case 3:
					iputs("A2DP FORWARD");
					break;
				case 4:
					iputs("A2DP BACKWARD");
					break;
				case 5:
					iputs("A2DP FF PRESS");
					break;
				case 6:
					iputs("A2DP FF RELEASE");
					break;
				case 7:
					iputs("A2DP REWIND PRESS");
					break;
				case 8:
					iputs("A2DP RDWIND RELEASE");
					break;
				case 9:
					iputs("A2DP PLAY/PAUSE");
					break;
				default:
					iputs("NON_DEF_CMD");
					break;
			
			}
	
	BTSendCmd(6,(u8*)&bt_tx_msg);
} 

void btNextMusic()
{
	avrcpSendCmd(3);
}

void btLastMusic()
{
	avrcpSendCmd(4);
}


void btChangeName()
{	
	u8 buf[22] = {0x06,0x00,12,0x00,0x07,0x00,'P','o','r','t','i','u','m',0,0,0,0,0,0,0};

	BTSendCmd(22,buf);
}

