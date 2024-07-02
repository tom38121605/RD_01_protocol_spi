/*-------------------------------------------------------------------------
 * @file:		config.c
 * @version:	V1.00	
 * @breif:	Config C source file. Config the mcu and other chips on the board	
 * @date: 	2013/2/21	
 * 
 * @note 		
 *  Copyright (C) 2013 Globla silicon . All rights reserved.
 * 
 *------------------------------------------------------------------------*/
#include "sysinc.h"
#define 	BDRATE	57600

enum SYSERROR sys_error = NONE_SYS_ERROR;
enum DEVSTATE dev_state = DEV_START;		// dev status
enum PT2314_INPUT_CHANNEL dev_input = LINEIN; 
enum LED_COLOR input_channel_color[15] = 	{ 
										  BLUETOOTH_COLOR,
										  WHITE_COLOR,
										  BLUETOOTH_COLOR
 										};


U32 	total_input_channel = 3;

