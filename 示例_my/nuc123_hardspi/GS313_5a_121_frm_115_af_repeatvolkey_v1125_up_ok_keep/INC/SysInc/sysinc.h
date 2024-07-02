/****************************************************************************
 * @file:		sysinc.h
 * @version:	V1.00	
 * @breif:	sysinc header file	
 * @date: 	2013/02/21			
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __sysinc_h
#define __sysinc_h


#include "NUC123Series.h"
#include "typedef.h"

#include "uart.h"
#include "SysTimerTask.h"
#include "appKey.h"
#include "power.h"
#include "adcKey.h" 

#include "i2cSys.h"
//#include "pt2314.h"
#include "cs8422.h"

#include "led.h"
#include "config.h"
#include "i2s_spi.h"
#include "sta339.h"

#include "dwhp83.h"

#include "74hc4096.h"

#include "datalog.h"

#include "deca.h"
#include "deca_types.h"
#include "deca_param_types.h"
#include "deca_regs.h"
#include "deca_device_api.h"


/*
--#include "typedef.h"
--#include "config.h"

#include "debug.h" 

--#include "SysTimerTask.h"
--#include "led.h"
--#include "adcKey.h" 
--#include "i2cSys.h"
--#include "irKey.h"
--#include "appKey.h"
--#include "pt2314.h"
--#include "power.h"

#include "seW58.h"
#include "test.h"

--#include "i2s_spi.h"

#include "mypdma.h"

--#include "sta339.h"

--#include "cs8422.h"
#include "csr_module.h"

--#include "uart.h"

//#include "gsensor.h"
--#include "dwhp83.h"
#include "pcm5121.h"
*/

// prevent all of exceptions with configurable priority means disable all irq and systick interrupt
#define ENTER_SECTION(status) 		{status = __get_PRIMASK(); __set_PRIMASK(1);}

// resume the status, the two MACRO should used in pairs
#define OUT_SECTION(status)		{ __set_PRIMASK(status);}


#endif


