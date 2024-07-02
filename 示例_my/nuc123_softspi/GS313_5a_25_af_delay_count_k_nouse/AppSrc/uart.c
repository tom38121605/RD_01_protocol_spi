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

#define UATCMD_COUNT		8
#define UINDCMD_COUNT		6
#define USYSCMD_COUNT		3

#define UART_RX_BUF_SIZE	(1<<8)
#define UART_BUF_SIZE_MASK (UART_RX_BUF_SIZE -1)
#define UART_TX_BUF_SIZE	(1<<9)
#define UART_TX_BUF_MASK	(UART_TX_BUF_SIZE-1)

static u8 UI_buf[UART_RX_BUF_SIZE];

//--static S32 column_offset = 0;
static u32 in_index = 0, out_index = 0;
static u8 	utx_buf[UART_TX_BUF_SIZE];
static u32 utx_in = 0, utx_out = 0;
static u32 txidle = TRUE;
static u32 lastkey = 0xffffffff;//--static u32 lastkey = -1; //store last two code
extern u8 battery_f;


enum SYSCMD
{
	BT_PAIRED,
	BT_PAIRING,
	BT_UNPAIRING,
	BT_GETCALL,
	BT_HANGUP,
	BT_WRITEB,
	BT_WRITEW,
	BT_WRITED,
	BT_WRITEC,
	SMSC_WB,
	BT_REST,
	BT_CALLING,
	BT_CALLCOME,
	BT_HANGUPED,
	SYS_READ_VAR,
	SYS_SET_VAR,
	SYS_INFO,
	DUMP_FI2S,
};

struct cmd_tbl
{
	enum SYSCMD cmd;
	u8* str;
};



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
	for(i = 0; i < count; i++)
	{
		utx_buf[utx_in&UART_TX_BUF_MASK] = buf[i];
		utx_in++;
	}
	
	if(txidle == TRUE)
	{
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
	
}


void smscSendCmd(u32 count, u8 *buf)
{
	
}


void iputc(u8 ch)
{
	
//#if(UART_EN)
	
	utx_buf[utx_in&UART_TX_BUF_MASK] = ch;
	utx_in++;

	if(txidle == TRUE)
	{
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
	
//#endif
	
}


void iputs(u8* buf)
{
	
//#if(UART_EN)
	u32 i = 0;
	
	for(i = 0;buf[i] != 0; i++)
	{
		utx_buf[utx_in&UART_TX_BUF_MASK] = buf[i];
		utx_in++;
	}

	if(txidle == TRUE)
	{
		txidle = FALSE;
		UART_ENABLE_INT(UART0, UART_IER_THRE_IEN);
	}
//#endif
	
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

//--u32 strlen(u8 *str)
//{
//	u32 i = 0;

//	while(*str++)
//		i++;

//	return i;
//}


// u8* strncpy(u8 *src, u8 *des, u32 n)
// {
// 	while(n--)
// 		src[n] = des[n];
// 	
// 	return des;
// }

static __inline bool isEffectChar(u8 cc)
{
	lastkey <<= 8;
	lastkey |= (u8)cc;
	if(cc >= 0x20 && cc <= 0x7e || cc == 0xa || cc == 0xd)
		return 1;
	return 0;
}

/*--void sendDevInfo()
{
	//--iputs("mcu: cortex-M0 GS123\n\r");
	//--iputs("PRJ: THEone\n\r");
	iputs("VERSION: ");
	iputs(VERSION);
	iputs("\n\r");
}


void sendStartInfo(void)
{
	clr();
	sendDevInfo();
	iputs("-----------------------\r\n\n");
	dps();
}
*/

void sendInfo(u8 *str)
{
//#if(UART_EN)
	iputs(str);
	dps();
//#endif
}


void UART0_IRQHandler(void)
{
	u8 cc;
	static u32  vc = 0; //--specail_key_flag = 0,
	
	
	if(UART_GET_RDA_INT(UART0) ||UART_GET_TOUT_INT(UART0))
	{
		// UART receive data
		while(UART_GET_RX_EMPTY(UART0) == 0)
		{			
			cc = UART0->RBR;
			
			if(cc == '\b')
			{
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
					else
					{
						column--;
						in_index--;
						if(out_index >= in_index)
							out_index--;
					}
					return;
				}
				
			}
			else if(isEffectChar(cc))
			{
				if(cc == 0x5b && ((lastkey&0xff00) == 0x1b00))
					return;
				if((lastkey&0XFFFF00) == 0x1b5b00)
					if(cc >=0x41 && cc <= 0x44) // deal with ->, ^, <-,key
						return;
			}
			else
				return;

			 			
			if( in_index > UART_RX_BUF_SIZE + out_index ||column > UART_BUF_SIZE_MASK)
			{
				iputc(cc);
				vc++;
				if(cc == '\r')
				{
					vc = 0;
					UI_buf[(in_index-1)&UART_BUF_SIZE_MASK] = cc;
					out_index--;
				}
				return;
			}
			
			UI_buf[(in_index++)&UART_BUF_SIZE_MASK] = cc;
		}
			
	}
	else if(UART_GET_THRE_INT(UART0))
  {
		
		while((utx_out < utx_in) && (UART_GET_TX_FULL(UART0)==0))
		{
			UART0->THR = utx_buf[utx_out&UART_TX_BUF_MASK];
			utx_out++;
		}

		if(utx_out == utx_in)
		{
			UART_DISABLE_INT(UART0, UART_IER_THRE_IEN);
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
	DrvUART_Init(UART0,&sParam);

	NVIC_SetPriority (UART0_IRQn, 2);
  /* Enable BT Interrupt */
	DrvUART_EnableInt(UART0, UART_IER_TIMEOUT_COUNTER_EN|
    							UART_IER_RDA_IEN|UART_IER_RTO_IEN);
									

	for(i = 0; i<UART_RX_BUF_SIZE; i++)
	{
		UI_buf[i] = 0;
	}

	for(i = 0; i<UART_TX_BUF_SIZE; i++)
	{
		utx_buf[i] = 0;
	}

	//--column_offset = 0;
	in_index = out_index = 0;
	utx_in = utx_out = 0;
	txidle = TRUE;

}

void i32toh(u32 bytes, u8* buf)
{
	u32 temp;
	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	temp = bytes;
	buf[0]=HEXSTR[ (temp&0xf0000000) >>28 ];
	buf[1]=HEXSTR[ (temp&0x0f000000) >>24 ];
	buf[2]=HEXSTR[ (temp&0x00f00000) >>20 ];
	buf[3]=HEXSTR[ (temp&0x000f0000) >>16 ];
	buf[4]=HEXSTR[ (temp&0x0000f000) >>12];
	buf[5]=HEXSTR[ (temp&0x00000f00) >>8 ];
	buf[6]=HEXSTR[ (temp&0x000000f0) >>4 ]; 
	buf[7]=HEXSTR[ (temp&0x0000000f) ];

	buf[8]=' ';
	buf[9]=0;

	//return buf;
}



 
 void cpld_send_data(u8 cDate)  
{ 
	u8 i ;

	for(i=0;i<8;i++) 
	{ 
			if(cDate&0x80)            
				 GPIO_SET_PIN(PD3);	 //CPLD_PORT->DOUT|=(1<<CPLD_RD);			
			else 
				 GPIO_CLR_PIN(PD3); //CPLD_PORT->DOUT&=~(1<<CPLD_RD);          
			
			GPIO_CLR_PIN(PD1);  //CPLD_PORT->DOUT&=~(1<<CPLD_SCK);              
			cDate=cDate<<1; 
			GPIO_SET_PIN(PD1);	//CPLD_PORT->DOUT|=(1<<CPLD_SCK);         
	} 

	GPIO_SET_PIN(PD2); //CPLD_PORT&=~(1<<RCK);             
	sysDelayUs(100); 
	GPIO_CLR_PIN(PD2); //CPLD_PORT|=(1<<RCK);
		
    
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
	iputs("\r\n");
 }
 
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

void uartdecodeH2(u32 x, u8* str)
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
 
