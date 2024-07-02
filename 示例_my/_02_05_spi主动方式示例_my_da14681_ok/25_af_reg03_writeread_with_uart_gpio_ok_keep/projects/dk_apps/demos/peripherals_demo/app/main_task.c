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

spi_device dev;
void dwt_readfromdevice( uint16_t recordNumber, uint16_t index, uint32_t length, uint8_t *buffer );
void dwt_writetodevice( uint16_t recordNumber, uint16_t index, uint32_t length, const uint8_t *buffer );
uint16_t writetospi( uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodylength, const uint8_t *bodyBuffer);


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
        char s[64];
        OS_TASK task_h;
        uint32_t i;
        uint8_t ibuf[4]={0};
        uint8_t ibuf2[4]={0};

        resource_init();

        console_init(SERIAL1, 256);

        app_tasklist_create();


        //--test decaspi
        hw_gpio_configure_pin(3, 6, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_EN, 1);
        hw_gpio_configure_pin(3, 7, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_DO, 0);
        hw_gpio_configure_pin(4, 0, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_DI, 0);
        hw_gpio_configure_pin(4, 1, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_CLK, 0);

        dev = ad_spi_open(DW1000SPI);


        printf( "\r\n12345\r\n" );   //--debuglog
        //da_delay_count(100000);

        hw_gpio_configure_pin(4, 6, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO, 0);

        //OS_TASK_DELETE( NULL );

        for(;;)
        {
                //app_menu_draw();
                //readline(s, sizeof(s));
                //app_menu_parse_selection(s);

                ad_spi_device_acquire(dev);

                printf( "\r\ni'm maintask\r\n" );   //--debuglog
                da_delay_count(20000);

                //----------------------read devid ---------------------------
                dwt_readfromdevice(0x00,0,4,ibuf);
                printf( "\r\nreg00: \r\n" );    //--debuglog
                printf( "%X\r\n", ibuf[0] );    //--debuglog
                printf( "%X\r\n", ibuf[1] );    //--debuglog
                printf( "%X\r\n", ibuf[2] );    //--debuglog
                printf( "%X\r\n", ibuf[3] );    //--debuglog
                printf( "\r\n " );    //--debuglog

                da_delay_count(20000);


                //----------------------write & read reg03 ---------------------------


                ibuf2[0]=0x18;
                ibuf2[1]=0x18;
                ibuf2[2]=0x18;
                ibuf2[3]=0x18;

                dwt_writetodevice(0x03,0,4,ibuf2);
                da_delay_count(20000);


                dwt_readfromdevice(0x03,0,4,ibuf);
                printf( "\r\nreg03: \r\n" );    //--debuglog
                printf( "%X\r\n", ibuf[0] );    //--debuglog
                printf( "%X\r\n", ibuf[1] );    //--debuglog
                printf( "%X\r\n", ibuf[2] );    //--debuglog
                printf( "%X\r\n", ibuf[3] );    //--debuglog
                printf( " \r\n" );    //--debuglog
                da_delay_count(20000);


                ibuf2[0]=0x28;
                ibuf2[1]=0x28;
                ibuf2[2]=0x28;
                ibuf2[3]=0x28;
                dwt_writetodevice(0x03,0,4,ibuf2);
                da_delay_count(20000);

                dwt_readfromdevice(0x03,0,4,ibuf);
                printf( "\r\nreg03: \r\n" );    //--debuglog
                printf( "%X\r\n", ibuf[0] );    //--debuglog
                printf( "%X\r\n", ibuf[1] );    //--debuglog
                printf( "%X\r\n", ibuf[2] );    //--debuglog
                printf( "%X\r\n", ibuf[3] );    //--debuglog
                printf( "\r\n " );    //--debuglog
                da_delay_count(20000);

//                //----------------------write & read reg03 --end-------------------------


                ad_spi_device_release(dev);

                hw_gpio_set_inactive(4, 6);
                da_delay_count(100000);

                hw_gpio_set_active(4, 6);
                da_delay_count(100000);


                //--vTaskDelay( 50);
        }

        ad_spi_close(dev);

}


//================================================================================================


void dwt_readfromdevice( uint16_t recordNumber, uint16_t index, uint32_t length, uint8_t *buffer )
{
    uint8_t header[3] ;
    int cnt = 0;

    // get header
    if (index == 0)
    {
        header[cnt++] = (uint8_t) recordNumber ;
    }
    else
    {
        header[cnt++] = (uint8_t)(0x40 | recordNumber) ;

        if (index <= 127)
        {
            header[cnt++] = (uint8_t) index ;
        }
        else
        {
            header[cnt++] = 0x80 | (uint8_t)(index) ;
            header[cnt++] =  (uint8_t) (index >> 7) ;
        }
    }

    //readfromspi(cnt, header, length, buffer);
    ad_spi_transact(dev, header, cnt, buffer, length);

//    ad_spi_write(dev, header, cnt);
//    ad_spi_read(dev, buffer, length);
}

void dwt_writetodevice( uint16_t recordNumber, uint16_t index, uint32_t length, const uint8_t *buffer )
{
    uint8_t header[3] ;
    int   cnt = 0;

    if (index == 0)
    {
        header[cnt++] = 0x80 | recordNumber ;
    }
    else
    {
        header[cnt++] = 0xC0 | recordNumber ;

        if (index <= 127)
        {
            header[cnt++] = (uint8_t)index ;
        }
        else
        {
            header[cnt++] = 0x80 | (uint8_t)(index) ;
            header[cnt++] =  (uint8_t) (index >> 7) ;
        }
    }

    //writetospi(cnt,header,length,buffer);
    ad_spi_transact_write(dev, header, cnt, buffer, length);
    //ad_spi_transact(dev, header, cnt, buffer, length);

    //ad_spi_write(dev, header, cnt);
    //ad_spi_write(dev, buffer, length);

}


uint16_t writetospi( uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodylength, const uint8_t *bodyBuffer)
{
        #define LEN1  20

        uint8_t idatabuf[LEN1]={0};
        uint8_t itempbuf[LEN1]={0};

        uint8_t * p1;
        uint32_t idatalength=0;

        memset(idatabuf, 0, LEN1);
        memset(itempbuf, 0, LEN1);


  //decaIrqStatus_t  stat ;
  //stat = decamutexon();


        p1=idatabuf;
        memcpy(p1,headerBuffer, headerLength);
        p1 += headerLength;
        memcpy(p1,bodyBuffer,bodylength);

        idatalength= headerLength + bodylength;

//        spi_xfer_done = false;
//        nrf_drv_spi_transfer(&spi, idatabuf, idatalength, itempbuf, idatalength);
//        while(!spi_xfer_done) ;
        //ad_spi_transact_write(dev, header, cnt, buffer, length);
        ad_spi_write(dev, idatabuf, idatalength);

        //decamutexoff(stat) ;

        return 0;
}

