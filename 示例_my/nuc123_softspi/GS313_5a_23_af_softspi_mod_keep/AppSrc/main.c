
#include <stdio.h>
#include <string.h>

#include "NUC123Series.h"
#include "sysinc.h" 
//#include "filter.h"
#include "gpio.h"
#include "74hc4096.h"

extern s32 breath_flag;
extern void fadeoff( u32 color);
extern DWHP83_RMSG dwhp83_receive_msg;

//--#define CMD_TOUCHKEY			0x30  

extern DWHP83_STATE dwhp83_status;
extern u32 Dwhp83_route_count;

//----------------- GPIO INT -------------------------------------------------
#define GPIO_PMD_QUASI          0x3UL        //!< Quasi-bidirectional Mode 
#define GPIO_IMD_EDGE           0UL            //!< IMD Setting for Edge Trigger Mode 
#define GPIO_IEN_IR_EN_Pos          16         //!< GPIO IEN: IR_EN Position 
#define GPIO_DBCLKSRC_LIRC      0x00000010UL   //!< DBNCECON setting for de-bounce counter clock source is the internal 10 kHz 
#define GPIO_IEN_IF_EN_Pos          0          //!< GPIO IEN: IF_EN Position 

extern u32 writeCAP8298(u32 data, u32 address);


u8 coco[3]={0};

u8 icmd=0;
enum KEYWORD ikey=NONE_KEY;
u8 ikey_vol=0;
u8 flg_irmute=0;



//--------------dw1000------------------

/* Example application name and version to display on LCD screen. */
#define APP_NAME "DS TWR INIT v1.2"

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 1000


static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Default antenna delay values for 64 MHz PRF. See NOTE 1 below. */
#define TX_ANT_DLY 16436
#define RX_ANT_DLY 16436

/* Frames used in the ranging process. See NOTE 2 below. */
static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0};
static uint8 rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
static uint8 tx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/* Length of the common part of the message (up to and including the function code, see NOTE 2 below). */
#define ALL_MSG_COMMON_LEN 10
/* Indexes to access some of the fields in the frames defined above. */
#define ALL_MSG_SN_IDX 2
#define FINAL_MSG_POLL_TX_TS_IDX 10
#define FINAL_MSG_RESP_RX_TS_IDX 14
#define FINAL_MSG_FINAL_TX_TS_IDX 18
#define FINAL_MSG_TS_LEN 4
/* Frame sequence number, incremented after each transmission. */
static uint8 frame_seq_nb = 0;
	

/* Buffer to store received response message.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 20
static uint8 rx_buffer[RX_BUF_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 祍 and 1 祍 = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 150
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3100

/* Receive response timeout. See NOTE 5 below. */
//#define RESP_RX_TIMEOUT_UUS 2700
#define RESP_RX_TIMEOUT_UUS  3300  //2700 + 600 +

/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
#define PRE_TIMEOUT 8

/* Time-stamps of frames transmission/reception, expressed in device time units.
 * As they are 40-bit wide, we need to define a 64-bit int type to handle them. */
typedef unsigned long long uint64;
static uint64 poll_tx_ts;
static uint64 resp_rx_ts;
static uint64 final_tx_ts;

/* Declaration of static functions. */
static uint64 get_tx_timestamp_u64(void);
static uint64 get_rx_timestamp_u64(void);
static void final_msg_set_ts(uint8 *ts_field, uint64 ts);


//--------------dw1000---end---------------



void CLK_Init(void)
{
	SYS_UnlockReg();                                        //En:Unlock protected registers
	GPIO_XT1();
	CLK_HXT_ENABLE();                                        //En:Enable External XTAL (4~24 MHz)
	
	while(!CLK_IS_STABLE(CLK_HXT_STB)); 		  //En:Waiting for 12MHz clock ready
	
	//CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4A54);   // 0X4A54 PLL 73.714M.  0x4016 is 72M 
  CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4016);       // 0x4016 is 72M 
  //CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4221);       //70M 
  //CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4012);       //60M 
	
	while(!CLK_IS_STABLE(CLK_PLL_STB));
	SET_HCLK_CLKSRC(CLK_HCLK_PLL);
	  
	//CLK_SET_APBDIV(CLK_APBDIV_DIV1);
	CLK_SET_APBDIV(CLK_APBDIV_DIV2);
	
	SystemCoreClockUpdate();  //En:Update the variable SystemCoreClock	
	
	SysTickInit(6000); 		// systick per 1ms 
	adcKeyClkInit();	// adc clk init
	
	//irClkInit();	  // IR KEY CLK INIT

	SYS_LockReg();     // En: Lock protected registers	
}

void Hal_Init(void)
{
	u32 status;	  
	
	ENTER_SECTION(status);	// disable irq and systick   
	
	CLK_Init();  //Init system clock to 72Mhz and other ip(bus) clk  //-- 73.714MHz  

	OUT_SECTION(status);  // enable irq and systick  
	
	//--I2SClkInit(48000); 

		
  sysTaskInit(); // systask init   
	
	ledInit();	// init the pin to pwm function  
	
	//penMcuCtrlPin(); // peripheral chip power is off 	  
}

/*
void bootLDROM()
{
	UNLOCKREG();   
	outpw(&SYS->RSTSRC, 3);   //clear bit
	outpw(&FMC->ISPCON, (inpw(&FMC->ISPCON) & 0xFFFFFFFC) |0x2);  
	outpw(&SCB->AIRCR, (0x05FA0000UL | 0x00000004UL));   //SYSRESETREQ
	
	while(1);   
}
*/

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
extern u32 dc_time_stamp;   
//--extern void devTest();  
extern u32 volatile motor_cc;  
extern u32 motor_direction;  
u32 bvb = 100;  
extern u32 bt_cycle;  
u8 upgrade_buf[4];  
extern u32 ack_error;  
extern void readBuf(u8 *buf, u32 address, u32 count);  
u8 DC_FLAG = 0;  //--u32 DC_FLAG = 0;  

extern u8 DEVICE_ADDRESS;  

u8 Key_Scan(u8 mode);

//volatile u32 delay_us = 0;

void resetHook(void);   


int main(void)   
{  
 	//u8  ibuf[4];
 	//u8  ibuf2[4];
  u32 idev=0;

	
	Hal_Init();  

	flg_softmute=1;
	ledTaskInit(); // flash the led in the borards

	
//	//openMcuCtrlPin(); 
	PD->PMD &= ~(3UL<<(5*2));	
	PD->PMD |= 1<<(5*2);	
	MAIN_POWER_ON();  

	
//--	//sysDelayMs(100);		
	
	uartInit();       //--zz debug 
	ledInit();
	breath_flag=0xff;
	ledColorSet(BLUE_COLOR);	 	//RED_COLOR
	
	i2cSysInit();   //PF2,PF3		
	
	
	adckeyTaskInit();	//adc key task init
	
	
//	//DWAM83 INI
//--  GPIO_SET_MODE(PB, 14, GPIO_QUASI); 
//	GPIO_CLR_PIN(PB14); 
//	sysDelayMs(200); 
//	GPIO_SET_PIN(PB14); 
//	sysDelayMs(50); 


//	DWPH83Init();
//  //GPIO_SET_MODE(PB, 10, GPIO_QUASI);  //--zz debug
//--  GPIO_SET_MODE(PB, 10, GPIO_INPUT);  
	
	
	
	//iputs("main--01.\r\n");  //--zz debug         
	iputs("\r\n------------------ ");  //--zz debug         
	iputs(VERSION);       
	iputs(" -----------------\r\n");  //--zz debug         
		
		
//	//openMcuCtrlPin(); 
//--	PD->PMD &= ~(3UL<<(5*2));	
//	PD->PMD |= 1<<(5*2);	
//	MAIN_POWER_ON();  	
//--	//sysDelayMs(100);	



	   
	//--AMP
	//--standbyPowerCtrlSet();
	//--standbyPowerCtrlCLR();
	//--GPIO_SET_PIN(PB9);	
	

//-------------dw1000  ini------------------------------------	


	//SPI ini
	spiFlashInit();   
	
	GPIO_SET_MODE(PA, 10, GPIO_OUTPUT);
	GPIO_CLR_PIN(PA10); 
	

	// Configure PD.3  (DW_IRQ)
	PD->IMD &= ~(1 << 3);
	PD->IEN |= (1 << (3+16) ); 		
	NVIC->ISER[0] |= (1 << 5);      //使能GPCDF中断 	

	// HCLK+1024
	GPIO->DBNCECON |= (1<<5 );   

	//GPIO->DBNCECON |= (1<<4 );   
	GPIO->DBNCECON &= ~(1<<4 );   

	GPIO->DBNCECON &= 0xf0;  GPIO->DBNCECON |= 0x0a;    //1024个clocks采样一次    
	PD->DBEN |= 1<<3;	           //使能PC9的去抖动功能	
	GPIO_SET_MODE(PD, 3, GPIO_INPUT);   


	reset_DW1000(); 

	spi_set_rate_low();			
	sysDelayMs(2);	   

	if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
	{
			iputs("INIT FAILED \r\n");
			while (1)
			{ };
	}

	spi_set_rate_high();  
	sysDelayMs(2);	 		


	/* Configure DW1000. See NOTE 7 below. */
	dwt_configure(&config);

	/* Apply default antenna delay value. See NOTE 1 below. */
	dwt_setrxantennadelay(RX_ANT_DLY);
	dwt_settxantennadelay(TX_ANT_DLY);

	/* Set expected response's delay and timeout. See NOTE 4, 5 and 6 below.
	 * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
	dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
	dwt_setrxtimeout(RESP_RX_TIMEOUT_UUS);
	dwt_setpreambledetecttimeout(PRE_TIMEOUT);		
		

//-------------dw1000  ini------end---------------------------	

	WDTInit();	    //init watch dog timer	 


  sysDelayMs(1000);	    
	
	//flg_softmute=0;
	
	//-----test----
	GPIO_SET_MODE(PB, 10, GPIO_OUTPUT);
	GPIO_CLR_PIN(PB10); 
	
	
	spi_set_rate_low();	
	//spi_set_rate_high();  
	
  while(1)     
	{	 
//--		static u32 j1=0;
		//u8 acoco[3];
		
//-- iputs("main--031.\r\n");  //--zz debug      

		
//-------------------test timer----------
		
 	u8 ibuf2[4]={0x18,0x18,0x18,0x18};
		
	  //ibuf2[0]=0x18;
		
		dwt_writetodevice(0x03,0,4,ibuf2);
		//idev= dwt_readdevid();  	 

		//deca_sleep(1);
		
		//GPIO_INVERSE_PIN(PB10); 
		
		
//-------------test dw1000--------------------------		
		

//		//idev= dwt_readdevid();     //0xDECA0130    
//		//uartdecodeH32(idev,"  idev: ");
		
//		spiFlashRead2(0x0,4,ibuf);
//    uartdecodeH(ibuf[0],"  0ibuf0: ");
//    uartdecodeH(ibuf[1],"  0ibuf1: ");
//    uartdecodeH(ibuf[2],"  0ibuf2: ");
//    uartdecodeH(ibuf[3],"  0ibuf3: ");
//		iputs("\r\n"); 

//		sysDelayMs(3000);	   
//	
////		spiFlashRead2(0x01,4,ibuf);
////    uartdecodeH(ibuf[0],"  1ibuf0: ");
////    uartdecodeH(ibuf[1],"  1ibuf1: ");
////    uartdecodeH(ibuf[2],"  1ibuf2: ");
////    uartdecodeH(ibuf[3],"  1ibuf3: ");
////		iputs("\r\n"); 

////		sysDelayMs(3000);	  
//		
//	  ibuf2[0]=0;
//	  ibuf2[1]=0;
//	  ibuf2[2]=0;
//	  ibuf2[3]=1;
//		
//		//spiFlashWrite2(0x03,4,ibuf2);
//		dwt_writetodevice(0x03,0,4,ibuf2);
//		sysDelayMs(1000);	  		
		
//		spiFlashRead2(0x04,4,ibuf);
//    uartdecodeH(ibuf[0],"  ibuf0: ");
//    uartdecodeH(ibuf[1],"  ibuf1: ");
//    uartdecodeH(ibuf[2],"  ibuf2: ");
//    uartdecodeH(ibuf[3],"  ibuf3: ");
//		iputs("\r\n"); 

//		sysDelayMs(2000);	  

		
//	  ibuf2[0]=0;
//	  ibuf2[1]=0;
//	  ibuf2[2]=0;
//	  ibuf2[3]=0x18;
//		
//		dwt_writetodevice(0x03,0,4,ibuf2);
//		sysDelayMs(1000);	  
//		
//		
//		spiFlashRead2(0x03,4,ibuf);
//    uartdecodeH(ibuf[0],"  ibuf0: ");
//    uartdecodeH(ibuf[1],"  ibuf1: ");
//    uartdecodeH(ibuf[2],"  ibuf2: ");
//    uartdecodeH(ibuf[3],"  ibuf3: ");
//		iputs("\r\n"); 

//		sysDelayMs(2000);	  			
		
		
//-------test dw1000 ----end-------------------- 



//-------------dw1000---code-----------------------		


//        /* Write frame data to DW1000 and prepare transmission. See NOTE 8 below. */
//        tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
//        dwt_writetxdata(sizeof(tx_poll_msg), tx_poll_msg, 0);      /* Zero offset in TX buffer. */
//        dwt_writetxfctrl(sizeof(tx_poll_msg), 0, 1);               /* Zero offset in TX buffer, ranging. */

//        /* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent and the delay
//         * set by dwt_setrxaftertxdelay() has elapsed. */
//        dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);

////iputs("tx out 1\r\n");					//--zz debug	

//        /* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
//        while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
//        { };
//				
////iputs("rx in 1\r\n");					//--zz debug	
//uartdecodeH32(status_reg,"  status_reg: ");   //--zz debug	
//				
//        /* Increment frame sequence number after transmission of the poll message (modulo 256). */
//        frame_seq_nb++;

//        if (status_reg & SYS_STATUS_RXFCG)
//        {
//            uint32 frame_len;
//					
////iputs("rx in 2\r\n");					//--zz debug	

//            /* Clear good RX frame event and TX frame sent in the DW1000 status register. */
//            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);

//            /* A frame has been received, read it into the local buffer. */
//            frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;
//            if (frame_len <= RX_BUF_LEN)
//            {
//                dwt_readrxdata(rx_buffer, frame_len, 0);
//            }

//            /* Check that the frame is the expected response from the companion "DS TWR responder" example.
//             * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
//            rx_buffer[ALL_MSG_SN_IDX] = 0;
//            if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0)
//            {
//                uint32 final_tx_time;
//                int ret;

////iputs("rx in 3\r\n");					//--zz debug	

//                /* Retrieve poll transmission and response reception timestamp. */
//                poll_tx_ts = get_tx_timestamp_u64();
//                resp_rx_ts = get_rx_timestamp_u64();

//                /* Compute final message transmission time. See NOTE 10 below. */
//                final_tx_time = (resp_rx_ts + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
//                dwt_setdelayedtrxtime(final_tx_time);

//                /* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
//                final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;

//                /* Write all timestamps in the final message. See NOTE 11 below. */
//                final_msg_set_ts(&tx_final_msg[FINAL_MSG_POLL_TX_TS_IDX], poll_tx_ts);
//                final_msg_set_ts(&tx_final_msg[FINAL_MSG_RESP_RX_TS_IDX], resp_rx_ts);
//                final_msg_set_ts(&tx_final_msg[FINAL_MSG_FINAL_TX_TS_IDX], final_tx_ts);

//							
//                /* Write and send final message. See NOTE 8 below. */
//                tx_final_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
//                dwt_writetxdata(sizeof(tx_final_msg), tx_final_msg, 0);  /* Zero offset in TX buffer. */
//                dwt_writetxfctrl(sizeof(tx_final_msg), 0, 1);  /* Zero offset in TX buffer, ranging. */
//                ret = dwt_starttx(DWT_START_TX_DELAYED);


//                /* If dwt_starttx() returns an error, abandon this ranging exchange and proceed to the next one. See NOTE 12 below. */
//                if (ret == DWT_SUCCESS)
//                {
//                    /* Poll DW1000 until TX frame sent event set. See NOTE 9 below. */
//                    while (!(dwt_read32bitreg(SYS_STATUS_ID) & SYS_STATUS_TXFRS))
//                    { };

//                    /* Clear TXFRS event. */
//                    dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);

//                    /* Increment frame sequence number after transmission of the final message (modulo 256). */
//                    frame_seq_nb++;
//                }
//            }
//        }
//        else
//        {
//            /* Clear RX error/timeout events in the DW1000 status register. */
//            dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

//            /* Reset RX to properly reinitialise LDE operation. */
//            dwt_rxreset();
//        }

//        /* Execute a delay between ranging exchanges. */
//        //sleep_ms(RNG_DELAY_MS);
//				deca_sleep(RNG_DELAY_MS);
//    

//-------------dw1000---code----end-------------------		



		//--u8 ieeprom_temp=0;
		
		
//		//-------------24c02--------------------------------------
//		
//		writeeeprom(69,0x06);         
//		sysDelayMs(1500);  

//		ieeprom_temp = readeeprom(0x06);
//		sysDelayMs(1500);    

//		uartdecode(ieeprom_temp,"  eeprom_01: ");			
//		sysDelayMs(30);	


//		writeeeprom(29,0x06); 
//		sysDelayMs(1500); 

//		ieeprom_temp = readeeprom(0x06); 
//		sysDelayMs(1500); 
 			
//		uartdecode(ieeprom_temp,"  eeprom_01B: ");	 		
//		sysDelayMs(30);	
		
//-------------24c02-----end---------------------------------

	
//		runTimedTasks();   //run timed tasks				
//		
//		SYS_UnlockReg();   
//		WDT_TIMER_RESET();   //Clear WDT timer
//		SYS_LockReg();     

	}		
	
}


//void resetHook(void)
//{;	
//}


/*
void GPCDF_IRQHandler()
{ 
	if(GPIO_GET_PIN_IF(PC,9))
	{
		NVIC->ISER[0]&= ~(1 << 5);   //Disable		
		
		iputs("PC9 INT in.\n");   //--zz debug
		sysDelayUs(50);
		
		flg_touchin=1;
			
		
		//gpio_int = SOC_TOUCH
		GPIO_CLR_PIN_IF(PC,9);   //Clear

		
		NVIC->ISER[0] |= (1 << 5);   //Enable
	}	
}
*/


/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_tx_timestamp_u64()
 *
 * @brief Get the TX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 get_tx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readtxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn get_rx_timestamp_u64()
 *
 * @brief Get the RX time-stamp in a 64-bit variable.
 *        /!\ This function assumes that length of time-stamps is 40 bits, for both TX and RX!
 *
 * @param  none
 *
 * @return  64-bit value of the read time-stamp.
 */
static uint64 get_rx_timestamp_u64(void)
{
    uint8 ts_tab[5];
    uint64 ts = 0;
    int i;
    dwt_readrxtimestamp(ts_tab);
    for (i = 4; i >= 0; i--)
    {
        ts <<= 8;
        ts |= ts_tab[i];
    }
    return ts;
}

/*! ------------------------------------------------------------------------------------------------------------------
 * @fn final_msg_set_ts()
 *
 * @brief Fill a given timestamp field in the final message with the given value. In the timestamp fields of the final
 *        message, the least significant byte is at the lower address.
 *
 * @param  ts_field  pointer on the first byte of the timestamp field to fill
 *         ts  timestamp value
 *
 * @return none
 */
static void final_msg_set_ts(uint8 *ts_field, uint64 ts)
{
    int i;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++)
    {
        ts_field[i] = (uint8) ts;
        ts >>= 8;
    }
}





//=====================================================




