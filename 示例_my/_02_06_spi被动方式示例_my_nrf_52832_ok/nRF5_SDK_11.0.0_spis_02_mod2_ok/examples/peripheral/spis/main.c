/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_spis.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>

#if defined(BOARD_PCA10036) || defined(BOARD_PCA10040)
#define SPIS_CS_PIN  29 /**< SPIS CS Pin. Should be shortened with @ref SPI_CS_PIN */
#elif defined(BOARD_PCA10028)
#define SPIS_CS_PIN  4 /**< SPIS CS Pin. Should be shortened with @ref SPI_CS_PIN */
#else

#error "Example is not supported on that board."
#endif

#define SPIS_INSTANCE 1 /**< SPIS instance index. */
static const nrf_drv_spis_t spis = NRF_DRV_SPIS_INSTANCE(SPIS_INSTANCE);/**< SPIS instance. */

//#define TEST_STRING "Nordic"
#define TEST_STRING "abcdef"
static uint8_t       m_tx_buf[] = TEST_STRING;           /**< TX buffer. */
static uint8_t       m_rx_buf[sizeof(TEST_STRING)+1];    /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */

static volatile bool spis_xfer_done; /**< Flag used to indicate that SPIS instance completed the transfer. */

/**
 * @brief SPIS user event handler.
 *
 * @param event
 */
void spis_event_handler(nrf_drv_spis_event_t event)
{
    if (event.evt_type == NRF_DRV_SPIS_XFER_DONE)
    {
        spis_xfer_done = true;
        NRF_LOG_PRINTF(" Transfer completed. Received: %s\n",m_rx_buf);
    }
}

int main(void)
{

    NRF_LOG_INIT();
    NRF_LOG_PRINTF("SPIS example\n");

    nrf_drv_spis_config_t spis_config = NRF_DRV_SPIS_DEFAULT_CONFIG(SPIS_INSTANCE);

   
    spis_config.csn_pin               = 12;
    spis_config.sck_pin               = 13;
    spis_config.miso_pin              = 15;
    spis_config.mosi_pin              = 14;
   
    
    nrf_drv_spis_init(&spis, &spis_config, spis_event_handler);

    while(1)
    {
        memset(m_rx_buf, 0, m_length);
        spis_xfer_done = false;

        nrf_drv_spis_buffers_set(&spis, m_tx_buf, m_length, m_rx_buf, m_length);

        while (!spis_xfer_done)
        {
            __WFE();
        }

    }
    
}
