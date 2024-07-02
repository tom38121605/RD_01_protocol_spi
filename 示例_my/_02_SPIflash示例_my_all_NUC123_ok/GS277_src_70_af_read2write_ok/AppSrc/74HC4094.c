/****************************************************************************
 * @file:		74HC4096.c
 * @version:	v1.0
 * @breif:		74HC4096 source file
 * @date: 	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "74hc4096.h"
#define SPI_DATA_LENGTH		8
#define SRC_GREEN_BIT 		0X4
#define SRC_RED_BIT			0X2
#define SRC_BLUE_BIT   		0X1
#define SOURCE_POS			5


#define W58_PRESEL1	 		0X2
#define W58_PRESEL2 		0X1
#define W58_PRESEL_POS		2



#define LED_INIT_STATE 0xf3

#define LED_STROB_PORT PD
#define LED_STROB_PIN	2
#define LED_STROB_PORT_PIN PD2


static u8 data_74hc4096 = LED_INIT_STATE;
extern u32 soft_mute;

/*-------------------------------------------------------------------------
 * @fucname:	 send74data
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void send74data(u16 data)
{
	GPIO_CLR_PIN(LED_STROB_PORT_PIN);
	SPI_WRITE_TX0(SPI2, data);        
	SPI_SET_GO(SPI2); 
	while(SPI_IS_BUSY(SPI2) != 0); 
	data_74hc4096 = data;
	GPIO_SET_PIN(LED_STROB_PORT_PIN);
}


void ledOff()
{
	GPIO_CLR_PIN(LED_STROB_PORT_PIN);
	SPI_WRITE_TX0(SPI2, (data_74hc4096 | 0xf3) );        
	SPI_SET_GO(SPI2); 
	while(SPI_IS_BUSY(SPI2) != 0); 
	GPIO_SET_PIN(LED_STROB_PORT_PIN);
}
/*-------------------------------------------------------------------------
 * @fucname:	 get74data
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u16 get74data(void )
{
	return data_74hc4096;
}

void set74data(u16 data)
{
	data_74hc4096 = data;
}

void showSourceLed(enum LED_COLOR color)
{
	U16 	src_bits_msk;
	
	switch(color){
		case DARK_COLOR:
							src_bits_msk = 0;	
							break;
		case RED_COLOR:	
							src_bits_msk = SRC_RED_BIT;
							break;
		case BLUE_COLOR:
							src_bits_msk = SRC_BLUE_BIT;
							break;
		case GREEN_COLOR:
							src_bits_msk = SRC_GREEN_BIT;
							break;
		case SOE_COLOR:
							src_bits_msk = SRC_RED_BIT+SRC_BLUE_BIT;
							break;
		case LIGHT_BLUE_COLOR:
							src_bits_msk = SRC_GREEN_BIT + SRC_BLUE_BIT;
							break;
		case BLUETOOTH_COLOR:
							src_bits_msk = SRC_BLUE_BIT;
							break;
		case YELLOW_COLOR:
							src_bits_msk = SRC_RED_BIT + SRC_GREEN_BIT;
							break;					
		case PURPLE_COLOR:
							src_bits_msk = SRC_RED_BIT+SRC_BLUE_BIT;
							break;
		case WHITE_COLOR:
							src_bits_msk = SRC_RED_BIT+SRC_BLUE_BIT+SRC_GREEN_BIT;
							break;
		default:
				return;
	}
	
	src_bits_msk = get74data()&~(0x7<<SOURCE_POS) |((~src_bits_msk&0x7)<<SOURCE_POS);
	send74data(src_bits_msk);
	
}
/*-------------------------------------------------------------------------
 * @fucname:	 setw58Presel
 * @brief: 	 SET SE_W58 PRESEL PIN TO CONTROL W3D MODE
 * @param: 	 mode : 0 , 1, 2, 3 
 * @return: 
 * @details: 	 
 *-----------------------------------------------------------------------*/
void setw58Presel(u32 mode)
{
	U8 bits_msk;
	
	bits_msk = (get74data()&0xf3)|((mode&0x3)<<W58_PRESEL_POS);
	send74data(bits_msk);
	
}





/*-------------------------------------------------------------------------
 * @fucname:	 flashVolumeLed
 * @brief: 	 when press inc/dec key to control volume down/up flash the volume led
 * @param: 
 * @return: 
 * @details: 	if volume if zero, mute the ad82584. else flash the corresponding led
 *-----------------------------------------------------------------------*/
void flashVolumeLed()
{
// 	u16 color_mask;    //ma 20150629  
/*
	if(amp_volume == 0){
		color_mask = get74data();
		color_mask = color_mask |(0x1f<<VOLUME_POS);
		send74data(color_mask);
		soft_mute = FALSE;
		seW58MuteSwitch();
	}else{
		led_flash = 4;
	}
	*/
}


void doflashSrcLed()
{
	u16 color_mask;	
	u16 color_bak;
	
	color_bak = color_mask = get74data();
	color_mask = color_mask|(0x7<<SOURCE_POS);
	send74data(color_mask);
	data_74hc4096 = color_bak;
}

/*-------------------------------------------------------------------------
 * @fucname:	 hc4096Init
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void hc4096Init(void)
{
/*
	GPIO_CLR_PIN(LED_STROB_PORT_PIN);
	GPIO_SET_MODE(LED_STROB_PORT, LED_STROB_PIN, GPIO_OUTPUT);
	GPIO_SPI2_CLK_PD1();
	GPIO_SPI2_MOSI0_PD3();
	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE2, SPI_DATA_LENGTH);
	SPI_SET_SUSPEND_CYCLE(SPI2, SPI_SUSPEND_0_5);
	SPI_SET_LSB_FIRST(SPI2);   //MSB first
	SPI_DISABLE_REORDER(SPI2);
	SPI_ENABLE_BCN(SPI2); //Backward compatible
	SPI_SET_DIVIDER(SPI2, 49); //SPIclk = 90khz
	send74data(LED_INIT_STATE);
*/
}








