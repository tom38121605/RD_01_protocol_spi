/****************************************************************************
 * @file:		filter.c
 * @version:	V1.00
 * @breif:		filter source file
 * @date: 	2013.4.12	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "filter.h"
#include "filter_config.h"
#include "limiter.h"

extern u32 daL, daR;


	
U32 firsr_trigger = FALSE;
U32 filter_trigger = FALSE;
u32 filter_rxbuf_flag;
u32 filter_bypass = FALSE;
u32 drc_bypass = FALSE;


s32 t_s, t_e, t_t, t_max;


/*============== these configuration can be changed ==============*/
#define F_L_K1_SHIFT   22 // LPF1

#include "asm.c" 

/*-------------------------------------------------------------------------
 * @fucname:	 dataProcess
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
#define 	WIN_SIZE		(48*1)
#define  	WIN_SIZE_ROOT	(3*4)
#define 	LOOKAHEAD		(48*1)
#define 	ATTACT_T		900
#define 	RELEASE_T		1019
#define 	THREAD_HOLD	0X3000

typedef s16 wavepoolT[2];

wavepoolT winpool[WIN_SIZE];

//--static u32 win_index = 0;
//--static u32 env = 0;
//--static u32 summ = 0;


s32 abs(s32 d)
{
	return d<0? -d : d;
}



/*--static void i2sDataSend(s32 L, s32 R)
{
	I2S_SET_DATA(L);
	I2S_SET_DATA(R); 
}
*/

/*-------------------------------------------------------------------------
 * @fucname:	 I2S_IRQHandler
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
#define COFLEVEL		0X800
#define BASE_K		0X5
u32 kkk = 0;
extern u32 BT_MATCH_FLAG;
extern s8 tws_out;
//--static s32 yo = 0;
s32 lpf_out=0;
extern u32 vol_set;
/*--static void uartdecode(u32 x, u8* str)
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

 
static S32 lpf(xi)
{
	xi -= yo;
	xi >>= 6;
	yo = xi + yo;
	
	return yo;
}
*/
 
 
// u32 lo[33] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31};
// S32 sum_H;	
// void I2S_IRQHandler()
// {
// 	static s32 sum_LL, sum_RL;//--, sum_H, sum_bak, sum;
// 	//--static u32 index = 0;
// 	//--s32 x_in;

// 	sum_LL = I2S_GET_DATA();
// 	sum_RL = I2S_GET_DATA();	
// 	
// //iputs("I2S_IRQ in --01.\r\n");  //--zz debug
// 	
// 	//my code ......
// 	if (flg_softmute==1)
// 	{
// 		//if(isBtStreaming())
// 		sum_LL = 0;
// 		sum_RL = 0;	 	
// 		
// 	}	
// 	
// 	//--if(dev_state == DEV_STANDBY|| (dev_input == BLUETOOTH && !isBtStreaming()))   //--2015.07.22
// 	//--	sum_LL = sum_RL = 0;
// 	
// 	audio_detect_L += ((sum_LL > 0 ? sum_LL : -sum_LL)>>16); // audio detect left channel
// 	audio_detect_R += ((sum_RL > 0 ? sum_RL : -sum_RL)>>16); // audio detect right channel

// 	
// // 	#if(SPIFLASH_ENABLE == TRUE)	
// // 		dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
// // 	#endif

// 	sum_LL >>= 1;
// 	sum_RL >>= 1;
// 	
// 	I2S_SET_DATA(sum_RL + sum_LL); 
// 	I2S_SET_DATA(sum_RL + sum_LL); 
// 	
// 	return;
// }

// #define DELAY_MS	10
#define DELAY_BOX_SIZE 902
S16 delay_box[DELAY_BOX_SIZE][2];
// u32 kkk = 0;
// extern u32 BT_MATCH_FLAG;
s32 sum_H;
// extern u32 device_subt;
void I2S_IRQHandler()
{
	static s32 sum;  // sum_bak, sum, sum_out=0;
	s32 sum_LL, sum_RL;
	//--static u32 out_d = 0, in_d = DELAY_BOX_SIZE - 1;
	static struct I2S_STATS{
		u32 i2s_int:1;
		u32 i2s_rx_int:1;
		u32 i2s_tx_int:1;
		u32 r_l:1;
		u32 reserve2:4;
		u32 rx_udf :1;
		u32 rx_ovf :1;
		u32 rx_thf :1;
		u32 rx_full :1;
		u32 rx_empty :1;
		u32 reserve1:3;
		u32 tx_udf :1;
		u32 tx_ovf :1;
		u32 tx_thf :1;
		u32 tx_full :1;
		u32 tx_empty :1;
		u32 tx_busy :1;
		u32 rz_cf :1;
		u32 lz_cf :1;
		u32 rx_level :4;
		u32 tx_level :4;
	} i2s_status;
	//--static u32 index = 0;

	
	t_s = SysTick->VAL;	
	

	sum = (I2S_GET_STATUS());
	i2s_status = *(struct I2S_STATS*)&sum;
	sum_LL = I2S_GET_DATA();
	sum_RL = I2S_GET_DATA();
	
//iputs("I2S_IRQ in --01.\r\n");  //--zz debug
	
	//my code ......
	if (flg_softmute==1)
	{
		//if(isBtStreaming())
		sum_LL = 0;
		sum_RL = 0;	 	
// 			
	
	if((i2s_status.rx_level&0x1) == (i2s_status.tx_level&0x1)){
		sum = sum_LL;
		sum_LL = sum_RL;
		sum_RL = sum;
	}
	
	sum_H = (sum_LL<<8) + sum_RL;
	
// 	if(dev_state == DEV_TEST || (dev_input == BLUETOOTH && BT_MATCH_FLAG == 0)) 
// 		sum_LL = sum_RL = 0;


	audio_detect_L += ((sum_LL > 0 ? sum_LL : -sum_LL)>>16); // audio detect left channel
	audio_detect_R += ((sum_RL > 0 ? sum_RL : -sum_RL)>>16); // audio detect right channel


	if(i2s_status.r_l == 1 && (i2s_status.tx_level&0x1)){
		sum = sum_LL;
		sum_LL = sum_RL;
		sum_RL = sum; 
	}
	
#if(SPIFLASH_ENABLE == TRUE)	
		dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
#endif
	
// 	if(dev_input != WIRELESS && flash_mode != FLASH_PLAY_MUSIC){
// 		delay_box[in_d][0] = (sum_LL>>16);
// 		delay_box[in_d][1] = (sum_RL>>16);
// 		if(++in_d >= DELAY_BOX_SIZE)
// 			in_d = 0;
// 		sum_LL = (delay_box[out_d][0]<<16);
// 		sum_RL = (delay_box[out_d][1]<<16);
// 		if(++out_d >= DELAY_BOX_SIZE)
// 			out_d = 0;	

		
	}
	
	I2S_SET_DATA(sum_RL); 
	I2S_SET_DATA(sum_LL);  
	return;

}

/*-------------------------------------------------------------------------
 * @fucname:	 limiter
 * @brief: 	 change the signal 
 * @param: 	 signal
 * @return: 	 signal
 * @details: 	 change the signal 
 *-----------------------------------------------------------------------*/
/*--s32 limiter( s32 data)
{
	if(data > 0){
		if(data > 0x7fff){
			return 0x7fff;
		}
		else if(data > Limiter_X2){
			// if input > x2 then output = y2+(input-x2)*k2
			// k2 = Limiter_K2_numerator / Limiter_K2_denominator
			return 	Limiter_Y2 + 
					(((data - Limiter_X2)
						*Limiter_K2_numerator) 
						>> Limiter_K2_denominator);
		}
		else if(data > Limiter_X1){
			// if  x2> input > x1 then output = y1+(input-x1)*k1
			// k1 = Limiter_K1_numerator / Limiter_K1_denominator 
			return 	Limiter_Y1 + 
					(((data - Limiter_X1)*
					    Limiter_K1_numerator) >>
						Limiter_K1_denominator);
		}
		else if(data > Limiter_X0)
			return data;
		else {
			// if INPUT <x1  then output 0
			return 	0;
		}
		
	}
	else{
		data = -data;
		if(data > 0x7fff){
			return 0x8000;
		}
		else if(data > Limiter_X2){
			// if input > x2 then output = y2+(input-x2)*k2
			// k2 = Limiter_K2_numerator / Limiter_K2_denominator
			return 	-(Limiter_Y2 + 
					(((data - Limiter_X2)
						*Limiter_K2_numerator) 
						>> Limiter_K2_denominator));
		}
		else if(data > Limiter_X1){
			// if  x2> input > x1 then output = y1+(input-x1)*k1
			// k1 = Limiter_K1_numerator / Limiter_K1_denominator 
			return 	-(Limiter_Y1 + 
					(((data - Limiter_X1)*
					    Limiter_K1_numerator) >>
						Limiter_K1_denominator));
		}
		else if (data > Limiter_X0)
			return -data;
		else {
			// input < x1, means if input <10 then output 0
			return 	0;
		}
	}
}
*/


/*-------------------------------------------------------------------------
 * @fucname:	 filterInit
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void filterInit()
{
	firsr_trigger = TRUE;
}


/*-------------------------------------------------------------------------
 * @fucname:	 filter
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void filter()
{
	
}



/*-------------------------------------------------------------------------
 * @fucname:	 trigger_filter
 * @brief: 	 init the filter param and enable filter
 * @param: 	 none
 * @return: 	 none
 * @details: 	 none
 *-----------------------------------------------------------------------*/
void triggerFilter()
{

	
}


