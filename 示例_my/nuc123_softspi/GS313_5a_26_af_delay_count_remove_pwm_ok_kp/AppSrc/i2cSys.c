
#include "sysinc.h"

#define i2c_scl		PF3
#define i2c_sda		PF2
#define set_scl		GPIO_SET_PIN(i2c_scl)
#define clr_scl		GPIO_CLR_PIN(i2c_scl)
#define set_sda		GPIO_SET_PIN(i2c_sda)
#define clr_sda		GPIO_CLR_PIN(i2c_sda)
#define scl_output	GPIO_SET_MODE(PF,3,GPIO_OPEN_DRAIN)
#define sda_output	GPIO_SET_MODE(PF,2,GPIO_OPEN_DRAIN)
#define i2c_ack		GPIO_GET_PIN(i2c_sda)
#define I2CRATE		6
u32 ack_error = 0;
static u8 DEVICE_ADDRESS = 0xAE;


enum I2C_CHIP i2c_current_chip = NONE_CHIP;
enum I2C_STATE i2c_state = I2C_IDLE;
i2cPoolType *i2c_block_chain_header = NULL;
i2cPoolType *i2c_block_chain_tail = NULL;
static i2cPoolType *i2c_current_block = NULL;

void resumeI2c()
{
	i2c_state =  I2C_IDLE;
	i2c_block_chain_header = NULL;
	i2c_block_chain_tail = NULL;
	i2c_current_block = NULL;
	//--ampResumeI2c();        //--2015.07.02
	//--ampResumeI2c();        //--2015.07.02
	
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


u32 addI2cChain(i2cPoolType *i2cDataBlock, u32 isfast)
{

	if(i2cDataBlock == i2c_current_block && i2c_current_block->byte_ptr[0] != I2C_IDLE)
  {		
		return 0;
	}
	
	i2cDataBlock->index = i2cDataBlock->index_bak;
	if(i2c_block_chain_tail)
	{
			// add in the header
			if(isfast)
			{
				if(i2cDataBlock == i2c_block_chain_header)
					return 0;
				i2cDataBlock->next = i2c_block_chain_header;
				i2c_block_chain_header = i2cDataBlock;
				
				i2cSpirit();				
			}
			else	// add in the tail
			{
				if(i2cDataBlock == i2c_block_chain_tail && i2c_block_chain_header == NULL )
					goto L1;
				
				i2c_block_chain_tail->next = i2cDataBlock;
				i2c_block_chain_tail = i2cDataBlock;
				i2cDataBlock->next = NULL;
			}
			
	}
	else
	{ 
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


void i2cSpirit()     
{
	static u32 repeat_time = 2;
	static u32 busy_time = 0;

	//--audio_detect_L = audio_detect_R = 0;    //--2015.07.02
	switch(i2c_state){
		case I2C_IDLE://this i2c transmission is done
			readI2cChain();
			if(i2c_current_block)
			{
//iputs("weeprom--06.\r\n");  //--zz debug
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
			I2C_SET_DIVIDER(I2C_PORT,I2C_RATE);
			I2C_ENABLE_I2C(I2C_PORT);
			I2C_ENABLE_I2C_INT(I2C_PORT);
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
	//--static u32 read_en = FALSE;

//iputs("weeprom--07.\r\n");  //--zz debug
	
	if(I2C_READ_TIMEOUT_FLAG(I2C_PORT))
	{
		I2C_DISABLE_TOC(I2C_PORT);   
		I2C_CLEAR_TIMEOUT_FLAG(I2C_PORT);   
		I2C_STOP(I2C_PORT);  
		i2c_state = I2C_ERROR;  
		return;  
	}		
	
	i2c_status = I2C_GET_I2CSTATUS(I2C_PORT);  
	if((i2c_current_block->addr&I2C_RD))   
		goto L1;	 
	
//iputs("weeprom--08.\r\n");  //--zz debug
//  uartdecode(i2c_status,"  i2c-status: ");	
	
	
	// i2c write
	switch(i2c_status)
	{
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
	switch(i2c_status)
	{
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


static void delayus(u32 c)
{
	sysDelayUs(c);
}

static u32 i2cAck()
{
	volatile u32 ack;
	
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE);
	set_scl;
	delayus(I2CRATE>>1);
	ack = i2c_ack;
	delayus(I2CRATE>>1);
	clr_scl;
	delayus(I2CRATE);
	return ack;
}

static void ack()
{
	clr_scl;
	delayus(I2CRATE>>1);
	clr_sda;
	delayus(I2CRATE>>1);
	set_scl;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE>>1);
}

static void nack()
{
	clr_scl;
	delayus(I2CRATE>>1);
	set_sda;
	delayus(I2CRATE>>1);
	set_scl;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE>>1);
}

static void i2cRestart()
{
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE*10);
	set_scl;
	delayus(I2CRATE);
	clr_sda;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
}

static void i2cStop()
{
	clr_sda;
	delayus(I2CRATE);
	set_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE);
}


static void i2cWriteByte(u8 data)
{
	u8 msk = 0x80;
	u32 i;
	
	for(i = 0; i<8; i++)
	{
		clr_scl;
		delayus(I2CRATE>>1);
		if(data&msk)
			set_sda;
		else
			clr_sda;
		delayus(I2CRATE>>1);
		set_scl;
		delayus(I2CRATE);
		msk >>= 1;
	}
	
}

static u8 i2cReadByte()
{
	u8 data = 0, msk = 0x80;
	u32 i;
	
	for(i = 0; i<8; i++)
	{
		clr_scl;
		delayus(I2CRATE);
		set_scl;
		delayus(I2CRATE);
		
		if(i2c_ack)
			data |= msk;
		msk >>= 1;
	}
	return data;
}


static u32 i2cStart(u8 address)
{
	I2C_DISABLE_I2C(I2C0);
	I2C_DISABLE_TOC(I2C0);
	outpw(&SYS->GPF_MFP, inpw(&SYS->GPF_MFP) & ~(0x3<<2));
	outpw(&SYS->ALT_MFP1, (inpw(&SYS->ALT_MFP1) & ~(0xF<<24)));
	set_sda;
	set_scl;
	sda_output;
	scl_output;
	delayus(10);  
	
	clr_sda;
	delayus(20);
	i2cWriteByte(address);
	if(i2cAck())
		return 1;
	else
		return 0;
}


void readBuf(u8 *buf, u32 address, u32 count)
{
	u32 i; //--,j;

	ack_error = 0;
	if(i2cStart(DEVICE_ADDRESS) == 0)
	{
		delayus(10);
		i2cWriteByte((address&0xff00)>>8);
		if(i2cAck() == 1)
		{
			ack_error = 1;
			i2cStop();
			return;
		}
		delayus(10);
		i2cWriteByte(address&0xff);
		if(i2cAck() == 1)
		{
			ack_error = 1;
			i2cStop();
			return;
		}
		delayus(10);
		i2cRestart();
		i2cWriteByte(DEVICE_ADDRESS | 0x1);
		if(i2cAck() == 0)
		{
			for(i = 0; count-1 > i; i++)
		  {
				delayus(250);
				buf[i] = i2cReadByte();
				ack();
			}		
			
			delayus(250);
			buf[i] = i2cReadByte();
			nack();
			i2cStop();
			
			return;
		}
		else
		{
			ack_error = 1;
			i2cStop();
		}
			
	}
	else
	{
		ack_error = 1;
		i2cStop();
	}
		
	
}


