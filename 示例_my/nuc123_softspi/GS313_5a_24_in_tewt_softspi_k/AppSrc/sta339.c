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
//--extern PT2314_STATU pt2314_status; //--2015.07.22
extern Timer_ID pt2314_id_CIC;
extern u8 pt2314_switch_remap[];
extern Timer_ID pt2314_id;

u8	amp_master_tbl[MAX_VOLUME] = {	
										0xFF,
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

u8	amp_treble_tbl[MAX_VOLUME] = {	
										0xFF,
										0xD8,
										0xC0,
										0xBC,
										0xB8,
										0xB4,
										0xB0,
										0xAC,
										0xA8,
										0xA4,
										0xA0,
										0x9C,
										0x98,
										0x94,
										0x90,
										0x8C,
										0x88,
										0x84,
										0x80,
										0x7C,
										0x78,
										0x74,
										0x70,
										0x6C,
										0x68,
										0x64,
										0x60,
										0x5C,
										0x58,
										0x54,
										0x50,
										0x4c,
						};  

u8	amp_bass_tbl[MAX_VOLUME] = {	
										0xFF,
										0xD8,
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
										0x7B,
										0x77,
										0x74,
										0x70,
										0x6D,
										0x69,
										0x66,
										0x62,
										0x5F,
										0x5B,
										0x58,
										0x54,
										0x50,
										0x4c,
									};

u8 amp_wireless_tbl[32] = {
										0xFF,
										0xA6,
										0xA2,
										0x8E,
										0x92,
										0x8E,
										0x8A,
										0x87,
										0x83,
										0x80,
										0x7C,
										0x78,
										0x75,
										0x72,
										0x6F,
										0x6B,
										0x68,
										0x65,
										0x62,
										0x5F,
										0x5C,
										0x59,
										0x56,
										0x53,
										0x50,
										0x4D,
										0x4A,
										0x47,
										0x44,
										0x40,
										0x3C,
										0x38,
};

u8 amp_conf_38[AMP_CONF_BYTES_COUNT] = {
			0x00, 0x63, // CONFA:
			0x01, 0x80, // CONFB:
			0x02, 0x97, // CONFC:
			0x03, 0x58, // CONFD: 
			0x04, 0xC2, // CONFE:
			0x05, 0xDF, // CONFF:
			0x06, 0x16, //MUTELOC: 
			0x07, 0x00, //MVOL:
			0x08, 0x60, //C1VOL:
			0x09, 0x60, //C2COL:
			0x0a, 0x34, //C3VOL:
			0x0b, 0x80, //AUTO1:
			0x0c, 0x00, //AUTO2:
			0x0D, 0x00, //AUTO3:
			0x0E, 0x00, //C1CFG:
			0x0F, 0x40, //C2CFG:
			0x10, 0x90, //C3CFG:
			0x11, 0x77, //TONE:
			0x12, 0x2C, //L1AR:
			0x13, 0x69, //L1ARTR:
			0x14, 0x6A, //L2AR:
			0x15, 0x69, //L2ATRT:
			//---------------------------------
			0x27, 0X1A, //MPCC1:
			0x28, 0XC0, //MPCC2:
			0x29, 0XF3, //DDC1:
			0X2A, 0X33, //DDC2: 
			0X2B, 0X00, //FDRC1: recovery time form fault
			0X2C, 0X0C, //FDRC2:
			0X2D, 0X7F, //STATUS
			0X31, 0X80, //EQCFG:
			0X32, 0XC0, //EATH1:
			0X33, 0XA8, //ERTH1:
			0X34, 0X30, //EATH2:
			0X35, 0X30, //ERTH2:
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
                    

static u32 amp_eq_tbl_38[64] = {
			0x0000C8, // Coef[000h] - C1H10 (b1/2)          
			0x0000C8, // Coef[001h] - C1H11 (b2)            
			0x7ED740, // Coef[002h] - C1H12 (a1/2)          
			0x824E5D, // Coef[003h] - C1H13 (a2)            
			0x000064, // Coef[004h] - C1H14 (b0/2)          
			0x000165, // Coef[005h] - C1H20 (b1/2)          
			0x000165, // Coef[006h] - C1H21 (b2)            
			0x7F5910, // Coef[007h] - C1H22 (a1/2)           
			0x81484A, // Coef[008h] - C1H23 (a2)            
			0x0000B2, // Coef[009h] - C1H24 (b0/2)          
			0x804F4C, // Coef[00Ah] - C1H30 (b1/2)          
			0x7FB0B4, // Coef[00Bh] - C1H31 (b2)            
			0x7FB0A5, // Coef[00Ch] - C1H32 (a1/2)          
			0x809E7B, // Coef[00Dh] - C1H33 (a2)            
			0x3FD85A, // Coef[00Eh] - C1H34 (b0/2)          
			0x802009, // Coef[00Fh] - C1H40 (b1/2)          
			0x7FDFF7, // Coef[010h] - C1H41 (b2)            
			0x7FDFE8, // Coef[011h] - C1H42 (a1/2)          
			0x803FF6, // Coef[012h] - C1H43 (a2)            
			0x3FEFFB, // Coef[013h] - C1H44 (b0/2)          
			0x806DCA, // Coef[014h] - C2H10 (b1/2)          
			0x7F52F9, // Coef[015h] - C2H11 (b2)            
			0x7F9236, // Coef[016h] - C2H12 (a1/2)          
			0x80D8DF, // Coef[017h] - C2H13 (a2)            
			0x3FEA13, // Coef[018h] - C2H14 (b0/2)          
			0x000000, // Coef[019h] - C2H20 (b1/2)          
			0x000000, // Coef[01Ah] - C2H21 (b2)            
			0x000000, // Coef[01Bh] - C2H22 (a1/2)          
			0x000000, // Coef[01Ch] - C2H23 (a2)            
			0x400000, // Coef[01Dh] - C2H24 (b0/2)          
			0x000000, // Coef[01Eh] - C2H30 (b1/2)          
			0x000000, // Coef[01Fh] - C2H31 (b2)            
			0x000000, // Coef[020h] - C2H32 (a1/2)          
			0x000000, // Coef[021h] - C2H33 (a2)            
			0x400000, // Coef[022h] - C2H34 (b0/2)          
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
// 			0x809335, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F6CCB, // Coef[029h] - C12H1 (b2)            
// 			0x7F6C5E, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x812592, // Coef[02Bh] - C12H3 (a2)            
// 			0x3FB665, // Coef[02Ch] - C12H4 (b0/2)     
// };

// static u32 eq_2nd[5] = {
// 			0x80787B, // Coef[028h] - C12H0 (b1/2)          
// 			0x7F8785, // Coef[029h] - C12H1 (b2)            
// 			0x7F873D, // Coef[02Ah] - C12H2 (a1/2)          
// 			0x80F064, // Coef[02Bh] - C12H3 (a2)            
// 			0x3FC3C2, // Coef[02Ch] - C12H4 (b0/2) 
// };

// static u32 eq_3rd[5] = {
// 			0x805DBC, // Coef[000h] - C1H10 (b1/2)          
// 			0x7FA244, // Coef[001h] - C1H11 (b2)            
// 			0x7FA218, // Coef[002h] - C1H12 (a1/2)          
// 			0x80BB20, // Coef[003h] - C1H13 (a2)            
// 			0x3FD122, // Coef[004h] - C1H14 (b0/2)      
// };

/*--static u32 eq_4th[5] = {
			0xC02723, // Coef[028h] - C12H0 (b1/2)          
			0x5BE116, // Coef[029h] - C12H1 (b2)            
			0x3FD8DD, // Coef[02Ah] - C12H2 (a1/2)          
			0xAE3D41, // Coef[02Bh] - C12H3 (a2)            
			0x3AF0D4, // Coef[02Ch] - C12H4 (b0/2) 
};

static u32 eq_5th[5] = {
			0xC02723, // Coef[028h] - C12H0 (b1/2)          
			0x5BE116, // Coef[029h] - C12H1 (b2)            
			0x3FD8DD, // Coef[02Ah] - C12H2 (a1/2)          
			0xAE3D41, // Coef[02Bh] - C12H3 (a2)            
			0x3AF0D4, // Coef[02Ch] - C12H4 (b0/2) 
};
*/

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
		;//logEvent(amp_i2c_werror);  //--205.07.22
		
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
		;//logEvent(amp_i2c_rerror); //--205.07.22
	
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

	//--logEvent(amp_init_start);  //--205.07.22
	while(1){
		
		if( i >= (AMP_CONF_BYTES_COUNT))
			break;
		
		ampSendCmd_38((enum AMP_REG)amp_conf_38[i],amp_conf_38[i+1]);
		
		i += 2; 
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	ampSendCmd_38((enum AMP_REG)0x05,0xDF);
		
	if(amp_status.state == I2C_IDLE)
		;//--logEvent(amp_init_ok);   //--205.07.22
}


void ampInit(void)
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
void ampGetStatus(void)
{
	u32 i = 0;

	for(i = 0; i<0x16; i++){
		amp_status.common.cmd = (enum AMP_REG)i;
		
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
		
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	
	amp_status.common.cmd = STATUS;
	amp_status.common.data = 0X0;
	amp_status.state = I2C_BUSY;
	amp_i2c_block.addr = STA339_I2C_ADDR38;
	addI2cChain(&amp_i2c_block, 0);
	
	while(amp_status.state == I2C_BUSY)
			;
	amp_info[0x16] = amp_status.common.data;
}


/*-------------------------------------------------------------------------
 * @fucname:	 ampSendEqs
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
//--static u32 cfg_address_tbl[5] = { 0x15, 0x18, 0x1b, 0x1e, 0x21 };
static U32_BYTE cfg_data;
void ampSendEqs_38(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;
	
	while(i <= end_addr)
	{
		ampSendCmd_38(EQCFG, 0);   // select ram bank0 for 48khz signal
		ampSendCmd_38(CFADDR, i);   // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_eq_tbl_38[i];
		ampSendCmd_38(B1CF1, cfg_data.byte2);
		ampSendCmd_38(B1CF2, cfg_data.byte1);
		ampSendCmd_38(B1CF3, cfg_data.byte0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD
		i++;
	}
	
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
		ampSendCmd_38((enum AMP_REG)(B1CF1 + 3*i), cfg_data.byte2);
		ampSendCmd_38((enum AMP_REG)(B1CF2 + 3*i), cfg_data.byte1);
		ampSendCmd_38((enum AMP_REG)(B1CF3 + 3*i), cfg_data.byte0);
		
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


/*-------------------------------------------------------------------------
 * @fucname:	 ampDecVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampDecVolume(void)
{
	if(amp_volume_lock || amp_lock)
		return;
	
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampDecVolume);
		return;
	}

	if(amp_volume > 0)
	{
		//led_flash = 1;
		amp_volume--;
		stopATask(amp_id);
		
//iputs("ampSetVol--A01\r\n");    //--zz debug
		
		ampSetVolume(amp_volume);
	}
	else
	{
	//	if(mytask[amp_id].task == NULL)
	//	 ;
	}
	//btSetVolume(amp_volume);
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
	if(amp_volume_lock || amp_lock)
		return;
	
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampAddVolume);
		return;
	}

	if(amp_volume < MAX_VOLUME-1)
	{
		//led_flash = 1;
		amp_volume++;
		stopATask(amp_id);
		
//iputs("ampSetVol--A02\r\n");    //--zz debug
		
		ampSetVolume(amp_volume);
	}
	else
	{
	//	if(mytask[amp_id].task == NULL)
	//		 ;
	}
	//btSetVolume(amp_volume);
}

void ampMute(void)
{
	ampSendCmd_38(MUTELOC, 0x1e);
}

void unMute(void)
{
	ampSendCmd_38(MUTELOC, 0x10);
	
}

void ampPowerDown(void)
{
	ampSendCmd_38(CONFF, 0x5c);
}

void ampPowerUp(void)
{
	ampSendCmd_38(CONFF, 0xdc);
}


// void ampSetVolume(u32 v)
// {
//   //uartdecode2(tws_out,"  tws_out=: ");	 //--zz debug
//   uartdecode(v,"  Volumesiize: ");	 //--zz debug
// 	
// 	if(v == 31)
// 	{
// 		ampSetEqDefault(0x0, 0x4, eq_1st);
// 	}else if(v == 30)
// 	{
// 		ampSetEqDefault(0x0, 0x4, eq_2nd);
// 	}else if(v == 29)
// 	{
// 		ampSetEqDefault(0x0, 0x4, eq_3rd);
// 	}
// 	

// 	if(v != 0)
// 	{
// 		ampSendCmd_38(C2VOL,amp_bass_tbl[v]+1);
// 		ampSendCmd_38(C1VOL,amp_treble_tbl[v]+1);
// 	}
// 	else
// 	{
// 		ampSendCmd_38(C2VOL,amp_bass_tbl[v]);
// 		ampSendCmd_38(C1VOL,amp_treble_tbl[v]);
// 	}
// 	
// }

// void ampSetVolume2(u32 v)
// {
//   //uartdecode2(tws_out,"  tws_out=: ");	 //--zz debug
//   uartdecode(v,"  Volumesiize: ");	 //--zz debug
// 	
// 	if(v == 31)
// 	{
// 		ampSetEqDefault(0x0, 0x4, eq_1st);
// 	}else if(v == 30){
// 		ampSetEqDefault(0x0, 0x4, eq_2nd);
// 	}else if(v == 29){
// 		ampSetEqDefault(0x0, 0x4, eq_3rd);
// 	}
// 	
// 	//else if(v == 28){
// 	//	ampSetEqDefault(0x28, 0x2C, eq_2nd);
// 	//}else{
// 	//	ampSetEqDefault(0x28, 0x2C, eq_1st);
// 	//}	
// 	
// 	if(v != 0)
// 	{
// 		ampSendCmd_38(C2VOL,amp_bass_tbl[v]+1);
// 		ampSendCmd_38(C1VOL,amp_treble_tbl[v]+1);
// 	}
// 	else
// 	{
// 		ampSendCmd_38(C2VOL,amp_bass_tbl[v]);
// 		ampSendCmd_38(C1VOL,amp_treble_tbl[v]);
// 	}
// 	
// }

void ampSetVolume(u32 v)
{
  uartdecode(v,"  Volumesiize: ");	 //--zz debug

	if(v == 0 || v == 33)
			ampSendCmd_38(C3VOL, 0xff);
	else
		ampSendCmd_38(C3VOL,amp_wireless_tbl[v] - 4);
}


void ampMuteBass()
{ 
	ampSendCmd_38(MUTELOC,0x18);
	ampSendCmd_38(AUTO2,0x20);
	amp_status.common.data = amp_treble_tbl[0];
}

void ampUnMuteBass(void)
{
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



void stopFlashLed(void)
{
	stopATask(amp_id);
}

static void turnDownVolume()
{
	u32 index=0;
	if(amp_status.state == I2C_IDLE){

		if(amp_status.common.data == amp_treble_tbl[0])
			goto end;
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
		
end:			
		//--ampSendCmd_38(MVOL,amp_master_tbl[index]);		

		ampSendCmd_38(C3VOL,amp_bass_tbl[index]);		
		ampSendCmd_38(C2VOL,amp_bass_tbl[index]);
		ampSendCmd_38(C1VOL,amp_treble_tbl[index]);
	}
	
	startATask(pt2314_id_CIC, 3, 0, ChangInputVolumeCheck);
}

static void turnUpVolume()
{
	u32 index=0;
	
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
		
		//--ampSendCmd_38(MVOL,amp_master_tbl[index]);
		
		ampSendCmd_38(C3VOL,amp_bass_tbl[index]);
		ampSendCmd_38(C2VOL,amp_bass_tbl[index]);
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
				
				if(amp_status.common.data > amp_treble_tbl[amp_volume_back])
				{
		
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
				if(amp_status.common.data == amp_treble_tbl[0])
				{
				
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

				break;
				
			case AUTO_MUTE:
			
				break;
				
			case UN_MUTE_STATE:

				break;
				
			case AD_SWITCH_STATE:
/*				
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
*/
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





