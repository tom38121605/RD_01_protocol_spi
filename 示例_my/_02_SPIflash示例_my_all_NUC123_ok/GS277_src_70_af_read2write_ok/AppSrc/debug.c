/*-------------------------------------------------------------------------
 * @file:		debug.c
 * @version:	V1.00	
 * @breif:	debug C  souce file.
 * @date: 	2013/2/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla Silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
#include "sysinc.h"
 
enum DEBUG debug = DNULL;		// debug type is init none

volatile u32	DMipsCounter = 0;	
volatile u32	DAdcCounter = 0;
static u32		DMipsCounterMax = 0;		//the max number of DMipsCounter, cpu is the lightest load
static u32		DMipsMin = 0;
Timer_ID 		debug_task = TASK_FULL;
static U32		debug_irq_bak = 0;
static u8		debug_task_bak[TOTAL_TASKS];
static U32_BYTE data_n;	// store data for output normal debug signal
static U32_BYTE data_s;	// store data for ouput specail debug signal
static i2cPoolType i2c_debug_block_n;
static i2cPoolType i2c_debug_block_s;
extern volatile u32 t_t;


/*-------------------------------------------------------------------------
 * @fucname:	 debug_init_task
 * @brief: 	 get the DMipsMaxMask value
 * @param: 	 none
 * @return: 	 none
 * @details:     DMipsMaxMask value = the first one bit in DMipsCounter
 *-----------------------------------------------------------------------*/
static void DMips_init_task()
{
	u32 i_task;

	DMipsCounter  >>= DEBUG_TASK_TIME; // get varage counter per 1 ms
	DMipsCounterMax = DMipsCounter;
	DMipsMin = 72000/DMipsCounterMax;
	DMipsCounter = 0;
	deleteATask(debug_task);	// debug_task is only do once
	debug = DEBUG_SIGNAL;		// debug signal can output now
	NVIC->ISER[0] = debug_irq_bak;	// resume all irq enable
	systick_counter = 0; // make DEV_TEST_TIMER count from 0
	for(i_task = 0; i_task < TOTAL_TASKS; i_task++)// resume all waked tasks
		mytask[i_task].is_wake = debug_task_bak[i_task];		

}


/*-------------------------------------------------------------------------
 * @fucname:	 debug_init_task
 * @brief: 	 start debug init task to get the DMipsMax value
 * @param: 	 none
 * @return: 	 none
 * @details:       disable all irq except systick, disable all task except this task
 			 and debug_init_task's sleep time	is 1<<DEBUG_TASK_TIME ms
 *-----------------------------------------------------------------------*/
void  debugInit()
{
	U32 i;

	debug_irq_bak = NVIC->ISER[0];	// store the nvic irq
	for(i = 0; i < TOTAL_TASKS; i++){
		debug_task_bak[i] = mytask[i].is_wake;	// store the task wake infomation
		mytask[i].is_wake = FALSE;			// turn off all task
	}
	data_n.byte0 = (u8)I2C_IDLE;
	data_s.byte0 = (u8)I2C_IDLE;
	debug_task = getATask();
	if(debug_task != TASK_FULL)
		// run debug_init_task after 1<<DEBUG_TASK_TIME
		startATask(debug_task, 1<<DEBUG_TASK_TIME, 0, DMips_init_task);

}

/*-------------------------------------------------------------------------
 * @fucname:	 loadSignal
 * @brief: 	 looad singal to debug signal
 * @param: 	 signal
 * @return: 	 none
 * @details: 	 looad singal to debug signal
 *-----------------------------------------------------------------------*/
void loadSignal(U32_BYTE *data, u32 signal)
{
	if(data->byte0 == (u8)I2C_BUSY)
		return ;
	*data = *(U32_BYTE*)&signal ; 
	data->byte2 = data->byte1 = data->byte0;
	data->byte1 >>= 4;
	data->byte1 |= DEBUG_MODE_OP;  	// DAC5571 Ctl/MS Byte
	data->byte2 <<= 4;				// DAC5571 LS Byte
}

/*-------------------------------------------------------------------------
 * @fucname:	 I2cToDac
 * @brief: 	 send debug counter to dac chip 
 * @param: 	 dbg : debug counter
 * @return: 	 whether can do i2c tx
 * @details: 	 send debug counter to dac chip 
 *-----------------------------------------------------------------------*/
static bool I2cToDac( i2cPoolType *i2c_debug_block, U32_BYTE* data)
{
	if(data->byte0 == (u8)I2C_BUSY)
		return FALSE;
	i2c_debug_block->addr = DEBUG_DAC_ADDRESS;
	i2c_debug_block->bcount = 2;
	i2c_debug_block->index = 1;
	i2c_debug_block->index_bak= 1;
	i2c_debug_block->wr_rd = I2C_WR;
	data->byte0 = (u8)I2C_BUSY;
	i2c_debug_block->byte_ptr = (u8*)data;
	addI2cChain(i2c_debug_block, FALSE);
	return TRUE;
}


/*-------------------------------------------------------------------------
 * @fucname:	 trace_pc
 * @brief: 	 recode the trace of interrupt
 * @param: 	 none
 * @return: 	 none
 * @details: 	 recode the trace of the position interrupt happened  
 *-----------------------------------------------------------------------*/
#if(INT_TRACE_EN)
void trace_pc()
{
	static u32 *_sp, pc_recode[8], recode_i = 0;

	__asm("mov _sp, __current_sp()");

	//_sp = __get_SP(); 
	
	pc_recode[recode_i++] = _sp[13];   // the index in _sp[] should be change when complier use different optimize
	recode_i &= 7;
}
#endif
/*-------------------------------------------------------------------------
 * @fucname:	 OutPutDebugSignal
 * @brief: 	output to dac chip 
 * @param: 	signal 
 * @return: 	none
 * @details: 	put the debug counter to DAC chip 
 *-----------------------------------------------------------------------*/
void OutPutDebugSignal(u8 signal)
{

	if(signal){
		loadSignal(&data_s,signal);
		I2cToDac(&i2c_debug_block_s, &data_s);
	}else{
#if(SYS_ERROR_OUTPUT_EN)
		if(sys_error != NONE_SYS_ERROR){	// output sys_error code
			I2cToDac(0xFF); 
			sys_error--;			// every sys_error has different pulse width which indicated by sys_error
		}else 	
#endif 
			if(data_n.byte0 == I2C_BUSY) // the last debug signal have not sent
				return;
			switch(debug){
				case DMIPS:

						//DMIpsCounter may be seval times than DMipsCounterMax
						//if(DMipsCounter > DMipsCounterMax)
							//DMipsCounter = DMipsCounterMax;
						
						//DMipsCounter = 72000/DMipsCounter; // the instructions excuted in one cycle
						//DMipsCounter = DMipsCounter - DMipsMin;
						if(t_t>5000) t_t = (~t_t) + 1;
						loadSignal(&data_n, t_t >>= 4); // use the second byte 
						I2cToDac(&i2c_debug_block_n, &data_n);
						DMipsCounter = 0;
						break;
						
				case DADC:	

						break;
						
				default: 		
					break; 
			}
		
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 debugReleaseI2c
 * @brief: 	 resume debug i2c source
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void debugResumeI2c(void)
{
	data_n.byte0 = (u8)I2C_IDLE;
	data_s.byte0 = (u8)I2C_IDLE;
	i2c_debug_block_n.index_bak = 1;
	i2c_debug_block_s.index_bak = 1;
}

