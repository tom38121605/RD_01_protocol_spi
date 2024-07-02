/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief Peripherals demo application
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
/* Standard includes. */
#include <string.h>
#include <stdio.h>

#include <platform_devices.h>
#include "osal.h"
#include "hw_gpio.h"
#include "sys_clock_mgr.h"
#include "sys_power_mgr.h"
#include "sys_watchdog.h"
#include "demos/common.h"
#include "ad_uart.h"


#include "serial_console.h"

spi_device dev;
void dwt_readfromdevice( uint16_t recordNumber, uint16_t index, uint32_t length, uint8_t *buffer );
void dwt_writetodevice( uint16_t recordNumber, uint16_t index, uint32_t length, const uint8_t *buffer );
uint16_t writetospi( uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodylength, const uint8_t *bodyBuffer);



/* Task priorities */
#define mainTEMPLATE_TASK_PRIORITY		( OS_TASK_PRIORITY_NORMAL )

/* The rate at which data is template task counter is incremented. */
#define mainCOUNTER_FREQUENCY_MS                ( 200 / portTICK_PERIOD_MS )

#if (dg_configTRACK_OS_HEAP == 1)
/*
 * ConstantsVariables used for Tasks Stack and OS Heap tracking
 * Declared global to avoid IDLE stack Overflows
 */
#define mainMAX_NB_OF_TASKS           10
#define mainMIN_STACK_GUARD_SIZE      8 /* words */
#define mainTOTAL_HEAP_SIZE_GUARD     64 /*bytes */

TaskStatus_t pxTaskStatusArray[mainMAX_NB_OF_TASKS];
uint32_t ulTotalRunTime;
#endif /* (dg_configTRACK_OS_HEAP == 1) */

/* The configCHECK_FOR_STACK_OVERFLOW setting in FreeRTOSConifg can be used to
check task stacks for overflows.  It does not however check the stack used by
interrupts.  This demo has a simple addition that will also check the stack used
by interrupts if mainCHECK_INTERRUPT_STACK is set to 1.  Note that this check is
only performed from the tick hook function (which runs in an interrupt context).
It is a good debugging aid - but won't catch interrupt stack problems until the
tick interrupt next executes. */
//#define mainCHECK_INTERRUPT_STACK			1
#if mainCHECK_INTERRUPT_STACK == 1
	const unsigned char ucExpectedInterruptStackValues[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
#endif

#if dg_configUSE_WDOG
INITIALISED_PRIVILEGED_DATA int8_t idle_task_wdog_id = -1;
#endif

/*
 * Perform any application specific hardware configuration.  The clocks,
 * memory, etc. are configured before main() is called.
 */
static void prvSetupHardware( void );
/*
 * Task functions .
 */
void main_task_func(void *param);

static OS_TASK xHandle;

/*
 * Initialize demos.
 *
 */
static void init_demos(void)
{
#if CFG_DEMO_AD_GPADC

        demo_ad_gpadc_init();
#endif // CFG_DEMO_AD_GPADC
}


extern void da_delay_count(uint32_t volatile number );

static void system_init( void *pvParameters )
{

        cm_sys_clk_init(sysclk_XTAL16M);
        cm_apb_set_clock_divider(apb_div1);
        cm_ahb_set_clock_divider(ahb_div1);
        cm_lp_clk_init();


        uint8_t ibuf[4]={0};
        uint8_t ibuf2[4]={0};

        sys_watchdog_init();


        //prvSetupHardware();

        //--test---prvSetupHardware
        #define UART_TX_PORT    HW_GPIO_PORT_1
        #define UART_TX_PIN     HW_GPIO_PIN_3
        #define UART_RX_PORT    HW_GPIO_PORT_2
        #define UART_RX_PIN     HW_GPIO_PIN_3

        hw_gpio_configure_pin(UART_RX_PORT, UART_RX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO, 1);
        hw_gpio_set_pin_function(UART_TX_PORT, UART_TX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_UART_TX);
        hw_gpio_set_pin_function(UART_RX_PORT, UART_RX_PIN, HW_GPIO_MODE_INPUT, HW_GPIO_FUNC_UART_RX);


        pm_system_init(periph_setup);

        //--test--prvSetupHardware--end---

        hw_gpio_configure_pin(4, 6, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO, 0);


        //--from main task
        console_init(SERIAL1, 256);

        //printf( "\r\n12345\r\n" );   //--debuglog
        //da_delay_count(10000);


        //--------dw1000----spi---new test----------

        //app_tasklist_create();

        //--test decaspi
        hw_gpio_configure_pin(3, 6, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_EN, 1);
        hw_gpio_configure_pin(3, 7, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_DO, 0);
        hw_gpio_configure_pin(4, 0, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_DI, 0);
        hw_gpio_configure_pin(4, 1, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_SPI_CLK, 0);

        dev = ad_spi_open(DW1000SPI);


        for(;;)
        {

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


                //hw_gpio_toggle(4,6);

//                hw_gpio_set_active(4, 6);
//                da_delay_count(5000);
//                hw_gpio_set_inactive(4, 6);
        }

//        OS_TASK_CREATE("main", main_task_func, NULL, 768, 1, task_h);
//        OS_TASK_DELETE( xHandle );
}

/**
 * @brief Template main creates a Template task
 */
int main( void )
{
        OS_BASE_TYPE status;

        cm_clk_init_low_level();                            /* Basic clock initializations. */


        status = OS_TASK_CREATE("SysInit",              /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                        system_init,                    /* The System Initialization task. */
                        ( void * ) 0,                   /* The parameter passed to the task. */
                        2 * configMINIMAL_STACK_SIZE * OS_STACK_WORD_SIZE,
                                                        /* The size of the stack to allocate to the task. */
                        //OS_TASK_PRIORITY_HIGHEST,       /* The priority assigned to the task. */
                        1,       /* The priority assigned to the task. */
                        xHandle );                      /* The task handle is not required, so NULL is passed. */
        OS_ASSERT(status == OS_TASK_CREATE_SUCCESS);

        vTaskStartScheduler();

        for( ;; );
}


static void prvSetupHardware( void )
{
;
////--test---
//       #define UART_TX_PORT    HW_GPIO_PORT_1
//       #define UART_TX_PIN     HW_GPIO_PIN_3
//       #define UART_RX_PORT    HW_GPIO_PORT_2
//       #define UART_RX_PIN     HW_GPIO_PIN_3
//
//        hw_gpio_configure_pin(UART_RX_PORT, UART_RX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO, 1);
//
//        hw_gpio_set_pin_function(UART_TX_PORT, UART_TX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_UART_TX);
//        hw_gpio_set_pin_function(UART_RX_PORT, UART_RX_PIN, HW_GPIO_MODE_INPUT, HW_GPIO_FUNC_UART_RX);
//
////--test---end---
//
//        pm_system_init(periph_setup);
}


/**
 * @brief Malloc fail hook
 */
void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to OS_MALLOC() fails.
	OS_MALLOC() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to OS_MALLOC() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */

	taskDISABLE_INTERRUPTS();
	for( ;; );
}


/**
 * @brief Application idle task hook
 */
void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call OS_QUEUE_GET() with a block time
	specified, or call OS_DELAY()).  If the application makes use of the
	OS_TASK_DELETE() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */

#if (dg_configTRACK_OS_HEAP == 1)
        OS_BASE_TYPE i = 0;
        OS_BASE_TYPE uxMinimumEverFreeHeapSize;

        // Generate raw status information about each task.
        UBaseType_t uxNbOfTaskEntries = uxTaskGetSystemState(pxTaskStatusArray,
                                                        mainMAX_NB_OF_TASKS, &ulTotalRunTime);

        for (i = 0; i < uxNbOfTaskEntries; i++) {
                /* Check Free Stack*/
                OS_BASE_TYPE uxStackHighWaterMark;

                uxStackHighWaterMark = uxTaskGetStackHighWaterMark(pxTaskStatusArray[i].xHandle);
                OS_ASSERT(uxStackHighWaterMark >= mainMIN_STACK_GUARD_SIZE);
        }

        /* Check Minimum Ever Free Heap against defined guard. */
        uxMinimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
        OS_ASSERT(uxMinimumEverFreeHeapSize >= mainTOTAL_HEAP_SIZE_GUARD);
#endif /* (dg_configTRACK_OS_HEAP == 1) */

#if dg_configUSE_WDOG
        sys_watchdog_notify(idle_task_wdog_id);
#endif
}

/**
 * @brief Application stack overflow hook
 */
void vApplicationStackOverflowHook( OS_TASK pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

/**
 * @brief Application tick hook
 */
void vApplicationTickHook( void )
{
#if mainCHECK_INTERRUPT_STACK == 1
extern unsigned long _pvHeapStart[];

	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */

	/* Manually check the last few bytes of the interrupt stack to check they
	have not been overwritten.  Note - the task stacks are automatically
	checked for overflow if configCHECK_FOR_STACK_OVERFLOW is set to 1 or 2
	in FreeRTOSConifg.h, but the interrupt stack is not. */
	OS_ASSERT( memcmp( ( void * ) _pvHeapStart, ucExpectedInterruptStackValues, sizeof( ucExpectedInterruptStackValues ) ) == 0U );
#endif /* mainCHECK_INTERRUPT_STACK */
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


