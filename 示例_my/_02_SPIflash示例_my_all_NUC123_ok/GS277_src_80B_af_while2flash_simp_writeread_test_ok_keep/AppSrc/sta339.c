/****************************************************************************
 * @file:		sta339.c
 * @version:		
 * @breif:		STA339 AMP source file
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/

#include "sysinc.h"
#define battery_14_8		FALSE
#define battery_7_4		TRUE

typedef struct{
	volatile enum I2C_STATE state;
	AMPDATTYPE common;
}AMP_CMDT;

AMP_CMDT amp_status;
static i2cPoolType amp_i2c_block;
Timer_ID amp_id;
U8 amp_info[0x17];
u32	amp_volume = 24;     //ma 20150508 default 20 to 23 volume  //ma 20150517   23  to 24 
//front and side drivers have the same volume table
u8	amp_treble_tbl[SE_MAX_VOLUME] = {			
										0xFF,
										0xD4,
										0xD0,
										0xCC,
										0xC8,
										0xC4,
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
										0x5C	
									}; 
// volume table for the bass driver
u8	amp_bass_tbl[SE_MAX_VOLUME] = {			
										0xFF,			// min volume
										0xBB,
										0xB9,
										0xB6,    
										0xB3,    
										0xB0,    
										0xAC,    
										0xA9,    
										0xA6,    
										0xA2,    
										0x9E,    
										0x9B,    
										0x97,    
										0x94,    
										0x90,    
										0x8C,    
										0x89,    
										0x85,    
										0x81,    
										0x7E,    
										0x7A,    
										0x76,    
										0x73,    
										0x6F,    
										0x6B,    
										0x67,    
										0x63,    
										0x5F,    
										0X5B,    
										0X57,    
										0X53,
										0X4F, 		//max volume
									};

// configuration for front drivers, no EQ
u8 amp_conf_38[AMP2_CONF_COUNT] = {
			0x00, 0x63, // CONFA:                 
			0x01, 0x80, // CONFB:                 
			0x02, 0x97, // CONFC:                 
			0x03, 0x50, // CONFD:                 
			0x04, 0xC2, // CONFE:                 
			0x05, 0xDC, // CONFF:                 
			0x06, 0x10, //MUTELOC:                
			0x07, 0x08, //MVOL:                   
			0x08, 0x54, //C1VOL: //4a             
			0x09, 0x54, //C2COL: //4a             
			0x0a, 0x60, //C3VOL:                  
			0x0b, 0x80, //AUTO1:                  
			0x0c, 0x00, //AUTO2:                  
			0x0D, 0x00, //AUTO3:
			0x0E, 0x00, //C1CFG:			
			0x0F, 0x40, //C2CFG:      
			0x10, 0x80, //C3CFG:      
			0x11, 0x77, //TONE:       
			0x12, 0x6A, //L1AR:       
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
			0X31, 0X00, //EQCFG:                            
			0X32, 0XA8, //EATH1:                            
			0X33, 0X98, //ERTH1:                            
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
// configuration for side drivers, no EQ
u8 amp_conf_38_back[AMP2_CONF_COUNT] = {
			0x00, 0x63, // CONFA:						
			0x01, 0x80, // CONFB:           
			0x02, 0x97, // CONFC:           
			0x03, 0x50, // CONFD:           
			0x04, 0xC2, // CONFE:           
			0x05, 0xDC, // CONFF:           
			0x06, 0x10, //MUTELOC:          
			0x07, 0x08, //MVOL:             
			0x08, 0x54, //C1VOL: //4a       
			0x09, 0x54, //C2COL: //4a       
			0x0a, 0x60, //C3VOL:            
			0x0b, 0x80, //AUTO1:            
			0x0c, 0x00, //AUTO2:            
			0x0D, 0x00, //AUTO3:
			0x0E, 0x00, //C1CFG:		
			0x0F, 0x40, //C2CFG:
			0x10, 0x80, //C3CFG:
			0x11, 0x77, //TONE:
			0x12, 0x6A, //L1AR:
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
			0X31, 0X00, //EQCFG:
			0X32, 0XA8, //EATH1:
			0X33, 0X98, //ERTH1:
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
// configuration for bass driver, has EQ applied and 5 different EQ profiles for the top 5 volume setting
u8 amp_conf_3A[AMP_CONF_BYTES_COUNT] = {
			0x00, 0x63, // CONFA:  
			0x01, 0x80, // CONFB:  
			0x02, 0x97, // CONFC:  
			0x03, 0x50, // CONFD:  
			0x04, 0xC2, // CONFE:  
			0x05, 0xDF, // CONFF:  
			0x06, 0x16, //MUTELOC: 
			0x07, 0x04, //MVOL:    
			0x08, 0x69, //C1VOL:   
			0x09, 0x56, //C2COL:   
			0x0a, 0x3A, //C3VOL:   
			0x0b, 0x80, //AUTO1:   
			0x0c, 0x00, //AUTO2:   
			0x0D, 0x00, //AUTO3:
			0x0E, 0x00, //C1CFG:        
			0x0F, 0x40, //C2CFG:        
			0x10, 0x90, //C3CFG:        
			0x11, 0x77, //TONE:         
			0x12, 0x2D, //L1AR:         
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
			0X31, 0X00, //EQCFG:                             
			0X32, 0XC4, //EATH1:                             
			0X33, 0XB0, //ERTH1:                             
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
			0X3F, 0X00,                                      
};

// front EQ
static u32 amp_eq_tbl_38[64] = {
			0x000000, // Coef[000h] - C1H10 (b1/2)          
			0x000000, // Coef[001h] - C1H11 (b2)            
			0x000000, // Coef[002h] - C1H12 (a1/2)          
			0x000000, // Coef[003h] - C1H13 (a2)            
			0x400000, // Coef[004h] - C1H14 (b0/2)          
			0x8067D1, // Coef[005h] - C1H20 (b1/2)          
			0x7F580E, // Coef[006h] - C1H21 (b2)            
			0x7F982F, // Coef[007h] - C1H22 (a1/2)           
			0x80C4AA, // Coef[008h] - C1H23 (a2)            
			0x3FF1A4, // Coef[009h] - C1H24 (b0/2)          
			0x807D55, // Coef[00Ah] - C1H30 (b1/2)          
			0x7F3540, // Coef[00Bh] - C1H31 (b2)            
			0x7F82AB, // Coef[00Ch] - C1H32 (a1/2)          
			0x80E1FD, // Coef[00Dh] - C1H33 (a2)            
			0x3FF461, // Coef[00Eh] - C1H34 (b0/2)          
			0x000000, // Coef[00Fh] - C1H40 (b1/2)          
			0x000000, // Coef[010h] - C1H41 (b2)            
			0x000000, // Coef[011h] - C1H42 (a1/2)          
			0x000000, // Coef[012h] - C1H43 (a2)            
			0x400000, // Coef[013h] - C1H44 (b0/2)          
			0x000000, // Coef[014h] - C2H10 (b1/2)          
			0x000000, // Coef[015h] - C2H11 (b2)            
			0x000000, // Coef[016h] - C2H12 (a1/2)          
			0x000000, // Coef[017h] - C2H13 (a2)            
			0x400000, // Coef[018h] - C2H14 (b0/2)          
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
			0x808B06, // Coef[028h] - C12H0 (b1/2)          
			0x7F74FA, // Coef[029h] - C12H1 (b2)            
			0x7F74A0, // Coef[02Ah] - C12H2 (a1/2)          
			0x811559, // Coef[02Bh] - C12H3 (a2)            
			0x3FBA7D, // Coef[02Ch] - C12H4 (b0/2)          
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

//side EQ
static u32 amp_eq_tbl_38_backward[64] = {
			0x000000, // Coef[000h] - C1H10 (b1/2)          
			0x000000, // Coef[001h] - C1H11 (b2)            
			0x000000, // Coef[002h] - C1H12 (a1/2)          
			0x000000, // Coef[003h] - C1H13 (a2)            
			0x400000, // Coef[004h] - C1H14 (b0/2)          
			0x8067D1, // Coef[005h] - C1H20 (b1/2)          
			0x7F580E, // Coef[006h] - C1H21 (b2)            
			0x7F982F, // Coef[007h] - C1H22 (a1/2)           
			0x80C4AA, // Coef[008h] - C1H23 (a2)            
			0x3FF1A4, // Coef[009h] - C1H24 (b0/2)          
			0x807D55, // Coef[00Ah] - C1H30 (b1/2)          
			0x7F3540, // Coef[00Bh] - C1H31 (b2)            
			0x7F82AB, // Coef[00Ch] - C1H32 (a1/2)          
			0x80E1FD, // Coef[00Dh] - C1H33 (a2)            
			0x3FF461, // Coef[00Eh] - C1H34 (b0/2)          
			0x000000, // Coef[00Fh] - C1H40 (b1/2)          
			0x000000, // Coef[010h] - C1H41 (b2)            
			0x000000, // Coef[011h] - C1H42 (a1/2)          
			0x000000, // Coef[012h] - C1H43 (a2)            
			0x400000, // Coef[013h] - C1H44 (b0/2)          
			0x000000, // Coef[014h] - C2H10 (b1/2)          
			0x000000, // Coef[015h] - C2H11 (b2)            
			0x000000, // Coef[016h] - C2H12 (a1/2)          
			0x000000, // Coef[017h] - C2H13 (a2)            
			0x400000, // Coef[018h] - C2H14 (b0/2)          
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
			0x808B06, // Coef[028h] - C12H0 (b1/2)          
			0x7F74FA, // Coef[029h] - C12H1 (b2)            
			0x7F74A0, // Coef[02Ah] - C12H2 (a1/2)          
			0x811559, // Coef[02Bh] - C12H3 (a2)            
			0x3FBA7D, // Coef[02Ch] - C12H4 (b0/2)          
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

//EQ for the bass driver
static u32 amp_eq_tbl_3A[64] = {
			0x805A7F, // Coef[000h] - C1H10 (b1/2)          
			0x7FA581, // Coef[001h] - C1H11 (b2)            
			0x7FA560, // Coef[002h] - C1H12 (a1/2)          
			0x80B4BE, // Coef[003h] - C1H13 (a2)            
			0x3FD2C0, // Coef[004h] - C1H14 (b0/2)          
			0x8088CC, // Coef[005h] - C1H20 (b1/2)          
			0x7EC2D7, // Coef[006h] - C1H21 (b2)            
			0x7F776C, // Coef[007h] - C1H22 (a1/2)           
			0x811005, // Coef[008h] - C1H23 (a2)            
			0x4016CA, // Coef[009h] - C1H24 (b0/2)          
			0x800BEE, // Coef[00Ah] - C1H30 (b1/2)          
			0x7FDCAE, // Coef[00Bh] - C1H31 (b2)            
			0x7FF412, // Coef[00Ch] - C1H32 (a1/2)          
			0x801796, // Coef[00Dh] - C1H33 (a2)            
			0x4005DE, // Coef[00Eh] - C1H34 (b0/2)          
			0x803623, // Coef[00Fh] - C1H40 (b1/2)          
			0x7FA9C2, // Coef[010h] - C1H41 (b2)             
			0x7FC9DD, // Coef[011h] - C1H42 (a1/2)          
			0x8069C1, // Coef[012h] - C1H43 (a2)            
			0x3FF63E, // Coef[013h] - C1H44 (b0/2)          
			0x000000, // Coef[014h] - C2H10 (b1/2)          
			0x000000, // Coef[015h] - C2H11 (b2)            
			0x000000, // Coef[016h] - C2H12 (a1/2)          
			0x000000, // Coef[017h] - C2H13 (a2)            
			0x400000, // Coef[018h] - C2H14 (b0/2)          
			0x8062DE, // Coef[019h] - C2H20 (b1/2)          
			0x7ECD9C, // Coef[01Ah] - C2H21 (b2)            
			0x7F9D22, // Coef[01Bh] - C2H22 (a1/2)          
			0x80C4D0, // Coef[01Ch] - C2H23 (a2)            
			0x4036C9, // Coef[01Dh] - C2H24 (b0/2)          
			0x000273, // Coef[01Eh] - C2H30 (b1/2)          
			0x000273, // Coef[01Fh] - C2H31 (b2)            
			0x7E6C92, // Coef[020h] - C2H32 (a1/2)          
			0x831D0D, // Coef[021h] - C2H33 (a2)            
			0x000139, // Coef[022h] - C2H34 (b0/2)          
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

// the first PEQ at 0dB volume level
static u32 eq_1st[5] = {
			0x805A7F, // Coef[028h] - C12H0 (b1/2)          
			0x7FA581, // Coef[029h] - C12H1 (b2)            
			0x7FA560, // Coef[02Ah] - C12H2 (a1/2)          
			0x80B4BE, // Coef[02Bh] - C12H3 (a2)            
			0x3FD2C0, // Coef[02Ch] - C12H4 (b0/2)     
}; 
// the first PEQ at -2dB volume level
static u32 eq_2nd[5] = {
			0x805A7F, // Coef[028h] - C12H0 (b1/2)          
			0x7FA581, // Coef[029h] - C12H1 (b2)            
			0x7FA560, // Coef[02Ah] - C12H2 (a1/2)          
			0x80B4BE, // Coef[02Bh] - C12H3 (a2)            
			0x3FD2C0, // Coef[02Ch] - C12H4 (b0/2) 
};
// the first PEQ at -4dB volume level
static u32 eq_3rd[5] = {
			0x805A7F, // Coef[028h] - C12H0 (b1/2)          
			0x7FA581, // Coef[029h] - C12H1 (b2)            
			0x7FA560, // Coef[02Ah] - C12H2 (a1/2)          
			0x80B4BE, // Coef[02Bh] - C12H3 (a2)            
			0x3FD2C0, // Coef[02Ch] - C12H4 (b0/2) 
};
// the first PEQ at -6dB volume level
 static u32 eq_4th[5] = {
 			0x805A7F, // Coef[028h] - C12H0 (b1/2)             //ma 20150629   no reference  here 
 			0x7FA581, // Coef[029h] - C12H1 (b2)            
 			0x7FA560, // Coef[02Ah] - C12H2 (a1/2)          
 			0x80B4BE, // Coef[02Bh] - C12H3 (a2)            
 			0x3FD2C0, // Coef[02Ch] - C12H4 (b0/2) 
 };
// the first PEQ at -8dB volume level
 static u32 eq_5th[5] = {                                 //ma 20150629   no reference  here 
 			0x805A7F, // Coef[028h] - C12H0 (b1/2)          
 			0x7FA581, // Coef[029h] - C12H1 (b2)            
 			0x7FA560, // Coef[02Ah] - C12H2 (a1/2)          
 			0x80B4BE, // Coef[02Bh] - C12H3 (a2)            
 			0x3FD2C0, // Coef[02Ch] - C12H4 (b0/2) 
 };

 
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
	return amp_status.common.data;
}
/*-------------------------------------------------------------------------
 * @fucname:	 ampSendCmd_3A
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampSendCmd_3A(AMP_REG reg, u8 data)
{
	initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR3A, 
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
}

u8 ampGetCmd_3A(AMP_REG reg)
{
	initI2cDataBlock(	&amp_i2c_block, 
					STA339_I2C_ADDR3A, 
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
	return amp_status.common.data;
}

/*-------------------------------------------------------------------------
 * @fucname:	 ampSendAllConf
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern void writeAmp38Cmd(u8 addr, u8 data);
extern void exitGpioI2c(void);
void ampInitAllConf(void)
{
	u32 i = 0;
	
	while(1){
		
		if( i >= (AMP_CONF_BYTES_COUNT))
			break;
		
		ampSendCmd_3A((AMP_REG)amp_conf_3A[i],amp_conf_3A[i+1]);        //ma 20150629  add typedef before data translating 
		
		i += 2; 
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}

	ampSendCmd_3A((AMP_REG)0x05,0xdf);

	i = 0;
	while(1){
		
		if( i >= (AMP2_CONF_COUNT))
			break;
		
		ampSendCmd_38((AMP_REG)amp_conf_38[i],amp_conf_38[i+1]);
		
		i += 2; 
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	ampSendCmd_38((AMP_REG)0x05,0xdc);

	i = 0;
	while(1){
		
		if( i >= (AMP2_CONF_COUNT))
			break;
		
		writeAmp38Cmd(amp_conf_38[i],amp_conf_38[i+1]);
		
		i += 2; 
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	}
	writeAmp38Cmd(0x05,0xdc);
	exitGpioI2c();
	
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
	u8 i = 0;                          //ma 20150728  change u32 to u8 

	for(i = 0; i<0x16; i++){
		amp_status.common.cmd = (AMP_REG)i;        //ma 20150728  add typedefine
		 
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
// static u32 cfg_address_tbl[5] = { 0x15, 0x18, 0x1b, 0x1e, 0x21 };        //ma 20150629   no reference  here
static U32_BYTE cfg_data;
void ampSendEqs_38(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;

	while(i <= end_addr){
		ampSendCmd_38(EQCFG, 0); // select ram bank0 for 48khz signal
		ampSendCmd_38(CFADDR, i); // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_eq_tbl_38[i];
		ampSendCmd_38(B1CF1, cfg_data.byte2);
		ampSendCmd_38(B1CF2, cfg_data.byte1);
		ampSendCmd_38(B1CF3, cfg_data.byte0);
		ampSendCmd_38(CFUD, 1); // Write 1 to w1 in CFUD
		i++;
	}
	
}


void ampSendEqs_38b(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;

	while(i <= end_addr){
		writeAmp38Cmd(EQCFG, 0); // select ram bank0 for 48khz signal
		writeAmp38Cmd(CFADDR, i); // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_eq_tbl_38_backward[i];
		writeAmp38Cmd(B1CF1, cfg_data.byte2);
		writeAmp38Cmd(B1CF2, cfg_data.byte1);
		writeAmp38Cmd(B1CF3, cfg_data.byte0);
		writeAmp38Cmd(CFUD, 1); // Write 1 to w1 in CFUD
		i++;
	}
	
}

void ampSendEqs_3A(U32 start_addr, U32 end_addr)
{
	u32 i;

	if(start_addr >63 || end_addr > 63 || start_addr > end_addr)
		return;
	
	i = start_addr;

	while(i <= end_addr){
		ampSendCmd_3A(EQCFG, 0); // select ram bank0 for 48khz signal
		ampSendCmd_3A(CFADDR, i); // Write ram addr
		cfg_data = *(U32_BYTE*)&amp_eq_tbl_3A[i];
		ampSendCmd_3A(B1CF1, cfg_data.byte2);
		ampSendCmd_3A(B1CF2, cfg_data.byte1);
		ampSendCmd_3A(B1CF3, cfg_data.byte0);
		ampSendCmd_3A(CFUD, 1); // Write 1 to w1 in CFUD
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

	ampSendCmd_3A(EQCFG, 0); // select ram bank0 for 48khz signal
	ampSendCmd_3A(CFADDR,start_addr); // Write ram addr
	while(i < 5){
		
		cfg_data = *(U32_BYTE*)&tbl[i];
		ampSendCmd_3A((AMP_REG)(B1CF1 + 3*i), cfg_data.byte2);     //ma 20150629  add typedef before data translating 
		ampSendCmd_3A((AMP_REG)(B1CF2 + 3*i), cfg_data.byte1);     //ma 20150629  add typedef before data translating 
		ampSendCmd_3A((AMP_REG)(B1CF3 + 3*i), cfg_data.byte0);      //ma 20150629  add typedef before data translating 
		
		i++;
	}
	ampSendCmd_3A(CFUD, 2); // Write 1 to w1 in CFUD
}


void ampMute(void)      //ma 20150618    
{ 
	ampSendCmd_38(MUTELOC, 0x1e);
}

void ampunMute(void)         //ma 20150618    
{
	ampSendCmd_38(MUTELOC, 0x10);
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 ampFlasfLed
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void ampFlasfLed()
{
	led_flash = 4;
	startATask(amp_id, 240, 0, ampFlasfLed);
}

void stopAmpFlash()
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
//  extern u32 Dwhp83_flag;   // 20150612
void ampDecVolume(void)
{
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampDecVolume);
		return;
	}

	if(amp_volume > 0){
// 		if(ismute()==0)           //ma 20150902      if mute ,unmute      //ma 20150929  delete useless function
			amp_volume--;
		led_flash = 4;
		stopATask(amp_id);
		//seW58SetVolume(amp_volume);
		//seW58sendCmd();
		ampSetVolume(amp_volume);
		DWP83CuCV();
		if( dwhp83_type != VOLUME_STREO )
			Dwhp83_flag |= 0x2;
// 		if(SB_flag&&dev_input == USERDEF)                              //MA 20150731  VOLUME to sub   //ma 20150818  del useless function 
// 			SB_volume_trans();    //ma 20150604   SB_volume_trans
	}

	
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
	if(amp_status.state == I2C_BUSY){
		startATask(amp_id, 2, 0, ampDecVolume);
		return; 
	}

	if(amp_volume < MAX_VOLUME-1){
// 		if(ismute()==0)           //ma 20150902      if mute ,unmute   //ma 20150929 delete useless function
			amp_volume++;
		led_flash = 4;
		stopATask(amp_id);
		//seW58SetVolume(amp_volume);
		//seW58sendCmd();
		ampSetVolume(amp_volume);
		DWP83CuCV();
		if( dwhp83_type != VOLUME_STREO )         
			Dwhp83_flag |= 0x2;
// 		if(SB_flag&&dev_input == USERDEF)                          //MA 20150731  VOLUME to sub does not work when rx     //ma 20150818  del useless function 
// 			SB_volume_trans();    //ma 20150604   SB_volume_trans
	}
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

	//amp_volume=19;  //--zz
	
	if(v == 31){
		ampSetEqDefault(0x0, 0x4, eq_1st);
	}else if(v == 30){
		ampSetEqDefault(0x0, 0x4, eq_2nd);
	}else if(v == 29){
		ampSetEqDefault(0x0, 0x4, eq_3rd);
	}
	else if(v == 28){
		ampSetEqDefault(0x0, 0x4, eq_4th);
	}else if(v == 27){
		ampSetEqDefault(0x0, 0x4, eq_5th);
	}
	
	/*else if(v == 28){
		ampSetEqDefault(0x0, 0x4, eq_4th);
	}else{
		ampSetEqDefault(0x0, 0x4, eq_5th);
	}*/
	
	writeAmp38Cmd(C1VOL,amp_treble_tbl[amp_volume]);
	writeAmp38Cmd(C2VOL,amp_treble_tbl[amp_volume]);
	ampSendCmd_38(C1VOL,amp_treble_tbl[amp_volume]);
	ampSendCmd_38(C2VOL,amp_treble_tbl[amp_volume]);
	ampSendCmd_3A(C3VOL,amp_bass_tbl[amp_volume]);
	pcm5121SetVolume(amp_volume);
}

void callOnMode()
{
	DWHP83Mute();
	sysDelayMs(5);
	seW58SetBypass(1);
	writeAmp38Cmd(MUTELOC, 0x1e);
	ampSendCmd_3A(MUTELOC, 0x1e);
}

void callOffMode()
{
	DWHP83UnMute();
	sysDelayMs(5);
	seW58SetBypass(0);
	writeAmp38Cmd(MUTELOC, 0x10);
	ampSendCmd_3A(MUTELOC, 0x10);
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

void amp1EQbypass()
{
	u8 data;
	
	data = ampGetCmd_38(C1CFG);
	ampSendCmd_38(C1CFG, data|0x2);
	data = ampGetCmd_38(C2CFG);
	ampSendCmd_38(C2CFG, data|0x2);
	data = ampGetCmd_38(C3CFG);
	ampSendCmd_38(C3CFG, data|0x2);
}

void amp1EQon()
{
	u8 data;
	
	data = ampGetCmd_38(C1CFG);
	ampSendCmd_38(C1CFG, data&~0x2);
	data = ampGetCmd_38(C2CFG);
	ampSendCmd_38(C2CFG, data&~0x2);
	data = ampGetCmd_38(C3CFG);
	ampSendCmd_38(C3CFG, data&~0x2);
}

void amp2EQbypass()
{
	u8 data;
	
	data = ampGetCmd_3A(C1CFG);
	ampSendCmd_3A(C1CFG, data|0x2);
	data = ampGetCmd_3A(C2CFG);
	ampSendCmd_3A(C2CFG, data|0x2);
	data = ampGetCmd_3A(C3CFG);
	ampSendCmd_3A(C3CFG, data|0x2);
}

void amp2EQon()
{
	u8 data;
	
	data = ampGetCmd_3A(C1CFG);
	ampSendCmd_3A(C1CFG, data&~0x2);
	data = ampGetCmd_3A(C2CFG);
	ampSendCmd_3A(C2CFG, data&~0x2);
	data = ampGetCmd_3A(C3CFG);
	ampSendCmd_3A(C3CFG, data&~0x2);
}

void ampPowerDown()
{
	writeAmp38Cmd(CONFF, 0x7c);
} 


