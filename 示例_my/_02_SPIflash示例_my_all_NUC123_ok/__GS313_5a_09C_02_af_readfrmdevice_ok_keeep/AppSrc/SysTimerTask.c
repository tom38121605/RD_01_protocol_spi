/*-------------------------------------------------------------------------
 * @file:		SysTimerTask.c
 * @version:	V1.00	
 * @breif:	SysTimerTask C source file. deal the timed task 	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013 Globla silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
#include "sysinc.h"

Timer_Task mytask[TOTAL_TASKS];
volatile u32  systick_counter = 0;	// the 
volatile u32 delay_us = 0;
volatile static u32 sys_delay_counter = 0;
extern s32 audio_detect_L,audio_detect_R;;
U32 audio_L = 0, audio_R = 0;
//--static u32 pwm_cc = 0;
u32 pwm_ccl = 30;
extern s32 breath_flag;

u8 flg_touchin=0;
		

void SysTickInit(u32 ticks)
{                                                              
	SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      // set reload register
	NVIC_SetPriority (SysTick_IRQn, 3);  //set Priority is lowest 
	SysTick->VAL   = 0;                                         
	SET_SYSTICK_CLKSRC(CLK_STCLK_HXT_2);
	// Enable SysTick IRQ and SysTick Timer 
	SysTick->CTRL  = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; 
}


/*-------------------------------------------------------------------------
 * @fucname:	 sysDelayUs
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void sysDelayUs(U32 c)
{
	while(c)
	{
		c--;
		delay_us = 0;
		while(delay_us < 5)
			delay_us++;
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 sysDelayMs
 * @brief: 	 delay sevral millisecond
 * @param: 	 delay_ms 
 * @return: 	 none
 * @details: 	 delay sevral millisecond
 *-----------------------------------------------------------------------*/
void sysDelayMs(u32 delay_ms)
{
	sys_delay_counter =  systick_counter + delay_ms;
	
	while(sys_delay_counter >= systick_counter )
	{
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();		
	}	
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 sysDelayAMs
 * @brief: 	 is more accalate than sysDelayMs(). it compose the systick become
						 the reloader value before read sys_delay_counter
 * @param: 	 delay_ms 
 * @return: 
 * @details: 	delay delay_ms ms then return
 *-----------------------------------------------------------------------*/
void sysDelayAMs(u32 delay_ms)
{
	SysTick->VAL = 0;
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	sys_delay_counter =  systick_counter + delay_ms;
	while(sys_delay_counter > systick_counter )
	{
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
		
	}
	
}

void SysTick_Handler()
{
	u32 i ;

//iputs("SysTick--01.\r\n");  //--zz debug

	systick_counter++;	// sys time counter
	
//iputs("SysTick_Handler--01.\r\n");  //--zz debug
	
#if(!Release)
//	if(debug != DNULL)
//		OutPutDebugSignal(DNULL);        // output debug counter to DAC5571
#endif

	for(i = 0; i < TOTAL_TASKS; i++)
	{
		if(mytask[i].is_wake)
		{		// this task status is to be wake

			if(mytask[i].time_occurs)	// time_occurs is not 0
				mytask[i].time_occurs--;
		
			if(mytask[i].time_occurs == 0 && mytask[i].is_fast) {
//iputs("SysTick_Handler--02.\r\n");  //--zz debug
				
			// whether the task should be excute here
				if(mytask[i].task)
					mytask[i].task();
			}
		}
	}

//iputs("SysTick_Handler--03.\r\n");  //--zz debug
	

	ADC_START_CONVERSION();	// adc conversion start  
	
#if(ADC_KEY_PDMA_EN)
	 PDMA_TRG_ENABLE(PDMA0);  
#endif

	
/*--
	//if(dev_state != DEV_STANDBY)
		i2cSpirit();
	
	audio_L += audio_detect_L ;

	audio_R += audio_detect_R ;

	audio_detect_L = audio_detect_R = 0;
	*/
	
//iputs("weeprom--03.\r\n");  //--zz debug	
  i2cSpirit();	
	
}

 
/*-------------------------------------------------------------------------
 * @fucname:	 runTimedTask
 * @brief: 	 run timed Task that have wakeup
 * @param: 	 none
 * @return: 	 none
 * @details: 	 run timed Task that have wakeup
 *-----------------------------------------------------------------------*/
void runTimedTasks()
{
	u32 i;
	
//iputs("runTimedTasks--01.\r\n");  //--zz debug	
	
	for(i = 0; i < TOTAL_TASKS; i++)
	{
		if(mytask[i].is_wake && mytask[i].time_occurs == 0 &&  !mytask[i].is_fast)
		{	
		// whether the task can run
			mytask[i].is_wake = FALSE;		// make the task stop
			if(mytask[i].task)
					mytask[i].task();			// do the task	
		}
	}
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 taskInit
 * @brief:	 	 Init all tasks
 * @param:	 	 none
 * @return: 	 none
 * @details: 	 Init all tasks
 *-----------------------------------------------------------------------*/
void sysTaskInit()
{
	u32 i;
	for(i = 0; i< TOTAL_TASKS; i++)
	{
		mytask[i].is_fast = FALSE;
		mytask[i].is_wake= FALSE;
		mytask[i].is_accupied = FALSE;
		mytask[i].time_occurs = 0;
		mytask[i].task= NULL;
	}
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 getATask
 * @brief: 	 get a task from the task 
 * @param: 	 none
 * @return: 	 the timer index in the tasks pool
 * @details:     get the timer index in the task pool
 *-----------------------------------------------------------------------*/
Timer_ID getATask()
{
	Timer_ID id;
	for(id = 0; id< TOTAL_TASKS; id++){
		if(mytask[id].is_accupied == FALSE){
			mytask[id].is_accupied = TRUE;
			return id;
		}
	}
	return TASK_FULL;		// task pool is full
}

void taskFlush(void)
{
	Timer_ID id;
	for(id = 0; id< TOTAL_TASKS; id++){
		stopATask(id);
		deleteATask(id);
	}
			
}
/*-------------------------------------------------------------------------
 * @brief: 	 start a timed task
 * @param: 	 id:			indicate the task in the tasks pool
 			 time_occurs: 	the task sleep time
 			 is_fast: 		whether the task excute in the ISR
 			 task:		the task function address
 * @return: 	 none
 * @details: 	 start a timed task  after time_occurs become 0
 *
 *-----------------------------------------------------------------------*/
void startATask(Timer_ID id, u32 time_occurs, bool is_fast, FUNC_Timer_Task task)
{
	DISABLE_SYSTICK();	// may dealy the systick int occurs
	__nop();
	__nop();
	mytask[id].is_fast = is_fast;
	mytask[id].time_occurs = time_occurs;
	mytask[id].task = task;
	mytask[id].is_wake = TRUE;
	ENABLE_SYSTICK();
}

