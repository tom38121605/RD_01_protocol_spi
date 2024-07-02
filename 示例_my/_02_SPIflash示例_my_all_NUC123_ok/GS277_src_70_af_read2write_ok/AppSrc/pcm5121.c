/****************************************************************************
 * @file:		PCM5121.C
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define PCM5121_ADDR 	0X9E

static i2cPoolType pcm_i2c_block;
static PCM5121_CMDT pcm_status;


u8 pcm5121_volume_tbl[32] = {
										0xFd,
										0x99,
										0x96,
										0x93,
										0x8F,
										0x8C,
										0x89,
										0x86,
										0x84,
										0x81,
										0x7F,
										0x7C,
										0x79,
										0x77,
										0x74,
										0x71,
										0x6E,
										0x6C,
										0x68,
										0x64,
										0x60,
										0x5C,
										0x58,
										0X54,
										0X50,
										0X4C,
										0X48, 
										0X44,
										0X40,	// -9db
										0x3c,
										0x38,
										0x34,	// -3db,

};

void pcmSendCmd(PCM5121_REG reg, u8 data)
{
	initI2cDataBlock(	&pcm_i2c_block, 
					PCM5121_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&pcm_status
				);
	
	pcm_status.cmd = reg;
	pcm_status.buf[0] = data;
	pcm_status.state = I2C_BUSY;
	addI2cChain(&pcm_i2c_block, 0);
	while(pcm_status.state == I2C_BUSY)
			;
}


u32 pcmGetData(PCM5121_REG reg)
{
	initI2cDataBlock(	&pcm_i2c_block, 
					PCM5121_ADDR, 
					I2C_RD, 
					2,
					1,
					(u8*)&pcm_status
				);
	
	pcm_status.cmd = reg;
	pcm_status.buf[0] = 0;
	pcm_status.state = I2C_BUSY;
	addI2cChain(&pcm_i2c_block, 0);
	while(pcm_status.state == I2C_BUSY)
			;
	
	return (pcm_status.state == I2C_IDLE);
}



/*-------------------------------------------------------------------------
 * @fucname:	 pcmReadData
 * @brief: 	count must < 16 and > 0
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u32 pcmReadBuf(PCM5121_REG reg, u32 count)
{
	initI2cDataBlock(	&pcm_i2c_block, 
					PCM5121_ADDR, 
					I2C_RD, 
					count+1,
					1,
					(u8*)&pcm_status
				);
	
	pcm_status.cmd = reg|0x80;
	pcm_status.state = I2C_BUSY;
	addI2cChain(&pcm_i2c_block, 0);
	while(pcm_status.state == I2C_BUSY)
			;
	
	return (pcm_status.state == I2C_IDLE);
}


u32 pcmWriteBuf(PCM5121_REG reg, u32 count, u8 *buf)
{
	initI2cDataBlock(	&pcm_i2c_block, 
					PCM5121_ADDR, 
					I2C_WR, 
					count+1,
					1,
					(u8*)&pcm_status
				);
	
	pcm_status.cmd = reg|0x80;
	pcm_status.state = I2C_BUSY;
	
	while(count!=0){
		count--;
		pcm_status.buf[count] = buf[count];
		
	}
	
	addI2cChain(&pcm_i2c_block, 0);
	while(pcm_status.state == I2C_BUSY)
			;
	
	return (pcm_status.state == I2C_IDLE);
}



void pcm5121Init()
{
// 	u32 count = 16;    //ma 20150629 
			//     1,     2,     3,    4,     5,     6,     7,     8,     9,     10,   11,   12,   13,  
// 	u8 buf[15] = {0x00,0x00,0X00,0X01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};     //ma 20150629 
// 	
// 	//pcmWriteBuf(0,13,buf);

// 	buf[0] = 0x00; //37
// 	buf[1] = 0x00; //38
// 	buf[2] = 0x00; //39
// 	buf[3] = 0x03; //40
// 	buf[4] = 0x00; //41
// 	buf[5] = 0x11; //42
// 	buf[6] = 0x01; //43
// 	buf[7] = 0x00; //44
		
	//pcmWriteBuf(37,8,buf);
	pcmSendCmd((PCM5121_REG)60,0x00);           //ma 20150629  add typedef before data translating 
	//pcmSendCmd(61,0x30);
	//pcmSendCmd(62,0x30); 
	pcmSendCmd((PCM5121_REG)63,0xbb);           //ma 20150629  add typedef before data translating 
	pcmSendCmd((PCM5121_REG)64,0xb0);           //ma 20150629  add typedef before data translating 
	pcmSendCmd((PCM5121_REG)65,0x00);           //ma 20150629  add typedef before data translating 	 
	//pcmSendCmd(0,0x01); 
	//pcmSendCmd(1,0x00);
	//pcmSendCmd(2,0x00);
	//sysDelayMs(100); 
	//pcmSendCmd(0,0x00);
	//pcmGetData(91);
}



void pcm5121SetVolume(u32 volume)
{
	pcmSendCmd((PCM5121_REG)0x0, 0x00);                          //ma 20150629  add typedef before data translating 
	pcmSendCmd((PCM5121_REG)61,pcm5121_volume_tbl[volume]+2);    //ma 20150629  add typedef before data translating 
	pcmSendCmd((PCM5121_REG)62,pcm5121_volume_tbl[volume]+2);     //ma 20150629  add typedef before data translating 
}











