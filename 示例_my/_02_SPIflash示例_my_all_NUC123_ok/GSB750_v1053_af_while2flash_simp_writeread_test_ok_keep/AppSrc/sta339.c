/****************************************************************************
 * @file:		sta339.c
 * @version:		
 * @breif:		STA339 AMP source file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/

#include "sysinc.h"
#define battery_14_8		FALSE
#define battery_7_4		TRUE

typedef struct{
	volatile enum I2C_STATE state;
	AMPDATTYPE common;
}AMP_CMDT;

static u32 vlmtbl_index = 0;
u32 	amp_volume_back = 0;
u8	amp_volume_lock = FALSE;
static u8	amp_lock = FALSE;
U32 soft_mute = FALSE;
AMP_CMDT amp_status;
static i2cPoolType amp_i2c_block;
Timer_ID amp_id;
U8 amp_info[0x17];
u32	amp_volume = VOLUME_MUSIC; 
i2cPoolType amp_i2c_block_change_input;
volatile AMP_CMDT amp_volume_change = {I2C_IDLE,{MVOL, 0x70}};
enum AD_STATE change_input_state = VOLUME_UP_STATE;	
extern PT2314_STATU pt2314_status;
extern Timer_ID pt2314_id_CIC;
extern u8 pt2314_switch_remap[];
extern Timer_ID pt2314_id;

u8	amp_master_tbl[MAX_VOLUME] = {	
										0xff,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
										0x00,
						}; 

#define _3COVER 1
u8	amp_treble_tbl[MAX_VOLUME] = {	
										0xFF,
										0XDF,
										0xC0,
										0xC6,
	                  0xC2,
										0xBE,
										0xBA,
										0xB6,	
										0xB2,
	                  0xAE, 
										0xAA,
	                  0xA6,
										0xA2,
										0x9E,
										0x9A,
										0x96,
										0x92,
										0x8E,
										0x8A,
										0x86,
										0x82,
										0x7E,
										0x7A,
										0x76,
										0x72,
										0x6E,
										0x6A,
										0x66,
										0x62,
										0x5E,
										0x5A,
										0x56,		
}; 
						

u8	amp_bass_tbl[MAX_VOLUME] = {	
										0xFF,
										0xCE,
										0xBA,
										0xAE,
										0xAA,
	                  0xA6,
										0xA2,
										0x9E,
										0x9A,
										0x96,
										0x92,
										0x8E,
										0x8A,
										0x86,
										0x82,
										0x7E,
										0x7A,
										0x76,
										0x72,
										0x6E,
										0x6A,
										0x66,
										0x62,
										0x5E,
										0x5A,
										0x56,
										0x52,
										0x4E,
										0x4A,
										0x46,
										0x42,
										0x3E,
}; 

#define amp_bass_tbl_dc  amp_bass_tbl
										


u8 amp_conf_38[AMP_CONF_BYTES_COUNT] = {
			0x00, 0x63, // CONFA:
			0x01, 0x80, // CONFB:
			0x02, 0x9F, // CONFC:
			0x03, 0x58, // CONFD: 
			0x04, 0xC2, // CONFE:
			0x05, 0xDD, // CONFF: - power on bit set when set for 'b1xxxxxx
			0x06, 0x10, //MUTELOC: 
			0x07, 0x00, //MVOL:
			0x08, 0x56, //C1VOL:
			0x09, 0x56, //C2COL:
			0x0a, 0x3E, //C3VOL:
			0x0b, 0x80, //AUTO1:
			0x0c, 0xA0, //AUTO2:
			0x0d,	0x00,	// *** Important not to change
			0x0E, 0x20, //C1CFG:
			0x0F, 0x60, //C2CFG:
			0x10, 0x90, //C3CFG:
			0x11, 0x77, //TONE:
			0x12, 0x0E, //L1AR:
			0x13, 0xB9, //L1ARTR:
			0x14, 0x14, //L2AR:
			0x15, 0x49,//L2ATRT:
			//---------------------------------
			0x27, 0X1A, //MPCC1:
			0x28, 0XC0, //MPCC2:
			0x29, 0XF3, //DDC1:
			0X2A, 0X33, //DDC2: 
			0X2B, 0X00, //FDRC1: recovery time form fault
			0X2C, 0X0C, //FDRC2:
			0X2D, 0X7F, //STATUS
			0X31, 0X00, //EQCFG:
			0X32, 0XBC, //EATH1:
			0X33, 0X98, //ERTH1:
			0X34, 0XA4, //EATH2:
			0X35, 0X98, //ERTH2:
			0X36, 0X07, //CONFX:
			0X37, 0X00, //SVCA:
			0X38, 0X00, //SVCB:
			0X39, 0X01, //RMS0A:
			0X3A, 0XEE, //RMS0B:
			0X3B, 0XFF, //RMS0C:
			0X3C, 0X7E, //RMS1A:
			0X3D, 0XC0, //RMS1B:
			0X3E, 0X26, //RMS1C: 
			0X3F, 0X00
};                  
                    


static u32 amp_48eq_tbl_38[64] = {
			0x8046BB, // Coef[000h] - C1H10 (b1/2)        
			0x7F7B3F, // Coef[001h] - C1H11 (b2)          
			0x7FB934, // Coef[002h] - C1H12 (a1/2)        
			0x808C52, // Coef[003h] - C1H13 (a2)          
			0x3FFC27, // Coef[004h] - C1H14 (b0/2) 	
			0x8023CB, // Coef[005h] - C1H20 (b1/2)        
			0x7FDC35, // Coef[006h] - C1H21 (b2)          
			0x7FDC1E, // Coef[007h] - C1H22 (a1/2)        
			0x80476A, // Coef[008h] - C1H23 (a2)          
			0x3FEE1A, // Coef[009h] - C1H24 (b0/2)  	
			0x8068E2, // Coef[00Ah] - C1H30 (b1/2)        
			0x7F971E, // Coef[00Bh] - C1H31 (b2)          
			0x7F9707, // Coef[00Ch] - C1H32 (a1/2)        
			0x80D197, // Coef[00Dh] - C1H33 (a2)          
			0x3FCB8F, // Coef[00Eh] - C1H34 (b0/2) 	
			0x8E5BAA, // Coef[00Fh] - C1H40 (b1/2)        
			0x6C3F94, // Coef[010h] - C1H41 (b2)          
			0x71A456, // Coef[011h] - C1H42 (a1/2)        
			0x9803F5, // Coef[012h] - C1H43 (a2)          
			0x3DDE3B, // Coef[013h] - C1H44 (b0/2) 			
			0x800A6A, // Coef[014h] - C2H10 (b1/2)        
			0x7FE39A, // Coef[015h] - C2H11 (b2)          
			0x7FF596, // Coef[016h] - C2H12 (a1/2)        
			0x80145F, // Coef[017h] - C2H13 (a2)          
			0x400403, // Coef[018h] - C2H14 (b0/2) 			
			0x809F7A, // Coef[019h] - C2H20 (b1/2)        
			0x7E9B42, // Coef[01Ah] - C2H21 (b2)          
			0x7F6086, // Coef[01Bh] - C2H22 (a1/2)        
			0x81335E, // Coef[01Ch] - C2H23 (a2)          
			0x4018B0, // Coef[01Dh] - C2H24 (b0/2)			
			0x9AA6B1, // Coef[01Eh] - C2H30 (b1/2)        
			0x6405D6, // Coef[01Fh] - C2H31 (b2)          
			0x65594F, // Coef[020h] - C2H32 (a1/2)        
			0xA04A76, // Coef[021h] - C2H33 (a2)          
			0x3DD7D9, // Coef[022h] - C2H34 (b0/2)			
			0x000000, // Coef[023h] - C2H40 (b1/2)        
			0x000000, // Coef[024h] - C2H41 (b2)          
			0x000000, // Coef[025h] - C2H42 (a1/2)        
			0x000000, // Coef[026h] - C2H43 (a2)          
			0x400000, // Coef[027h] - C2H44 (b0/2) 			
			0x000000, // Coef[028h] - C12H0 (b1/2)        
			0x000000, // Coef[029h] - C12H1 (b2)          
			0x000000, // Coef[02Ah] - C12H2 (a1/2)        
			0x000000, // Coef[02Bh] - C12H3 (a2)          
			0x400000, // Coef[02Ch] - C12H4 (b0/2) 			
			0x000000, // Coef[02Dh] - C3H0  (b1/2)        
			0x000000, // Coef[02Eh] - C3H1  (b2)          
			0x000000, // Coef[02Fh] - C3H2  (a1/2)        
			0x000000, // Coef[030h] - C3H3  (a2)          
			0x400000, // Coef[031h] - C3H4  (b0/2)        
			0x7FFFFF, // Coef[032h] - Channel 1 Pre-scale 
			0x7FFFFF, // Coef[033h] - Channel 2 Pre-scale 
			0x7FFFFF, // Coef[034h] - Channel 1 Post-scale
			0x7FFFFF, // Coef[035h] - Channel 2 Post-scale
			0x7FFFFF, // Coef[036h] - Channel 3 Post-scale
			0x5A9DF7, // Coef[037h] - Twarn/Oc Limit      
			0x7FFFFF, // Coef[038h] - Channel 1 Mix 1     
			0x000000, // Coef[039h] - Channel 1 Mix 2     
			0x000000, // Coef[03Ah] - Channel 2 Mix 1     
			0x7FFFFF, // Coef[03Bh] - Channel 2 Mix 2     
			0x400000, // Coef[03Ch] - Channel 3 Mix 1     
			0x400000, // Coef[03Dh] - Channel 3 Mix 2     
			0x000000, // Coef[03Eh]Reserved                         
			0x000000  // Coef[03Fh]Reserved               
};

  
static u32 amp_44eq_tbl_38[64] = {
			0x8046BB, // Coef[000h] - C1H10 (b1/2)        
			0x7F7B3F, // Coef[001h] - C1H11 (b2)          
			0x7FB934, // Coef[002h] - C1H12 (a1/2)        
			0x808C52, // Coef[003h] - C1H13 (a2)          
			0x3FFC27, // Coef[004h] - C1H14 (b0/2) 	
			0x806BAC, // Coef[005h] - C1H20 (b1/2)        
			0x7F9454, // Coef[006h] - C1H21 (b2)          
			0x7F938B, // Coef[007h] - C1H22 (a1/2)        
			0x80D5C4, // Coef[008h] - C1H23 (a2)          
			0x3FCA2A, // Coef[009h] - C1H24 (b0/2)  	
			0x81392A, // Coef[00Ah] - C1H30 (b1/2)        
			0x7EC6D6, // Coef[00Bh] - C1H31 (b2)          
			0x7EC60E, // Coef[00Ch] - C1H32 (a1/2)        
			0x8270C3, // Coef[00Dh] - C1H33 (a2)          
			0x3F636B, // Coef[00Eh] - C1H34 (b0/2) 	
			0x8E5BAA, // Coef[00Fh] - C1H40 (b1/2)        
			0x6C3F94, // Coef[010h] - C1H41 (b2)          
			0x71A456, // Coef[011h] - C1H42 (a1/2)        
			0x9803F5, // Coef[012h] - C1H43 (a2)          
			0x3DDE3B, // Coef[013h] - C1H44 (b0/2) 			
			0x800A6A, // Coef[014h] - C2H10 (b1/2)        
			0x7FE39A, // Coef[015h] - C2H11 (b2)          
			0x7FF596, // Coef[016h] - C2H12 (a1/2)        
			0x80145F, // Coef[017h] - C2H13 (a2)          
			0x400403, // Coef[018h] - C2H14 (b0/2) 			
			0x809F7A, // Coef[019h] - C2H20 (b1/2)        
			0x7E9B42, // Coef[01Ah] - C2H21 (b2)          
			0x7F6086, // Coef[01Bh] - C2H22 (a1/2)        
			0x81335E, // Coef[01Ch] - C2H23 (a2)          
			0x4018B0, // Coef[01Dh] - C2H24 (b0/2)			
			0x9AA6B1, // Coef[01Eh] - C2H30 (b1/2)        
			0x6405D6, // Coef[01Fh] - C2H31 (b2)          
			0x65594F, // Coef[020h] - C2H32 (a1/2)        
			0xA04A76, // Coef[021h] - C2H33 (a2)          
			0x3DD7D9, // Coef[022h] - C2H34 (b0/2)			
			0x000000, // Coef[023h] - C2H40 (b1/2)        
			0x000000, // Coef[024h] - C2H41 (b2)          
			0x000000, // Coef[025h] - C2H42 (a1/2)        
			0x000000, // Coef[026h] - C2H43 (a2)          
			0x400000, // Coef[027h] - C2H44 (b0/2) 			
			0x000000, // Coef[028h] - C12H0 (b1/2)        
			0x000000, // Coef[029h] - C12H1 (b2)          
			0x000000, // Coef[02Ah] - C12H2 (a1/2)        
			0x000000, // Coef[02Bh] - C12H3 (a2)          
			0x400000, // Coef[02Ch] - C12H4 (b0/2) 			
			0x000000, // Coef[02Dh] - C3H0  (b1/2)        
			0x000000, // Coef[02Eh] - C3H1  (b2)          
			0x000000, // Coef[02Fh] - C3H2  (a1/2)        
			0x000000, // Coef[030h] - C3H3  (a2)          
			0x400000, // Coef[031h] - C3H4  (b0/2)        
			0x7FFFFF, // Coef[032h] - Channel 1 Pre-scale 
			0x7FFFFF, // Coef[033h] - Channel 2 Pre-scale 
			0x7FFFFF, // Coef[034h] - Channel 1 Post-scale
			0x7FFFFF, // Coef[035h] - Channel 2 Post-scale
			0x7FFFFF, // Coef[036h] - Channel 3 Post-scale
			0x5A9DF7, // Coef[037h] - Twarn/Oc Limit      
			0x7FFFFF, // Coef[038h] - Channel 1 Mix 1     
			0x000000, // Coef[039h] - Channel 1 Mix 2     
			0x000000, // Coef[03Ah] - Channel 2 Mix 1     
			0x7FFFFF, // Coef[03Bh] - Channel 2 Mix 2     
			0x400000, // Coef[03Ch] - Channel 3 Mix 1     
			0x400000, // Coef[03Dh] - Channel 3 Mix 2     
			0x000000, // Coef[03Eh]Reserved                         
			0x000000  // Coef[03Fh]Reserved        
};

// static u32 eq_1st[5] = {
// 			0x804492, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F7F55, // Coef[029h] - C12H1 (b2)            
// 			0x7FBB5D, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x808800, // Coef[02Bh] - C12H3 (a2)            
// 			0x3FFC45, // Coef[02Ch] - C12H4 (b0/2)     
// };

// static u32 eq_2nd[5] = {
// 			0x804336, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F7E69, // Coef[029h] - C12H1 (b2)            
// 			0x7FBCC1, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x808549, // Coef[02Bh] - C12H3 (a2)            
// 			0x3FFE1E, // Coef[02Ch] - C12H4 (b0/2) 
// };

// static u32 eq_3rd[5] = {
// 			0x804182, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F7E1F, // Coef[029h] - C12H1 (b2)            
// 			0x7FBE7E, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x8081E1, // Coef[02Bh] - C12H3 (a2)            
// 			0x400000, // Coef[02Ch] - C12H4 (b0/2) 
// };

// static u32 eq_4th[5] = {
// 			0x803F7E, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F7E76, // Coef[029h] - C12H1 (b2)            
// 			0x7FC08B, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x807DD8, // Coef[02Bh] - C12H3 (a2)            
// 			0x4001E1, // Coef[02Ch] - C12H4 (b0/2) 
// };

// static u32 eq_5th[5] = {
// 			0x803D31, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F7F6F, // Coef[029h] - C12H1 (b2)            
// 			0x7FC2E0, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x80793D, // Coef[02Bh] - C12H3 (a2)            
// 			0x4003BB, // Coef[02Ch] - C12H4 (b0/2) 
// };

/*-------------------------------------------------------------------------
 * @fucname:	 ampSendCmd_38
 * @brief: 
 * @param: 
 * @return: 
 * @details:  should make sure the amp is power on and in operation
 *-----------------------------------------------------------------------*/
void ampSendCmd_38(AMP_REG reg, u8 data)
{
	initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR38, 
					I2C_WR, 
					2,
					1,
					(u8*)&amp_status
				);
	
	amp_status.common.cmd = reg;
	amp_status.common.data = data;
	amp_status.state = I2C_BUSY;
	addI2cChain(&amp_i2c_block, 0);
	while(amp_status.state == I2C_BUSY)
			;
	if(amp_status.state == I2C_ERROR)
		logEvent(amp_i2c_werror);
		
}

u8 ampGetCmd_38(AMP_REG reg)
{
	initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR38, 
					I2C_RD, 
					2,
					1,
					(u8*)&amp_status
				);
	
	amp_status.common.cmd = reg;
	amp_status.common.data = 0;
	amp_status.state = I2C_BUSY;
	addI2cChain(&amp_i2c_block, 0);
	while(amp_status.state == I2C_BUSY)
			;
	if(amp_status.state == I2C_ERROR)
		logEvent(amp_i2c_rerror);
	
	return amp_status.common.data;
}

/*-------------------------------------------------------------------------
 * @fucname:	 ampSendAllConf
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampInitAllConf(void)
{
	u32 i = 0;
// 	u32 j = 0;
	logEvent(amp_init_start);

	while(1){
		
		if( i >= (AMP_CONF_BYTES_COUNT))
			break;
	
		ampSendCmd_38((AMP_REG)amp_conf_38[i],amp_conf_38[i+1]);
		i += 2; 
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	ampSendCmd_38((AMP_REG)0x05,0xdd);
	if(amp_status.state == I2C_IDLE)
		logEvent(amp_init_ok);
}


void ampInit()
{
	amp_id = getATask();
}

/*-------------------------------------------------------------------------
 * @fucname:	 ampGetStatus
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u8 amp_reg_mute = 0;
u8 ampGetStatus(void)
{
	u32 i = 0;
	u8 amp_reg_error = 0;
	for(i = 0; i<0x16; i++){
		amp_status.common.cmd = (AMP_REG)i;
		
		amp_status.common.data = 0X0;
		amp_status.state = I2C_BUSY;
		initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR38, 
					I2C_RD, 
					2,
					1,
					(u8*)&amp_status
				);
		addI2cChain(&amp_i2c_block, 0);
		while(amp_status.state == I2C_BUSY)
			;
		amp_info[i] = amp_status.common.data;
		if(i == 6)
		{
			if(amp_reg_mute)
			{
				if(amp_info[i] != 0x1e)
					amp_reg_error = 1;
			}
			else
			{
				if(amp_info[i] != 0x10)
					amp_reg_error = 1;
			}
			
		}
		else if(i == 8||i == 9||i == 10)
		{
			if(amp_info[i] < amp_conf_38[i*2 + 1])
			{
				amp_reg_error = 1;
			}
			else if(amp_info[i] > 0xDF && amp_volume != 0)
			{
				amp_reg_error = 1;
			}
		}
		else if(amp_info[i] != amp_conf_38[i*2 + 1])
		{
			amp_reg_error = 1;
		}
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	
	amp_status.common.cmd = STATUS;
	amp_status.common.data = 0X0;
	amp_status.state = I2C_BUSY;
	initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR38, 
					I2C_RD, 
					2,
					1,
					(u8*)&amp_status
				);
	addI2cChain(&amp_i2c_block, 0);
	while(amp_status.state == I2C_BUSY)
			;
	amp_info[0x16] = amp_status.common.data;
	if(amp_info[0x16] != 0x7f)
		amp_reg_error = 1;
	return amp_reg_error;
}


/*-------------------------------------------------------------------------
 * @fucname:	 ampSendEqs
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// static u32 cfg_address_tbl[5] = { 0x15, 0x18, 0x1b, 0x1e, 0x21 };
static U32_BYTE cfg_data;
void ampSetEqDefault(U32 start_addr, U32 end_addr, u32* tbl);
void ampSendEqs_38(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;


	while(i < 50){
		ampSetEqDefault(i,i+4,&amp_48eq_tbl_38[i]);
		i += 5;
		sysDelayMs(2);
	}
	
	sysDelayMs(20);
	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	i = 50;
	while(i <= end_addr){
		ampSendCmd_38(CFADDR, i); // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_48eq_tbl_38[i];
		ampSendCmd_38(B1CF1, cfg_data.byte2);
		ampSendCmd_38(B1CF2, cfg_data.byte1);
		ampSendCmd_38(B1CF3, cfg_data.byte0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD
		i++;
	}

}
void ampSendCallEqs_38(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;


	while(i < 50){
		ampSetEqDefault(i,i+4,&amp_44eq_tbl_38[i]);
		i += 5;
		sysDelayMs(2);
	}
	
	sysDelayMs(20);
	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	i = 50;
	while(i <= end_addr){
		ampSendCmd_38(CFADDR, i); // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_44eq_tbl_38[i];
		ampSendCmd_38(B1CF1, cfg_data.byte2);
		ampSendCmd_38(B1CF2, cfg_data.byte1);
		ampSendCmd_38(B1CF3, cfg_data.byte0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD
		i++;
	}

}
void ampSetL()
{
// 	u32 i = 0x38;
	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	
		ampSendCmd_38(CFADDR, 0x38); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x39); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD


		ampSendCmd_38(CFADDR, 0x3a); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x3b); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		amp_status.common.data = amp_treble_tbl[0];
		ampSendCmd_38((AMP_REG)0x01,0x00);
		
}


void ampSetR()
{
// 	u32 i = 0x38;
	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	
		ampSendCmd_38(CFADDR, 0x39); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x38); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD


		ampSendCmd_38(CFADDR, 0x3b); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x3a); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38((AMP_REG)0x01,0xc0);
		amp_status.common.data = amp_treble_tbl[0];
		
}


void ampSetS()
{
// 	u32 i = 0x38;
	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	
		ampSendCmd_38(CFADDR, 0x38); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x39); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD


		ampSendCmd_38(CFADDR, 0x3b); // Write ram addr
		ampSendCmd_38(B1CF1, 0x7f);
		ampSendCmd_38(B1CF2, 0xff);
		ampSendCmd_38(B1CF3, 0xff);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38(CFADDR, 0x3a); // Write ram addr
		ampSendCmd_38(B1CF1, 0);
		ampSendCmd_38(B1CF2, 0);
		ampSendCmd_38(B1CF3, 0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD

		ampSendCmd_38((AMP_REG)0x01,0x80);
		amp_status.common.data = amp_treble_tbl[0];
		
}


/*-------------------------------------------------------------------------
 * @fucname:	 ampSetEqDefault
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampSetEqDefault(U32 start_addr, U32 end_addr, u32* tbl)
{
	u32 i;
	
	i = 0;

	ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
	ampSendCmd_38(CFADDR,start_addr); // Write ram addr
	while(i < 5){
		
		cfg_data = *(U32_BYTE*)&tbl[i];
		ampSendCmd_38((AMP_REG)(B1CF1 + 3*i), cfg_data.byte2);
		ampSendCmd_38((AMP_REG)(B1CF2 + 3*i), cfg_data.byte1);
		ampSendCmd_38((AMP_REG)(B1CF3 + 3*i), cfg_data.byte0);
		
		i++;
	}
	ampSendCmd_38(CFUD, 2); // Write 1 to w1 in CFUD
}




/*-------------------------------------------------------------------------
 * @fucname:	 ampMuteSwitch
 * @brief: 	 mute or open the se_w58_ii
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampMuteSwitch()
{
	if(amp_volume == 0)
		soft_mute = FALSE;
	if(amp_lock == FALSE)
		if(soft_mute == FALSE)
			changeInputVolumeDown(MUTE_STATE);
		else
			changeInputVolumeDown(UN_MUTE_STATE);

}


/*-------------------------------------------------------------------------
 * @fucname:	 ampFlasfLed
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void stopAmpDD()
{
	stopATask(amp_id);
}

void ampFlasfDD()
{
	playMusic(DD_SOUND);
	startATask(amp_id, 300, 0, stopAmpDD);
}


/*-------------------------------------------------------------------------
 * @fucname:	 ampDecVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern u8 dwhp83_type;
extern u32 Dwhp83_flag;
extern void DWP83CuCV(void);
void ampDecVolume(void)
{
// 	u32 b;
	
	if(amp_volume_lock || amp_lock)
		return;
	
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampDecVolume);
		return;
	}

	
	
	if(amp_volume > 0){
// 		led_flash = 2;
		amp_volume--;
		stopATask(amp_id);
		ampSetVolume(amp_volume);
		DWP83CuCV();
// 		b = breath_flag;
// 		breath_flag = 0xff;
// 		ledColorSet(state_color);
// 		sysDelayMs(100);
		ledColorSet(DARK_COLOR);
		sysDelayMs(100);
		ledColorSet(state_color);
// 		breath_flag = b;
	}else{
	//	if(mytask[amp_id].task == NULL)
	//	 ampFlasfDD();
	}

	if(dev_input == WIRELESS && dwhp83_type != VOLUME_STREO )
			Dwhp83_flag |= 0x2;

}

/*-------------------------------------------------------------------------
 * @fucname:	 ampAddVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampAddVolume(void)
{ 
// 	u32 b;
	
	if(amp_volume_lock || amp_lock)
		return;
	
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampDecVolume);
		return;
	}

	
	if(amp_volume < MAX_VOLUME-1){
// 		led_flash = 2;
		amp_volume++;
		stopATask(amp_id);
		ampSetVolume(amp_volume);
		DWP83CuCV();
// 		b = breath_flag;
// 		breath_flag = 0xff;
// 		ledColorSet(state_color);
// 		sysDelayMs(100);
		ledColorSet(DARK_COLOR);
		sysDelayMs(100);
		ledColorSet(state_color);
// 		breath_flag = b;
		
	}else{
	//	if(mytask[amp_id].task == NULL)
	//		 ampFlasfDD();
	}

	if(dev_input == WIRELESS && dwhp83_type != VOLUME_STREO )
			Dwhp83_flag |= 0x2;
	
}

void ampMute()
{
	amp_reg_mute = 1;
	ampSendCmd_38(MUTELOC, 0x1e);
}

void unMute()
{
	amp_reg_mute = 0;
	ampSendCmd_38(MUTELOC, 0x10);
	
}

void ampPowerDown()
{
	ampSendCmd_38(CONFF, 0x7D);
} 

/*-------------------------------------------------------------------------
 * @fucname:	 ampSetVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampSetVolume(u32 v)
{
	
// 	if(v == 31){
// 		ampSetEqDefault(0x0, 0x4, eq_1st);
// 	}else if(v == 30){
// 		ampSetEqDefault(0x0, 0x4, eq_2nd);
// 	}else if(v == 29){
// 		ampSetEqDefault(0x0, 0x4, eq_3rd);
// 	}else if(v == 28){
// 		ampSetEqDefault(0x0, 0x4, eq_4th);
// 	}else{
// 		ampSetEqDefault(0x0, 0x4, eq_5th);
// 	}
	
	ampSendCmd_38(MVOL,0);
	//if(DECT_DC())
		ampSendCmd_38(C3VOL,amp_bass_tbl[v]);
	//else
	//	ampSendCmd_38(C3VOL,amp_bass_tbl[v]);
	ampSendCmd_38(C2VOL,amp_treble_tbl[v]);
	ampSendCmd_38(C1VOL,amp_treble_tbl[v]);
}

void ampMuteBass()
{ 
	sysDelayMs(5);
	DWHP83Mute();
	sysDelayMs(5);
	ampSendCmd_38(MUTELOC,0x18);
	ampSendCmd_38(AUTO2,0x20);
	amp_status.common.data = amp_treble_tbl[0];
}

void ampUnMuteBass()
{
	sysDelayMs(5);
	DWHP83UnMute();
	sysDelayMs(5);
	ampSendCmd_38(MUTELOC,0x10);
	ampSendCmd_38(AUTO2,0xc0);
	amp_status.common.data = amp_treble_tbl[0];
}

/*-------------------------------------------------------------------------
 * @fucname:	 ampResumeI2c
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampResumeI2c(void)
{
	amp_i2c_block.index = amp_i2c_block.index_bak;
	amp_status.state = I2C_IDLE;
	amp_volume_lock = amp_lock = 0;
}

void ampSetL1ATRT(u8 cfg)
{
	ampSendCmd_38(L1ATRT, cfg);
}

void ampEQbypass() 
{
	u8 data;
	
	data = ampGetCmd_38(C1CFG);
	ampSendCmd_38(C1CFG, data|0x2);
	data = ampGetCmd_38(C2CFG);
	ampSendCmd_38(C2CFG, data|0x2);
	data = ampGetCmd_38(C3CFG);
	ampSendCmd_38(C3CFG, data|0x2);
}

void ampEQon()
{
	u8 data;
	 
	data = ampGetCmd_38(C1CFG);
	ampSendCmd_38(C1CFG, data&~0x2);
	data = ampGetCmd_38(C2CFG);
	ampSendCmd_38(C2CFG, data&~0x2);
	data = ampGetCmd_38(C3CFG);
	ampSendCmd_38(C3CFG, data&~0x2);
}


void ampFlasfLed()
{
	led_flash = 8;
	startATask(amp_id, 1000, 0, ampFlasfLed);
}
void stopFlashLed()
{
	stopATask(amp_id);
}

static void turnDownVolume()
{
	u32 index;
	if(amp_status.state == I2C_IDLE){
		
		for(; vlmtbl_index > 0; vlmtbl_index-- ){

			if(amp_status.common.data <  amp_treble_tbl[vlmtbl_index-1]
				&& amp_status.common.data >= amp_treble_tbl[vlmtbl_index ]
			){
				amp_status.common.data = amp_treble_tbl[vlmtbl_index-1];
				index = vlmtbl_index-1;
				break;
			}		
			if(vlmtbl_index == 1)
				index = vlmtbl_index = MAX_VOLUME;
		}
		
		if(vlmtbl_index <= 1){
			index = vlmtbl_index = MAX_VOLUME-1;
			startATask(pt2314_id_CIC, 3, 0, ChangInputVolumeCheck);
			return;
		}
		
			
		ampSendCmd_38(MVOL,amp_master_tbl[index]);
		if(DECT_DC())
			ampSendCmd_38(C3VOL,amp_bass_tbl_dc[index]);
		else
			ampSendCmd_38(C3VOL,amp_bass_tbl[index]);
		ampSendCmd_38(C2VOL,amp_treble_tbl[index]);
		ampSendCmd_38(C1VOL,amp_treble_tbl[index]);
	}
	
	startATask(pt2314_id_CIC, 3, 0, ChangInputVolumeCheck);
}

static void turnUpVolume()
{
	u32 index = 0;
	
	if(amp_status.state == I2C_IDLE){
		
		if(vlmtbl_index >= MAX_VOLUME-1)
				index = vlmtbl_index = 0;
		
		for(; vlmtbl_index < MAX_VOLUME-1; vlmtbl_index++ ){
			if(amp_status.common.data <= amp_treble_tbl[vlmtbl_index]
				&& amp_status.common.data > amp_treble_tbl[vlmtbl_index +1]){
				index = vlmtbl_index+1;
				break;
			}		

		}
		
		ampSendCmd_38(MVOL,amp_master_tbl[index]);
		if(DECT_DC())
			ampSendCmd_38(C3VOL,amp_bass_tbl_dc[index]);
		else
			ampSendCmd_38(C3VOL,amp_bass_tbl[index]);
		ampSendCmd_38(C2VOL,amp_treble_tbl[index]);
		ampSendCmd_38(C1VOL,amp_treble_tbl[index]);
	}
	
	startATask(pt2314_id_CIC, 3, 0, ChangInputVolumeCheck);
}

/*-------------------------------------------------------------------------
 * @fucname:	 ampChangInputVolumeCheck
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static void ChangInputVolumeCheck()
{
	if(amp_volume_change.state == I2C_IDLE){

		switch(change_input_state){

			case INPUT_CHANGE_STATE:

// 				if(amp_status.common.data == amp_treble_tbl[0] ||
// 					soft_mute == TRUE){
// 					ampSetVolume(0);
				
					amp_status.common.data = amp_treble_tbl[0];
					amp_volume_lock = FALSE;
					changeInputChannel();
	
// 				}
// 				else{
// 					turnDownVolume();
// 				}
				
				break;

			case VOLUME_UP_STATE:
				if(amp_status.common.data > amp_treble_tbl[amp_volume]){
					turnUpVolume();
				}else{
					stopATask(pt2314_id_CIC);
					amp_status.common.data = amp_treble_tbl[0];
					amp_volume_lock = FALSE;
				}
				
				break;
				
			case VOLUME_DOWN_STATE:
				
				 if(amp_status.common.data < amp_treble_tbl[amp_volume]){
					turnDownVolume();
				}else{
					stopATask(pt2314_id_CIC);

					amp_status.common.data = amp_treble_tbl[0];
					amp_volume_lock = FALSE;
				}
				
				break;

			case VOLUME_RECOVER:
				
				if(amp_status.common.data > amp_treble_tbl[amp_volume_back]){
					amp_volume =amp_volume_back;
					change_input_state = VOLUME_UP_STATE;
					turnUpVolume();
				}else{
					amp_volume =amp_volume_back;
					change_input_state = VOLUME_DOWN_STATE;
					turnDownVolume();
				}
				
				break;
				
			case POWER_OFF_STATE:
			case VOLUME_DOWN_UP:
				if(amp_status.common.data == amp_treble_tbl[0]){
				
					if(change_input_state == POWER_OFF_STATE){
						ampMute();
						powerOff();
					}else{
						if(flash_mode == FLASH_PLAY_MUSIC){
							flash_mode = FLASH_CONFIG;
							SPI_CLR_SS0(SPI2);  
							filter_bypass = FALSE;							
						}
						
						change_input_state = VOLUME_UP_STATE;
						turnUpVolume();
					}
					amp_volume_lock = FALSE;
					//change_input_state = AD_IDLE_STATE;
					
				}else {
					turnDownVolume();
				}
				
				break;
			
				
			case MUTE_STATE:

				if(amp_status.common.data == amp_treble_tbl[0]){
					
					amp_lock = FALSE;
					ampMute();
					amp_volume_lock = FALSE;
					startATask(amp_id, 2, 0, ampFlasfLed);
					soft_mute = TRUE;
					change_input_state = AD_IDLE_STATE;
				}
				else{
					amp_lock = TRUE;
					turnDownVolume();
				}

				break;
				
			case AUTO_MUTE:
				
				if(amp_status.common.data == amp_treble_tbl[0]){
					soft_mute = FALSE;
					amp_lock = FALSE;
					ampMute();
					ampSetVolume(amp_volume);
					
					amp_volume_lock = FALSE;
					change_input_state = AD_IDLE_STATE;
				}
				else{
					amp_lock = TRUE;
					soft_mute = TRUE;
					turnDownVolume();
				}

				break;
				
			case UN_MUTE_STATE:

				if(amp_status.common.data == amp_treble_tbl[0]){
					amp_lock = FALSE;
					unMute();
					amp_lock = TRUE; // to avoid quick switch between MUTE_STATE and UN_MUTE_STATE
					sysDelayMs(2);	// wait for mute cmd sent 
					turnUpVolume();
				}else if(amp_status.common.data > amp_treble_tbl[amp_volume]){
					turnUpVolume();
				}else{
					stopFlashLed();
					amp_lock = FALSE;
					amp_volume_change.common.data = amp_treble_tbl[0];
					amp_volume_lock = FALSE;
					soft_mute = FALSE;
					change_input_state = AD_IDLE_STATE;
				}
				
				break;
				
			case AD_SWITCH_STATE:
				
			if(amp_status.common.data == amp_treble_tbl[0] ||
				soft_mute == TRUE){
				amp_lock = FALSE;
				amp_status.common.data = amp_treble_tbl[0];

				unMute();
				amp_volume_lock = FALSE;
				change_input_state = VOLUME_UP_STATE;
				startATask(pt2314_id_CIC, 3, 0, ChangInputVolumeCheck);
			}else{
				amp_lock = TRUE;
				turnDownVolume();
			}
				break;
			default:
				break;
				
		}
	}
	else if(amp_volume_change.state == I2C_BUSY)
		startATask(pt2314_id_CIC, 1, 0, ChangInputVolumeCheck);
	else{
		amp_volume_change.state = I2C_IDLE;
		amp_volume_lock = FALSE;
		amp_lock = FALSE;
		change_input_state = AD_IDLE_STATE;  
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 getChangeVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void getChangeVolume()
{	
	switch(change_input_state){
		case INPUT_CHANGE_STATE: 
		case POWER_OFF_STATE:
		case AD_SWITCH_STATE:
		case MUTE_STATE:
		case VOLUME_DOWN_UP:
		case AUTO_MUTE:
			amp_status.common.data = amp_treble_tbl[amp_volume];
			amp_volume_lock = TRUE;
			break;
		default:
			break;
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 changeInputVolumeDown
 * @brief: 	 dec the volume to lowesr before change input 
 * @param: 	 u32 state
 * @return: 	 none 
 * @details: 	 dec the volume to lowesr before change input 
 *-----------------------------------------------------------------------*/
void changeInputVolumeDown(enum AD_STATE state)
{
	if(amp_volume_lock || amp_lock)
		return;
	
	change_input_state = state;

	getChangeVolume();
	
	startATask(pt2314_id_CIC, 2, 0, ChangInputVolumeCheck);
}


/*-------------------------------------------------------------------------
 * @fucname:	 changeInputVolumeUp
 * @brief: 	 add the volume to lowesr after change input 
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 add the volume to lowesr after change input 
 *-----------------------------------------------------------------------*/
U32 changeInputVolumeUp()
{
	if(amp_volume_lock || amp_lock)
		return FALSE;
	
	change_input_state = VOLUME_UP_STATE;
	amp_status.common.data = amp_treble_tbl[0];
	ChangInputVolumeCheck();

	return TRUE;
}

u8 amp_i2c_status_check()
{
	if(amp_status.state == I2C_ERROR)
		return 1;
	else 
		return 0;
}

