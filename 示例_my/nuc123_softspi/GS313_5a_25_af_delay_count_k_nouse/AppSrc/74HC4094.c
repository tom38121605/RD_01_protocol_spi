/****************************************************************************
 * @file:		74HC4096.c
 * @version:	v1.0
 * @breif:		74HC4096 source file
 * @date: 	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "74hc4096.h"
#define SPI_DATA_LENGTH		16
#define TLED_RED_BIT		0X1
#define TLED_WHITE_BIT		0X2
#define TLED_NEXT_BIT   		0X4
#define TLED_PRE_BIT			0X8
#define TLED_VOL_ADD_BIT	0X10
#define TLED_VOL_DEC_BIT	0X20
#define TLED_FH_BIT			0X40
#define TLED_MUTE_BIT		0X80





#define LED_INIT_STATE 0x02
#define LED_STROB_PORT PD
#define LED_STROB_PIN	2
#define LED_STROB_PORT_PIN PD2


u8 gain;
u8 cross;
static u16 data_74hc4096 = LED_INIT_STATE;
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
	SPI_WRITE_TX0(SPI2, (data_74hc4096 | 0xffff) );        
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
void showResetLed()     // ma 20150525 reset led
{
	U16 	src_bits_msk;
	src_bits_msk = get74data();
	src_bits_msk &= 0x0000;
	send74data(src_bits_msk);
}
void showGainLed(u32 d)
{
	U16 	src_bits_msk;
	
	src_bits_msk = get74data();
// 	src_bits_msk &= 0xff;
// 	gain = ~(0x100>>d);
// 	send74data(src_bits_msk | ((gain << 8)&0xff00));
	src_bits_msk &= 0xff01;
	gain = ~(0x100>>d);
	send74data(src_bits_msk | (gain&0xfe));
	
}

void showCrossLed(u32 d)
{
	U16 	src_bits_msk;
	
	src_bits_msk = get74data();
// 	src_bits_msk &= 0xff00;
//kmf 	cross = ~(0x100>>d);
// 	send74data(src_bits_msk | (cross&0xff));
	src_bits_msk &= 0x01ff;
	cross = ~(0x100>>d);
	send74data(src_bits_msk | ((cross << 8)&0xfe00));
}


void showPhaseLed(void)
{
	static int i=0;  // i must be static
 	U16 src_bits_msk;
	i++;
 	src_bits_msk = get74data();
 	src_bits_msk &= 0xfefe;
	if(i%2==1)
		send74data(src_bits_msk | ~(0xfffe));
	else 
	{
		send74data(src_bits_msk | ~(0xfeff));
		i=0;
	}
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
	//--U8 bits_msk;
	
	//bits_msk = (get74data()&0xf3)|((mode&0x3)<<W58_PRESEL_POS);
	//send74data(bits_msk);
	
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
	//--u16 color_mask;
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


void doBreathingLed()
{
	//--u16 color_mask;	
	//--u16 color_bak;
	

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
	
	GPIO_CLR_PIN(LED_STROB_PORT_PIN); 
	GPIO_SET_MODE(LED_STROB_PORT, LED_STROB_PIN, GPIO_OUTPUT); 
	
	GPIO_SPI2_CLK_PD1(); 
	GPIO_SPI2_MOSI0_PD3(); 
	
	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS); 
	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE2, SPI_DATA_LENGTH); 
	
	SPI_SET_SUSPEND_CYCLE(SPI2, SPI_SUSPEND_0_5); 
	//SPI_SET_LSB_FIRST(SPI2);   //MSB first  //--LSB first 
	SPI_SET_MSB_FIRST(SPI2);  //MSB first
	
	SPI_DISABLE_REORDER(SPI2); 
	SPI_ENABLE_BCN(SPI2); //Backward compatible 
	SPI_SET_DIVIDER(SPI2, 49); //SPIclk = 90khz 
	
	//send74data(0XFFFF);
	send74data(0X0000);
}








