/****************************************************************************
 * @file:		i2cSys.c
 * @version:	V1.00
 * @breif:	I2C TO DAC5571, SE-W58, PT-2314 	
 * @date: 	2013/2/25
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"


enum I2C_CHIP i2c_current_chip = NONE_CHIP;
enum I2C_STATE i2c_state = I2C_IDLE;
i2cPoolType *i2c_block_chain_header = NULL;
i2cPoolType *i2c_block_chain_tail = NULL;
static i2cPoolType *i2c_current_block = NULL;


/*-------------------------------------------------------------------------
 * @fucname:	 freeI2c
 * @brief: 	 free i2c before into standy
 * @param: 	 none
 * @return: 	 none
 * @details: 	 free i2c before into standy
 *-----------------------------------------------------------------------*/
void resumeI2c()
{
	i2c_state =  I2C_IDLE;
	i2c_block_chain_header = NULL;
	i2c_block_chain_tail = NULL;
	i2c_current_block = NULL;
	ampResumeI2c();
	seW58ResumeI2c();
#if(Release == FALSE)
	//debugResumeI2c();
#endif
}
/*-------------------------------------------------------------------------
 * @fucname:	 initI2cDataBlock
 * @brief: 	 init I2c Data Block
 * @param: 	  
 * @return: 
 * @details: 	 init I2c Data Block
 *-----------------------------------------------------------------------*/
void initI2cDataBlock(i2cPoolType *ptr_block, u8 addr, u8 wr_rd, u8 bcount, u8 index, u8*byte_ptr)
{
	ptr_block->addr = addr;
	ptr_block->wr_rd = wr_rd;
	ptr_block->bcount = bcount;
	ptr_block->index = index;
	ptr_block->index_bak = index;
	ptr_block->byte_ptr = byte_ptr;
	ptr_block->next = NULL;
}


/*-------------------------------------------------------------------------
 * @fucname:	 addI2cChain 
 * @brief: 	 add a transmission issue to i2c 
 * @param: 	 i2cDataBlock: i2c data block
 			 isfast: whether should be the next i2c transmission
 * @return:	 none 
 * @details:	 add a transmission issue to i2c , first in first out
 * @note:		 don't add a i2cDataBlock that have been sent but not completed,this function 
 			 don't check whether this case happened
 *---------------- -------------------------------------------------------*/
u32 addI2cChain(i2cPoolType *i2cDataBlock, u32 isfast)
{

	if(i2cDataBlock == i2c_current_block && 
	   i2c_current_block->byte_ptr[0] != I2C_IDLE)
		return 0;
	
	i2cDataBlock->index = i2cDataBlock->index_bak;
	if(i2c_block_chain_tail){
			 // add in the header
			if(isfast){
				if(i2cDataBlock == i2c_block_chain_header)
					return 0;
				i2cDataBlock->next = i2c_block_chain_header;
				i2c_block_chain_header = i2cDataBlock;
				
				i2cSpirit();
				
			}
			else{	// add in the tail
				if(i2cDataBlock == i2c_block_chain_tail && 
					i2c_block_chain_header == NULL )
					goto L1;
				i2c_block_chain_tail->next = i2cDataBlock;
				i2c_block_chain_tail = i2cDataBlock;
				i2cDataBlock->next = NULL;
			}
	}
	else{ 
L1:		// the first i2c data block
		i2c_block_chain_header = i2cDataBlock;
		i2c_block_chain_tail = i2cDataBlock;
		i2cDataBlock->next = NULL;
	}
	return 1;
		
}

/*-------------------------------------------------------------------------
 * @fucname:	 readI2cChain
 * @brief: 	 get i2c data block form the chain 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get i2c data block form the chain and point next data block
 *-----------------------------------------------------------------------*/
void  readI2cChain()
{
	
	if(i2c_block_chain_header == NULL){
		i2c_current_block = NULL;
		i2c_block_chain_tail = NULL;
		return;
	}

	i2c_current_block = i2c_block_chain_header ; // get i2c data block
	
	i2c_block_chain_header = i2c_block_chain_header->next; // header point next block

	if(!i2c_block_chain_header){	// if next block is next means no i2c data block
		i2c_block_chain_header = NULL;
		i2c_block_chain_tail = NULL;
	}
}


/*-------------------------------------------------------------------------
 * @fucname:	 i2cSpirit
 * @brief: 	 transmission i2c data in i2c_block_chain
 * @param: 	 none
 * @return: 	 none
 * @details: 	 transmission i2c data in i2c_block_chain
 *-----------------------------------------------------------------------*/
void i2cSpirit()
{
	static u32 repeat_time = 2;
	static u32 busy_time = 0;

	switch(i2c_state){
		case I2C_IDLE://this i2c transmission is done
			readI2cChain();
			if(i2c_current_block){
				i2c_state = I2C_BUSY;
				I2C_START(I2C_PORT);
				I2C_ENABLE_TOC(I2C_PORT);
				repeat_time = 0;
			}
			break;
			
		case I2C_ERROR:
			if(++repeat_time > 1){ // RESEND NO MORE THAN 2 TIMES
				if(i2c_current_block)
						i2c_current_block->byte_ptr[0] = (U8)I2C_ERROR;
				readI2cChain();
				i2c_state = I2C_IDLE;
				if(i2c_current_block){
					i2c_state = I2C_BUSY;
					I2C_START(I2C_PORT);
				}
			}
			else{
				if(i2c_current_block){
					i2c_current_block->index = i2c_current_block->index_bak;
					i2c_state = I2C_BUSY;
					I2C_START(I2C_PORT);
				}
			}
			break;
			
		case I2C_LOST_ARBITRATION:
			I2C_DISABLE_I2C(I2C_PORT);
			APBCLK_DISABLE(CLK_I2C0);
			SYS_RESET_IP2(SYS_RST_I2C0);
			APBCLK_ENABLE(CLK_I2C0);
			DrvI2C_Init(I2C_PORT, I2C_RATE, TRUE);
			I2C_ENABLE_I2C(I2C_PORT);
			I2C_ENABLE_I2C_INT(I2C_PORT);
			I2C_ENABLE_TOC_DIV4(I2C_PORT);
			I2C_ENABLE_TOC(I2C_PORT);
			i2c_state = I2C_ERROR;
			break;
		
		case I2C_BUSY:
			if (++busy_time > 20)
				i2c_state = I2C_LOST_ARBITRATION;
			else
				return;
			break;
		
		default: 
			break;
	}

	busy_time = 0;
}

/*-------------------------------------------------------------------------
 * @fucname:	 I2C_IRQHandler
 * @brief: 	 do all i2c translate in here contain DAC5571, SE-W58, PT-2314
 * @param: 	 none
 * @return: 	 none
 * @details: 	 do all i2c translate in here contain DAC5571, SE-W58, PT-2314
 *-----------------------------------------------------------------------*/
void I2C_IRQHandler()
{
	u32 i2c_status;
// 	static u32 read_en = FALSE;    //ma 20150629 
	
	if(I2C_READ_TIMEOUT_FLAG(I2C_PORT)){
		I2C_DISABLE_TOC(I2C_PORT);
		I2C_CLEAR_TIMEOUT_FLAG(I2C_PORT);
		I2C_STOP(I2C_PORT);
		i2c_state = I2C_ERROR;
		return;
	}
		
	
	i2c_status = I2C_GET_I2CSTATUS(I2C_PORT);
	if(i2c_current_block->addr&I2C_RD == 1)
		goto L1;
	
	
	// i2c write
	switch(i2c_status){
			case 0x08:	// START has benn sent
					I2C_SET_DATA(I2C_PORT, i2c_current_block->addr );// SET SLA+W/R
					I2C_CLEAR_SI(I2C_PORT);
					return;
			case 0x18:	// SLA+W has been sent
					I2C_SET_DATA(I2C_PORT, i2c_current_block->byte_ptr[i2c_current_block->index++]);
					I2C_CLEAR_SI(I2C_PORT);
					return; 
			case 0X28:	// DATA	has benn sent	ACK received
					if(i2c_current_block->wr_rd == I2C_RD){
						i2c_current_block->addr |= I2C_RD;
						I2C_START(I2C_PORT);
						return;
					}
					
					if(i2c_current_block->bcount < i2c_current_block->index){ // all data have benn sent	
						I2C_STOP(I2C_PORT);
						I2C_DISABLE_TOC(I2C_PORT);
						i2c_state = I2C_IDLE;
						i2c_current_block->byte_ptr[0] = (U8)I2C_IDLE;
						i2c_current_block = NULL;
						return; 
					}
					I2C_SET_DATA(I2C_PORT, i2c_current_block->byte_ptr[i2c_current_block->index++] );
					I2C_CLEAR_SI(I2C_PORT);
					return; 
			case 0x38:	// lost Arbitration
					I2C_STOP(I2C_PORT);
					I2C_DISABLE_TOC(I2C_PORT);
					i2c_state = I2C_LOST_ARBITRATION;
					return;
			default:	 
					I2C_STOP(I2C_PORT);
					I2C_DISABLE_TOC(I2C_PORT);
					i2c_state = I2C_ERROR;
					return;
		}

L1:	// i2c read
	switch(i2c_status){
		case 0x10:
		case 0x08:	// START has benn sent
					I2C_SET_DATA(I2C_PORT, i2c_current_block->addr |I2C_RD );// SET SLA+W/R
					I2C_CLEAR_SI(I2C_PORT);
					return;
		case 0x48:	// SLA+R sent NOT ACK received
					I2C_STOP(I2C_PORT);
					i2c_state =  I2C_ERROR;
					return;
		case 0x40:	// SLA+R SENT AND ACK received
					if(i2c_current_block->bcount > i2c_current_block->index)
						I2C_SET_AA(I2C_PORT);
					else
						I2C_CLEAR_AA(I2C_PORT);
					I2C_CLEAR_SI(I2C_PORT);
					return;
		case 0x58:  // data has been received and Nck returned
					i2c_current_block->byte_ptr[i2c_current_block->index++] = I2C_GET_DATA(I2C_PORT);
					I2C_STOP(I2C_PORT);
					if(i2c_current_block->bcount <  i2c_current_block->index){
						i2c_state = I2C_IDLE;	
						i2c_current_block->byte_ptr[0] = (U8)I2C_IDLE;
						i2c_current_block = NULL;
					}else{
						i2c_state = I2C_ERROR;
						i2c_current_block = NULL;
					}
					return; 
		case 0x50: // data has been received and ack returned
					i2c_current_block->byte_ptr[i2c_current_block->index++] = I2C_GET_DATA(I2C_PORT);
					if(i2c_current_block->bcount > i2c_current_block->index)
						I2C_SET_AA(I2C_PORT);
					else
						I2C_CLEAR_AA(I2C_PORT);
					I2C_CLEAR_SI(I2C_PORT);
					return;
		default:
					I2C_STOP(I2C_PORT);
					i2c_state = I2C_ERROR;
					return;
	}
}


/*-------------------------------------------------------------------------
 * @fucname:	 i2cSysInit
 * @brief: 	 init i2c sys
 * @param: 	 none
 * @return:	 none 
 * @details:	 init i2c sys, set PF.2 AND PF.3 to i2c faction, set i2c rate and disable i2c interrupt
 *---------------- -------------------------------------------------------*/
void i2cSysInit()
{
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_CLC_PIN, GPIO_OPEN_DRAIN);
	GPIO_SET_MODE(I2C_OUT_PORT, I2C_DAT_PIN, GPIO_OPEN_DRAIN);
	GPIO_I2C0();
	NVIC_SetPriority(I2C_IRQN, I2C_INT_PRIORITY);
	DrvI2C_Init(I2C_PORT, I2C_RATE, TRUE);	// SET I2C CLK and enable I2C, disable interrupt
	I2C_ENABLE_TOC_DIV4(I2C_PORT);	// the max hang up time is (4*2^14) / 72M  = 0.9ms
	I2C_ENABLE_TOC(I2C_PORT);
}


