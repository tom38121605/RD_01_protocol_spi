/*-------------------------------------------------------------------------
 * @file:		config.c
 * @version:	V1.00	
 * @breif:	Config C source file. Config the mcu and other chips on the board	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013 Global Silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
#include "sysinc.h"

enum SYSERROR sys_error = NONE_SYS_ERROR;
enum DEVSTATE dev_state = DEV_STANDBY;		// dev status
enum CS8422_INPUT_CHANNEL dev_input = BLUETOOTH;
enum LED_COLOR input_channel_color[4] = 	{
										  BLUETOOTH_COLOR,
										  PURPLE_COLOR,
										  WHITE_COLOR,
										  YELLOW_COLOR,
 										};


U32 	total_input_channel = 4;

// __ASM volatile void __nop()        //ma 20150629 
// {
// 	nop
// }


