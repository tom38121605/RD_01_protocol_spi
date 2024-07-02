/****************************************************************************
 * @file:		i2cSys.h
 * @version:	V1.00
 * @breif:	I2C SYS header file	
 * @date: 	2013/2/25
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __i2cSys_h
#define __i2cSys_h

#include "sysinc.h" 

#define I2C_OUT_PORT	PF 
#define I2C_DAT_PIN		2 
#define I2C_CLC_PIN		3 
#define I2C_PORT			I2C0 
#define I2C_IRQN			I2C0_IRQn 
#define I2C_CLK			  CLK_I2C0 
#define I2C_INT_PRIORITY	2  
#define I2C_IRQHandler	I2C0_IRQHandler 
#define I2C_RATE			50000	// 150K 
#define I2C_RD			0X01 
#define I2C_WR			0X00 
#define ENABLE_I2C_IRQ  I2C_ENABLE_I2C_INT(I2C0) 
#define DISABLE_I2C_IRQ I2C_DISABLE_I2C_INT(I2C0) 

enum I2C_CHIP
{
		NONE_CHIP,
		I2C_DAC5571,
		I2C_SE_W58_II,
		I2C_PT2314
};
extern enum I2C_CHIP i2c_current_chip;

enum I2C_STATE{
	I2C_IDLE = 0XA0,
	I2C_LOST_ARBITRATION = 0XA1,
	I2C_BUSY = 0XA2,
	I2C_ERROR = 0XA3
};

typedef struct I2C_DATA
{
	u8 addr;
	u8 wr_rd;
	u8 bcount ;
	u8 index;		 // the transmited data index  always begin as 1, and 0 is store the i2c transmisiion state.
	u8 index_bak;
	u8 *byte_ptr;	// i2c_byte_ptr[0] store whether i2c transmit is ok, i2c_byte_ptr[1] is the fisrt data to transmite.
	struct I2C_DATA *next;
}i2cPoolType;

extern void i2cSysInit(void);
extern void i2cSpirit(void);
extern void resumeI2c(void);
extern u32 addI2cChain(i2cPoolType * i2cDataBlock, u32 isfast);
extern void initI2cDataBlock(i2cPoolType *ptr_block, u8 addr, u8 wr_rd, u8 bcount, u8 index, u8*byte_ptr);
#endif


