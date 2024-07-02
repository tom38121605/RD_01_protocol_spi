/****************************************************************************
 * @file:		PCM5121 Header file
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __pcm5121_h
#define __pcm5121_h




typedef enum {
	PCM_CTRL = 0X00,
	
}PCM5121_REG;

typedef struct{
	volatile enum I2C_STATE state;
	U8 cmd;
	u8 buf[16];
}PCM5121_CMDT;






extern u32  pcmSendData(PCM5121_REG reg);
extern u32  pcmGetData(PCM5121_REG reg);
extern u32 pcmReadData(PCM5121_REG reg, u32 count);
extern void pcm5121Init(void);
extern void pcm5121SetVolume(u32 volume);





#endif


