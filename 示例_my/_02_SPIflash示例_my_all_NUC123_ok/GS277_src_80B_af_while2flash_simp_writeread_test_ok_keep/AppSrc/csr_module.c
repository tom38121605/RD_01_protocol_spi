/****************************************************************************
 * @file:		csr_module.c
 * @version:		
 * @breif:	csr module source file.
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

#define REQCMD_COUNT		1
#define INDCMD_COUNT		12      //ma 20150610  change 10 to 11  //ma 20150630 to 12
#define RSPCMD_COUNT		1

#define BT_UR	UART0
#define BT_RX_BUF_SIZE	(1<<7)
#define BT_RX_SIZE_MASK (BT_RX_BUF_SIZE -1)
#define BT_TX_BUF_SIZE	(1<<6)
#define BT_TX_BUF_MASK	(BT_TX_BUF_SIZE-1)



static u8 BT_RX_buf[BT_RX_BUF_SIZE];
// static u8 lex_buf[BT_RX_BUF_SIZE];       //ma 20150630  no referenced
static u32 BT_in_index = 0, BT_out_index = 0;
static u8  BT_TX_buf[BT_TX_BUF_SIZE];
static u32 BT_utx_in = 0, BT_utx_out = 0;
static u32 BT_txidle = TRUE;
// static s8 gen_status = 0xff;           //ma 20150629  no  reference 
// static s8 pair_status = 0xff;               //ma 20150703  change signed char to unsigned char   //ma 20150728  del useless valume pair_status
static s8 hf_status = 0Xff;
static s8 a2dp_status = 0Xff;
static s8 pwr_status = 0xff;
static s8 audio_status = 0xff;

u8 bt_hfp_flag = 0;   //ma 20150527  add  HFP_flag  //ma 20150609  0: no hfp  1£º hfp  
u8 bt_lost_flag = 0;    //ma 20150610  add bt lost flag
struct BT_cmd_tbl{
	enum BTMSG cmd;
	const u8* str; 
	u8 state;
};

u8 bt_amp_start_flag =0;     //ma 20150630    add  bt_amp_start_flag to pause  10sec then play 
u8 bt_change_name_mode = 0; //ma 20150616   add  bt_change_name_flag
u8 bt_reconnect_flag = 0;   // ma 20150521 add bt reconnect flag 
//------------------=   ind message   =------------------//
static const u8 gen_init_sucess_ind[] = "INITOK";
static const u8 gen_version_ind[] = "VER";
static const u8 gen_pairing_status_ind[] = "PAIR";
static const u8 gen_power_ind[] = "PWR";
static const u8 gen_hfp_ind[] = "AG";
static const u8 gen_a2dp_ind[] = "PEER";
static const u8 gen_avrcp_ind[] = "AVRCP";
// static const u8 gen_iap_ind[] = "IAP";             //ma 20150629  no  reference  
// static const u8 gen_spp_ind[] = "SPP";             //ma 20150629  no  reference 
static const u8 gen_audio_ind[] = "AUDIO";
static const u8 gen_mpbs_ind[] = "AMP";          //   ma 20150630  change  apbs  to AMP
// static const u8 gen_call_ind[] = "CID";            //ma 20150629  no  reference 
// static const u8 gen_call_w_ind[] = "CWT";          //ma 20150629  no  reference 
// static const u8 gen_amp_ind[] = "MP";               //ma 20150629  no  reference 
// static const u8 gen_apbs_ind[] = "MPBS";            //ma 20150629  no  reference 
// static const u8 gen_mpav_ind[] = "ABSV";            //ma 20150629  no  reference 
// static const u8 gen_amsr_ind[] = "RATE";            //ma 20150629  no  reference 
static const u8 gen_a2dp_link_lost_ind[] = "ALOST";
static const u8 gen_hfp_link_lost_ind[] = "HLOST";
// static const u8 gen_a2dp_volume_up[] = "VS+";      //ma 20150629  no  reference 
// static const u8 gen_a2dp_volume_down[] = "VS-";     //ma 20150629  no  reference 
// static const u8 gen_hfp_volume_ind[] = "VS";        //ma 20150629  no  reference 
static const u8 bt_lost_ind[] = "LOST";         //MA 20150610
//------------------=    req message   =------------------//




//------------------=    rsp message   =------------------//






static struct BT_cmd_tbl INDcmdtbl[INDCMD_COUNT] = {
		{GEN_INIT_SUCESS_IND,gen_init_sucess_ind},
		{GEN_POWER_IND,gen_power_ind},
		{GEN_PAIRING_STATUS_IND,gen_pairing_status_ind},
		{GEN_SW_VERSION_IND,gen_version_ind},
    {GEN_AMP_IND,gen_mpbs_ind},            // ma 20150630  add AMP for  bt pause or play
		{HF_STATUS_IND,gen_hfp_ind},
		{GEN_A2DP_IND,gen_a2dp_ind},
		{GEN_AVRCP_IND,gen_avrcp_ind},
		{GEN_HFP_LINK_LOST_IND,gen_hfp_link_lost_ind},
		{GEN_A2DP_LINK_LOST_IND,gen_a2dp_link_lost_ind},
		{AVRCP_AUDIO_IND,gen_audio_ind},
		{BT_LOST,bt_lost_ind},    //MA 20150610
};


// static struct BT_cmd_tbl REQcmdtbl[REQCMD_COUNT] = {{AVRCP_STATUS_IND,gen_avrcp_ind},};       //ma 20150629  no  reference 


// static struct BT_cmd_tbl RSPcmdtbl[RSPCMD_COUNT] = {{AVRCP_STATUS_IND,gen_avrcp_ind},};        //ma 20150629  no  reference 



// static u32 column = 0;    //ma 20150630 no reference this .c
/*-------------------------------------------------------------------------
 * @fucname:	 BTSendCmd
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void BTSendCmd(u8* buf)
{
	u32 i;	
	for(i = 0; buf[i] != 0; i++){
		BT_TX_buf[BT_utx_in&BT_TX_BUF_MASK] = buf[i];
		BT_utx_in++;
	}
	BT_TX_buf[BT_utx_in&BT_TX_BUF_MASK] = 0x0d;
	BT_utx_in++;
	
	if(BT_txidle == TRUE){
		BT_txidle = FALSE;
		UART_ENABLE_INT(BT_UR, UART_IER_THRE_IEN);
	}
}


void closeCall(void)
{
// 	sendInfo("BTsend: hang up call\r\n");                //ma 20150928  do  not send info for saving space
	if(hf_status == 4) //inooming call
		BTSendCmd("AT+CR");
	else  // outgoing call
		BTSendCmd("AT+CH");
}
void openCall(void)
{
// 	sendInfo("BTsend: Answer call\r\n");                 //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+CA");
}

void btPairing(void)
{
// 	sendInfo("BTsend: Enter pairing\r\n");              //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+PAIR=600");
}

void btStopPairing(void)
{
	sendInfo("BTsend: Enter pairing\r\n");
	BTSendCmd("AT+PAIR=0");
}

void btStartUp(void)
{
// 	sendInfo("BTsend: POWER ON\r\n");                        //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+ON");
}

void btShutDown(void)
{
// 	sendInfo("BTsend: POWER OFF\r\n");                        //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+OFF");
}


void btDiscontect(void)
{
	sendInfo("BTsend: disconnect all profies\r\n");
	//BTSendCmd("AT+AGD");
	//sysDelayMs(20);
	BTSendCmd("AT+GATTD");
}
/*****dis last connect *****/ 
void bt_dis_last_connect()   //ma 20150527  dis last connect       
{
// 	sendInfo("BTsend: dis last connect\r\n");            //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+AGD1");
}

void btReconnect(void)
{
// 	sendInfo("BTsend: reconnect to last device\r\n");       //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+AGC");
}


void btEnableMic()
{
// 	sendInfo("BTsend: MIC enable\r\n");           //ma 20150928  do  not send info for saving space        
	BTSendCmd("AT+MIC=1");
}

void btDisableMic()
{
// 	sendInfo("BTsend: MIC disable\r\n");         //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+MIC=0");
}

/******add HFP CONTROL*********/
/***set hfp need bt restart ***/
void btEnaleHFP()        //ma 20150527
{
// 	sendInfo("BTsend: HFP enable\r\n");            //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+SOCIAL=0\r");
}
void btDisableHFP()     //ma 20150527
{
// 	sendInfo("BTsend: HFP disable\r\n");          //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+SOCIAL=1\r");
}

void btForgetAll()       //MA 20150805   add  bluetooth forget function 
{
// 	sendInfo("BTsend: BT forget\r\n");                 //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+DFLT");	
}

u32 callDetect(void)
{
	if(hf_status > 3)
		return TRUE;
	else
		return FALSE;
	
}

u32 isCallIn()
{
	if(hf_status == 4)
		return TRUE;
	else
		return FALSE;
}

u32 isBtPaired(void)
{
	
	if(hf_status > 3 || a2dp_status > 2)
		return TRUE;
	else	
		return FALSE;
	
}


u32 isBtStreaming()
{
	static u32 count = 0;

	if(++count == 200){ 
	//	BTSendCmd("AT+PAIR=?");
		sysDelayMs(2);
		count = 0;
	} 
	
	if(audio_status > 0)
		return 1;
	else
		return 0;
}
 

void btNextMusic()
{
	sendInfo("BTsend: FWD\r\n");
	BTSendCmd("AT+FWD");
}

void btLastMusic()
{
	sendInfo("BTsend: BWD\r\n");
	BTSendCmd("AT+BWD");
}


void btPlay()
{
	sendInfo("BTsend: PLAY\r\n");
	BTSendCmd("AT+PLAY");
}

void btPause()
{
	sendInfo("BTsend: PAUSE\r\n");
	BTSendCmd("AT+PAUSE");
}
/******read  bt name *****/
void bt_name_read()
{
	sendInfo("BTsend: read bt name\r\n");
	BTSendCmd("AT+NAME");
}
/******read  bt name *****/
void bt_version_read()
{
// 	sendInfo("BTsend: read bt version\r\n");            //ma 20150928  do  not send info for saving space
	BTSendCmd("AT+VER");
}

/*****write bt name*****/
void bt_name_send(u8 *bt_name)
{
// 	sendInfo("BTsend: read bt name\r\n");                //ma 20150928  do  not send info for saving space
	BTSendCmd(bt_name);
}
void btChangeName()
{	
	
}



u8 BtGetc(void)
{
	return BT_RX_buf[BT_RX_SIZE_MASK&BT_out_index++];
}


void BtUngetc(u8 ch)
{
	;
}

// static bool strncmp(u8 *src, const u8 *des, u32 n)     //ma 20150629   no referenced
// {
// 	while(n--)
// 		if(src[n] != des[n])
// 			return 0;
// 	return 1;
// }

static bool istrcmp(u8 *src, const u8 *des)
{
	s32 i = 0;

	while(des[i]){
		if(src[i] != des[i])
			return 0;
		i++;
	}

	return 1;
}

// static u8* strncpy(u8 *src, u8 *des, u32 n)          //ma 20150629   no referenced
// {
// 	while(n--)
// 		src[n] = des[n];
// 	
// 	return des;
// }


static enum BTMSG findCmd(u8* ptr, u32 cmd_count, struct BT_cmd_tbl *tbl)
{
	U32 i;
	
	for(i = 0; i < cmd_count; i++){
		if(istrcmp(ptr, (u8*)tbl[i].str)) 
			return tbl[i].cmd;
	}
	
	return BT_UNDEF;
}

static enum BTMSG lookUp( u8 *ptr)
{	
	enum BTMSG cmd;
	
	cmd = findCmd(ptr, INDCMD_COUNT, INDcmdtbl);
		
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
	u8 payload[32];
}bt_rx_msg;

// static struct{              //ma 20150629  no referenced
// 	enum BTMSG cmd;
// 	u16 length;
// 	u16 payload[32];
// }bt_tx_msg;

static u32 cmd_index = 0;

static void dumpMsg()
{
	u32 i = 0;
	while((bt_rx_msg.payload[i]<='Z' && bt_rx_msg.payload[i]>='A') 
		|| bt_rx_msg.payload[i] == '2')
		i++;
		cmd_index = i;
}

static void BT_analyse()
{
// 	u8 cc;       //ma 20150629  
// 	u32 i = 0;      //ma 20150629  
// 	static u32 ct = 0;     //ma 20150629  
	// load cmd into cmd_list and analyse

	
	switch(bt_rx_msg.cmd){
		case GEN_INIT_SUCESS_IND:
				sysDelayMs(10);
		if(dev_input == BLUETOOTH)   //MA 20150724    deal power on not at bt paried phone can connect issue
 				btStartUp();        
				sysDelayMs(20);      //ma  20150521
			break;	
		case GEN_POWER_IND:
			dumpMsg();
			if(bt_rx_msg.payload[cmd_index] == '=')
				pwr_status = bt_rx_msg.payload[cmd_index+1]-'0';
			/*****when bt poweron  bt reconnect******/
			if(pwr_status == 1)	     //ma 20150527
			{
 				if(NFC_flag == 0&&dev_input == BLUETOOTH&&(!factory_mode_flag))	   // ma 20150527	  //MA 20150609
				{						
					btReconnect();        //MA   AT+AGC
					sysDelayMs(20);      //ma  20150521
					ccp = 0;   						//ma 20150525 reset ccp 
					bt_paring_flag = 1;
				}
				else if(factory_mode_flag)      //ma 20150928   fatory test mode do not reconnect 
				{
					btPairing();
					sysDelayMs(20);    
					ccp = 0;   						
					bt_paring_flag = 1;
				}
// 				else                                   //ma 20150805
// 				{
// 					btPairing();
// 					sysDelayMs(20);      //ma  20150521
// 				}
// 				else if(NFC_flag ==1&&bt_paring_flag == 0)
// 				{
// 					bt_dis_last_connect();    //ma 20150527
// 					sysDelayMs(20);      //ma  20150521
// 					btPairing();        //MA				
// 					sysDelayMs(20);      //ma  20150521
// 					bt_paring_flag =1;
// 					ccp = 251;	       //ma 20150609
// 				}
			}
			break;
		case GEN_PAIRING_STATUS_IND:
			dumpMsg();
// 			if(bt_rx_msg.payload[cmd_index] == '=')                            //ma 20150728  del useless valume pair_status
// 				pair_status = bt_rx_msg.payload[cmd_index+1]-'0';
			break;
		case GEN_SW_VERSION_IND:
			if(bt_rx_msg.payload[cmd_index] == '=')
			{
				BT_VERSION_NUMBER[0] = bt_rx_msg.payload[cmd_index+1];
				BT_VERSION_NUMBER[2] = bt_rx_msg.payload[cmd_index+3];
			}
			dumpMsg();
			break;
		case GEN_AMP_IND:
			dumpMsg();
			if(bt_rx_msg.payload[cmd_index] == '='){
				
				/********* play music handle *****/ 
				if(bt_rx_msg.payload[cmd_index+1]== '1')
				{
					bt_amp_start_flag =1;    //ma 20150630   set  bt amp start flag  
				}
			}
			break;
		case HF_STATUS_IND:
			dumpMsg();
			if(bt_rx_msg.payload[cmd_index+1] == '='){
				hf_status = bt_rx_msg.payload[cmd_index+2]-'0';
				a2dp_status = bt_rx_msg.payload[cmd_index+4]-'0';
			}
			break;

		case GEN_A2DP_IND:
			dumpMsg();
			if(bt_rx_msg.payload[cmd_index] == '='){
				a2dp_status = bt_rx_msg.payload[cmd_index+1]-'0';
			}
			break;
		case GEN_HFP_LINK_LOST_IND:
		case GEN_A2DP_LINK_LOST_IND:
		case BT_LOST:
			bt_lost_flag = 1;    //ma 20150610  add bt lost flag
			break;
		case AVRCP_AUDIO_IND:
			dumpMsg();
			if(bt_rx_msg.payload[cmd_index] == '=')
				audio_status = bt_rx_msg.payload[cmd_index+1]-'0';
			break;
		default:
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
#define ON 1
#define OFF 0
u32 time_clean = OFF;
void btGetMsg()
{
// 	u8 *ptr8;    //ma 20150629  
	u32 index, i = 0;
	
// 	u32  j =0;     //ma 20150629  
// 	static u32 cmd_time = 0;     //ma 20150629  
// 	struct S_msg{
// 		u16 cmd;
// 		u16 length;
// 	}cmd_msg;        //ma 20150629   no reference  here 

	//---------------------- code ----------------------------

	
	if(BT_in_index > BT_out_index+1){
		if(BT_RX_buf[BT_out_index&BT_RX_SIZE_MASK] == 0x0d
		  && BT_RX_buf[(BT_out_index+1)&BT_RX_SIZE_MASK] == 0x0a){
			for(i = 0, index = BT_out_index+2; index < BT_in_index-1 ; index++){
				
				bt_rx_msg.payload[i++] = BT_RX_buf[index&BT_RX_SIZE_MASK];
				if(BT_RX_buf[index&BT_RX_SIZE_MASK] == 0x0d && BT_RX_buf[(index+1)&BT_RX_SIZE_MASK] == 0x0a){
					bt_rx_msg.payload[--i] = 0;
					BT_out_index = index+2;
					iputs("BT get msg: ");
					iputs(bt_rx_msg.payload);
					goto L1;
				}
			}					
			return;
		}else{
			BT_out_index += 2;
			return;
		}
		
	}else
		return;
	
L1:
	bt_rx_msg.cmd = lookUp(bt_rx_msg.payload);
	if(BT_out_index > BT_RX_SIZE_MASK){
			UART_DISABLE_INT(BT_UR, UART_IER_RDA_IEN);
			BT_out_index &= BT_RX_SIZE_MASK;
			BT_in_index &= BT_RX_SIZE_MASK;
			UART_ENABLE_INT(BT_UR, UART_IER_RDA_IEN);
	}
	sendInfo("\r\n");
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
			//BT_in_index &= (BT_RX_BUF_SIZE<<1) -1;
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
	hf_status = a2dp_status = 0xff;      //ma 20150629   change 0xffff   to 0xff
	BT_txidle = TRUE;
	time_clean = OFF;
}

 

