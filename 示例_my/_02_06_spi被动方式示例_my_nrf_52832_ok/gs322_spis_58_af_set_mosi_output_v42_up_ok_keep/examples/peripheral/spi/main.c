

#include "nrf_drv_spis.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>

 
#include "nrf_delay.h" 
#include "app_uart.h"   
 

#include "nrf_drv_timer.h"   

#include "icrc.h"
#include "fsm.h"

#include "core_cm4.h"
#include "nrf_drv_common.h"

#define VERSION	"v42"   


uint8_t ichecksum=0;
uint8_t flg_timer0=0;


#define DATAMAXLEN 50
uint8_t itxbuf_nano[DATAMAXLEN]={0};
uint8_t irxbuf_nano[DATAMAXLEN]={0};
uint16_t inanobuf_crc =0;
uint32_t idatalen_nano=0;

//发送数据 tx  rx

#define CODE1   1 
#define CODE2   2 

#define DUMMYCODE  0xFF

//#define TX_CODE  CODE1    //CODE1 and CODE2  switch  ( for TX unit)

uint8_t itxcode = CODE1;    


//#define Tx_Enable         //TX and RX switch

#define TXENABLE  1
#define RXENABLE  0
uint8_t flg_txenable =TXENABLE;    //0 -- rx, 1 -- tx 



//=====================================================

#define SPIS_CS_PIN  29 

#define SPIS_INSTANCE 1 
static const nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE); 

#define TEST_STRING "abcdeabcde1234567890abcdeabcde1234567890abcdeabcde1234567890"

#define TXBUFLEN  60 

static uint8_t       m_tx_buf[TXBUFLEN] = TEST_STRING;            
static uint8_t       m_rx_buf[TXBUFLEN+1];     
static uint8_t       m_rx_buf2[TXBUFLEN+1];     
static const uint8_t m_length = sizeof(m_tx_buf);      

static volatile bool spis_xfer_done; /**< Flag used to indicate that SPIS instance completed the transfer. */


void spis_event_handler(nrf_drv_spis_event_t event)
{
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
    {
        //NRF_LOG_PRINTF(" Transfer completed. Received: %s\n",m_rx_buf);
        spis_xfer_done = true;
    }
}



//====================================================================


//---pin define--- 

   //txrx

//   #define TX_POWER_PORT_PIN       31
//   #define RX_POWER_PORT_PIN       30

   #define TX_POWER_PORT_PIN       17
   #define RX_POWER_PORT_PIN       16


   #define RSSI_PORT_PIN             2
   #define BATT_DET_PORT_PIN         3
   #define USB_DET_PORT_PIN          6

   #define I2C_SCL_PORT_PIN          22
   #define I2C_SDA_PORT_PIN          23

   #define INT1_PORT_PIN            29

  //rx   

  //#define DATA_IN_PORT_PIN        27 
  #define DATA_IN_PORT_PIN        19
           
  #define USER_LED_PORT_PIN    28 
  #define PLL_TEST_PORT_PIN    8 

  #define TIME0_US  25             //--10Hz
  #define DELAY_COUNT 280
 
  //#define TIME0_US    20         //--12.5Hz
  //#define DELAY_COUNT 90

  //#define TIME0_US    10         //--25Hz
  //#define DELAY_COUNT 90
 
  //tx

  #define DATA_OUT_PORT_PIN        25  

  #define TX_TEST_PORT_PIN         8    
  #define BUTTON_DET_PORT_PIN      4  
  
//  #define SPI_MOSI_PORT_PIN 			15 
//  #define SPI_MISO_PORT_PIN  	      14 
//  #define SPI_SCK_PORT_PIN 			   13
//  #define SPI_SS_PORT_PIN 			   12

  #define SPI_MOSI_PORT_PIN 			14 
  #define SPI_MISO_PORT_PIN  	      15 
  #define SPI_SCK_PORT_PIN 			13
  #define SPI_SS_PORT_PIN 			   12

   
  //#define TIME0_US  25         //--10Hz     
  
  #define OUTPUT_LOW        0
  #define OUTPUT_HIGH       1
  #define OUTPUT_INVERT     2
   
//---pin define---end-----


  //rx

  //u8 flg_exint=0;  
  u8 flg_reset_t3=0;  
  u8 flg_start=0;
  u8 flg_off=0;  
  u8 flg_flash=0;
  
  u8 idatain2[16];
  u8 icrc2[4];

  u8 idatain3[16];
  u8 icrc3[4];  
  
  u8 flg_getdata=0;  

  u8 DATA_CODE1[16] = {0, 1 ,2 ,3 ,4 ,5, 6, 7, 8 ,9 ,10 ,11 ,12 ,13 ,14 ,15};
  u8 DATA_CODE2[16] = {15, 14 ,13 ,12 ,11 ,10, 9, 8, 7 ,6 ,5 ,4 ,3 ,2 ,1 ,0};   
  
  u8 DATA_CODE3[16] = {1, 1 ,2 ,2 ,3 ,3, 4, 4, 5 ,5 ,6 ,6 ,7 ,7 ,8 ,8};   
  

  u8 FSM_CODE1[16] = {0x0e, 0x0d, 0x0b, 0x1c, 0x19, 0x1a, 0x13, 0x16, 0x31, 0x32, 0x34, 0x23, 0x26, 0x25, 0x2c, 0x29};
  u8 FSM_CODE2[16] = {0x29, 0x2c, 0x25, 0x26, 0x23, 0x34, 0x32, 0x31, 0x16, 0x13, 0x1a, 0x19, 0x1c, 0x0b, 0x0d, 0x0e};
  
  

  u8 CRC1_FSM_CODE[4] = { 0x04, 0xc2 ,0 ,0 };
  u8 CRC2_FSM_CODE[4] = { 0x79, 0xe6 ,0 ,0 };     

  u8 CRC3_FSM_CODE[4] = { 0xd7, 0xaf ,0 ,0 };    //D1 16bytes  FSM 

    
  void GPIOTE_IRQHandler(void);  
  void TMR0_IRQHandler2(void);


  //tx
  void TMR0_IRQHandler(void);
 
 

void uart_config(void);

void iputs( uint8_t *msg);
void uartdecode(uint32_t x, char* str);
void uartdecode2(uint32_t x, char* str);
void uartdecodeH(uint32_t x, char* str); 
void uartdecodeH2(uint32_t x, char* str); 
void uartdecodeH16(uint32_t x, char* str); 
void uartdecodeH32(uint32_t x, char* str);



void rx_init( )
{
//   //close timer0： 
//   NVIC_DisableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//   nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_STOP);   
//   flg_timer0=0;   
   
   flg_start=0;	

   memset(idatain2,0,16);	
   memset(icrc2,0,4);	

   nrf_gpio_pin_set(TX_POWER_PORT_PIN);    		
   nrf_gpio_pin_set(RX_POWER_PORT_PIN);   
   
   flg_txenable=RXENABLE; 
}


void tx_init( )
{
   
//   if(flg_timer0==0)
//   {
//      //reopen timer0 ：              
//      NVIC_ClearPendingIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//      NVIC_EnableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );    
//      nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   

//      nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);   
//      
//      flg_timer0=1;
//   }

   nrf_gpio_pin_clear(TX_POWER_PORT_PIN);    		
   nrf_gpio_pin_clear(RX_POWER_PORT_PIN);   
 
   flg_txenable=TXENABLE;   
}


void delay_count(uint32_t volatile number )
{
    while(number != 0)
        number--;
}


void log_uart_write_string( char* msg)
{
    while( *msg )
    {
        (void)app_uart_put(*msg++);
    }
}


void log_uart_write_hex_char(uint8_t c)
{
    uint8_t nibble;
    uint8_t i = 2;

    while( i-- != 0 )
    {
        nibble = (c >> (4 * i)) & 0x0F;
        (void)app_uart_put( (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble) );
    }
    
}


//------------------------------------------------

uint32_t itoa(uint32_t i, uint8_t* buf)
{
	uint32_t temp, k, j=0;

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


uint8_t* itoh(uint8_t byte, uint8_t* buf)
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

void iputs( uint8_t *msg)
{
    while( *msg )
    {
        (void)app_uart_put(*msg++);
    }
}

void iputchars(uint8_t *msg,uint32_t ilen)
{
	
	uint32_t i = 0;
	
	for(i = 0; i<ilen; i++)
	{
		(void)app_uart_put(*msg++);
	}
	
}


void uartdecode(uint32_t x, char* str)
 {
	uint8_t buf[16];
	uint32_t index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	 
	iputs((uint8_t*)str);
	iputs((uint8_t*)buf);
	iputs((uint8_t*)"\r\n");
 }
 
void uartdecode2(u32 x, char* str)
 {
     
	u8 buf[16];
	u32 index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	iputs((uint8_t*)str);
	iputs((uint8_t*)buf);
	//iputs((uint8_t*)"\r\n");
     
}
 
void uartdecodeH(uint32_t x, char* str)  
{
	uint8_t itemp;	 
	uint8_t buf[16];

	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  iputs((uint8_t*)str);   
	
   itemp = (x&0x000000f0)>>4;
   buf[0]=HEXSTR[itemp];
   itemp = (x&0x0000000f);
   buf[1]=HEXSTR[itemp];
   buf[2]='\0';;
	
	iputs((uint8_t*)buf); 	

 	iputs((uint8_t*)"\r\n");
}


void uartdecodeH2(u32 x, char* str)
 {

   u8 itemp;	 
   u8 buf[16];
   static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

   iputs((uint8_t*)str);

   itemp = (x&0x000000f0)>>4;
   buf[0]=HEXSTR[itemp];
   itemp = (x&0x0000000f);
   buf[1]=HEXSTR[itemp];
   buf[2]='\0';;

   iputs((uint8_t*)buf); 

	 
 }
 
void uartdecodeH16(u32 x, char* str)
 {

   u8 itemp;	 
   u8 buf[16];
   static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

   iputs((uint8_t*)str);

   itemp = (x&0x0000f000)>>12;
   buf[0]=HEXSTR[itemp];
   itemp = (x&0x00000f00)>>8;
   buf[1]=HEXSTR[itemp];
   itemp = (x&0x000000f0)>>4;
   buf[2]=HEXSTR[itemp];
   itemp = (x&0x0000000f);
   buf[3]=HEXSTR[itemp];
   buf[4]='\0';;

   iputs((uint8_t*)buf); 

   iputs((uint8_t*)"\r\n");

}
 
void uartdecodeH32(uint32_t x, char* str)
{
	uint8_t itemp;	 
	uint8_t buf[16];

	static const char HEXSTR[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	iputs((uint8_t*)str);

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
	buf[8]='\0';

	iputs((uint8_t*)buf); 

 	iputs((uint8_t*)"\r\n");
}


const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);



//timer0 irt for rx
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    //static uint32_t icount_t0=0;
    
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:					
             			
             //iputs((u8*)" t0 in\r\n");  //--zz debug 				
             //nrf_gpio_pin_toggle(8);  //toggle  

//            icount_t0++;
//            if(icount_t0>=100000)
//            {               
//               icount_t0=0;
//               iputs((u8*)" t0 in\r\n");  //--zz debug                
//            }
				
//            #ifndef Tx_Enable   
//              TMR0_IRQHandler2();
//            #else
//              TMR0_IRQHandler();
//            #endif
        
            if (flg_txenable==0)   
              TMR0_IRQHandler2();
            else
              TMR0_IRQHandler();
             
        

            break;
        
        default:
            //Do nothing.
            break;
    }
    
}

 
void TMR0_IRQHandler2(void)   
{
	static u32 itimer3_count=0; 
	u8 bvalue=0;

	static u8 icheck=0;
	u8 icheck2=0;
	
	static u32 ichk=0xffffffff;
	u32 ichk2=0;	
	
	u8 flg_begin=0;	 
	u8 flg_end=0;	 	

	u8 j=0;	
	static u8 icomein =0; 
	static u8 idatain[16]={0};
	static u8 icrc[4]={0};

	static u32 m1=0;
	static u32 m2=0;
	static u32 m3=0;
   u8 k1=0;
   u8 k2=0;
   
   static u32 iprintoffcount = 0;   
    
   itimer3_count++;

   nrf_gpio_pin_toggle(PLL_TEST_PORT_PIN);   
   

   bvalue=0;  
	if(nrf_gpio_pin_read(DATA_IN_PORT_PIN) )  	 
      bvalue =1;

   //if (itimer3_count==1) iputs((u8*)"chking\r\n");  //--zz debug 

   //get 6 bits checkvalue
   icheck<<=1;
   if( bvalue )
      icheck |= 0x01;

   icheck2 = icheck & 0x3f;

   ichk<<=1;
   if( bvalue)
      ichk |= 0x01;
   else
      ichk &= ~0x01;

   ichk2= ichk & 0xfffff; 
    

   if ( icheck2==0x38) 
   {
      flg_begin=1;   
      
   }

   else if (icheck2==0x07) 
   {
      flg_end=1;	
      //iputs((u8*)"end\r\n"); 
   } 

   //else if  (icheck2==0x3f) 
   else if  (ichk2==0x00)
   {
      flg_off=1;

      //iputs((u8*)"off1\r\n");     
   } 

	
	switch (icomein)
	{		
		case 0:
				
				if (flg_begin==1)
				{
					icomein=1;
					//iputs((u8*)"begin\r\n");
					
					return;
				} 

				break;  

		case 1:
			   k1=(u8)(m1/6);
		
				idatain[k1]<<=1;	
		 
				if( bvalue )
					 idatain[k1] |= 0x01;
				//else
				//	 idatain[k1] &= ~0x01;				

				m1++;
				if (m1==96)  		
				{			
                m1=0;					
					 icomein=2;	
					 //--iputs((u8*)"icomein=2\r\n");		
         
				}			
				
				break;

		case 2:
		
            k2= (u8)(m2/6);
      
				icrc[k2]<<=1;	
				if( bvalue )
					 icrc[k2] |= 0x01;
				//else
				//	 icrc[k2] &= ~0x01;

				m2++;
				if (m2==24)
				{  		
                m2=0;					
					 icomein=3;
					 //iputs((u8*)"icomein=3\r\n");	
				}				  			
		  
			  break;	

		case 3:		   
				m3++;  
      
            if (flg_end==1)
            {  
               //iputs((u8*)"end\r\n"); 
               
               if (m3==6)
               {
                  m3=0;                  
                  icomein=4;  
                  //--iputs((u8*)"end\r\n");                  
               }   	
               
				}				

				break;

		case 4:			
			
			break;

		case 5:
			
			break;
		
	} 

	// ----- off -----
   if (flg_off==1)
   {		 
      
      itimer3_count=0;   
      m1=0;
      m2=0;
      m3=0;      
      
      iprintoffcount++;
      
      //get data		
      if(icomein==4)
      {
         iprintoffcount=0;
         
         if(flg_getdata==0)
         {
            for (j=0; j<16; j++)
               idatain2[j] =idatain[j];                  
            for (j=0; j<4; j++)
               icrc2[j] =icrc[j];	
            
            flg_getdata=1;  
         }         
      }  
      
      icheck=0;
		ichk=0xffffffff;
      flg_start=0;	  
      icomein=0;		 
      
      memset(idatain,0,16);	
      memset(icrc,0,4);	
      
//      if(iprintoffcount>=10)
//      {
//         iprintoffcount=0;
         //iputs((u8*)"off\r\n");		 
//      }       
      
      nrf_gpio_pin_clear(PLL_TEST_PORT_PIN);	

      //close timer0： 
      NVIC_DisableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
      nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_STOP);    // TASK_STOP=1;  

      flg_timer0=0;
		
      return;
   }	 
	 
}
 

u32 FSM_test_data = 0xe;
void TMR0_IRQHandler(void)
{	
	static u8 data_index = 6;
	u8 output_flag = 0;
   static u8 four_bit_datas = 0x0;
	static u32 timer_20k_counter = 0;
	static u8 fsm_data_index = 0;
       
   
   if (fsm_channel == FSM_CHANEL_DUMMY)  
      return;
      
	switch(transfer_mode)
	{
		case FSM_IDLE_MODE: 
		{
			data_index = 6;   
			
         
			output_flag = OUTPUT_LOW;
			if(timer_20k_counter >= 3788)   // 724  18.1ms   644  16.1ms
			{
				transfer_mode = FSM_20K_MODE;
				timer_20k_counter = 0;
			}
			else
			{
				timer_20k_counter++;
			}
			break;
         
//				if (timer_20k_counter<=21)
//					output_flag = OUTPUT_LOW;

//				else if (timer_20k_counter<3700)   //add the high level to save power //3700 
//					output_flag = OUTPUT_HIGH;

//				else if(timer_20k_counter < 3788)   // 724  18.1ms   644  16.1ms
//					 output_flag = OUTPUT_LOW;    
//					 
//				else
//				{
//					transfer_mode = FSM_20K_MODE;
//					timer_20k_counter = 0;
//							
//					break;
//				}  
//         
//        timer_20k_counter++;
//         
//			  break;      
         
		}
		case FSM_20K_MODE:
		{
			data_index = 6;
			output_flag = OUTPUT_INVERT;
// 			output_flag = OUTPUT_INVERT;
			if(timer_20k_counter >= 78)   // 724  18.1ms   80 2ms
			{
				transfer_mode = FSM_SYNC_MODE;
				timer_20k_counter = 0;
			}
			else
			{
				timer_20k_counter++;
			}
			break;
		}
		case FSM_SYNC_MODE:
		{
			FSM_test_data = FSM_SYNC;
			switch(data_index)   //choose a meathod to save time
			{
				case 0:
// 				{
// 					data_index= 6;
// 					break;
// 				}
				case 1:
				{
					data_index= 0;
					break;
				}
				case 2:
				{
					data_index= 1;
					break;
				}
				case 3:
				{
					data_index= 2;
					break;
				}
				case 4:
				{
					data_index= 3;
					break;
				}
				case 5:
				{
					data_index= 4;
					break;
				}
				case 6:
				{
					data_index= 5;
					break;
				}
				
			}


			output_flag = (FSM_test_data >> data_index)  &  (0x1 );
			
			if(0 == data_index)
			{
				transfer_mode = FSM_DATA_MODE;
				data_index = 6;
				fsm_data_index  = 0;
				if(fsm_channel == FSM_CHANEL_1)
					four_bit_datas = FSM_data_in_1[0];
				else if(fsm_channel == FSM_CHANEL_2)
					four_bit_datas = FSM_data_in_2[0];
				FSM_test_data = FSM_encoder(four_bit_datas);
			}
			break;
		}
		
		case FSM_DATA_MODE:
		{
			
				if(0 == data_index)
				{
					fsm_data_index = fsm_data_index + 1;
					if(fsm_channel == FSM_CHANEL_1)
						four_bit_datas = FSM_data_in_1[fsm_data_index];
					else if(fsm_channel == FSM_CHANEL_2)
						four_bit_datas = FSM_data_in_2[fsm_data_index];
					FSM_test_data = FSM_encoder(four_bit_datas);
					data_index = 6;
				}


				// if(1 == data_index)
				// {
				// 	data_index = 6;
				// }
				// else data_index = data_index -1;


				switch(data_index)   //choose a meathod to save time
				{
// 					case 0:
// 					{
// 						data_index= 6;
// 						break;
// 					}
					case 1:
					{
						data_index= 0;
						break;
					}
					case 2:
					{
						data_index= 1;
						break;
					}
					case 3:
					{
						data_index= 2;
						break;
					}
					case 4:
					{
						data_index= 3;
						break;
					}
					case 5:
					{
						data_index= 4;
						break;
					}
					case 6:
					{
						data_index= 5;
						break;
					}
					
				}

				output_flag = (FSM_test_data >> data_index)  &  (0x1 );
				
				if(15 == fsm_data_index)
				{
					if(0 == data_index)
					{
						transfer_mode = FSM_CRC_MODE;
						data_index = 6;
						fsm_data_index = 0;
						if(fsm_channel == FSM_CHANEL_1)
							four_bit_datas = crc_fsm_data1[0];
						else if(fsm_channel == FSM_CHANEL_2)
							four_bit_datas = crc_fsm_data2[0];
						FSM_test_data = FSM_encoder(four_bit_datas);
					}
				}
			
			break;
		}
		
		case FSM_CRC_MODE:
		{
				if(0 == data_index)
				{
					fsm_data_index = fsm_data_index + 1;
					if(fsm_channel == FSM_CHANEL_1)
						four_bit_datas = crc_fsm_data1[fsm_data_index];
					else if(fsm_channel == FSM_CHANEL_2)
						four_bit_datas = crc_fsm_data2[fsm_data_index];
					FSM_test_data = FSM_encoder(four_bit_datas);
					data_index = 6;
				}
			
// 			if(fsm_channel == FSM_CHANEL_1)
// 				output_flag = (crc_data_1 >> data_index)  &  (0x1 );
// 			else if(fsm_channel == FSM_CHANEL_2)
// 				output_flag = (crc_data_2 >> data_index)  &  (0x1 );
				
				switch(data_index)   //choose a meathod to save time
				{
// 					case 0:
// 					{
// 						data_index= 6;
// 						break; 
// 					}
					case 1:
					{
						data_index= 0;
						break;
					}
					case 2:
					{
						data_index= 1;
						break;
					}
					case 3:
					{
						data_index= 2;
						break;
					}
					case 4:
					{
						data_index= 3;
						break;
					}
					case 5:
					{
						data_index= 4;
						break;
					}
					case 6:
					{
						data_index= 5;
						break;
					}
					
				} 

				
				output_flag = (FSM_test_data >> data_index)  &  (0x1 );
				
			if(3 == fsm_data_index)
			{				
				if(0 == data_index)
				{
					transfer_mode = FSM_END_MODE;
					data_index = 6;
				}

			}
				
			break;
		}
			
		case FSM_END_MODE:
		{
			
			FSM_test_data = FSM_END;
			switch(data_index)   //choose a meathod to save time
			{
// 				case 0:
// 				{
// 					data_index= 6;
// 					break;
// 				}
				case 1:
				{
					data_index= 0;
					break;
				}
				case 2:
				{
					data_index= 1;
					break;
				}
				case 3:
				{
					data_index= 2;
					break;
				}
				case 4:
				{
					data_index= 3;
					break;
				}
				case 5:
				{
					data_index= 4;
					break;
				}
				case 6:
				{
					data_index= 5;
					break;
				}
				
			}

			output_flag = (FSM_test_data >> data_index)  &  (0x1 );
			
			if(0 == data_index)
			{
				transfer_mode = FSM_IDLE_MODE;
				data_index = 6;
			}
			
			break;
		}		
	}
 
	
	
	switch(output_flag)
	{
		case OUTPUT_LOW:
		{
			nrf_gpio_pin_clear(DATA_OUT_PORT_PIN);
			nrf_gpio_pin_clear(TX_TEST_PORT_PIN);
			break;
		}
		case OUTPUT_HIGH:
		{
			nrf_gpio_pin_set(DATA_OUT_PORT_PIN);
			nrf_gpio_pin_set(TX_TEST_PORT_PIN);
			break;
		}
		case OUTPUT_INVERT:
		{
			nrf_gpio_pin_toggle(DATA_OUT_PORT_PIN);
			nrf_gpio_pin_toggle(TX_TEST_PORT_PIN);
			break;
		}
		
		default:
		{
			break;
		}
		
	}	
   
}

 
//void split8(uint8_t *buf1, uint32_t ilen)
//{
//   uint8_t itemp[DATAMAXLEN];
//   uint32_t i;
//   
//   memcpy(itemp, buf1, ilen);
//   
//   for(i=0; i<ilen; i++)
//   {
//      buf1[i*2] = itemp[i] & 0x0f;
//      buf1[i*2 +1] = (itemp[i]>>4) & 0x0f;
//   }   
//   
//}

void split8(uint8_t *buf1, uint32_t ilen)
{
   uint8_t itemp[DATAMAXLEN];
   uint32_t i;
   
   memcpy(itemp, buf1, ilen);
   
   for(i=0; i<ilen; i++)
   {
      buf1[i*2] = (itemp[i]>>4) & 0x0f;
      buf1[i*2 +1] = itemp[i] & 0x0f;
   }   
   
}



void combin8(uint8_t *buf1, uint32_t ilen)
{
   uint8_t itemp[DATAMAXLEN];
   uint32_t i;
   
   uint8_t a1=0;
   uint8_t a2=0;
   
   memcpy(itemp, buf1, ilen);
   
   for(i=0; i<(ilen/2); i++)
   {
      
      a1 = itemp[i*2+1] & 0x0f;
      a2= (itemp[i*2]<<4) & 0xf0;
      
      buf1[i] = a1 + a2;
   }   
   
}


void uart_init(void)
{

   uart_config();   
   nrf_delay_ms(2);	
   
   
	iputs((uint8_t*)"\r\n------------------ ");  //--zz debug         
	iputs((uint8_t*)VERSION);  

   if (flg_txenable==0)   
	   iputs((uint8_t*)" rx---------------\r\n");   //--zz debug         
   else
	   iputs((uint8_t*)" tx---------------\r\n");   //--zz debug   
	
}


void timer0_init(void)
{

   uint32_t time_ticks;
   uint32_t err_code = NRF_SUCCESS;

   uint32_t time_us = 0; 	
   
 
   //---timer0 ini---25us
   time_us=TIME0_US;  //25;   
   err_code = nrf_drv_timer_init(&TIMER_LED, NULL, timer_led_event_handler);
   APP_ERROR_CHECK(err_code);    
   time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_LED, time_us);      
   nrf_drv_timer_extended_compare( &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true); 
 
	
}

void peripheral_init(void)
{
   //---RX init---
 
   //led  
   nrf_gpio_cfg_output(USER_LED_PORT_PIN);    
   nrf_gpio_pin_clear(USER_LED_PORT_PIN);    
   nrf_delay_ms(1000);	
   nrf_gpio_pin_set(USER_LED_PORT_PIN);    

   //pll gpio 
   nrf_gpio_cfg_output(PLL_TEST_PORT_PIN);   
   nrf_gpio_pin_clear(PLL_TEST_PORT_PIN);   


   //---set p27 as exint

   //set p27 input
   //nrf_gpio_cfg_input(DATA_IN_PORT_PIN, NRF_GPIO_PIN_NOPULL); 
   nrf_gpio_cfg_input(DATA_IN_PORT_PIN, NRF_GPIO_PIN_PULLDOWN); 
   //nrf_gpio_cfg_input(DATA_IN_PORT_PIN, NRF_GPIO_PIN_PULLUP); 

   //set IN0 as event 
   NRF_GPIOTE->CONFIG[0] &= ~(1<<0 | 1<<1);
   NRF_GPIOTE->CONFIG[0] |= (1<<0);		

   //IN0  select p27 
   NRF_GPIOTE->CONFIG[0] &= ~(1<<8 | 1<<9 | 1<<10 | 1<<11 | 1<<12);
   NRF_GPIOTE->CONFIG[0] |= (DATA_IN_PORT_PIN<<8);			

   //set IN0 rising
   NRF_GPIOTE->CONFIG[0] &= ~(1<<16 | 1<<17 );
   NRF_GPIOTE->CONFIG[0] |= (1<<16);	

   //open IN0 int
   NRF_GPIOTE->INTENSET  = 1<<0;  

   //set Priority
   NVIC_SetPriority(GPIOTE_IRQn,3); 

   //enable interrupt
   NVIC_EnableIRQ(GPIOTE_IRQn);		 
		
      
   //---TX init---  
   
   //button
   nrf_gpio_cfg_input(BUTTON_DET_PORT_PIN, NRF_GPIO_PIN_PULLUP );  

   //get fsm and crc
   FSM_data_trans(FSM_data_in_2);
   crc_data_2 = crc16(0, FSM_data_out, 16);
   crc_fsm_trans2(crc_data_2);		
      
   FSM_data_trans(FSM_data_in_1);
   crc_data_1 = crc16(0, FSM_data_out, 16);
   crc_fsm_trans1(crc_data_1); 	
   
   nrf_gpio_cfg_output(DATA_OUT_PORT_PIN);   //output
   //nrf_gpio_cfg_output(TX_TEST_PORT_PIN);   //output
   
   //select txrx
   nrf_gpio_cfg_output(TX_POWER_PORT_PIN);  
   nrf_gpio_cfg_output(RX_POWER_PORT_PIN);
   
   if (flg_txenable==0)  
   {    
      //set to rx mode
      nrf_gpio_pin_set(TX_POWER_PORT_PIN);    		
      nrf_gpio_pin_set(RX_POWER_PORT_PIN);           
   }  
   else
   {      
      nrf_drv_timer_enable(&TIMER_LED);	
      flg_timer0=1;
      
      //set to tx mode
      nrf_gpio_pin_clear(TX_POWER_PORT_PIN);    		
      nrf_gpio_pin_clear(RX_POWER_PORT_PIN);   
   }
      
   //int1 gpio 
   nrf_gpio_cfg_output(INT1_PORT_PIN);   
   nrf_gpio_pin_set(INT1_PORT_PIN); 
	
}

void spis_init(void)
{
   nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG(SPIS_INSTANCE);
   
   spis_config.csn_pin               = SPI_SS_PORT_PIN; 
   spis_config.miso_pin              = SPI_MISO_PORT_PIN; 
   spis_config.mosi_pin              = SPI_MOSI_PORT_PIN; 
   spis_config.sck_pin               = SPI_SCK_PORT_PIN;    
   

   APP_ERROR_CHECK(nrf_drv_spis_init(&spis, &spis_config, spis_event_handler));
   spis_xfer_done = false;
    
   memset(m_rx_buf, 0, m_length);
   nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length, m_rx_buf, m_length); 
   
}

int main(void)
{
	
   //timer0 init();
   timer0_init();
   
   
   //uart init
   uart_init();   
   
	
   //peripheral init
   peripheral_init();   


   //spis init
   spis_init();


   while(1)
   {		
      uint8_t j =0;
      
      
      __WFE(); 
      
       
      if (spis_xfer_done==true)
      {         
         spis_xfer_done = false;                        
         
         memcpy(m_rx_buf2,m_rx_buf,sizeof(m_rx_buf));
         //NRF_LOG_PRINTF(" Transfer completed. Received: %s\n",m_rx_buf2);

         memset(m_rx_buf, 0, m_length);
         nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length, m_rx_buf, m_length);  
      }   

      
      //receive the command and datas
      switch (m_rx_buf2[0])
      {         
         
         case 0xA0: 
            
               ichecksum = (m_rx_buf2[0] + m_rx_buf2[1] + m_rx_buf2[2] ) & 0xff;
//               if( ichecksum !=0) 
//                  break;        
            
               switch (m_rx_buf2[1])
               {
//                  case 0xA1:

//                        rx_init();
//                  
//                  
//                        //enable interrupt
//                        NRF_GPIOTE->INTENSET  = 1<<0;  
//                        NVIC_EnableIRQ(GPIOTE_IRQn);	 

//                        
//                        if(flg_timer0 == 1)  
//                        {
//                           //close timer0： 
//                           NVIC_DisableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//                           nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_STOP); 
//                           
//                           flg_timer0=0;
//                        }                           
//                  
//                        break;  
                        
                        
//                  case 0xA2:   

//                        //nrf_gpio_pin_clear(TX_POWER_PORT_PIN);                   

//                        tx_init();  
//                  
//                        //open exint
//                        NRF_GPIOTE->INTENSET  = 1<<0;  
//                        NVIC_EnableIRQ(GPIOTE_IRQn);	 
//                  

//                        if(flg_timer0==0)
//                        {
//                           //reopen timer0 ：              
//                           NVIC_ClearPendingIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//                           NVIC_EnableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );    
//                           nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   

//                           nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);   
//                           
//                           flg_timer0=1;
//                        }                    
//                  
//                        break;   

//                  case 0xA3:                     
//      
//                           ;
//                  
//                        break;    
                        
//                  case 0xA4:
//                     
//                           //nrf_gpio_pin_set(TX_POWER_PORT_PIN); 
//                  
//                           //close exint
//                           NVIC_DisableIRQ(GPIOTE_IRQn);                   
//                           NRF_GPIOTE->INTENSET  &= ~(1<<0);  

//                           if(flg_timer0 == 1)  
//                           {
//                              //close timer0： 
//                              NVIC_DisableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//                              nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_STOP); 
//                              
//                              flg_timer0=0;
//                           }   
//                  
//                                    
//                        break;   

                  case 0xA1:

                           nrf_gpio_pin_clear(TX_POWER_PORT_PIN);                   
                  
                        break;   
                        

                  case 0xA2:

                           nrf_gpio_pin_clear(TX_POWER_PORT_PIN);                   
                  
                        break;   
                        

                  case 0xA3:                     
      
                           ;
                  
                        break;    
                        

                  case 0xA4: 
                     
                           nrf_gpio_pin_set(TX_POWER_PORT_PIN);  
                                        
                  
                        break;    
                 
                  
                  case 0xA6:
                     
                           NVIC_SystemReset();
                  
                        break;      
                  
                  
                  case 0xB0:

                        tx_init();  
                  
                        ////open exint
                        //NRF_GPIOTE->INTENSET  = 1<<0;  
                        //NVIC_EnableIRQ(GPIOTE_IRQn);	 
                  

                        if(flg_timer0==0)
                        {
                           //reopen timer0 ：              
                           NVIC_ClearPendingIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
                           NVIC_EnableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );    
                           nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   

                           nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);   
                           
                           flg_timer0=1;
                        }                    
                  
                        break;  
                        
                        
                  case 0xB1: 
                     
                           ;
                  
                        break;  

                  default:
                  
                        break;  

               }            
            
               break;           
               
         
         case 0xD1:  //D1 + 8bytes  + CHECKSUM
                
               ichecksum=0;
               for(j=0; j<10; j++)
               {
                  ichecksum += m_rx_buf2[j];
               }
               ichecksum &= 0xff;
               
         
               itxcode = DUMMYCODE;
               
               if( ichecksum !=0) 
                  break;  

               
               itxcode = CODE1;

               //tx
               memcpy(itxbuf_nano, m_rx_buf2+1, 8);
               
               
               split8(itxbuf_nano,8);
               
               inanobuf_crc = crc16(0, itxbuf_nano,16);
               itxbuf_nano[16] = (uint8_t)(inanobuf_crc & 0x000f);
               itxbuf_nano[17] = (uint8_t)( (inanobuf_crc) >>4 & 0x000f);
               itxbuf_nano[18] = (uint8_t)( (inanobuf_crc) >>8 & 0x000f);
               itxbuf_nano[19] = (uint8_t)( (inanobuf_crc) >>12 & 0x000f);
               
               idatalen_nano=20;
               
               
               //put data to CODE1'databuf   --test 
               for (j=0; j<16; j++)
               {
                  FSM_data_in_1[j] = itxbuf_nano[j];                  
               }
    
               
               //get crc
               FSM_data_trans(FSM_data_in_1);        
               crc_data_1 = crc16(0, FSM_data_out, 16);    
               crc_fsm_trans1(crc_data_1); 	  


               break;
         
         case 0xD2:            
            
               break;
          
         case 0xD3:            
            
               break;      

         default:
         
               break;           
         
      }

      if (flg_txenable==0)  
      {
           
         static u32 imain_count=0;		
         static u32 imain_count2=0;
         u8 j=0;
         static u32 iledcount=0;
         static u32 ioffcount=0;      

         u16 icrcvalue=0; 
         u16 icrcvalue2=0;
          
             
         if(imain_count2++>1) 
         {
            imain_count2=0;

            imain_count++;		
         }

         if(imain_count>10000   )
         { 
            imain_count=0;  
            
            if (ioffcount++>=10)
            {
               ioffcount=20;
               flg_flash=0;
               
               iputs((u8*)"nodata\r\n");
            }               
            
            if (flg_flash==1)            
               nrf_gpio_pin_clear(USER_LED_PORT_PIN);
            else if(flg_flash==2)   
               nrf_gpio_pin_toggle(USER_LED_PORT_PIN);
            else if (flg_flash==0)   
            {
               iledcount++;   
               if (iledcount>=1)   
               {
                  iledcount=10;      
                  nrf_gpio_pin_set(USER_LED_PORT_PIN);   
               }   
               
            }     
            
         }            

         if ( flg_off ==1  )	  
         {         
            u8 flg_errdata =0;
            
            u8 stempbuf[TXBUFLEN];
            
            flg_off =0;         
            //ioffcount=0;                 
            
            
            if ( flg_getdata ==1  )	
            {
               //flg_getdata	=0;
               ioffcount=0;
               
               for (j=0; j<16; j++)
               {
                  idatain3[j]=FSM_decoder(idatain2[j]);
                  
                  stempbuf[j] = idatain3[j]; 
                  
                  //uartdecode2(j,"d: "); 	
                  //uartdecodeH(idatain3[j]," 0x"); 	
                  
                  //nrf_delay_ms(1);
                  //delay_count(3000); 
               }	
               
               combin8(stempbuf,16);
               

               memcpy(m_tx_buf+1, stempbuf,8 );

               
//               for (j=8; j>0; j--)
//               {
//                   m_tx_buf[j] =  m_tx_buf[j-1];
//               }
               
               m_tx_buf[0] = 0xD1;
               
               //get checksums
               ichecksum=0;
               for(j=0; j<9; j++)
               {
                  ichecksum += m_tx_buf[j];
               }
               ichecksum &= 0xff;               
               
               m_tx_buf[9] = 0x100-ichecksum;
               
               
               for (j=0; j<4; j++)
               {
                  icrc3[j]=FSM_decoder(icrc2[j]);

                  //uartdecode2(j,"c: "); 	
                  //uartdecodeH(icrc3[j]," 0x"); 	
                  
                  //nrf_delay_ms(1);
                  //delay_count(3000);
               }				
               iputs((u8*)"\r\n");	            
               
               //crc check             
               icrcvalue = (u16)(icrc3[3]<<12) + (u16)(icrc3[2]<<8) +  (u16)(icrc3[1]<<4) + icrc3[0];          
               uartdecodeH16(icrcvalue,"crc: ");
               
               
               icrcvalue2 = crc16(0, idatain2, 16);
               //uartdecodeH16(icrcvalue2,"crc2: ");
               
               
               if(icrcvalue != icrcvalue2)
               {
                  flg_errdata=1;
                  iputs((u8*)"errdata\r\n");	
               }  
               else            
               {
                  iputs((u8*)"dataok\r\n");	
               }            
              
               //get led state            
               flg_flash=0;
               if( (icrcvalue == 0xc204) && ( memcmp(FSM_CODE1,idatain2,16)==0 ) )            
                  flg_flash=1;            
               else if(  (icrcvalue == 0xe679)  && ( memcmp(FSM_CODE2,idatain2,16)==0 ) )            
                  flg_flash=2;  
               //else if(  (icrcvalue == 0x9F59)  && ( memcmp(FSM_CODE3,idatain3,16)==0 ) )            
               //   flg_flash=3;    
               
               
               if(flg_errdata==1)
                  flg_flash=0;
               
               
               if(flg_flash>0) 
                  iledcount=0;
               
               memset(idatain2,0,16);	      		
               memset(icrc2,0,4);	
               
               memset(idatain3,0,16);	      		
               memset(icrc3,0,4);	
              
               
               flg_getdata	=0;
            }

         }
         

         if (flg_start==1 && flg_off==0)
         {			
            flg_start=2;
            
            //iputs((u8*)"sta2\r\n");	
            
            
//            //reopen timer0 - 1：                
//            NVIC_ClearPendingIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
//            NVIC_EnableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );    
//            nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   // TASK_CLEAR=1;  
            
            //nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);   // TASK_START=1;         
            
            flg_reset_t3=1;

         }	
         
      }            
      else
      {
                  
         //if(!nrf_gpio_pin_read(BUTTON_DET_PORT_PIN))  
         if(itxcode==CODE1)
         {
            fsm_channel = FSM_CHANEL_1;
         }
         else if(itxcode==CODE2)
         {
            fsm_channel = FSM_CHANEL_2;
         } 
         else            
            fsm_channel=FSM_CHANEL_DUMMY;
            
               
      }


    }		
		
}


//========================================================================



#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */


void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        ;//APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        ;//APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


void uart_config(void)
{
    uint32_t err_code;
	
    const app_uart_comm_params_t comm_params =
    {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,    //关闭流控
          false,
          UART_BAUDRATE_BAUDRATE_Baud115200  //波特率设置为115200bps
    };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOW,
                         err_code);

    APP_ERROR_CHECK(err_code);
    
}


//#ifndef Tx_Enable

void GPIOTE_IRQHandler(void)  
{  
    if((NRF_GPIOTE->EVENTS_IN[0]==1)&&(NRF_GPIOTE->INTENSET&GPIOTE_INTENSET_IN0_Msk))       
    {  
        NRF_GPIOTE->EVENTS_IN[0]=0;  
			
        //flg_exint=1; 
        //iputs((u8*)" in0 come \r\n");  //--zz debug      
			
			
				if(flg_start==0) 
				{
					//iputs((u8*)"sta\r\n");							
					flg_start =1;            
				}
	 
					
				if (flg_reset_t3==1)
				{								
					flg_reset_t3=0;		
               
               //iputs((u8*)"sta3\r\n");		//can't open this iputs							
    
               delay_count(DELAY_COUNT);       



               //reopen timer0 - 1：              
               NVIC_ClearPendingIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );
               NVIC_EnableIRQ( (IRQn_Type) ((u32)NRF_TIMER0>>12) );    
               nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   // TASK_CLEAR=1;  
               

					
					//reopen timer0 - 2：              
					//NVIC_ClearPendingIRQ( (u32)NRF_TIMER0>>12 );
					//NVIC_EnableIRQ( (u32)NRF_TIMER0>>12);    
					//nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);   // TASK_CLEAR=1；  
               

					nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);   // TASK_START=1；                
               flg_timer0=1;					
					
					flg_start=3;	
				}            
				
    }  
} 

//#endif




