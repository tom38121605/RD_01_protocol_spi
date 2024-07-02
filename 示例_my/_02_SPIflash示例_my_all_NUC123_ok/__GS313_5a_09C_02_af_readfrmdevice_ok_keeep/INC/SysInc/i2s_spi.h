/****************************************************************************
 * @file:		i2s.h
 * @version:	v1.00	
 * @breif:		i2s header file
 * @date: 	2013.4.8
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/

#ifndef __I2S_H
#define __I2S_H

/*================= i2s congfiguration ====================== */
#define MS_ADPCM			0x0002
#define IMA_ADPCM			0x0011
#define AUDIO_RATE			48000
#define AUDIO_BIT			32
#define AUDIO_CHANNELS 		2
#define ADPCM_FORMAT		MS_ADPCM
#define COFE_COUNT_OFFSET			0X28
#define COFE_OFFSET			0X2A
#define DATA_BSIZE_OFFSET			0X20
#define SAMPLES_PBLOCK_OFFSET		0X26
#define DATA_BLOCK_OFFSET			0X5A
#define DATA_SIZE_OFFSET			0X56
#define POWER_ON_SOUND			6
#define POWER_ON_PAIRING_SOUND	6
#define POWER_OFF_SOUND			5
#define DISCONECTED_SOUND			2
#define DD_SOUND					1
#define PAIRED_SOUND				3
#define PAIRING_SOUND				4
#define BATTERY_LOW_SOUND			0
#define DEMO_SOUND					7
#define I2S_RX_FIFO_COUNT(c)	{c = ((I2S->CON & (0X7 << 12)) >> 12)+1;} 	

/*================= spi congfiguration ================== */
#define M25P80_WRENABLE		0X06
#define M25P80_WRDISABLE		0X04
#define M25P80_PAGE_PROG		0X02
#define M25P80_RDSTATUS			0X05
#define M25P80_RDSR				0X05
#define M25P80_WRSR				0X01
#define M25P80_RDDATA			0X03
#define M25P80_FRDDATA			0X0B	
#define M25p80_SE				0xD8
#define M25p80_BE				0xC7

/*================== flash music configuration =================*/
#define TOTAL_MUSIC_COUNT		20
#define RECODE_AUDIO_RATE		24000
#define OPEN_MUSIC				0x0
#define OFF_MUSIC				0x1
#define  flash_data_buf1		((s8*)&i2s_rxbuf[0])
#define  flash_data_buf2		((s8*)&i2s_txbuf[0])

void spiFlashRead(u32 address, u32 count, u8* databuf );



/*-------------------------------------------------------------------------
 * @fucname:	 spiSendWord
 * @brief: 	 spi write indicated bit length data
 * @param: 	 ulData : data	ucLength: data bit length
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
 static __inline void spiSendWord(u32 ulData, u32 ucLength)
{   
	SPI_SET_TX_BIT_LEN(SPI2, ucLength);
	SPI_WRITE_TX0(SPI2, ulData);                           
	SPI_SET_GO(SPI2); 
	while(SPI_IS_BUSY(SPI2) != 0); 
}


#define SF_CMD_READ(addr)	{	SPI_CLR_SS0(SPI2);	\
								SPI_DISABLE_REORDER(SPI2);\
								SPI_SET_SS0(SPI2);	\
								spiSendWord(M25P80_RDDATA, 8);\
								spiSendWord(addr, 24);\
								SPI_ENABLE_REORDER(SPI2);\
							}
	
#define SF_CMD_PAGEWR(addr)	{	SPI_CLR_SS0(SPI2);	\
									SPI_DISABLE_REORDER(SPI2);\
									SPI_SET_SS0(SPI2);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI2);\
									__nop();\
									SPI_SET_SS0(SPI2);\
									spiSendWord(M25P80_PAGE_PROG, 8);\
									spiSendWord(addr, 24);\
									SPI_ENABLE_REORDER(SPI2);\
								}

#define SF_CMD_PAGEWRB(addr)	{	SPI_CLR_SS0(SPI2);	\
									SPI_DISABLE_REORDER(SPI2);\
									SPI_SET_SS0(SPI2);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI2);\
									__nop();\
									SPI_SET_SS0(SPI2);\
									spiSendWord(M25P80_PAGE_PROG, 8);\
									spiSendWord(addr, 24);\
								}

#define SF_CMD_DISWRITE()	{	SPI_CLR_SS0(SPI2);\
								SPI_DISABLE_REORDER(SPI2);\
								SPI_SET_SS0(SPI2);\
								spiSendWord(M25P80_WRDISABLE, 8);\
								SPI_CLR_SS0(SPI2);\
							}

#define SF_SET_STATUS(status) {		SPI_CLR_SS0(SPI2);	\
									SPI_DISABLE_REORDER(SPI2);\
									SPI_SET_SS0(SPI2);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI2);\
									__nop();\
									SPI_SET_SS0(SPI2);\
									spiSendWord(M25P80_WRSR, 8);\
									spiSendWord(status, 8);\
									SPI_CLR_SS0(SPI2);\
							}

#define SF_GET_STATUS()		{	SPI_CLR_SS0(SPI2);\
								SPI_DISABLE_REORDER(SPI2);\
								SPI_SET_SS0(SPI2);\
								spiSendWord(M25P80_RDSR, 8);\
								spiSendWord(0xff, 8);\
								flash_status = SPI2->RX[0];\
								SPI_CLR_SS0(SPI2);\
							}

#define SF_SECTOR_ERASE(addr)	{	SPI_CLR_SS0(SPI2);	\
									SPI_DISABLE_REORDER(SPI2);\
									SPI_SET_SS0(SPI2);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI2);\
									__nop();\
									SPI_SET_SS0(SPI2);\
									spiSendWord(M25p80_SE, 8);\
									spiSendWord(addr, 24);\
									SPI_CLR_SS0(SPI2);\
								}

#define SF_BULK_ERASE()			{	SPI_CLR_SS0(SPI2);	\
									SPI_DISABLE_REORDER(SPI2);\
									SPI_SET_SS0(SPI2);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI2);\
									__nop();\
									SPI_SET_SS0(SPI2);\
									spiSendWord(M25p80_BE, 8);\
									SPI_CLR_SS0(SPI2);\
								}

typedef enum Eflash_mode{
	FLASH_CONFIG,
	FLASH_PLAY_MUSIC,
	FLASH_RECODE_MUSIC
}EFLASH_MODE;

typedef struct music_data_zu{
	U32	count;
	u32 address[TOTAL_MUSIC_COUNT];
}SFLASH_MUISC_INFO;

typedef struct Sflash_info{
	u32 falsh_sn;
	SFLASH_MUISC_INFO info;
}SFLASH_INFO;


extern EFLASH_MODE flash_mode ;
extern S32 music_size;
extern U32 audio_L, audio_R;
extern s32 audio_detect_L,audio_detect_R;
extern u32 flash_write_triggle ;
extern u32 flash_recode_stop;
extern u32 filter_bypass;
extern u32 flash_music_index;
extern volatile u8 flash_status;

extern void I2SClkInit(u32 audio_bit_rate);
extern void i2sStart(void);
extern void playMusic(s32 index);
extern s32 doSpi2i2s(void);
extern void recodeMusic(s32 index);
extern void spiFlashInit(void);
extern void writeFlashRecodeData(void);
extern void dealFlashEvent(s32 *L, s32 *R);
extern void playSFMusicEnd(void);
extern U32 spiFlashPageWrite(u32 address, u32 length, u8* databuf);


u32 dwt_readdevid(void);
u32 dwt_read32bitoffsetreg(u16 regFileID, u16 regOffset);
void dwt_readfromdevice( u16 recordNumber, u16 index, u32 length, u8 *buffer );
int readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer);
void spiFlashRead2(u32 address, u32 count, u8* databuf );

void dwt_writetodevice( u16 recordNumber, u16 index, u32 length, const u8 *buffer );
u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer);
void spiFlashWrite2(u32 address, u32 count, u8* databuf );

void reset_DW1000(void);
void spi_set_rate_low(void);
void spi_set_rate_high(void);


#endif

