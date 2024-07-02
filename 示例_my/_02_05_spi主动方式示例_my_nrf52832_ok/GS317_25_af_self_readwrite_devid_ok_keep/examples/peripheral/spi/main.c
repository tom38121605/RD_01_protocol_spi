/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>

#include "app_uart.h"  //--zz

void uart_config(void);


#define VERSION	"v11.26"     

	
//-----------------dw1000----------------------------



typedef unsigned          	char u8;
typedef unsigned short	int 	u16;
typedef unsigned           	int 	u32;
typedef unsigned       	long long u64;

typedef	signed          	char S8;
typedef   	signed short     int 	S16;
typedef   	signed           	int   	S32;
typedef   	signed       long long	S64;


//u32 dwt_readdevid(void);
//u32 dwt_read32bitoffsetreg(u16 regFileID, u16 regOffset);
void dwt_readfromdevice( u16 recordNumber, u16 index, u32 length, u8 *buffer );
int readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer);

void dwt_writetodevice( u16 recordNumber, u16 index, u32 length, const u8 *buffer );
u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer);

void reset_DW1000(void);
//void spi_set_rate_low(void);
//void spi_set_rate_high(void);



u8 ibuf[4]={0,0,0,0};
u8 ibuf2[4]={0,0,0,0};


//---------------------------------------------


#if defined(BOARD_PCA10036) || defined(BOARD_PCA10040)

//#define SPI_CS_PIN   29 /**< SPI CS Pin.*/
#define SPI_CS_PIN   17


#elif defined(BOARD_PCA10028)
#define SPI_CS_PIN   4  /**< SPI CS Pin.*/
#else
#error "Example is not supported on that board."
#endif

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

//#define TEST_STRING "Nordic"
#define TEST_STRING "Nordic 1234567890 abcde12345"
//--static uint8_t       m_tx_buf[] = TEST_STRING;           /**< TX buffer. */
//--static uint8_t       m_rx_buf[sizeof(TEST_STRING)+1];    /**< RX buffer. */
//--static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */


/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
//    NRF_LOG_PRINTF(" Transfer completed.\r\n");
//    if (m_rx_buf[0] != 0)
//    {
//        NRF_LOG_PRINTF(" Received: %s\r\n",m_rx_buf);
//    }
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
	buf[8]='\0';;

	iputs((uint8_t*)buf); 

 	iputs((uint8_t*)"\r\n");
}



int main(void)
{
  uint8_t c1=0x5a;
	
//    LEDS_CONFIGURE(BSP_LED_0_MASK);
//    LEDS_OFF(BSP_LED_0_MASK);

//    APP_ERROR_CHECK(NRF_LOG_INIT());
//    NRF_LOG_PRINTF("SPI example\r\n");
	
	  uart_config();  //串口初始化
	
	
//log_uart_write_string("12345\r\n");	
//uartdecodeH32(c1,"c1: ");   

	iputs((uint8_t*)"\r\n------------------ ");  //--zz debug         
	iputs((uint8_t*)VERSION);       
	iputs((uint8_t*)" -----------------\r\n");  //--zz debug         
			
	

    nrf_drv_spi_config_t  spi_config = NRF_DRV_SPI_DEFAULT_CONFIG(SPI_INSTANCE);
    spi_config.ss_pin = SPI_CS_PIN;
    APP_ERROR_CHECK( nrf_drv_spi_init(&spi, &spi_config, spi_event_handler) );
	
	
	  //reset_DW1000();  //--zz
	
    nrf_gpio_cfg_output2(24); 
	  nrf_delay_ms(1); 
	  nrf_gpio_pin_set(24);
	
	  nrf_gpio_cfg_input(19, NRF_GPIO_PIN_NOPULL); 		
	
	
	  //NRF_GPIO->OUTSET = (1UL << 24);     //设置pin17为高电平
	
    nrf_delay_ms(1000);		
	
    while(1)
    {		
			
        // Reset rx buffer and transfer done flag
//        memset(m_rx_buf, 0, m_length);			
//        spi_xfer_done = false;	
//        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, m_length, m_rx_buf, m_length));
//        while (!spi_xfer_done)
//        {
//            __WFE();
//        }			
			
//			  dwt_readfromdevice(0x00,0,4,ibuf);
//			  nrf_delay_ms(2000);
//			
//    uartdecodeH(ibuf[0],"  ibuf0: ");
//    uartdecodeH(ibuf[1],"  ibuf1: ");
//    uartdecodeH(ibuf[2],"  ibuf2: ");
//    uartdecodeH(ibuf[3],"  ibuf3: ");
//		iputs((uint8_t*)"\r\n"); 
			
			
			  //write to spi
				ibuf2[0]=0x18;
				ibuf2[1]=0x18;
				ibuf2[2]=0x18;
				ibuf2[3]=0x18;
				dwt_writetodevice(0x03,0,4,ibuf2);		
		  	nrf_delay_ms(100);

			  //read from spi
			  dwt_readfromdevice(0x03,0,4,ibuf);
			  nrf_delay_ms(2000);
				
    uartdecodeH(ibuf[0],"  ibuf0: ");
    uartdecodeH(ibuf[1],"  ibuf1: ");
    uartdecodeH(ibuf[2],"  ibuf2: ");
    uartdecodeH(ibuf[3],"  ibuf3: ");
		iputs((uint8_t*)"\r\n"); 

				
				//write to spi
				ibuf2[0]=0x28;
				ibuf2[1]=0x28;
				ibuf2[2]=0x28;
				ibuf2[3]=0x28;
				dwt_writetodevice(0x03,0,4,ibuf2);		
		  	nrf_delay_ms(100);
				
				
        //read from spi
			  dwt_readfromdevice(0x03,0,4,ibuf);
			  nrf_delay_ms(2000);
			
    uartdecodeH(ibuf[0],"  ibuf0: ");
    uartdecodeH(ibuf[1],"  ibuf1: ");
    uartdecodeH(ibuf[2],"  ibuf2: ");
    uartdecodeH(ibuf[3],"  ibuf3: ");
		iputs((uint8_t*)"\r\n"); 
				


//        LEDS_INVERT(BSP_LED_0_MASK);
//        nrf_delay_ms(200);

    }		
		
}




//================================================================================================



//--------------------------read-------------------------


//u32 dwt_readdevid(void)
//{
//    return dwt_read32bitoffsetreg(0x00,0);  //DEV_ID_ID
//}


//u32 dwt_read32bitoffsetreg(u16 regFileID, u16 regOffset)
//{
//    u32  regval = 0 ;
//    int     j ;
//    u8   buffer[4] ;

//    dwt_readfromdevice(regFileID,regOffset,4,buffer);  

//    for (j = 3 ; j >= 0 ; j --)
//    {
//        regval = (regval << 8) + buffer[j] ;
//    }
//		
//    return regval;
//} 


void dwt_readfromdevice( u16 recordNumber, u16 index, u32 length, u8 *buffer )
{
    u8 header[3] ;  
    int cnt = 0;  

    // get header
    if (index == 0)  
    {
        header[cnt++] = (u8) recordNumber ;  
    }
    else
    {
        header[cnt++] = (u8)(0x40 | recordNumber) ; 

        if (index <= 127)  
        {
            header[cnt++] = (u8) index ;  
        }
        else
        {
            header[cnt++] = 0x80 | (u8)(index) ; 
            header[cnt++] =  (u8) (index >> 7) ;  
        }
    }

    readfromspi(cnt, header, length, buffer);   
} 


//u16 readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
//{

//	int i=0;

//	//decaIrqStatus_t  stat ;
//	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq   //--keep
//	
//	
//	SPI_CLR_SS0(SPI2);	 
//	SPI_DISABLE_REORDER(SPI2); 
//  SPI_SET_SS0(SPI2);
//		

//	for(i=0; i<headerLength; i++)
//	{
//		spiSendWord(headerBuffer[i],8);
//		//readBuffer[0] = SPI2->RX[0];
//	}
//	
//	SPI_ENABLE_REORDER(SPI2);
//	

//	for(i=0; i<readlength; i++)
//	{
//		spiSendWord(0xff,8);
//		readBuffer[i] = SPI2->RX[0];		
//	}

//	//SPIx_CS_GPIO->BSRR = SPIx_CS;
//	SPI_CLR_SS0(SPI2);	

//	//decamutexoff(stat) ;    //--keep

//	return 0;
//} 


//int readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
//{
//	
//  u8 itempbuf[200]={0};	
//	memset(itempbuf, 0, 200);
//	
//	
////	//decaIrqStatus_t  stat ;
////	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq

//	spi_xfer_done = false;
//  nrf_drv_spi_transfer(&spi, headerBuffer, headerLength, itempbuf, headerLength);
//	while(!spi_xfer_done)				
//	;
//	
//	spi_xfer_done = false;
//  nrf_drv_spi_transfer(&spi, itempbuf, readlength, readBuffer, readlength);	
//	while(!spi_xfer_done)
//				;
//	
//	//decamutexoff(stat) ;

//	return 0;
//} 

int readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
{
	const u8 LEN1 =20;
  //u8 idatabuf[200]={0};
  //u8 itempbuf[200]={0};
  u8 idatabuf[LEN1]={0};
  u8 itempbuf[LEN1]={0};
	
	u8 * p1;
	u32 idatalength=0;
	
	memset(idatabuf, 0, LEN1);
	memset(itempbuf, 0, LEN1);	
	

  //decaIrqStatus_t  stat ;
  //stat = decamutexon();	
	
	
	p1=idatabuf;	
	memcpy(p1,headerBuffer, headerLength);
	
	p1 += headerLength;
	//memset(p1,0xff,readlength);
	memset(p1,0x00,readlength);
	
	idatalength= headerLength + readlength;
	
	
//	//decaIrqStatus_t  stat ;
//	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq

	spi_xfer_done = false;
  nrf_drv_spi_transfer(&spi, idatabuf, idatalength, itempbuf, idatalength);
	while(!spi_xfer_done)				
	  ;
	
	p1=itempbuf + headerLength;
	
	
//	//test
//	if (p1[0]!=0xff)
//	{
//		p1= p1+1; 
//	}
//	else
//		p1=itempbuf+1; 
	
	
  memcpy(readBuffer, p1, readlength);
	
	//decamutexoff(stat) ;

	return 0;
} 

//int readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
//{
//	int i=0;
//	
//	const u8 LEN1 =20;
//  //u8 idatabuf[LEN1]={0};
//  u8 itempbuf[LEN1]={0};	

//	//decaIrqStatus_t  stat ;
//	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq   //--keep


//	for(i=0; i<headerLength; i++)
//	{
//		//spiSendWord(headerBuffer[i],8);

//		spi_xfer_done = false;
//		nrf_drv_spi_transfer(&spi, headerBuffer, 1, itempbuf, 1);
//		while(!spi_xfer_done)				
//	    ;		
//		
//		nrf_delay_ms(1);
//		
//	}	

//	for(i=0; i<readlength; i++)
//	{
//		//spiSendWord(0xff,8);
//		//readBuffer[i] = SPI2->RX[0];	

//		spi_xfer_done = false;
//		nrf_drv_spi_transfer(&spi, itempbuf, 1, readBuffer+i, 1);
//		while(!spi_xfer_done)				
//	    ;				
//		
//		nrf_delay_ms(1);	
//		
//	}

//	//decamutexoff(stat) ;    //--keep

//	return 0;
//} 

	
//-----------------------write reg -----------------------


void dwt_writetodevice( u16 recordNumber, u16 index, u32 length, const u8 *buffer )
{
    u8 header[3] ;  
    int   cnt = 0;  

    if (index == 0)  
    {
        header[cnt++] = 0x80 | recordNumber ;  
    }
    else
    {
        header[cnt++] = 0xC0 | recordNumber ; 

        if (index <= 127)  
        {
            header[cnt++] = (u8)index ; 
        }
        else
        {
            header[cnt++] = 0x80 | (u8)(index) ;  
            header[cnt++] =  (u8) (index >> 7) ;  
        }
    }
   
    writetospi(cnt,header,length,buffer);
} 

u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer)
{
	const u8 LEN1 =20;
  //u8 idatabuf[200]={0};
  //u8 itempbuf[200]={0};
  u8 idatabuf[LEN1]={0};
  u8 itempbuf[LEN1]={0};
	
	u8 * p1;
	u32 idatalength=0;
	
	memset(idatabuf, 0, LEN1);
	memset(itempbuf, 0, LEN1);		 


  //decaIrqStatus_t  stat ;
  //stat = decamutexon();	
	
	
	p1=idatabuf;	
	memcpy(p1,headerBuffer, headerLength);
	p1 += headerLength;
	memcpy(p1,bodyBuffer,bodylength);
	
	idatalength= headerLength + bodylength;
	
	spi_xfer_done = false;
	nrf_drv_spi_transfer(&spi, idatabuf, idatalength, itempbuf, idatalength);
	while(!spi_xfer_done)
				;
	

	//decamutexoff(stat) ;

	return 0;
} 


//------------------------------other---------------------------


//void reset_DW1000(void)
//{
//  GPIO_SET_MODE(PB, 9, GPIO_QUASI);  
//  //GPIO_SET_MODE(PB, 9, GPIO_OUTPUT);   
//	
//	GPIO_CLR_PIN(PB9); 
//	sysDelayMs(200); 
//	GPIO_SET_PIN(PB9); 
//	sysDelayMs(50); 
//	
//  GPIO_SET_MODE(PB, 9, GPIO_INPUT); 
//	sysDelayMs(2); 
//	
//}

void reset_DW1000(void)
{
  nrf_gpio_cfg_output2(24);   
	
	nrf_gpio_pin_clear(24);  
	nrf_delay_ms(200); 
	nrf_gpio_pin_set(24);  
	nrf_delay_ms(50); 
	
  nrf_gpio_cfg_input(24, NRF_GPIO_PIN_NOPULL); 
	nrf_delay_ms(2); 
	
}



//void reset_DW1000(void)
//{
//  nrf_gpio_cfg_output2(24);   
//	
////	nrf_gpio_pin_clear(24);  
////	nrf_delay_ms(200); 
//	nrf_gpio_pin_set(24);  
//	nrf_delay_ms(50); 
////	
////  nrf_gpio_cfg_input(24, NRF_GPIO_PIN_PULLUP); 
////	nrf_delay_ms(2); 
//	
//}

//void spi_set_rate_low(void)
//{
//	
//	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
//	SPI_SET_SS0(SPI2);
//	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE0, 32);
//	SPI_SET_MSB_FIRST(SPI2);   //MSB first
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_ENABLE_BCN(SPI2); //Backward compatible
//	SPI_SET_DIVIDER(SPI2, 13); //SPIclk = 2.7xx MHZ	
//}

//void spi_set_rate_high(void)
//{
//	
//	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
//	SPI_SET_SS0(SPI2);
//	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE0, 32);
//	SPI_SET_MSB_FIRST(SPI2);   //MSB first
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_ENABLE_BCN(SPI2); //Backward compatible
//	SPI_SET_DIVIDER(SPI2, 1); //SPIclk = 18.xx MHZ	
//}




//========================================================================


#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */


void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
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




