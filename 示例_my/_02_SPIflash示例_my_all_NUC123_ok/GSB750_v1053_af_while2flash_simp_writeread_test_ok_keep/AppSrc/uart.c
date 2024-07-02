/****************************************************************************
 * @file:		uart.c
 * @version:			
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

#define UART_RX_BUF_SIZE	(1<<8)
#define UART_BUF_SIZE_MASK (UART_RX_BUF_SIZE -1)
#define UART_TX_BUF_SIZE	(1<<9)
#define UART_TX_BUF_MASK	(UART_TX_BUF_SIZE-1)

enum BTSTATE bt_state = BT_PAIRING;
static u8 UI_buf[UART_RX_BUF_SIZE];
// static u8 lex_buf[UART_RX_BUF_SIZE];
// static S32 column_offset = 0;
static u32 in_index = 0, out_index = 0;
static u8 	utx_buf[UART_TX_BUF_SIZE];
static u32 utx_in = 0, utx_out = 0;
static u32 txidle = TRUE;

//--static u32 lastkey = -1; //store last two code
// static u32 lastkey = 0xFFFFFFFF;                //ma 20151116 

struct cmd_tbl{
	enum BTSTATE cmd;
	u8* str;
};

// static struct cmd_tbl ATcmdtbl[] = {
// 		{BT_PAIR, "PI"},
// 		{BT_UNPAIRING, "UP"},
// 		{BT_CLRLOG,"CLRLOG"},
// 		{PRINT_LOG,"ELOG"},
// 		{DUMP_FLASH,"DUMP"}
// };

// static struct cmd_tbl INDcmdtbl[] = {
// 		{BT_REST,"IS"},
// 		{BT_PAIRING, "II"},
// 		{BT_PAIRED, "IJ"},
// 		{BT_CALLCOME,"IC"},
// 		{BT_CALLING,"ICI"},
// 		{BT_HANGUPED,"IU"}
// };

// static struct cmd_tbl SYScmdtbl[] = {
// 		{SYS_INFO,"INFO"}
// };

static u32 column = 0;
/*-------------------------------------------------------------------------
 * @fucname:	 uartSendCmd
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void uartSendCmd(u32 count, u8* buf)
{
	u32 i;	
	for(i = 0; i < count; i++){
		utx_buf[utx_in&UART_TX_BUF_MASK] = buf[i];
		utx_in++;
	}
	
	if(txidle == TRUE){
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
}

void iputc(u8 ch)
{
// #if(UART_EN)
	UART_DISABLE_INT(UART0, UART_IER_THRE_IEN);
	utx_buf[utx_in&UART_TX_BUF_MASK] = ch;
	utx_in++;
	UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	if(txidle == TRUE){
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
// #endif
}

void iputs(u8* buf)
{
// #if(UART_EN)
	u32 i = 0;

	UART_DISABLE_INT(UART0, UART_IER_THRE_IEN);
	for(i = 0;buf[i] != 0; i++){
		utx_buf[utx_in&UART_TX_BUF_MASK] = buf[i];
		utx_in++;
	}
	UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	if(txidle == TRUE){
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
// #endif
}

u8 igetc(void)
{
	return UI_buf[UART_BUF_SIZE_MASK&out_index++];
}

void iungetc(u8 ch)
{
	;
}


void clr(void)
{
	u32 i = 0;

	while(i < 80 ){
		iputc(0x0a);
		i++;
	}
		
	iputc(0x0d);
}
u32 itoa(u32 i, u8* buf)
{
	u32 temp, k, j=0;

	while(i>0){
		buf[j++] = i%10 + '0';
		i /= 10;
	}
	
	if(j==0){
		j=1;
		buf[0] = '0';
	}
	 
	buf[j] = 0;
	k = j;
	while(i<j){
		temp = buf[i];
		buf[i++] = buf[--j];
		buf[j] = temp;
	}
		
	return k;
}


u8* itoh(u8 byte, u8* buf)
{
	unsigned char temp;
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	temp = byte;
	buf[0]=HEXSTR[byte>>4];
	temp=temp<<4;
	temp=temp>>4;
	buf[1]=HEXSTR[temp];
	buf[2]='\0';
	return buf;
}
void uartdecode(u32 x, u8* str)
{
u8 buf[16];
u32 index;

index = itoa(x, &buf[0]);
for(; index<6; index++)
	buf[index] = ' ';
buf[index++] = 0;
iputs(str);
iputs(buf);
}

// static u32 atoi(u8* ptr8, u32 *v)
// {
// 	u8 cc;
// 	u32 value = 0;
// 	
// 	//--while((cc=*ptr8++)){
// 	while(  (cc=*ptr8++) !=0 ){
// 		if((cc > 0x40 && cc < 0x47) || (cc > 0x60 && cc < 0x67)){
// 			cc &= ~0x20;
// 			value *= 16;
// 			value += cc -0x41 + 0xA;
// 		}else if(cc >= 0x30 && cc <= 0x39){
// 			value *= 16;
// 			value += cc - 0x30;
// 		}else
// 			return 1;
// 	}
// 	*v = value;
// 	return 0;
// }

void dps()
{
	 iputs("GLSN$ ");
	 column = 0;
}


bool strncmp(u8 *src, u8 *des, u32 n)
{
	while(n--)
		if(src[n] != des[n])
			return 0;
	return 1;
}

bool strcmp(u8 *src, u8 *des)
{
	s32 i = 0;

	while(src[i] || des[i]){
		if(src[i] != des[i])
			return 0;
		i++;
	}

	return 1;
}
bool strcmp_last_3_bytes(u8 *src, u8 *des)     //ma 20151104
{
	s32 i = 0;
	s32 j = 0;
	while(src[i]){
		i++;
	}
	while(des[j]){
		j++;
	}
		
	if(src[i-1]==des[j-1])
		if(src[i-2]==des[j-2])
			if(src[i-3]==des[j-3])
				return 1;
	return 0;
}
u8* strncpy(u8 *src, u8 *des, u32 n)
{
	u32 i;
	for(i = 0; i < n; i++)
		des[i] = src[i];
	
	return des;
}

// static __inline bool isEffectChar(u8 cc)               //ma 20151116 
// {
// 	lastkey <<= 8;
// 	lastkey |= (u8)cc;
// 	if(cc >= 0x20 && cc <= 0x7e || cc == 0xa || cc == 0xd)
// 		return 1;
// 	return 0;
// }

void sendDevInfo()
{
	iputs("mcu: GSB750\n\r");
	iputs("PRJ: MultiRoom\n\r");
	iputs("VERSION: ");
	iputs(VERSION);
	iputs("\n\r");
}

void sendStartInfo()
{
// #if(UART_EN)
	clr();
	sendDevInfo();
	iputs("-----------------------\r\n\n");
	dps();
// #endif	
}

void sendInfo(u8 *str)
{
// #if(UART_EN)
	iputs(str);
	dps();
// #endif
}


// static enum BTSTATE findCmd(u8* ptr, u32 cmd_count, struct cmd_tbl *tbl)
// {
// 	U32 i;
// 	
// 	for(i = 0; i < cmd_count; i++){
// 		if(strcmp(ptr, tbl[i].str)) 
// 			return tbl[i].cmd;
// 	}
// 	
// 	return BT_UNDEF;
// }

// static enum BTSTATE lookUp()
// {	
// 	u8 *ptr = &lex_buf[0];

// 	if(strncmp(ptr, "AT#", 3) ){
// 		ptr += 3;
// 		return findCmd(ptr, ATCMD_COUNT, ATcmdtbl);
// 	}else if(strncmp(ptr, "IND#", 4)){
// 		ptr += 4;
// 		return findCmd(ptr, INDCMD_COUNT, INDcmdtbl);
// 	}else{
// 		return findCmd(ptr, SYSCMD_COUNT,SYScmdtbl);
// 	}

// }


// static u32 depart(u8* buf)
// {
// 	u8 cc;
// //--	u32 index = 0, length = 0;
// 	u32 index = 0;
// 	
// 	u32 value32 = 0;
// 	u32 i = 0, j = 0;
// 	u32 count = 0;

// 	
// 	for(; i <= column; i++){
// 		cc = buf[index++];
// 		if(cc == ' ' || cc == 0xd)
// 			cc = 0;
// 		if(cc == 0 && (i == 0 || lex_buf[j-1] == 0))
// 			continue;
// 		lex_buf[j++] = cc;
// 		if(cc == 0)
// 			count++;
// 	}
// 	lex_buf[j] = 0;

// 	index = 0;
// 	
// 	while(lex_buf[index++])
// 		;

// 	atoi(&lex_buf[index], &value32);

// 	return value32;
// }


extern u32 temp_buf[];
void dumpFlash(u32 addr, u32 flag)
{
	u8 *ptr8 = (u8*)&temp_buf[0];
	u8 buf[10];
	u32 i = 0, j = 0, index = 0;
	U32_BYTE cfg_data;
	static u32 addr_bak;

	 if(flash_mode == FLASH_PLAY_MUSIC){
	 	flash_mode = FLASH_CONFIG;
		playSFMusicEnd();
	 }
	
	if(flag){
		addr_bak += 0x100;
		addr = addr_bak;
	}else
		addr_bak = addr;

	cfg_data = *(U32_BYTE*)&addr;
	iputs(itoh(cfg_data.byte3,buf));
	iputs(itoh(cfg_data.byte2,buf));
	iputs(itoh(cfg_data.byte1,buf));
	iputs(itoh(cfg_data.byte0,buf));
	iputs("\r\n");
	spiFlashRead((addr&~0xff), 256,(u8*)&temp_buf[0]);
	for(j = 0; j < 16; j++){
		for(i = 0; i < 16; i++){
			iputs(itoh(ptr8[index++],buf));
			iputc(' ');
		}
		sysDelayMs(6);
		iputs("\r\n");
	}
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 analyse
 * @brief: 	 analyse the data in uart_buf
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// static void analyse(u32 index)
// {
// 	enum BTSTATE cmd;
// 	static enum BTSTATE cmd_bak = BT_UNDEF;
// 	u8 cc; 
// 	u32 i = 0, j = 0;
// 	u32 count = 0;
// 	
// 	for(; i <= column; i++){
// 		cc = UI_buf[(index++)&UART_BUF_SIZE_MASK];
// // 		if(cc == ' ' || cc == 0xd)               //MA 201511113
// // 			cc = 0;
// // 		if(cc == 0 && (i == 0 || lex_buf[j-1] == 0))
// // 			continue;
// 		lex_buf[j++] = cc;
// 		if(cc == 0)
// 			count++;
// 	}

// 	lex_buf[j] = 0;
// 	if(strncmp(lex_buf, "HX1", 3))          //MA 20151111
// 	{
// 		WB_paired_flag = 1;
// 	}
// 	else if(strcmp_last_3_bytes(lex_buf,WB_paried_msg))     //ma 20151103   set wb paired flag 
// 	{
// 		WB_paired_flag = 1;
// 	}
// 	if(i == 1){
// 		if(cmd_bak == PRINT_LOG){
// 			cmd = cmd_bak;
// 			goto L1;
// 		}

// 		if(cmd_bak == DUMP_FLASH){
// 			cmd = cmd_bak;
// 			goto L2;
// 		}
// 		
// 		dps();
// 		return;
// 	}
// 	
// 	cmd = lookUp();
// // 	if(cmd == BT_UNDEF){           //MA 201511113
// // 		iputs(&lex_buf[0]);
// // // 		iputs("\n\rUnkown cmd!\n\r\a");
// // 	}
// 	switch(cmd){
// 		case BT_PAIR:
// 			iputs("AT#PI\r\nPair cmd send\n\r");
// 			break;
// 		case BT_PAIRED:
// 			iputs("IND#IJ\\r\n Paired cmd recevied\n\r");
// 			break;
// 		case BT_PAIRING:
// 			iputs("IND#II\r\nPairing cmd recevied\n\r");
// 			break;
// 		case SYS_INFO:
// 			sendDevInfo();
// 			break;
// 		case PRINT_LOG: 
// 			cmd_bak = BT_UNDEF;
// L1:			
// 			if(cmd_bak == PRINT_LOG)
// 				printLog(depart(lex_buf),1);
// 			else
// 				printLog(depart(lex_buf),0);
// 			
// 			//cmd_bak == PRINT_LOG;
// 			cmd_bak = PRINT_LOG;
// 			break;
// 		case BT_CLRLOG:
// 			earseAllLog();
// 			break;
// 		case DUMP_FLASH:
// 			cmd_bak = BT_UNDEF;
// L2:			
// // 			if(cmd_bak == DUMP_FLASH)               //MA 20151017
// // 				dumpFlash(depart(lex_buf),1);
// // 			else
// // 				dumpFlash(depart(lex_buf),0);
// 			
// 			//cmd_bak == DUMP_FLASH;
// 			cmd_bak = DUMP_FLASH;
// 			break;
// 		default:
// 			break;
// 	}

// 	if(out_index == in_index)
// 		out_index = in_index = (in_index&UART_TX_BUF_MASK);
// 	cmd_bak = cmd;
// // 	dps();

// }
void WB_message_handele(u8 message)
{
	u8 message_h = message/16;
	u8 message_l = message%16;
	static u8 WB_message[20] ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	static u8 WB_flag = 0;
	static u8 message_index =0;
	u8 i = 0;
	u8 sum = 0xff;
	if(message == 0xaa)
	{
		iputs("\r\n");
		WB_flag = 1;
		message_index = 0;
	}
	if(WB_flag && (message_index<18))
	{
		WB_message[message_index] = message;
		message_index++;
	}
	for(i=1;i<7;i++)
	{
		sum+=WB_message[i];
	}
// 	if(sum == 0)
// 	{
		if(WB_message[3]== 0x01)
		{
			if(WB_message[4]== 0x06)
			{
				if(WB_paired_flag == 0)
					WB_paired_flag = 1;
			}
			else if(WB_message[4]== 0x08)
			{
				if(WB_paired_flag == 1)
					WB_paired_flag = 0;
			}
		}
		else if(WB_message[3]== 0x02)
		{
			if(WB_message[5]== 0x00)
			{
				WB_call_status = WB_NO_CALL;
			}
			else if(WB_message[5]== 0x02)
			{
				WB_call_status = WB_CALL_COME;
			}
			else if(WB_message[5]== 0x04)
			{
				WB_call_status = WB_CALLING;
			}
		}
		if(WB_message[2] == 0x0e && WB_message[3] == 0x1a && WB_message[5] == 0x0F )
		{
			if( WB_message[16] == 0x02 )
				bt_play_music_flag = 0;
			else if( WB_message[16] == 0x01)
				bt_play_music_flag = 1;
		}
// 		WB_flag = 0;
// 		message_index = 0;
// 	}
	if(message_index == 17)
	{
		WB_flag = 0;
		message_index = 0;
	}
	if(message_h<10)
	{
		iputc(message_h+'0');		
	}
	else if(message_h >= 10) 
	{
		iputc(message_h+'A'-10);
	}
	if(message_l<10)
	{
		iputc(message_l+'0');		
	}
	else if(message_l >= 10) 
	{
		iputc(message_l+'A'-10);
	}
	iputc(' ');
}
/*-------------------------------------------------------------------------
 * @fucname:	 sendBack
 * @brief: 	
 * @param: 	
 * @return: 	
 * @details: 	
 *-----------------------------------------------------------------------*/
void sendBack()
{
	u8 cc;
	
//	if(column == 0)
//		column_offset = out_index;
	
	while(out_index < in_index){
// 		if(column == 0)
// 			column_offset = out_index;
		
		cc = UI_buf[(out_index++)&UART_BUF_SIZE_MASK];
			
// 		if(cc == '\r'){	
// // 			iputs("\r\n");
// // 			analyse(column_offset);
// 		}else if(cc == 0){
// // 			iputs("\r\n");
// // 			analyse(column_offset);
// 		}else{
// 			column++;
// 			iputc(cc);        //ma 20151028
// 		}
		WB_message_handele(cc);
		
	}
// 	if(column_offset<out_index)
// 		analyse(column_offset); //ma	 20151103
}

/*-------------------------------------------------------------------------
 * @fucname:	 UART0_IRQHandler
 * @brief: 	 deal the communication with blue teeth module
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
	u8 cc;
//--	static u32 specail_key_flag = 0, vc = 0;
	static u32 vc = 0;
	
	if(UART_GET_RDA_INT(UART0) ||UART_GET_TOUT_INT(UART0)){
		// UART receive data
		while(UART_GET_RX_EMPTY(UART0) == 0){
			
			cc = UART0->RBR;
			
// 			if(cc == '\b'){                                      //ma 20151116 
// 				if(column == 0){
// 					iputc(0x7);
// 					return;
// 				}else{
// 					WB_paired_flag = 0;      //ma 20151103  clear bt paired flag
// 					iputs("\b \b");
// 					if(vc != 0)
// 						vc--;
// 					else{
// 						column--;
// 						in_index--;
// 						if(out_index >= in_index)
// 							out_index--;
// 					}
// 					return;
// 				}
// 			}else if(isEffectChar(cc)){
// 				if(cc == 0x5b && ((lastkey&0xff00) == 0x1b00))
// 					return;
// 				if((lastkey&0XFFFF00) == 0x1b5b00)
// 					if(cc >=0x41 && cc <= 0x44) // deal with ->, ^, <-,key
// 						return;
// 			}else
// 				return;

			 

			
			if( in_index > UART_RX_BUF_SIZE + out_index ||column > UART_BUF_SIZE_MASK){
				iputc(cc);
				vc++;
				if(cc == '\r'){
					vc = 0;
					UI_buf[(in_index-1)&UART_BUF_SIZE_MASK] = cc;
					out_index--;
				}
				return;
			}
			
			UI_buf[(in_index++)&UART_BUF_SIZE_MASK] = cc;
		}
			
	}else if(UART_GET_THRE_INT(UART0)){
		while((utx_out < utx_in) && (UART_GET_TX_FULL(UART0)==0)){
			UART0->THR = utx_buf[utx_out&UART_TX_BUF_MASK];
			utx_out++;
		}

		if(utx_out == utx_in){
			UART_DISABLE_INT(UART0, UART_IER_THRE_IEN);
			txidle = TRUE;
			utx_out = utx_in = utx_in&UART_BUF_SIZE_MASK;
		}
	}

}


/*-------------------------------------------------------------------------
 * @fucname:	 uartInit
 * @brief: 	 init uart baund rate and length of data bits, length of stop bit
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void uartInit(void)
{    
// #if(UART_EN)
	STR_UART_T sParam;
	u32 i;
	
	GPIO_UART0_RXTX_PC();//set PC.4 = RX, PC.5 =TX
	
	/* UART Setting */
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

    /* Set UART Configuration */
	DrvUART_Init(UART0,&sParam);

	NVIC_SetPriority (UART0_IRQn, 2);
    /* Enable UART Interrupt */
	DrvUART_EnableInt(UART0, UART_IER_TIMEOUT_COUNTER_EN|
    							UART_IER_RDA_IEN|UART_IER_RTO_IEN);

	for(i = 0; i<UART_RX_BUF_SIZE; i++){
		UI_buf[i] = 0;
	}

	for(i = 0; i<UART_TX_BUF_SIZE; i++){
		utx_buf[i] = 0;
	}

// 	column_offset = 0;
	in_index = out_index = 0;
	utx_in = utx_out = 0;
	txidle = TRUE;
// #endif
}


//=========================================================

void uartdecodeH(u32 x, u8* str)  
 {
	u8 itemp;	 
	u8 buf[16];
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  iputs(str);   
	
	itemp = (x&0x000000f0)>>4;
  buf[0]=HEXSTR[itemp];
	itemp = (x&0x0000000f);
  buf[1]=HEXSTR[itemp];
	buf[2]='\0';;
	
	iputs(buf); 
	 
 	iputs("\r\n");
 }
 
 void uartdecodeH32(u32 x, u8* str)
 {
	u8 itemp;	 
	u8 buf[16];
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  iputs(str);

	itemp = (x&0xf0000000)>>28;
  buf[0]=HEXSTR[itemp];
	itemp = (x&0x0f000000)>>24;
  buf[1]=HEXSTR[itemp];
	itemp = (x&0x00f00000)>>20;
  buf[2]=HEXSTR[itemp];
	itemp = (x&0x000f0000)>>16;
  buf[3]=HEXSTR[itemp];
	
	itemp = (x&0x0000f000)>>12;
  buf[4]=HEXSTR[itemp];
	itemp = (x&0x00000f00)>>8;
  buf[5]=HEXSTR[itemp];
	itemp = (x&0x000000f0)>>4;
  buf[6]=HEXSTR[itemp];
	itemp = (x&0x0000000f);
  buf[7]=HEXSTR[itemp];
	buf[8]='\0';;
	
	iputs(buf); 

 	iputs("\r\n");
 }


