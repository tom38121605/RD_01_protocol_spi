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

//--enum PT2314_INPUT_CHANNEL dev_input = LINEIN; 

enum CS8422_INPUT_CHANNEL dev_input = BLUETOOTH;

Timer_ID pt2314_id_CIC = TASK_FULL; // for change input channel 

u8 iselsource = BLUETOOTH;

enum LED_COLOR input_channel_color[15] = 	{ 
										  BLUETOOTH_COLOR,
										  WHITE_COLOR,
										  PURPLE_COLOR
 										};


//U32 	total_input_channel = 2;
U32 	total_input_channel = 4;
