

#include <string.h>
#include <stdio.h>

//#include "sysinc.h"
#include "deca.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "boards.h"
#include "app_error.h"


u8 ilocaladdr=0;
u8 idestaddr=0;
u8 idestaddr_poll=0;

extern void spi_set_rate_low(void);
extern void spi_set_rate_high(void);


extern void iputs( uint8_t *msg);
extern void uartdecode(uint32_t x, char* str);
extern void uartdecodeH(uint32_t x, char* str); 
extern void uartdecodeH32(uint32_t x, char* str);

extern void deca_sleep(u32 time_ms);
extern u16 decamutexon(void);   
extern void decamutexoff(u16 j);  




//--------------dw1000------------------

/* Example application name and version to display on LCD screen. */
#define APP_NAME "DS TWR INIT v1.2"

/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 90 //1000


//static dwt_config_t config = {
//    2,               /* Channel number. */
//    DWT_PRF_64M,     /* Pulse repetition frequency. */
//    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
//    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
//    9,               /* TX preamble code. Used in TX only. */
//    9,               /* RX preamble code. Used in RX only. */
//    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
//    DWT_BR_110K,     /* Data rate. */
//    DWT_PHRMODE_STD, /* PHY header mode. */
//    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
//};
static dwt_config_t config = {
    2,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    1,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8, //DWT_BR_110K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Default antenna delay values for 64 MHz PRF. See NOTE 1 below. */
#define TX_ANT_DLY 16436
#define RX_ANT_DLY 16436

/* Frames used in the ranging process. See NOTE 2 below. */
//static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0};
//static uint8 rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0};
//static uint8 tx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 tx_poll_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
static uint8 rx_resp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'V', 'E', 'W', 'A', 0x10, 0x02, 0, 0, 0, 0, 0, 0};
static uint8 tx_final_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8 rx_finalresp_msg[] = {0x41, 0x88, 0, 0xCA, 0xDE, 'W', 'A', 'V', 'E', 0x27, 0, 0, 0, 0, 0, 0, 0, 0};	
	
	
	
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
//#define RX_BUF_LEN 20
#define RX_BUF_LEN 50
static uint8 rx_buffer[RX_BUF_LEN];

/* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
static uint32 status_reg = 0;

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 祍 and 1 祍 = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
//#define POLL_TX_TO_RESP_RX_DLY_UUS 150
#define POLL_TX_TO_RESP_RX_DLY_UUS 650

#define FINAL_TX_TO_FINALRESP_RX_DLY_UUS 650  //--zz

/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3100

#define FINALRESP_RX_TIMEOUT_UUS 3200 //--zz

/* Receive response timeout. See NOTE 5 below. */
//#define RESP_RX_TIMEOUT_UUS 2700
#define RESP_RX_TIMEOUT_UUS  4500  

/* Preamble timeout, in multiple of PAC size. See NOTE 6 below. */
//#define PRE_TIMEOUT 8
#define PRE_TIMEOUT 100

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
static void final_msg_get_ts(const uint8 *ts_field, uint32 *ts); //--zz

//--------------dw1000---end---------------

void dw1000_init(void)
{


//	spi_set_rate_low();			
//	nrf_delay_ms(2);	   

	if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
	{
			iputs((uint8_t*)"INIT FAILED \r\n");
			while (1)
			{ };
	}

	spi_set_rate_high();  
	nrf_delay_ms(2);	 		

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
	
}

#define POLL_LOCALADDR_IDX 10
#define POLL_DESTADDR_IDX  11

#define POLL_DISTANCE0_IDX  12
#define POLL_DISTANCE1_IDX  16
#define POLL_DISTANCE2_IDX  20
#define POLL_DISTANCE3_IDX  24

#define POLL_VOLADJUST_IDX  28
#define POLL_POWERCTRL_IDX  29


#define RESP_LOCALADDR_IDX 13
#define RESP_DESTADDR_IDX  14
#define FINAL_LOCALADDR_IDX 22
#define FINAL_DESTADDR_IDX  23

#define FINALRESP_LOCALADDR_IDX 10
#define FINALRESP_DESTADDR_IDX  11
#define FINALRESP_DISTANCE_IDX  12

void dw1000_twr_measure(u8 idestaddr)
{	
			idestaddr_poll=idestaddr;	
	
			tx_poll_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
			tx_poll_msg[POLL_LOCALADDR_IDX] = ilocaladdr;
			tx_poll_msg[POLL_DESTADDR_IDX] = idestaddr;	
	
	
	    tx_poll_msg[POLL_VOLADJUST_IDX] = 0; //'+'  '-';
	    tx_poll_msg[POLL_POWERCTRL_IDX] = 0;
	
	
			dwt_writetxdata(sizeof(tx_poll_msg), tx_poll_msg, 0);      /* Zero offset in TX buffer. */
			dwt_writetxfctrl(sizeof(tx_poll_msg), 0, 1);               /* Zero offset in TX buffer, ranging. */
	
	
			dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);  //--zz
			dwt_setrxtimeout(RESP_RX_TIMEOUT_UUS);              //--zz
	
	
			/* Start transmission, indicating that a response is expected so that reception is enabled automatically after the frame is sent and the delay
			 * set by dwt_setrxaftertxdelay() has elapsed. */
			dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
	
 
			/* We assume that the transmission is achieved correctly, poll for reception of a frame or error/timeout. See NOTE 9 below. */
			while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
			{ };
 			
			/* Increment frame sequence number after transmission of the poll message (modulo 256). */
			frame_seq_nb++;

			if (status_reg & SYS_STATUS_RXFCG) 
			{
					uint32 frame_len;   

 				
					/* Clear good RX frame event and TX frame sent in the DW1000 status register. */  
					dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);  

					/* A frame has been received, read it into the local buffer. */  
					frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;  
					if (frame_len <= RX_BUF_LEN)
					{
							dwt_readrxdata(rx_buffer, frame_len, 0);
					}

					/* Check that the frame is the expected response from the companion "DS TWR responder" example.
					 * As the sequence number field of the frame is not relevant, it is cleared to simplify the validation of the frame. */
					rx_buffer[ALL_MSG_SN_IDX] = 0;
					
					//if (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0)
					if ( (memcmp(rx_buffer, rx_resp_msg, ALL_MSG_COMMON_LEN) == 0) && (ilocaladdr == rx_buffer[RESP_DESTADDR_IDX])  && (idestaddr_poll == rx_buffer[RESP_LOCALADDR_IDX]) )
					{
							uint32 final_tx_time;
							int ret;
						  //static u8 ifinalcount=0;
 
							/* Retrieve poll transmission and response reception timestamp. */
							poll_tx_ts = get_tx_timestamp_u64();
							resp_rx_ts = get_rx_timestamp_u64();

							/* Compute final message transmission time. See NOTE 10 below. */
							final_tx_time = (resp_rx_ts + (RESP_RX_TO_FINAL_TX_DLY_UUS * UUS_TO_DWT_TIME)) >> 8;
							dwt_setdelayedtrxtime(final_tx_time);
						
							dwt_setrxaftertxdelay(FINAL_TX_TO_FINALRESP_RX_DLY_UUS);   //--zz
							dwt_setrxtimeout(FINALRESP_RX_TIMEOUT_UUS);						     //--zz
						

							/* Final TX timestamp is the transmission time we programmed plus the TX antenna delay. */
							final_tx_ts = (((uint64)(final_tx_time & 0xFFFFFFFEUL)) << 8) + TX_ANT_DLY;

							/* Write all timestamps in the final message. See NOTE 11 below. */
							final_msg_set_ts(&tx_final_msg[FINAL_MSG_POLL_TX_TS_IDX], poll_tx_ts);
							final_msg_set_ts(&tx_final_msg[FINAL_MSG_RESP_RX_TS_IDX], resp_rx_ts);
							final_msg_set_ts(&tx_final_msg[FINAL_MSG_FINAL_TX_TS_IDX], final_tx_ts);

						
							/* Write and send final message. See NOTE 8 below. */
							tx_final_msg[ALL_MSG_SN_IDX] = frame_seq_nb;
							tx_final_msg[FINAL_LOCALADDR_IDX] = ilocaladdr;      //--zz
							tx_final_msg[FINAL_DESTADDR_IDX] = idestaddr_poll;	 //--zz
							
							
							
							dwt_writetxdata(sizeof(tx_final_msg), tx_final_msg, 0);  /* Zero offset in TX buffer. */
							dwt_writetxfctrl(sizeof(tx_final_msg), 0, 1);  /* Zero offset in TX buffer, ranging. */			


							//ret = dwt_starttx(DWT_START_TX_DELAYED);
							ret = dwt_starttx(DWT_START_TX_DELAYED | DWT_RESPONSE_EXPECTED);				
							
              frame_seq_nb++;   //--zz

							if (ret == DWT_ERROR)
							{
								//dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_TXFRS);    //nouse  --no success flag to clear
								return;
							}
							
 															
							while (!((status_reg = dwt_read32bitreg(SYS_STATUS_ID)) & (SYS_STATUS_RXFCG | SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR)))
							{ };
							
 							
							if (status_reg & SYS_STATUS_RXFCG)
							{	
  
								dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_RXFCG | SYS_STATUS_TXFRS);  
 
								frame_len = dwt_read32bitreg(RX_FINFO_ID) & RX_FINFO_RXFLEN_MASK;  
								if (frame_len <= RX_BUF_LEN)
								{
										dwt_readrxdata(rx_buffer, frame_len, 0);
								}
 
								rx_buffer[ALL_MSG_SN_IDX] = 0;			
								
								if( (memcmp(rx_buffer, rx_finalresp_msg, ALL_MSG_COMMON_LEN) == 0) && (ilocaladdr == rx_buffer[FINALRESP_DESTADDR_IDX])  && (idestaddr_poll == rx_buffer[FINALRESP_LOCALADDR_IDX]) )
								{
									uint32 idistance_temp;
									//float fdistance;
									int16 iidistance;     //--int16
									char dist_str[33] = {0};
									
	 
									//resp_tx_ts = get_tx_timestamp_u64();
									//final_rx_ts = get_rx_timestamp_u64();
									

									//get the distance
									final_msg_get_ts(&rx_buffer[FINALRESP_DISTANCE_IDX], &idistance_temp);	
									
 									
									//把itempdistance写入到poll中
									switch (idestaddr)
									{
										case 0:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE0_IDX], idistance_temp);
											break;
										case 1:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE1_IDX], idistance_temp);
											break;
										case 2:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE2_IDX], idistance_temp);
											break;
										
										case 3:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE3_IDX], idistance_temp);
											break;
										  
										default:
											break;
									}
									

									//fdistance = *(float *)&idistance_temp;
									//sprintf(dist_str, "destaddr %d  DIST: %3.2f m\r\n", idestaddr_poll, fdistance);

									iidistance = *(int16 *)&idistance_temp;                                           //--int16
									sprintf(dist_str, "destaddr %d  DIST: %d cm\r\n", idestaddr_poll, iidistance);	  //--int16								
									
									iputs((u8*)dist_str);									
								}										
								
							}			
							else
							{
									switch (idestaddr)
									{
										case 0:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE0_IDX], 0);
											break;
										case 1:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE1_IDX], 0);
											break;
										case 2:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE2_IDX], 0);
											break;
										case 3:
											final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE3_IDX], 0);
											break;
										default:
											break;
									}		

									/* Clear RX error/timeout events in the DW1000 status register. */
									dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

									/* Reset RX to properly reinitialise LDE operation. */
									dwt_rxreset();
							}		
							
					}
			}
			else
			{
				
					switch (idestaddr)
					{
						case 0:
							final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE0_IDX], 0);
							break;
						case 1:
							final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE1_IDX], 0);
							break;
						case 2:
							final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE2_IDX], 0);
							break;
						case 3:
							final_msg_set_ts(&tx_poll_msg[POLL_DISTANCE3_IDX], 0);
							break;
						default:
							break;
					}				
				
					/* Clear RX error/timeout events in the DW1000 status register. */
					dwt_write32bitreg(SYS_STATUS_ID, SYS_STATUS_ALL_RX_TO | SYS_STATUS_ALL_RX_ERR);

					/* Reset RX to properly reinitialise LDE operation. */
					dwt_rxreset();
			}

			/* Execute a delay between ranging exchanges. */
			//sleep_ms(RNG_DELAY_MS);

}


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

static void final_msg_get_ts(const uint8 *ts_field, uint32 *ts)
{
    int i;
    *ts = 0;
    for (i = 0; i < FINAL_MSG_TS_LEN; i++)
    {
        *ts += ts_field[i] << (i * 8);
    }
}

