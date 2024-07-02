/*-------------------------------------------------------------------------
 * @file:		SysTimerTask.h
 * @version:	V1.00	
 * @breif:	SysTimerTask header source file.  	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
#ifndef __systimertask_h
#define __systimertask_h

#include "Sysinc.h"

#define TOTAL_TASKS		10
#define TASK_FULL		0xFF
#define  ENABLE_SYSTICK()			{SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;}
#define  DISABLE_SYSTICK()			{SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;}


typedef struct {
	bool is_fast;			// whether excute in the systimer ISR 
	u8	is_wake;		// whethe the task can be waken
	bool is_accupied;
	u32 time_occurs;		// time remained to  excute the task
	FUNC_Timer_Task task;	// a pointer pointed this task fanction address
}Timer_Task;			// the timed task struct
extern Timer_Task mytask[TOTAL_TASKS];
extern volatile u32 systick_counter;
extern void sysTaskInit(void);
extern Timer_ID getATask(void);
extern void runTimedTasks(void);
extern void startATask(Timer_ID id, u32 time_occurs, bool is_fast, FUNC_Timer_Task task);
extern void sysDelayMs(u32 delay_ms);


/*-------------------------------------------------------------------------
 * @fucname:	 SysClkInit
 * @brief: 	config the sysclk interrupt every 1ms 
 * @param:  	ticks between two int
 * @return: 	none
 * @details:    to make a sys clock timer  and  make the interruption priority is the lowest 
 *-----------------------------------------------------------------------*/
static __inline void SysTickInit(u32 ticks)
{                                                              
	SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      // set reload register
	NVIC_SetPriority (SysTick_IRQn, 3);  //set Priority is lowest 
	SysTick->VAL   = 0;                                         
	SET_SYSTICK_CLKSRC(CLK_STCLK_HXT_2);
	// Enable SysTick IRQ and SysTick Timer 
	SysTick->CTRL  = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk; 
}




/*-------------------------------------------------------------------------
 * @brief: 	 stop a task	
 * @param: 	 id:		indicate the task to be stop
 * @return: 	 none
 * @details: 	 stop the task with the index of id in the tasks pool
 *-----------------------------------------------------------------------*/
static __inline void stopATask(Timer_ID id)
{
	mytask[id].is_wake = FALSE; // stop the task
	mytask[id].task = NULL;
}

/*-------------------------------------------------------------------------
 * @brief: 	 delete a task	
 * @param: 	 id:		indicate the task to be delete
 * @return: 	 none
 * @details: 	 delete the task with the index of id in the tasks pool
 *-----------------------------------------------------------------------*/
static __inline void deleteATask(Timer_ID id)
{
	mytask[id].task= NULL; 
	mytask[id].is_accupied = FALSE;
}


extern void sysDelayAMs(u32 delay_ms);     //ma 20150527  add declare sysDelayAMs
extern void sysDelayUs(u32 c);         //20150624

extern void taskFlush(void);     //ma 20150629  declare  taskFlush
#endif

