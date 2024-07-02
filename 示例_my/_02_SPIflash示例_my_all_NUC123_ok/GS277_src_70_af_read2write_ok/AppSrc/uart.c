/****************************************************************************
 * @file:		uart.c
 * @version:			
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

#define UATCMD_COUNT		7
#define UINDCMD_COUNT		6
#define USYSCMD_COUNT		3

#define UART_RX_BUF_SIZE	(1<<8)
#define UART_BUF_SIZE_MASK (UART_RX_BUF_SIZE -1)
#define UART_TX_BUF_SIZE	(1<<9)
#define UART_TX_BUF_MASK	(UART_TX_BUF_SIZE-1)

static u8 UI_buf[UART_RX_BUF_SIZE];
// static u8 lex_buf[UART_RX_BUF_SIZE];               //ma 20150703  del   useless function for aprom
// static S32 column_offset = 0;                    //ma 20150707
static u32 in_index = 0, out_index = 0;
static u8 	utx_buf[UART_TX_BUF_SIZE];
static u32 utx_in = 0, utx_out = 0;
static u32 txidle = TRUE;
static u32 lastkey = 0xffffffff; //store last two code     //ma 201050630  change -1 to 0xffff


enum SYSCMD{
	BT_PAIRED,
	BT_PAIRING,
	BT_UNPAIRING,
	BT_GETCALL,
	BT_HANGUP,
	BT_WRITEB,
	BT_WRITEW,
	BT_WRITED,
	SMSC_WB,
	BT_REST,
	BT_CALLING,
	BT_CALLCOME,
	BT_HANGUPED,
	SYS_READ_VAR,
	SYS_SET_VAR,
	SYS_INFO,
};

struct cmd_tbl{
	enum SYSCMD cmd;
	u8* str;
};

// static struct cmd_tbl ATcmdtbl[UATCMD_COUNT] = {                         //ma 20150703  del   useless function for aprom
// 		{BT_UNPAIRING, "UP"},
// 		{BT_GETCALL,"CI"},
// 		{BT_HANGUP,"HU"},
// 		{BT_WRITEB,"BWB"},
// 		{BT_WRITEW,"BWW"},
// 		{BT_WRITED,"BWD"},
// 		{SMSC_WB,"SWB"},
// };

// static struct cmd_tbl INDcmdtbl[UINDCMD_COUNT] = {                        //ma 20150703  del   useless function for aprom
// 		{BT_REST,"IS"},
// 		{BT_PAIRING, "II"},
// 		{BT_PAIRED, "IJ"},
// 		{BT_CALLCOME,"IC"},
// 		{BT_CALLING,"ICI"},
// 		{BT_HANGUPED,"IU"}
// };

// static struct cmd_tbl SYScmdtbl[USYSCMD_COUNT] = {                          //ma 20150703  del   useless function for aprom
// 		{SYS_INFO,"INFO"},
// 		{SYS_SET_VAR,"WRITE"},
// 		{SYS_READ_VAR,"READ"}
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
		UART_ENABLE_INT(UART1, UART_IER_THRE_IEN);
	}
}


void smscSendCmd(u32 count, u8 *buf)
{

}


void iputc(u8 ch)
{
#if(UART_EN)
	utx_buf[utx_in&UART_TX_BUF_MASK] = ch;
	utx_in++;

	if(txidle == TRUE){
		txidle = FALSE;
		UART_ENABLE_INT(UART1, UART_IER_THRE_IEN);
	}
#endif
	//SendChar(ch);
}


void iputs(u8* buf)
{

#if(UART_EN)	
  u32 i = 0;
	for(i = 0;buf[i] != 0; i++){
		utx_buf[utx_in&UART_TX_BUF_MASK] = buf[i];
		utx_in++;
	}

	if(txidle == TRUE){
		txidle = FALSE;
		UART_ENABLE_INT(UART1, UART_IER_THRE_IEN);
	}
#endif
	//for(i = 0;buf[i] != 0; i++){
	//	SendChar(buf[i]);
	//}

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
u8* strncpy(u8 *src, u8 *des, u32 n)     
{
	while(n--)
		des[n] = src[n];
	
	return des;
}

static __inline bool isEffectChar(u8 cc)
{
	lastkey <<= 8;
	lastkey |= (u8)cc;
	if(cc >= 0x20 && cc <= 0x7e || cc == 0xa || cc == 0xd)
		return 1;
	return 0;
}

void sendDevInfo()
{
	iputs("mcu: cortex-M0 GS123\n\r");
	iputs("PRJ: THEone\n\r");
	iputs("VERSION: ");
	iputs("v1.0");
	iputs("\n\r");
}

void sendStartInfo()
{
	clr();
	sendDevInfo();
	iputs("-----------------------\r\n\n");
	dps();
}

void sendInfo(u8 *str)
{
	iputs(str);
	dps();
}


// static enum SYSCMD findCmd(u8* ptr, u32 cmd_count, struct cmd_tbl *tbl)          //ma 20150703  del   useless function for aprom
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

// static enum SYSCMD lookUp()
// {	
// 	u8 *ptr = &lex_buf[0];

// 	if(strncmp(ptr, "AT#", 3) ){
// 		ptr += 3;
// 		return findCmd(ptr, UATCMD_COUNT, ATcmdtbl);
// 	}else if(strncmp(ptr, "IND#", 4)){
// 		ptr += 4;
// 		return findCmd(ptr, UINDCMD_COUNT, INDcmdtbl);
// 	}else{
// 		return findCmd(ptr, USYSCMD_COUNT,SYScmdtbl);
// 	}

// }

/*-------------------------------------------------------------------------
 * @fucname:	 atoi
 * @brief: 
 * @param: 
 * @return: 
 * @details: 	not detect overflow
 *-----------------------------------------------------------------------*/
// static u32 atoi(u8* ptr8, u32 *v)                                            //ma 20150703  del   useless function for aprom
// {
// 	u8 cc;
// 	u32 value = 0;
// 	
// 	while((cc=*ptr8++)){
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

/*-------------------------------------------------------------------------
 * @fucname:	 depart
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// static u32 depart(u8* buf, u32 count, u32 type)                                         //ma 20150703  del   useless function for aprom
// {
// // 	u8 cc;    	//ma 20150629 
// 	u32 i = 0, index = 0, length = 0;
// // 	u8 value8;   //ma 20150629 
// // 	u16 value16;  //ma 20150629 
// 	u32 value32 = 0;

// 	while(buf[index++])
// 		;
// 	for(i = 0; count > 1; count-- ){
// 		switch(type){
// 			case 0:
// 				if(atoi(&buf[index],&value32) || value32 > 0xff)
// 					return 0xffff;
// 				buf[i] = value32&0xff;
// 				length++;
// 				break;
// 			case 1:
// 				if(atoi(&buf[index],&value32) || value32 > 0xffff)
// 					return 0xffff;
// 				((u16*)buf)[i] = value32&0xffff;
// 				length += 2;
// 				break;
// 			case 2:
// 				if(atoi(&buf[index],&value32))
// 					return 0xffff;
// 				((u32*)buf)[i] = value32;
// 				length += 4;
// 				break;
// 			default:
// 				break;
// 		}
// 		i++;
// 		while(buf[index++])
// 			;
// 	}

// 	return length;
// }

/*-------------------------------------------------------------------------
 * @fucname:	 analyse
 * @brief: 	 analyse the data in uart_buf
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// static void analyse(u32 index)                                                        //ma 20150703  del   useless function for aprom
// {
// 	enum SYSCMD cmd;
// 	u8 cc; 
// 	u32 i = 0, j = 0;
// 	u32 count = 0;

// 	
// 	for(; i <= column; i++){
// 		cc = UI_buf[(index++)&UART_BUF_SIZE_MASK];
// 		if(cc == ' ' || cc == 0xd)
// 			cc = 0;
// 		if(cc == 0 && (i == 0 || lex_buf[j-1] == 0))
// 			continue;
// 		lex_buf[j++] = cc;
// 		if(cc == 0)
// 			count++;
// 	}

// 	lex_buf[j] = 0;
// 	
// 	if(i == 0){
// 		dps();
// 		return;
// 	}
// 	
// 	cmd = lookUp();
// 	if(count > 1)
// 		switch(cmd){
// 			case BT_WRITEB:
// 				if(( i = depart(lex_buf,count,0)) == 0xffff){
// 					iputs("cmd error!\r\n");
// 					goto end;
// 				}	
// 				BTSendCmd(i, lex_buf);
// 				goto end;
// 			case BT_WRITEW:
// 				if( ( i = depart(lex_buf,count,1)) == 0xffff){
// 					iputs("cmd error!\r\n");
// 					goto end;
// 				}
// 				BTSendCmd(i, lex_buf);
// 				goto end;
// 			case BT_WRITED:
// 				if(( i = depart(lex_buf,count,2))  == 0xffff){
// 					iputs("cmd error!\r\n");
// 					goto end;
// 				}
// 				BTSendCmd(i, lex_buf);
// 				goto end;
// 			case SMSC_WB:
// 				if(( i = depart(lex_buf,count,0)) == 0xffff){
// 					iputs("cmd error!\r\n");
// 					goto end;
// 				}	
// 				smscSendCmd(i, lex_buf);
// 				goto end;
// 			default:
// 				break;
// 	}
// 	
// 	if(cmd == BT_UNDEF){
// 		iputs(&lex_buf[0]);
// 		iputs("\n\rUnkown cmd!\n\r\a");
// 		goto end;
// 	}

// 	if(count != 1){
// 		iputs(&lex_buf[0]);
// 		iputs(" ");
// 		--count;
// 		for(i = 1; i < j; i++)
// 			if(lex_buf[i] == 0){
// 				iputs(&lex_buf[i+1]);
// 				iputs(" ");
// 				if(--count == 0)
// 					break;
// 			}
// 		iputs("\n\rUnkown cmd!\n\r\a");
// 		goto end;
// 	}
// 	
// 	switch(cmd){
// 		case BT_PAIRED:
// 			iputs("AT#PI\r\n Paired cmd recevied\n\r");
// 			break;
// 		case BT_PAIRING:
// 			iputs("IND#II\r\nPairing cmd recevied\n\r");
// 			break;
// 		case SYS_INFO:
// 			sendDevInfo();
// 			break;
// 		case SYS_READ_VAR:
// 			break;
// 		case SYS_SET_VAR:
// 			break;
// 		default:
// 			break;
// 	}

// end:
// 	
// 	if(out_index == in_index)
// 		out_index = in_index = (in_index&UART_TX_BUF_MASK);

// 	dps();

// }

/*-------------------------------------------------------------------------
 * @fucname:	 sendBack
 * @brief: 	
 * @param: 	
 * @return: 	
 * @details: 	
 *-----------------------------------------------------------------------*/
void sendBack()                                                          //ma 20150703  del   useless function for aprom
{
// 	u8 cc;
// 	
// //	if(column == 0)
// //		column_offset = out_index;
// 	
// 	while(out_index < in_index){
// 		if(column == 0)
// 			column_offset = out_index;
// 		
// 		cc = UI_buf[(out_index++)&UART_BUF_SIZE_MASK];
// 			
// 		if(cc == '\r'){	
// 			iputs("\r\n");
// 			analyse(column_offset);
// 		}else if(cc == 0){
// 			iputs("\r\n");
// 			analyse(column_offset);
// 		}else{
// 			column++;
// 			iputc(cc);
// 		}
// 		
// 	}
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 UART1_IRQHandler
 * @brief: 	 deal the communication with blue teeth module
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void UART1_IRQHandler(void)
{
	u8 cc;
	static u32  vc = 0;
	//static u32 specail_key_flag = 0;       //ma 20150629 
	 
	if(UART_GET_RDA_INT(UART1) ||UART_GET_TOUT_INT(UART1)){
		// UART receive data
		while(UART_GET_RX_EMPTY(UART1) == 0){
			
			cc = UART1->RBR;
			
			if(cc == '\b'){
				if(column == 0)
				{
					iputc(0x7);
					return;
				}
				else
				{
					iputs("\b \b");
					if(vc != 0)
						vc--;
					else{
						column--;
						in_index--;
						if(out_index >= in_index)
							out_index--;
					}
					return;
				}
			}else if(isEffectChar(cc)){
				if(cc == 0x5b && ((lastkey&0xff00) == 0x1b00))
					return;
				if((lastkey&0XFFFF00) == 0x1b5b00)
					if(cc >=0x41 && cc <= 0x44) // deal with ->, ^, <-,key
						return;
			}else
				return;

			 

			
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
			
	}else if(UART_GET_THRE_INT(UART1)){
		while((utx_out < utx_in) && (UART_GET_TX_FULL(UART1)==0)){
			UART1->THR = utx_buf[utx_out&UART_TX_BUF_MASK];
			utx_out++;
		}

		if(utx_out == utx_in){
			UART_DISABLE_INT(UART1, UART_IER_THRE_IEN);
			txidle = TRUE;
			utx_out = utx_in = utx_in&UART_BUF_SIZE_MASK;
		}
	}
}


/*-------------------------------------------------------------------------
 * @fucname:	 uartInit
 * @brief: 	 	 init uart baund rate and length of data bits, length of stop bit
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void uartInit(void)
{    
	STR_UART_T sParam;
	u32 i;

	GPIO_UART1_RXTX();//set Pb.4 = RX, Pb.5 =TX
	
	// UART Setting 
	sParam.u32ClkSrc	= CLK_UART_HXT;
	sParam.u32ClkDiv	= 1;
	sParam.u32ClkFreq	= __XTAL;			//HXT/1 == __XTAL
	sParam.u32BaudRate = 115200;
	sParam.u32cDataBits = UART_WORD_LEN_8;
	sParam.u32cStopBits = UART_STOP_BIT_1; 
	sParam.u32cRxTriggerLevel	= UART_FCR_RFITL_4BYTES;
	sParam.u32BaudMode    = UART_BAUD_MODE2;
	sParam.u8RxTimeOut	= 100;

    // Set UART Configuration 
	DrvUART_Init(UART1,&sParam);

	NVIC_SetPriority (UART1_IRQn, 2);
    // Enable UART Interrupt 
	DrvUART_EnableInt(UART1, UART_IER_TIMEOUT_COUNTER_EN|
    							UART_IER_RDA_IEN|UART_IER_RTO_IEN);

	for(i = 0; i<UART_RX_BUF_SIZE; i++){
		UI_buf[i] = 0;
	}

	for(i = 0; i<UART_TX_BUF_SIZE; i++){
		utx_buf[i] = 0;
	}

// 	column_offset = 0;                    //ma 20150707
	in_index = out_index = 0;
	utx_in = utx_out = 0;
	txidle = TRUE;

}


void uartdecode(u32 x, u8* str)
 {
	u8 buf[16];
	u32 index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	 
	BTSendCmd(str);
	BTSendCmd(buf);
	BTSendCmd("\r\n");
 }
 
 void uartdecode2(u32 x, u8* str)
 {
	u8 buf[16];
	u32 index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	BTSendCmd(str);
	BTSendCmd(buf);
 }
 
void uartdecodeH(u32 x, u8* str)  
 {
	u8 itemp;	 
	u8 buf[16];
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  BTSendCmd(str);   
	
	itemp = (x&0x000000f0)>>4;
  buf[0]=HEXSTR[itemp];
	itemp = (x&0x0000000f);
  buf[1]=HEXSTR[itemp];
	buf[2]='\0';;
	
	BTSendCmd(buf); 
	 
 	BTSendCmd("\r\n");
 }

void uartdecodeH2(u32 x, u8* str)
 {
	u8 itemp;	 
	u8 buf[16];
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  BTSendCmd(str);

	itemp = (x&0x000000f0)>>4;
  buf[0]=HEXSTR[itemp];
	itemp = (x&0x0000000f);
  buf[1]=HEXSTR[itemp];
	buf[2]='\0';;
	
	BTSendCmd(buf); 
	 
 }
 
 void uartdecodeH32(u32 x, u8* str)
 {
	u8 itemp;	 
	u8 buf[16];
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  BTSendCmd(str);

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
	
	BTSendCmd(buf); 

 	BTSendCmd("\r\n");
 }
 
void iputchar(u8 ch)
{
	utx_buf[utx_in&UART_TX_BUF_MASK] = ch;
	utx_in++;

	if(txidle == TRUE)
	{
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
	
}

