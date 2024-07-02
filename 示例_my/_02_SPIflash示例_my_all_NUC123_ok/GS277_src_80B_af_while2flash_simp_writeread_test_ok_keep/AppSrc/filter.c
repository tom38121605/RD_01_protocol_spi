/****************************************************************************
 * @file:		filter.c
 * @version:	V1.00
 * @breif:		filter source file
 * @date: 	2013.4.12	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "filter.h"
#include "filter_config.h"
#include "limiter.h"

extern s32 i2s_rxbuf[];
extern s32 i2s_txbuf[];
extern s32 i2s_pdma_rxbuf_flag;

U32 firsr_trigger = FALSE;
U32 filter_trigger = FALSE;
u32 filter_rxbuf_flag;
s32 *filter_srcbuf = i2s_rxbuf;
s32 *filter_destbuf = i2s_txbuf;
u32 filter_bypass = FALSE;

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


s32 K1_HP = 0x68;
s32 K2_HP = 0x68;

u32 t_s, t_e, t_t, t_a[8];

/*============== filter bypass configuration ==============*/
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

/*============ these configuration can be changed ===========*/
#define F_L_K1_SHIFT   14 // 24bit filter1
S32 F_L_K11	= 0x3fc371;
S32 F_L_K12	= 0xff80791d;
S32 F_L_K13	= 0x3fc371;
S32 F_L_K14	= 0x7f86a9;
S32 F_L_K15	= 0xffc078e3;

#define  F_L_K2_SHIFT  14; // 24bit filter2
S32 F_L_K21	= 0x83223f13;
S32 F_L_K22	= 0x7cde3ede;  
S32 F_L_K23	= 0x3fcac20f;    
S32 F_L_K24	= 0x3f60811b;    
S32 F_L_K25	= 0xC0d57ee5;    

#define F_L_K3_SHIFT = 14;	 // 16bit filter3 and filter4		 
S32 F_L_K31	= 0x7fff0000;		// k31  k32
S32 F_L_K32	= 0x00000000;  	// k33  k34
S32 F_L_K33	= 0x0000c20f;    	// k35  k41
S32 F_L_K34	= 0x3f60811b;    	// k42  k43
S32 F_L_K35	= 0xC0d57ee5;     	// k44  k45

s32 F_L_K4_SHIFT = 14;			
S32 F_L_K41	= 0x83223f13;		// k31  k32
S32 F_L_K42	= 0x7cde3ede;  	// k33  k34       
S32 F_L_K43	= 0x3fcac20f;    	// k35  k41
S32 F_L_K44	= 0x3f60811b;    	// k42  k43
S32 F_L_K45	= 0xC0d57ee5;     	// k44  k45   

s32 F_L_K5_SHIFT = 15;						   
S32 F_L_K51	= 0x7fff;
S32 F_L_K52	= 0x0;   
S32 F_L_K53	= 0x0;    
S32 F_L_K54	= 0x0;    
S32 F_L_K55	= 0x0;    

static u8 fsamples[960];
static u32 index = 0, fsum = 0;

void filterInit()
{
	u32 i = 0;
	
	for(; i<960; i++){
		fsamples[i] = 0;
	}

	fsum = 0;
}

u8 filter_50(u8 sdin)
{
	u8 sdout;
// 	u32 temp;                               //ma 20150728  del useless  value 

	if(index >= 960)
		index = 0;
	fsum += sdin;
	fsum -= fsamples[index];
	fsamples[index] = sdin;
	index++;

// 	temp = fsum * 17;                     //ma 20150728  del useless  value 
	sdout = fsum>>14;
	
	return sdout;
}

/*-------------------------------------------------------------------------
 * @fucname:	 limiter
 * @brief: 	 change the signal 
 * @param: 	 signal
 * @return: 	 signal
 * @details: 	 change the signal 
 *-----------------------------------------------------------------------*/
s32  limiter( s32 data)
{
	if(data > 0){
		if(data < Limiter_X1)
			return data;
		else if(data < Limiter_X2)
			return 	Limiter_Y1 + 
					(((data - Limiter_X1)*
					    Limiter_K1_numerator) >>
						Limiter_K1_denominator);
		else
			return 	Limiter_Y2 + 
					(((data - Limiter_X2)
						*Limiter_K2_numerator) 
						>> Limiter_K2_denominator);
		
	}
	else{
		if(data > -Limiter_X1)
			return data;
		else if(data > -Limiter_X2)
			return 	(-Limiter_Y1 + 
					(((data + Limiter_X1)*
					    Limiter_K1_numerator) >>
						Limiter_K1_denominator));
		else
			return 	(-Limiter_Y2 + 
					(((data + Limiter_X2)
						*Limiter_K2_numerator) 
						>> Limiter_K2_denominator));
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 I2S_IRQHandler
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern s16 rawData[];
//extern GsData gsdata; 
extern u32 BT_MATCH_FLAG;
S32 sum_H;
void I2S_IRQHandler()
{
	static s32 sum_LL, sum_RL, sum;
// 	static s32 sum_bak;       //ma 20150629    no referenced
	// 	static s32 sum_out=0;       //ma 20150629    no referenced
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
// 	static u32 index = 0;        //ma 20150629   no reference  here 

		
	sum = (I2S_GET_STATUS());
	i2s_status = *(struct I2S_STATS*)&sum;
	sum_LL = I2S_GET_DATA();
	sum_RL = I2S_GET_DATA();	
	
		
	//return; //--zz test
	
	
  if(flash_mode == FLASH_PLAY_MUSIC)
	{	
// 		sum_LL = 0; 
// 		sum_RL = 0;		
		
		#if(SPIFLASH_ENABLE == TRUE)	
			dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
		#endif
	}
	else
	{
		
		if(bt_amp_start_flag)   //ma 20150630
		{
			sum_LL = 0; 
			sum_RL = 0;	
		}
		if(soft_mute_flag)     // ma 20150703   when play music ,before playing  flash music   clear i2s  data 
		{
			sum_LL = 0; 
			sum_RL = 0;	 
		}
		if(bt_change_name_mode)     //ma 20150707
		{
			sum_LL = 0;
			sum_RL = 0;	
		}
		if(sb_change_source_mute_flag)                   //ma 20150819  add soft mute i2s flag when sb change source 
		{
			sum_LL = 0;
			sum_RL = 0;
		}
		
		sum_H = sum_RL + sum_LL;
		
		if((i2s_status.rx_level&0x1) == (i2s_status.tx_level&0x1))
		{
			sum = sum_LL;
			sum_LL = sum_RL;
			sum_RL = sum;
		}
		
		if(dev_input == BLUETOOTH && BT_MATCH_FLAG == 0)
		{
			audio_detect_L = audio_detect_R = 0;
		}
		else
		{
			audio_detect_L += ((sum_LL > 0 ? sum_LL : -sum_LL)>>16); // audio detect left channel
			audio_detect_R += ((sum_RL > 0 ? sum_RL : -sum_RL)>>16); // audio detect right channel
		}
	}
	
// 	#if(SPIFLASH_ENABLE == TRUE)	
// 		dealFlashEvent(&sum_LL, &sum_RL); // recode music to flash or play music from flash
// 	#endif	
	
	if(i2s_status.r_l == 1 && (i2s_status.tx_level&0x1))
	{
		sum = sum_LL;  
		sum_LL = sum_RL;  
		sum_RL = sum; 
	}
	
	I2S_SET_DATA(sum_RL); 
	I2S_SET_DATA(sum_LL); 
	return;
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
// 	u32 index, i = 0;                                            //ma 20150707  del useless function 
// 	volatile s32 sum;  

// 	if(filter_trigger){
// 		filter_trigger = FALSE;

// 		t_s = SysTick->VAL;
// 		while(i < (AUDIO_DMA_BUF_SIZE >> 1)){

// 			index = 2*i;
// 			
// 			sum = (filter_srcbuf[index] >> 16); 
// 			audio_detect_L += (sum > 0 ? sum: -sum);/*
// 			
// 			//HPF(3,sum);
// 			L_IIR(1, sum, 14);
// 			L_IIR(2, sum, 14);
// 			L_IIR(3, sum, 14);
// 			L_IIR(4, sum, 14);
// 			L_IIR(5, sum, 13);*/
// 			filter_destbuf[index] = (sum << 16);
// 			
// 			
// 			sum = (filter_srcbuf[index+1] >> 16);
// 			audio_detect_R += (sum > 0 ? sum: -sum);/*
// 		
// 			//HPF(2,sum);
// 			R_IIR(1, sum, 14);
// 			R_IIR(3, sum, 14);
// 			R_IIR(2, sum, 14);
// 			R_IIR(4, sum, 14);
// 			R_IIR(5, sum, 13);*/
// 			filter_destbuf[index+1] = (sum << 16);
// 			
// 			
// 			i++;
// 			
// 		}
// 		

// 		
// 		t_e = SysTick->VAL;
// 		t_t = t_s - t_e;
// 		y1_L1 = y2_L1 = y3_L1= y4_L1= y5_L1= y6_L1 = y7_L1 = y8_L1= y9_L1 = sum;
// 		y1_R1 = y2_R1 = y3_R1= y4_R1= y5_R1= y6_R1 = y7_R1 = y8_R1= y9_R1 = sum;

// 	}
// 	
// 	if(firsr_trigger){
// 		PDMA_SET_SRC_ADDR(PDMA2, (u32)filter_destbuf);
// 		I2S_ENABLE_TXDMA(); 
// 		I2S_CLR_TXFIFO();
// 		I2S_ENABLE_TX();
// 		PDMA_TRG_ENABLE(PDMA2);	
// 		firsr_trigger = FALSE;
// 	}
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
	if(i2s_pdma_rxbuf_flag == RXBUF2){
		
		filter_srcbuf = &i2s_rxbuf[AUDIO_DMA_BUF_SIZE];
		filter_destbuf = &i2s_txbuf[AUDIO_DMA_BUF_SIZE];	
	}
	else{
		
		filter_srcbuf = i2s_rxbuf; 
		filter_destbuf = i2s_txbuf;
	}

	filter_trigger = TRUE;

	
}


