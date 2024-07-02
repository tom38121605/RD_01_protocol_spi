/**
 ****************************************************************************************
 *
 * @file main_task.c
 *
 * @brief Main task of the peripherals demo application
 *
 * Copyright (C) 2015. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include <osal.h>
#include <resmgmt.h>
#include "app/menu.h"
#include "app/tasklist.h"
#include "platform_devices.h"
#include "serial_console.h"
#include "osal.h"

//#include "common.h"
//#include "at45db011d.h"



static inline void readline(char *buf, size_t size)
{
        int c;

        do {
                c = getchar();
                *buf = c;
                buf++;
                size--;
        } while (c != '\n' && c != '\r' && size > 1); // wait for CR/LF or reserve 1 char for \0

        /* make sure it's null-terminated */
        *buf = '\0';
}

/*void decaspi_task_func(void *param)   //--debuglog
{
        int u2;

        for(;;)
        {
            printf( "12345" );   //--debuglog
            //vTaskDelay( 2000 / portTICK_RATE_MS );

            for( u2 = 0; u2 < 1000; u2++ )
            {
                 ;
            }
        }
}
*/

void da_delay_count(uint32_t volatile number )
{
    while(number != 0)
    {
        number--;
    }
}

void main_task_func(void *param)
{
//        console_init(SERIAL1, 256);
//
//        printf( "\r\n12345\r\n" );   //--debuglog
//        da_delay_count(100000);
//
//        for(;;)
//        {
//                printf( "\r\ni'm maintask\r\n" );   //--debuglog
//                da_delay_count(500000);
//        }
}


