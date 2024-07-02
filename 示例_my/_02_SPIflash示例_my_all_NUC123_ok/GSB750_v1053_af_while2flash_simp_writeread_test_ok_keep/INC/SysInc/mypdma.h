/****************************************************************************
 * @file:		pdma.h		
 * @version:	v1.00
 * @breif:		pdma header file
 * @date: 	2013.4.12
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/

#ifndef __MYPDMA_H
#define  __MYPDMA_H

#include "sysinc.h"

#define PDMA_INT_PRIORITY 0
#define RXBUF1	1
#define RXBUF2	0
#define TXBUF1	1
#define TXBUF2	0
#define AUDIO_DMA_BUF_SIZE	48 // shoule be even
#define SPI2I2S	TRUE


/*-------------------------------------------------------------------------
 * @fucname:	 filterDmaSetSrc
 * @brief: 	 set filter dma src addr and dest addr
 * @param: 	 filter dma src addr
 * @return: 	 none
 * @details: 	 none
 *-----------------------------------------------------------------------*/
static __inline void filterDmaSetSrc(volatile s32 src[])
{
	PDMA3->SAR = (u32)&src[1];
	PDMA3->DAR = (u32)&src[0];
}

extern void adcDmaInit(void);
extern void DmaInit(void);
#endif


