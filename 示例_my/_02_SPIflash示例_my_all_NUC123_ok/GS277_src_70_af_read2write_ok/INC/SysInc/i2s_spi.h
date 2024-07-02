/****************************************************************************
 * @file:		i2s.h
 * @version:	v1.00	
 * @breif:		i2s header file
 * @date: 	2013.4.8
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/

#ifndef __I2S_H
#define __I2S_H

extern u8 flg_playing;
U32 spiFlashPageWrite(u32 address, u32 length, u8* databuf);
void spiFlashRead(u32 address, u32 count, u8* databuf );


// extern u32 imusicdatalen;
// extern u32 idatasum;	
// extern u32 iDataCount;
// extern u32 inowaddress;

// extern s16 sample0x;
// extern s16 sample0y;

// extern s16 presamplex;
// extern s16 presampley;
// extern s8  index0_x;
// extern s8  index0_y;
// extern s8 newindex_x;
// extern s8 newindex_y;


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


#define POWER_ON_SOUND			2//7//2    //--test
#define POWER_ON_PAIRING_SOUND	2
#define POWER_OFF_SOUND			1   //--test
#define DISCONECTED_SOUND			5
#define DD_SOUND					1
#define PAIRED_SOUND				4
#define PAIRING_SOUND				0                          //1.387s
#define SWITCH_SOUND				3
#define BATTERY_LOW_SOUND		6  
#define DEMO_SOUND					7 
#define LIVING_ROOM_SOUND   7
#define KITCHEN_SOUND             8
#define DINING_ROOM_SOUND         9

#define PARTY_ROOM_SOUND          10
#define BED_ROOM1_SOUND           11
#define BED_ROOM2_SOUND           12
#define PATIO_SOUND               13
#define OFFICE_SOUND              14
#define SALON_SOUND               15
#define CUISINE_SOUND             16
#define SALLE_A_MANGER_SOUND      17
#define SALLE_DE_JEUX_SOUND       18
#define CHAMBER_1_SOUND           19

#define CHAMBER_2_SOUND          	20
#define PATIO_FR_SOUND            21
#define BUREAU_SOUND              22
#define WOHNZIMMER_SOUND          23
#define KUCHE_SOUND               24
#define ESSZIMMER_SOUND           25
#define PARTYRAUM_SOUND           26
#define SCH1AFZIMMER1_SOUND       27 
#define SCH1AFZIMMER2_SOUND       28
#define TERRASSE_SOUND            29

#define BURO_SOUND                30
#define NUMBER_0            31
#define NUMBER_1            32
#define NUMBER_2            33
#define NUMBER_3            34
#define NUMBER_4            35
#define NUMBER_5            36
#define NUMBER_6            37
#define NUMBER_7            38
#define NUMBER_8            39

#define NUMBER_9            40
#define POINT_SOUND         41
#define VERSION_SOUND    		42
#define MCU_SOUND    				43
#define BLUETOOTH_SOUND    	44
#define SOUND_SOUND    			45
#define WIRELESS_SOUND      46
#define CORE_SOUND          47

#define TX_MULTIROOM_SOUND      48                           //ma 20150901  add new bt pairing sound and tx multiroom sound  2.681 s
#define NEW_BT_PAIRING_SOUND    49                           //  2.681 s


// #define SDD0                0
// #define POWER_ON_SOUND			    SDD0//2
// #define POWER_ON_PAIRING_SOUND	SDD0//2
// #define POWER_OFF_SOUND			    SDD0//1
// #define DISCONECTED_SOUND		   	SDD0//5
// #define DD_SOUND					      SDD0//1
// #define PAIRED_SOUND				SDD0//4
// #define PAIRING_SOUND				SDD0//0                          //1.387s
// #define SWITCH_SOUND				SDD0//3
// #define BATTERY_LOW_SOUND		SDD0//6  
// #define LIVING_ROOM_SOUND   SDD0//7
// #define KITCHEN_SOUND             SDD0//8
// #define DINING_ROOM_SOUND         SDD0//9
// #define PARTY_ROOM_SOUND          SDD0//10
// #define BED_ROOM1_SOUND           SDD0//11
// #define BED_ROOM2_SOUND           SDD0//12
// #define PATIO_SOUND               SDD0//13
// #define OFFICE_SOUND              SDD0//14
// #define SALON_SOUND               SDD0//15
// #define CUISINE_SOUND             SDD0//16
// #define SALLE_A_MANGER_SOUND      SDD0//17
// #define SALLE_DE_JEUX_SOUND       SDD0//18
// #define CHAMBER_1_SOUND           SDD0//19
// #define CHAMBER_2_SOUND          	SDD0//20
// #define PATIO_FR_SOUND            SDD0//21
// #define BUREAU_SOUND              SDD0//22
// #define WOHNZIMMER_SOUND          SDD0//23
// #define KUCHE_SOUND               SDD0//24
// #define ESSZIMMER_SOUND           SDD0//25
// #define PARTYRAUM_SOUND           SDD0//26
// #define SCH1AFZIMMER1_SOUND       SDD0//27 
// #define SCH1AFZIMMER2_SOUND       SDD0//28
// #define TERRASSE_SOUND            SDD0//29
// #define BURO_SOUND                SDD0//30
// #define NUMBER_0            SDD0//31
// #define NUMBER_1            SDD0//32
// #define NUMBER_2            SDD0//33
// #define NUMBER_3            SDD0//34
// #define NUMBER_4            SDD0//35
// #define NUMBER_5            SDD0//36
// #define NUMBER_6            SDD0//37
// #define NUMBER_7            SDD0//38
// #define NUMBER_8            SDD0//39
// #define NUMBER_9            SDD0//40
// #define POINT_SOUND         SDD0//41
// #define VERSION_SOUND    		SDD0//42
// #define MCU_SOUND    				SDD0//43
// #define BLUETOOTH_SOUND    	SDD0//44
// #define SOUND_SOUND    			SDD0//45
// #define WIRELESS_SOUND      SDD0//46
// #define CORE_SOUND          SDD0//47
// #define TX_MULTIROOM_SOUND      SDD0//48    //ma 20150901  add new bt pairing sound and tx multiroom sound  2.681 s
// #define NEW_BT_PAIRING_SOUND    SDD0//49    //  2.681 s



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
#define TOTAL_MUSIC_COUNT		50        //ma 20150616   change 20  to 43        //ma 20150619   change 43  to  46   //ma 20150901 48 to 50
                                               //ma 20150703  change 46 to 48   the flash music size  v1.05
#define RECODE_AUDIO_RATE		24000
#define OPEN_MUSIC				0x0
#define OFF_MUSIC				0x1
#define  flash_data_buf1		((s8*)&i2s_rxbuf[0])
#define  flash_data_buf2		((s8*)&i2s_txbuf[0])

/*-------------------------------------------------------------------------
 * @fucname:	 spiSendWord
 * @brief: 	 spi write indicated bit length data
 * @param: 	 ulData : data	ucLength: data bit length
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
 static __inline void spiSendWord(u32 ulData, u32 ucLength)
{   
	SPI_SET_TX_BIT_LEN(SPI1, ucLength);
	SPI_WRITE_TX0(SPI1, ulData);                           
	SPI_SET_GO(SPI1); 
	while(SPI_IS_BUSY(SPI1) != 0); 
}


#define SF_CMD_READ(addr)	{	SPI_CLR_SS0(SPI1);	\
								SPI_DISABLE_REORDER(SPI1);\
								SPI_SET_SS0(SPI1);	\
								spiSendWord(M25P80_RDDATA, 8);\
								spiSendWord(addr, 24);\
								SPI_ENABLE_REORDER(SPI1);\
							}
	
#define SF_CMD_PAGEWR(addr)	{	SPI_CLR_SS0(SPI1);	\
									SPI_DISABLE_REORDER(SPI1);\
									SPI_SET_SS0(SPI1);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI1);\
									__nop();\
									SPI_SET_SS0(SPI1);\
									spiSendWord(M25P80_PAGE_PROG, 8);\
									spiSendWord(addr, 24);\
									SPI_ENABLE_REORDER(SPI1);\
								}

#define SF_CMD_PAGEWRB(addr)	{	SPI_CLR_SS0(SPI1);	\
									SPI_DISABLE_REORDER(SPI1);\
									SPI_SET_SS0(SPI1);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI1);\
									__nop();\
									SPI_SET_SS0(SPI1);\
									spiSendWord(M25P80_PAGE_PROG, 8);\
									spiSendWord(addr, 24);\
								}

#define SF_CMD_DISWRITE()	{	SPI_CLR_SS0(SPI1);\
								SPI_DISABLE_REORDER(SPI1);\
								SPI_SET_SS0(SPI1);\
								spiSendWord(M25P80_WRDISABLE, 8);\
								SPI_CLR_SS0(SPI1);\
							}

#define SF_SET_STATUS(status) {		SPI_CLR_SS0(SPI1);	\
									SPI_DISABLE_REORDER(SPI1);\
									SPI_SET_SS0(SPI1);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI1);\
									__nop();\
									SPI_SET_SS0(SPI1);\
									spiSendWord(M25P80_WRSR, 8);\
									spiSendWord(status, 8);\
									SPI_CLR_SS0(SPI1);\
							}

#define SF_GET_STATUS()		{	SPI_CLR_SS0(SPI1);\
								SPI_DISABLE_REORDER(SPI1);\
								SPI_SET_SS0(SPI1);\
								spiSendWord(M25P80_RDSR, 8);\
								spiSendWord(0xff, 8);\
								flash_status = SPI1->RX[0];\
								SPI_CLR_SS0(SPI1);\
							}

#define SF_SECTOR_ERASE(addr)	{	SPI_CLR_SS0(SPI1);	\
									SPI_DISABLE_REORDER(SPI1);\
									SPI_SET_SS0(SPI1);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI1);\
									__nop();\
									SPI_SET_SS0(SPI1);\
									spiSendWord(M25p80_SE, 8);\
									spiSendWord(addr, 24);\
									SPI_CLR_SS0(SPI1);\
								}

#define SF_BULK_ERASE()			{	SPI_CLR_SS0(SPI1);	\
									SPI_DISABLE_REORDER(SPI1);\
									SPI_SET_SS0(SPI1);	\
									spiSendWord(M25P80_WRENABLE, 8);\
									SPI_CLR_SS0(SPI1);\
									__nop();\
									SPI_SET_SS0(SPI1);\
									spiSendWord(M25p80_BE, 8);\
									SPI_CLR_SS0(SPI1);\
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
	u32 datasize;	
	SFLASH_MUISC_INFO info;
}SFLASH_INFO;


extern EFLASH_MODE flash_mode ;
extern S32 music_size;
extern U32 audio_L, audio_R;
extern s32 audio_detect_L,audio_detect_R;
extern u32 flash_write_triggle ;
extern u32 flash_recode_stop;
extern u32 filter_bypass;



extern void I2SClkInit(u32 audio_bit_rate);
extern void i2sStart(void);
extern void playMusic(s32 index);
extern s32 doSpi2i2s(void);
extern void recodeMusic(s32 index);
extern void spiFlashInit(void);
extern void writeFlashRecodeData(void);
extern void dealFlashEvent(s32 *L, s32 *R);
extern void playSFMusicEnd(void);

/*****add version number record****/   //MA 20150615 
extern u8 BT_VERSION_NUMBER[4];
extern u8 MCU_VERSION_NUMBER[4];
extern u8 WL_VERSION_NUMBER[4];
extern u8 FLASH_VERSION_NUMBER[4];


/*****add play_music_flag****/
extern u8 play_music_flag;     //ma 20150616
/*******play version sound********/    //ma 20150615
extern void version_sound_play(void);    //ma 20150615
/*******play all sound********/    //ma 20150615
extern void play_all_sound(void);       //ma 20150615
extern u8 MUSIC_FLASH_SIZE;        //ma 20150615 
extern u8 BT_name_block[25][30];     //ma 20150616   declare bt name block 
#endif

