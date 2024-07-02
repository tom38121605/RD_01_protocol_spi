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

//extern s32 i2s_rxbuf[];
//extern s32 i2s_txbuf[];
//extern s32 i2s_pdma_rxbuf_flag;
extern u32 daL, daR;

s16 test_sound_data[48] = {
	0x039c,
	0x130c,
	0x2254,
	0x2f59,
	0x3b75,
	0x45d6,
	0x4f83,
	0x5822,
	0x6960,
	0x67de,
	0x6eaf,
	0x7421,
	0x7813,
	0x79b1,
	0x792e,
	0x75de,
	0x7022,
	0x67c8,
	0x5d54,
	0x50c3,
	0x4297,
	0x3301,
	0x226d,
	0x115e,
	0x00e1,
	0xff8b,
	0xdf98,
	0xd138,
	0xc3d1,
	0xb82c,
	0xad34,
	0xa390,
	0x9a44,
	0x9203,
	0x8a95,
	0x84b2,
	0x81e1,
	0x80a0,
	0x82bf,
	0x880d,
	0x91c9,
	0x9ceb,
	0xabb6,
	0xb9dc,
	0xca93,
	0xd8f2,
	0xe8ef,
	0xf888,
};
	
U32 firsr_trigger = FALSE;
U32 filter_trigger = FALSE;
u32 filter_rxbuf_flag;
s32 *filter_srcbuf = 0;
s32 *filter_destbuf = 0;
u32 filter_bypass = FALSE;
u32 drc_bypass = FALSE;

// filter 1
s32 y1_L3,y1_L2,y1_L1,x1_L3,x1_L2,x1_L1;
s32 y2_L3,y2_L2,y2_L1,x2_L3,x2_L2,x2_L1;

// filter 2
s32 y3_L3,y3_L2,y3_L1,x3_L3,x3_L2,x3_L1;
s32 y4_L3,y4_L2,y4_L1,x4_L3,x4_L2,x4_L1;

// filter 3
s32 y5_L3,y5_L2,y5_L1,x5_L3,x5_L2,x5_L1;
s32 y6_L3,y6_L2,y6_L1,x6_L3,x6_L2,x6_L1;

// filter 4
s32 y7_L3,y7_L2,y7_L1,x7_L3,x7_L2,x7_L1;
s32 y8_L3,y8_L2,y8_L1,x8_L3,x8_L2,x8_L1;

// filter 5
s32 y9_L3,y9_L2,y9_L1,x9_L3,x9_L2,x9_L1;
s32 y10_L3,y10_L2,y10_L1,x10_L3,x10_L2,x10_L1;

// filter 1
s32 y1_R3,y1_R2,y1_R1,x1_R3,x1_R2,x1_R1;
s32 y2_R3,y2_R2,y2_R1,x2_R3,x2_R2,x2_R1;

// fiRter 2
s32 y3_R3,y3_R2,y3_R1,x3_R3,x3_R2,x3_R1;
s32 y4_R3,y4_R2,y4_R1,x4_R3,x4_R2,x4_R1;

// fiRter 3
s32 y5_R3,y5_R2,y5_R1,x5_R3,x5_R2,x5_R1;
s32 y6_R3,y6_R2,y6_R1,x6_R3,x6_R2,x6_R1;

// fiRter 4
s32 y7_R3,y7_R2,y7_R1,x7_R3,x7_R2,x7_R1;
s32 y8_R3,y8_R2,y8_R1,x8_R3,x8_R2,x8_R1;

// fiRter 5
s32 y9_R3,y9_R2,y9_R1,x9_R3,x9_R2,x9_R1;
s32 y10_R3,y10_R2,y10_R1,x10_R3,x10_R2,x10_R1;

// HPF1
s32 x1_hpL = 0, x1_hpH = 0, acc1_L = 0, acc1_H = 0, y1_hpL = 0, y1_hpH = 0;

// HPF2
s32 x2_hpL = 0, x2_hpH = 0, acc2_L = 0, acc2_H = 0, y2_hpL = 0, y2_hpH = 0;

// HPF3
s32 x3_hpL = 0, x3_hpH = 0, acc3_L = 0, acc3_H = 0, y3_hpL = 0, y3_hpH = 0;

// HPF4
s32 x4_hpL = 0, x4_hpH = 0, acc4_L = 0, acc4_H = 0, y4_hpL = 0, y4_hpH = 0;

s32 K1_HP = 0x5c;
s32 K2_HP = 0x5c;
s32 K3_HP = 0x5c;
s32 K4_HP = 0x5c;

s32 t_s, t_e, t_t, t_max;


/*============== these configuration can be changed ==============*/
#define F_L_K1_SHIFT   22 // LPF1
S32 F_L_K11	= 0xFF;//0xb2;
S32 F_L_K12	= 0x1FF;//0x164;
S32 F_L_K13	= 0xFF;//0xb2;
S32 F_L_K14	= 0x7e93FF;//0x7ed0aa;
S32 F_L_K15	= 0xffc16801;//0xffc12c8e;

#define  F_L_K2_SHIFT  22 //LPF2
S32 F_L_K21	= 0x10C3;
S32 F_L_K22	= 0x2187;
S32 F_L_K23	= 0x10C3;
S32 F_L_K24	= 0x7A144C;
S32 F_L_K25	= 0xffc5A8A6;

#define F_L_K3_SHIFT  22   // HPF1
S32 F_L_K31	= 0x3F4aff;
S32 F_L_K32	= 0xff816A01;   
S32 F_L_K33	= 0x3F4aff;   
S32 F_L_K34	= 0x7E93ff;   
S32 F_L_K35	= 0xffc16801;   

s32 F_L_K4_SHIFT = 15;			
S32 F_L_K41	= 0x7fff;
S32 F_L_K42	= 0x0;   
S32 F_L_K43	= 0x0;   
S32 F_L_K44	= 0x0;   
S32 F_L_K45	= 0x0;   

s32 F_L_K5_SHIFT = 15;						   
S32 F_L_K51	= 0x7fff;
S32 F_L_K52	= 0x0;   
S32 F_L_K53	= 0x0;    
S32 F_L_K54	= 0x0;    
S32 F_L_K55	= 0x0;    

/*================= filter bypass configuration ==================*/
S32 F_R_K11	= 0x7fff;
S32 F_R_K12	= 0x0;
S32 F_R_K13	= 0x0;
S32 F_R_K14	= 0x0;
S32 F_R_K15	= 0x0;

S32 F_R_K21	= 0x7fff; 
S32 F_R_K22	= 0x0;    
S32 F_R_K23	= 0x0;    
S32 F_R_K24	= 0x0;    
S32 F_R_K25	= 0x0;    
             
S32 F_R_K31	= 0x7fff;
S32 F_R_K32	= 0x0;   
S32 F_R_K33	= 0x0;   
S32 F_R_K34	= 0x0;   
S32 F_R_K35	= 0x0;   
            
S32 F_R_K41	= 0x7fff;
S32 F_R_K42	= 0x0;   
S32 F_R_K43	= 0x0;   
S32 F_R_K44	= 0x0;   
S32 F_R_K45	= 0x0;   
                           
S32 F_R_K51	= 0x7fff;
S32 F_R_K52	= 0x0;   
S32 F_R_K53	= 0x0;    
S32 F_R_K54	= 0x0;    
S32 F_R_K55	= 0x0;  

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

static u32 win_index = 0;
static u32 env = 0;
static u32 summ = 0;


s32 abs(s32 d)
{
	return d<0? -d : d;
}

void dataProcess(s32 *sum_L, s32 *sum_R)
{
	static u32 rms;
	s16 left,right;
	u32 threta;
	u32 gain = 1024;

	left = *sum_L>>16;
	right = *sum_R>>16;
	// caculate the rms
	{
		summ -= ((winpool[win_index][1] + winpool[win_index][0])>>1);
		winpool[win_index][0] = abs(left);
		winpool[win_index][1] = abs(right);
		summ += ((winpool[win_index][0] + winpool[win_index][1]) >> 1) ;

		if(++win_index >= WIN_SIZE){
			win_index = 0;
		}
	}
		
		
	rms = ((summ*21)>>10);
	threta = rms > env ? ATTACT_T : RELEASE_T;
	
	env = ((threta*rms + (1024 - threta)*env)>>10);

	gain = 1024;
	if(env > THREAD_HOLD)
		gain = gain - (12*(env - THREAD_HOLD)>>8);

	*sum_L = ((left*gain>>10)<<16);
	*sum_R = ((right*gain>>10)<<16);

}


// static void i2sDataSend(s32 L, s32 R)
// {
// 	I2S_SET_DATA(L);
// 	I2S_SET_DATA(R); 
// }


/*-------------------------------------------------------------------------
 * @fucname:	 I2S_IRQHandler
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
#define DELAY_MS	10
#define DELAY_BOX_SIZE 902
S16 delay_box[DELAY_BOX_SIZE][2];
u32 kkk = 0;
extern u32 BT_MATCH_FLAG;
s32 sum_H;
extern u32 device_subt;
extern u8 dwhp83_type;
u8 reset_en = 0;
void I2S_IRQHandler()
{
	static s32  sum;
	s32 sum_LL, sum_RL;
	static u32 count = 0;
	static u32 out_d = 0, in_d = DELAY_BOX_SIZE - 1;
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
// 	static u32 index = 0;

	
	t_s = SysTick->VAL;	
	

	sum = (I2S_GET_STATUS());
	i2s_status = *(struct I2S_STATS*)&sum;
	sum_LL = I2S_GET_DATA();
	sum_RL = I2S_GET_DATA();
	
	
   /*******ESD check adc  function *******/
	if((((sum_LL & 0xffff0000)== 0xffff0000)||((sum_LL & 0xffff0000)== 0x00000000))
		&&((sum_RL & 0xffff0000)== 0xffff0000)||((sum_RL & 0xffff0000)== 0x00000000)){
			count++;
			if(count > 24000){
				count = 0;
				reset_en = 1;
			}	
		}else
		{
				reset_en = 0;
				count = 0;
		}
	
  if(flash_mode == FLASH_PLAY_MUSIC)
	{	
		sum_LL = 0; 
		sum_RL = 0;		
		
		#if(SPIFLASH_ENABLE == TRUE)	
			dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
		#endif
	}
	else
	{			
		if((i2s_status.rx_level&0x1) == (i2s_status.tx_level&0x1)){
			sum = sum_LL;
			sum_LL = sum_RL;
			sum_RL = sum;
		}
		
		sum_H = (sum_LL<<8) + sum_RL;
		
	// 	if(dev_state == DEV_TEST || (dev_input == BLUETOOTH && BT_MATCH_FLAG == 0)) 
	// 		sum_LL = sum_RL = 0;
			if(change_source_mute)
			{
				sum_LL = 0;
				sum_RL = 0;
			}


		audio_detect_L += ((sum_LL > 0 ? sum_LL : -sum_LL)>>16); // audio detect left channel
		audio_detect_R += ((sum_RL > 0 ? sum_RL : -sum_RL)>>16); // audio detect right channel


		if(i2s_status.r_l == 1 && (i2s_status.tx_level&0x1)){
			sum = sum_LL;
			sum_LL = sum_RL;
			sum_RL = sum; 
		}

		
		if(play_demo_sound_mode) 
		{
			sum_LL = 0;
			sum_RL = 0;
		}
	}
	
// #if(SPIFLASH_ENABLE == TRUE)	
// 		dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
// #endif
	
	if(dev_input != WIRELESS && flash_mode != FLASH_PLAY_MUSIC){
		delay_box[in_d][0] = (sum_LL>>16);
		delay_box[in_d][1] = (sum_RL>>16);
		if(++in_d >= DELAY_BOX_SIZE)
			in_d = 0;
		sum_LL = (delay_box[out_d][0]<<16);
		sum_RL = (delay_box[out_d][1]<<16);
		if(++out_d >= DELAY_BOX_SIZE)
			out_d = 0;	
	}
	else if(flash_mode == FLASH_PLAY_MUSIC)
	{	
		delay_box[in_d][0] = 0;
		delay_box[in_d][1] = 0;
		if(++in_d >= DELAY_BOX_SIZE)
			in_d = 0;		
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
s32 limiter( s32 data)
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
#if 0
	u32 index, i = 0;
	volatile s32 sum;

	if(filter_trigger){
		filter_trigger = FALSE;

		t_s = SysTick->VAL;
		while(i < (AUDIO_DMA_BUF_SIZE >> 1)){

			index = 2*i;
			
			sum = (filter_srcbuf[index] >> 16); 
			audio_detect_L += (sum > 0 ? sum: -sum);/*
			
			//HPF(3,sum);
			L_IIR(1, sum, 14);
			L_IIR(2, sum, 14);
			L_IIR(3, sum, 14);
			L_IIR(4, sum, 14);
			L_IIR(5, sum, 13);*/
			filter_destbuf[index] = (sum << 16);
			
			
			sum = (filter_srcbuf[index+1] >> 16);
			audio_detect_R += (sum > 0 ? sum: -sum);/*
		
			//HPF(2,sum);
			R_IIR(1, sum, 14);
			R_IIR(3, sum, 14);
			R_IIR(2, sum, 14);
			R_IIR(4, sum, 14);
			R_IIR(5, sum, 13);*/
			filter_destbuf[index+1] = (sum << 16);
			
			
			i++;
			
		}
		

		
		t_e = SysTick->VAL;
		t_t = t_s - t_e;
		y1_L1 = y2_L1 = y3_L1= y4_L1= y5_L1= y6_L1 = y7_L1 = y8_L1= y9_L1 = sum;
		y1_R1 = y2_R1 = y3_R1= y4_R1= y5_R1= y6_R1 = y7_R1 = y8_R1= y9_R1 = sum;

	}
	
	if(firsr_trigger){
		PDMA_SET_SRC_ADDR(PDMA2, (u32)filter_destbuf);
		I2S_ENABLE_TXDMA(); 
		I2S_CLR_TXFIFO();
		I2S_ENABLE_TX();
		PDMA_TRG_ENABLE(PDMA2);	
		firsr_trigger = FALSE;
	}
#endif
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
#if 0
	if(i2s_pdma_rxbuf_flag == RXBUF2){
		
		filter_srcbuf = &i2s_rxbuf[AUDIO_DMA_BUF_SIZE];
		filter_destbuf = &i2s_txbuf[AUDIO_DMA_BUF_SIZE];	
	}
	else{
		
		filter_srcbuf = i2s_rxbuf; 
		filter_destbuf = i2s_txbuf;
	}

	filter_trigger = TRUE;

#endif	
}


