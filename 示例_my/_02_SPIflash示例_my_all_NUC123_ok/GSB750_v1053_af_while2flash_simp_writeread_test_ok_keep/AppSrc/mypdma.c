/****************************************************************************
 * @file:		pdma.c
 * @version:	V1.00
 * @breif:		pdma source file
 * @date: 	2013.4.11	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/

#include "sysinc.h"
#include "filter.h"

#if(DMAI2S == FALSE && ADPCM_FORMAT == MS_ADPCM)
#define AUDIO_DMA_BUF_SIZE	1
#endif

s32 i2s_rxbuf[AUDIO_DMA_BUF_SIZE<<1];
s32 i2s_txbuf[AUDIO_DMA_BUF_SIZE<<1];
u32 i2s_pdma_rxbuf_flag = RXBUF1;	
u32 i2s_pdma_txbuf_flag = TXBUF1;
extern s32 C_filter[];
extern u32 filter_rxbuf_flag; 

#if(ADC_KEY_PDMA_EN)
/*-------------------------------------------------------------------------
 * @fucname:	 adcDmaInit
 * @brief: 	 init adc dma 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 init adc dma, adc value is in key_dma
 *-----------------------------------------------------------------------*/
extern u16 u16ADCBuf;
void adcDmaInit()
{

    	STR_PDMA_T sParam;
	
	//ADC_PDMA_ENABLE();

	sParam.u32SrcCtrl	= PDMA_SAD_FIXED;
	sParam.u32SrcAddr	= (u32)&ADC->PDMA;
	sParam.u32DestCtrl		= PDMA_DAD_FIXED;
	sParam.u32DestAddr	= (u32)&u16ADCBuf;
	sParam.u32TransWidth = PDMA_APB_TWS_16BIT;
	sParam.u32Mode       = PDMA_MD_APB2MEM;
	sParam.u32ByteCnt    = 2;

	DrvPDMA_Init(PDMA0, &sParam);
	PDMA_SET_ADC_RX_CH(PDMA_ADC_RX_CH0); //En:Select PDMA0 as ADC_RX PDMA transfer channel
	DrvPDMA_EnableInt(PDMA0, PDMA_BLKD_INT | PDMA_TABORT_INT);
	NVIC_SetPriority (PDMA_IRQn, PDMA_INT_PRIORITY); 
	DrvADC_DisableInt();
	ADC_PDMA_ENABLE();
}
#endif

/*-------------------------------------------------------------------------
 * @fucname:	 i2s_dma_Init
 * @brief: 	 use dma channel 1 for i2s RX and dma channel 2 for i2s TX
 * @param: 	 none
 * @return: 	 none
 * @details: 	
 *-----------------------------------------------------------------------*/
void i2sDmaInit(void)
{
	STR_PDMA_T sParam;

	sParam.u32SrcCtrl    = PDMA_SAD_FIXED;	// I2S DATA IN SOURCE ADDR IS FIXED MOD 
	sParam.u32SrcAddr    = (u32)&I2S->RXFIFO;
	sParam.u32DestCtrl   = PDMA_DAD_INC;	// I2S DATA IN DESTdestination ADDR IS INC MOD
	sParam.u32DestAddr   = (u32)&i2s_rxbuf[0];
	sParam.u32TransWidth = PDMA_APB_TWS_32BIT;	// DMA DATA UINT IS 32BIT
	sParam.u32Mode       = PDMA_MD_APB2MEM;	// DMA IS TRANS FORM APB TO MEM
	sParam.u32ByteCnt    = AUDIO_DMA_BUF_SIZE*sizeof(u32); // DMA TRANS BYTE COUNT
	DrvPDMA_Init(PDMA1, &sParam);
	
	 i2s_pdma_rxbuf_flag = RXBUF1;	

	sParam.u32SrcCtrl    = PDMA_SAD_INC; // I2S DATA OUT SOURCE ADDR IS INC MOD 
	sParam.u32SrcAddr    = (u32)&i2s_txbuf[0];
	sParam.u32DestCtrl   = PDMA_DAD_FIXED; // I2S DATA OUT DESTdestination ADDR IS FIXED MOD
	sParam.u32DestAddr   = (u32)&I2S->TXFIFO;
	sParam.u32TransWidth = PDMA_APB_TWS_32BIT; // DMA DATA UINT IS 32BIT
	sParam.u32Mode       = PDMA_MD_MEM2APB;	// DMA IS TRANS FORM MEM TO APB
	sParam.u32ByteCnt    = AUDIO_DMA_BUF_SIZE*sizeof(u32); // DMA TRANS BYTE COUNT
	DrvPDMA_Init(PDMA2, &sParam);

	i2s_pdma_txbuf_flag = TXBUF1;

	
	PDMA_SET_I2S_RX_CH(PDMA_I2S_RX_CH1); //Select PDMA0 as I2S_RX PDMA transfer channel
	PDMA_SET_I2S_TX_CH(PDMA_I2S_TX_CH2); //Select PDMA3 as I2S_TX PDMA transfer channel
	DrvPDMA_EnableInt(PDMA1, PDMA_BLKD_INT | PDMA_TABORT_INT); //Enable PDMA1 transfer done and target abort interrupt
	DrvPDMA_EnableInt(PDMA2, PDMA_BLKD_INT | PDMA_TABORT_INT); //Enable PDMA2 transfer done and target abort interrupt
	NVIC_SetPriority (PDMA_IRQn, PDMA_INT_PRIORITY); 

	
}


/*-------------------------------------------------------------------------
 * @fucname:	 spiDmaInit
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void spi2I2sDmaInit()
{

}

void DmaInit(void)
{
	
	SYS_RESET_IP1(SYS_RST_PDMA);                  //En:Reset PDMA IP
	AHBCLK_ENABLE(CLK_PDMA);                  //En:Enable PDMA clock
#if(DMAI2S == TRUE)
	i2sDmaInit();
#endif
#if(ADC_KEY_PDMA_EN)
	adcDmaInit();
#endif
#if(SPI2I2S)
	spi2I2sDmaInit();
#endif
}

/*-------------------------------------------------------------------------
 * @fucname:	 PDMA_IRQHandler
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void PDMA_IRQHandler()
{
	if(PDMA_GET_CH_INTR_STS(1)){
		if(PDMA_GET_IF(PDMA1, PDMA_BLKD_INT)){

			PDMA_CLR_IF(PDMA1, PDMA_BLKD_INT);               //Clear PDMA interrupt status

			triggerFilter();

			
			if(i2s_pdma_rxbuf_flag == RXBUF2){
				PDMA_SET_DEST_ADDR(PDMA1, (u32)&i2s_rxbuf[0]); //Set PDMA1 source address
				filter_rxbuf_flag = RXBUF2;
				i2s_pdma_rxbuf_flag = RXBUF1;
			}
			else{
				PDMA_SET_DEST_ADDR(PDMA1, (u32)&i2s_rxbuf[AUDIO_DMA_BUF_SIZE]); //Set PDMA1 source address
				filter_rxbuf_flag = RXBUF1;
				i2s_pdma_rxbuf_flag = RXBUF2;
			}                  
			
		}
		else if(PDMA_GET_IF(PDMA1, PDMA_TABORT_INT)){
			PDMA_CLR_IF(PDMA1, PDMA_TABORT_INT);               //En:Clear PDMA interrupt status
			i2sDmaInit();	
		}
		
		PDMA_TRG_ENABLE(PDMA1);
		
	}

	if(PDMA_GET_CH_INTR_STS(2)){
		if(PDMA_GET_IF(PDMA2, PDMA_BLKD_INT)){

			PDMA_CLR_IF(PDMA2, PDMA_BLKD_INT);               //Clear PDMA interrupt status
			
			if(i2s_pdma_txbuf_flag == TXBUF2){
				PDMA_SET_SRC_ADDR(PDMA2, (u32)&i2s_txbuf[0]); //Set PDMA2 source address
				i2s_pdma_txbuf_flag = TXBUF1;
			}
			else{
				PDMA_SET_SRC_ADDR(PDMA2, (u32)&i2s_txbuf[AUDIO_DMA_BUF_SIZE]); //Set PDMA2 source address
				i2s_pdma_txbuf_flag = TXBUF2;
			}              
			
		}
		else if(PDMA_GET_IF(PDMA2, PDMA_TABORT_INT)){
			PDMA_CLR_IF(PDMA2, PDMA_TABORT_INT);               //En:Clear PDMA interrupt status
			i2sDmaInit();
		}

		PDMA_TRG_ENABLE(PDMA2);
	}

#if(ADC_KEY_PDMA_EN)
	if(PDMA_GET_IF(PDMA0, PDMA_BLKD_INT)){
		PDMA_CLR_IF(PDMA0, PDMA_BLKD_INT);
		adcKeyRemap(u16ADCBuf);
	}else if(PDMA_GET_IF(PDMA0, PDMA_TABORT_INT)){
		PDMA_CLR_IF(PDMA0, PDMA_TABORT_INT); // pdma target abort occurred
		adcDmaInit();	 // or do it in sysTick_Handler  and transfer those data again
	}
#endif

}


