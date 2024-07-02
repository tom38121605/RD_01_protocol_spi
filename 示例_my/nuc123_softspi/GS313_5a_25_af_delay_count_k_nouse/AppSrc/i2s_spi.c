/****************************************************************************
 * @file:		i2s.c
 * @version:	V1.00
 * @breif:		i2s source file
 * @date: 	2013.4.8
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/


#include "sysinc.h"
#include "filter.h"

//extern s32 i2s_rxbuf[];
//extern s32 i2s_txbuf[];
extern u32 filter_bypass;
extern FUNC_Timer_Task adcKeySubHook;

EFLASH_MODE flash_mode = FLASH_CONFIG;
u32 flash_music_index = 0;
S32 music_size = 0;
SFLASH_INFO sflash_info_cache;
s32 audio_detect_L = 0,audio_detect_R = 0;
u32 flash_program_address = 0;
u32 flash_data_buf_mux = 1;
volatile u8 flash_status = 0;
u32 flash_write_triggle = 0;
u32 flash_recode_stop = FALSE;


volatile static s32 recode_index = 0; //used in i2s 48k interrupt
volatile static s32 pre_Rdata_encode = 0, pre_Ldata_encode = 0; // used in adpcm encode
static s32 pre_Rdata_decode = 0, pre_Ldata_decode = 0;
static s32 adpcm_step_size_index_L_encode = 0;
static s32 adpcm_step_size_index_R_encode = 0;
static s32 adpcm_step_size_index_L_decode = 0;
static s32 adpcm_step_size_index_R_decode = 0;

// adpcm Chunk, a block is 256 bytes, contain 250 encode byte, 6 control bytes
typedef struct adpcm_block{
	s16	Ldata;	// 16 bits original left channel data
	s16 	Rdata;	// 16 bits original right channel data
	u8	Lindex;	// index in step size table
	u8	Rindex;	// index in step size table
}ADPCMCHUNK;
volatile static u8 chuck_index = 0;

#if(ADPCM_FORMAT == IMA_ADPCM)
static s32 adpcm_step_index_table[16] = {
			-1, -1, -1, -1, 2, 4, 6, 8
			-1, -1, -1, -1, 2, 4, 6, 8
		};

static u16 adpcm_step_size_table[89] = {
			  7,	8,   	9,  	10,  	11,  	12,  	13,  	14,
			16, 	17, 	19,  	21,  	23,  	25, 	28,  	31, 
			34, 	37, 	41, 	45,	50, 	55, 	60, 	66, 
			73, 	80, 	88, 	97, 	107, 	118,	130, 143, 
			157, 173, 	190, 209, 	230, 	253, 	279, 	307,
    			337, 371,	408, 449, 	494, 	544, 	598, 	658, 
    			724, 	796,	876, 963, 	1060, 1166, 1282, 1411, 
    			1552,	1707, 	1878, 	2066,	2272, 	2499, 	2749, 	3024, 
    			3327, 	3660, 	4026, 	4428, 	4871, 	5358,	5894, 	6484, 
    			7132, 	7845, 	8630, 	9493, 	10442, 	11487, 	12635, 	13899,
    			15289, 	16818, 	18500, 	20350, 	22385, 	24623, 	27086, 	29794,
    			32767
		};

#elif(ADPCM_FORMAT == MS_ADPCM)

typedef struct ms_data_block_header{
	u8 predictor[2];
	u16 delta[2];	// diff = nibble * delta; and delta is adp
	u16 sample2[2];
	u16 sample1[2];
}MS_DATA_BLOCK_HEADER;

int AdaptationTable [] = { 
  230, 230, 230, 230, 307, 409, 512, 614, 
  768, 614, 512, 409, 307, 230, 230, 230 
} ;

typedef struct {
		u16	data_block_size;
		u16	data_block_count;
		u16	cofe_num;
		s16	cofe_sets[20];
}ADPCMFILEHEADER;

static ADPCMFILEHEADER file_head;
static u32 block_sample_index;   //sample index  inside the block
static u32 samples_per_block;
static u32 data_block_size;
static s16 cofe_set[20];
volatile static Timer_ID flash_music_id = TASK_FULL;
u32 volume_back = 0x00;

#endif

/*-------------------------------------------------------------------------
 * @fucname:	 I2S_Init
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void I2SClkInit(u32 audio_rate)
{
	u32 divider;

    	
	GPIO_I2S();	//Configure I2S PINs
	GPIO_I2S_MCLK_PC12();

	SET_I2S_CLKSRC(CLK_I2S_PLL);
	APBCLK_ENABLE(CLK_I2S);			//  Enable I2S Clock
	SYS_RESET_IP2(SYS_RST_I2S);		//Reset I2S Module

	I2S_CLR_RXFIFO();
	I2S_CLR_TXFIFO();

	if(audio_rate == 48000)
		I2S_SET_MCK_DIV(3);				//  set MCLK 12.286
	else if(audio_rate == 24000)
		I2S_SET_MCK_DIV(6);				//  set MCLK 6.143
	else if(audio_rate == 12000)
		I2S_SET_MCK_DIV(12);				//  set MCLK  3.07
		
	divider = ((__HSI*10)/((audio_rate * (AUDIO_BIT / 8) * 16) * 2) + 5) / 10 - 1;
	I2S_SET_BITCLK_DIV(divider);	//  set BCLK divider

	divider = (I2S_CFG_MASTER|I2S_CFG_RX_THRES_2 | I2S_CFG_TX_THRES_1|
			I2S_CFG_EN | I2S_CFG_MCK_EN|
#if 2 == AUDIO_CHANNELS
                      I2S_CFG_CHANNEL_STEREO
#elif 1 == AUDIO_CHANNELS
                      I2S_CFG_CHANNEL_MONO_LEFT
#else
    #error invalid AUDIO_CHANNELS
#endif		
		);

	switch(AUDIO_BIT){
		case 32:
			divider |= I2S_CFG_WORDWIDTH_32;
			break;
		case 24:
			divider |= I2S_CFG_WORDWIDTH_24;
			break;
		case 16:
			divider |= I2S_CFG_WORDWIDTH_16;
			break;	
		case 8:
			divider |= I2S_CFG_WORDWIDTH_8;
			break;		
		default:
			break;
	}
	
    // Configure I2S
	I2S_ASSIGN_CONFIG(divider);

}


/*-------------------------------------------------------------------------
 * @fucname:	 i2sStart
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void i2sStart()
{
#if(DMAI2S == TRUE)
	I2S_ENABLE_RXDMA();
	PDMA_TRG_ENABLE(PDMA1);
	filterInit();
#else
	I2S_CLR_TXFIFO();
	I2S_ENABLE_TX();
	I2S_CLR_RXFIFO();
	I2S_ENABLE_RX();
	DrvI2S_EnableInt(I2S_INT_RXTH); 
	NVIC_SetPriority(I2S_IRQn, 1);
#endif
	
	//GPIO_SET_MODE(PB,13,GPIO_OUTPUT);
	//GPIO_SET_MODE(PB,8,GPIO_INPUT);
}


/*-------------------------------------------------------------------------
 * @fucname:	 SPI2_IRQHandler
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void SPI2_IRQHandler (void)
{

}


/*-------------------------------------------------------------------------
 * @fucname:	 spiFlashRead
 * @brief: 	 read bytes  form address.
 * @param: 	 address: spi address ; count: read byte count; databuf: read data put place
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void spiFlashRead(u32 address, u32 count, u8* databuf )
{
	u32 index = 0;
	
	SF_CMD_READ(address);  
	
	for( ; index < count; index++){
		spiSendWord(0xff,8);
		databuf[index] = SPI2->RX[0];
	}
	SPI_CLR_SS0(SPI2);

}


/*-------------------------------------------------------------------------
 * @fucname:	 spiFlashPageWrite
 * @brief: 	 write a page data , if write data across a page size this function will 
 			 use more than 2ms.	
 * @param: 
 * @return: 	FALL OR SUCESS
 * @details: 	
 *-----------------------------------------------------------------------*/
U32 spiFlashPageWrite(u32 address, u32 length, u8* databuf)
{
	u32 index = 0;
	
	if( length == 0 ||((address&0xfffff) < 0XE0000) || length > 256)
		return FALSE;

	
	SF_CMD_PAGEWRB(address&(~0xff));
		
	for(index = 0; index < length; index++)
		spiSendWord(databuf[index],8);	 // write the data of this page
	
	SF_CMD_DISWRITE();
	SF_GET_STATUS();
	while(flash_status&1)
		SF_GET_STATUS();
	
	return TRUE;
}


/*-------------------------------------------------------------------------
 * @fucname:	 spiFlashInit
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/

//void spiFlashInit()
//{

//	GPIO_SPI2_SS0_PB4();
//	GPIO_SPI2_CLK_PB5();
//	GPIO_SPI2_MOSI0_PB6();
//	GPIO_SPI2_MISO0_PB7();
//	
//	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
//	SPI_SET_SS0(SPI2);

//	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE0, 32);
//	SPI_SET_MSB_FIRST(SPI2);   //MSB first
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_ENABLE_BCN(SPI2); //Backward compatible
//	
//	//SPI_SET_DIVIDER(SPI2, 1); //SPIclk = sysckl/ ( (1+1)*2)  =72 MHz /4 = 18 MHZ
//	SPI_SET_DIVIDER(SPI2, 15); //SPIclk = sysckl/ ( (15+1)*2)  =72 MHz /32 = 2.25 MHZ	
//}

void spiFlashInit()
{
	//pb4,pb5,pb6  -- CS0,CLK,MO
  GPIO_SET_MODE(PB, 4, GPIO_OUTPUT);    
  GPIO_SET_MODE(PB, 5, GPIO_OUTPUT);    
  GPIO_SET_MODE(PB, 6, GPIO_OUTPUT);   
	
	//pb7  -- MI
  GPIO_SET_MODE(PB, 7, GPIO_INPUT);   
	
	//CS0置高电平
	GPIO_SET_PIN(PB4);      
	
	//CLK置低电平
	GPIO_CLR_PIN(PB5);  
}


   
void adpcmDataInit()
{
	adpcm_step_size_index_L_decode = 0;
	adpcm_step_size_index_R_decode = 0;
	pre_Rdata_decode = 0;
	pre_Ldata_decode = 0;
	adpcm_step_size_index_L_encode = 0;
	adpcm_step_size_index_R_encode = 0;
	pre_Rdata_encode = 0;
	pre_Ldata_encode = 0;
	recode_index = 0;
	flash_write_triggle = 0;
	chuck_index = 0;
	block_sample_index = 0;
}

#if(ADPCM_FORMAT == MS_ADPCM)
void readWaveHeaderInfo(u32 start_address)
{
	u32 i;

	SF_CMD_READ(start_address);
	for(i = 0; i < sizeof(file_head); i++){
		spiSendWord(0xff, 8);
		((u8*)&file_head)[i] = SPI2->RX[0];
	}
	
	samples_per_block = (file_head.data_block_size -14) +2;
	data_block_size = file_head.data_block_size;

	for(i = 0; i < 20; i++){
		cofe_set[i] = file_head.cofe_sets[i];
	}
	//start_address +=  sizeof(file_head);
	//SF_CMD_READ(start_address);
}
#endif 

/*-------------------------------------------------------------------------
 * @fucname:	 playMusic
 * @brief: 	 play a muisc store in flash
 * @param: 	 u32 index, indicate which music should be played. 
 			 Index is 0 to sflash_info_cache.info.count -1
 * @return: 	 none
 * @details: 
 *-----------------------------------------------------------------------*/
void playMusic(s32 index)
{
//	u32 read_address = 0;
//	u32 v;

////--uartdecode(index,"  playMusic: ");   //--zz debug  
//	
//	flash_music_index = index;
//	if(flash_music_index >= sflash_info_cache.info.count)
//		return;

//	flg_softmute=1;
//	
//// 	if(dev_state != DEV_TEST)
//// 	{
//// 		if(!(volume_back&0x80))
//// 			volume_back = amp_volume | 0x80;
//// 		if(index == POWER_ON_SOUND )
//// 			v = 0;
//// 		else if(index == POWER_OFF_SOUND)
//// 			v = 19;
//// 		else
//// 			v = 16;
//// 		if(index != POWER_ON_SOUND)
//// 			amp_volume = v;
//// 		
//// 		ampSetVolume(v);
//// 	}
//	
//	if(dev_state != DEV_TEST)
//	{
//		if(!(volume_back&0x80))
//			volume_back = amp_volume | 0x80;
//		
//		v = 26;
//		
//		if(index == POWER_ON_SOUND )
//			v = 30;
//		else if(index == POWER_OFF_SOUND)
//			v = 32;
//		else if(index == PAIRED_SOUND)
//			v = 28;
//		else if(index == PAIRING_SOUND)
//			v = 27;
//		else
//			v = 26; //28
//		
//// 		if(index != POWER_ON_SOUND)
//// 			amp_volume = v;
//		
//		ampSetVolume3(v);
//	}
//	
//	flash_mode = FLASH_CONFIG;


//	DWHP83Mute();
//	sysDelayMs(10);
//	
//	adpcmDataInit();

//	read_address = sflash_info_cache.info.address[index];
//	music_size = 	sflash_info_cache.info.address[index+1] -read_address;

////	if(++flash_music_index >= sflash_info_cache.info.count)
////		flash_music_index = 0; 

//#if(ADPCM_FORMAT == MS_ADPCM)
//	readWaveHeaderInfo(read_address);
//#else
//	SF_CMD_READ(read_address);
//#endif
//	filter_bypass = TRUE;
//	adcKeySubHook = NULL;
//	flash_mode = FLASH_PLAY_MUSIC;
//	
//	if(flash_music_index != PAIRING_SOUND)
//		//logEvent(Flash_audio_track1_started + flash_music_index);
//		logEvent((Log_Type)(Flash_audio_track1_started + flash_music_index));
//	
}

/*-------------------------------------------------------------------------
 * @fucname:	 spiflashInfoWrite
 * @brief: 	 write spi flash info header
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void spiflashInfoWrite()
{
	spiFlashPageWrite(0, sizeof(sflash_info_cache), (u8*)&sflash_info_cache);
}

/*-------------------------------------------------------------------------
 * @fucname:	 init_spiflash_info
 * @brief: 	 init spi flash info header
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void init_spiflash_info(void)
{	
	sflash_info_cache.falsh_sn = 0x4E534C47;
	sflash_info_cache.info.count = 1;
	sflash_info_cache.info.address[0] = 0x100;
	sflash_info_cache.info.address[1] = flash_program_address;
	
	spiflashInfoWrite();
}


/*-------------------------------------------------------------------------
 * @fucname:	 recodeMusic
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void recodeMusic(s32 index)
{

//	flash_mode = FLASH_CONFIG;
//	flash_recode_stop = FALSE; // the flash is start to be written
//	flash_program_address = 0x100;
//	flash_write_triggle = 0;

///*----------- adpcm data init -----------*/
//	adpcmDataInit();

///*------------ erase the whole flash  -------------*/
//	SF_BULK_ERASE();
//	flash_status = 1;
//	ledColorSet(YELLOW_COLOR);
//	
//	while(flash_status&1){
//		sysDelayMs(500);
//		ledColorSet(YELLOW_COLOR); //during  erasing led is purple color
//		SF_GET_STATUS();
//	}
//	
//	ledColorSet(GREEN_COLOR);	// show green data when flash erase is done
//	/*
//	I2S_DISABLE_MCK();
//	APBCLK_DISABLE(CLK_I2S);
//	I2SClkInit(RECODE_AUDIO_RATE);	// init i2s
//	i2sStart();
//	*/
//	sysDelayMs(1000);
//	BT_POWER_OFF();
//	filter_bypass = TRUE;
//	flash_mode = FLASH_RECODE_MUSIC;
}


void recodeEnd(void)
{	
	init_spiflash_info();
	SPI_CLR_SS0(SPI2);
	flash_mode = FLASH_CONFIG;
	filter_bypass = FALSE;
}

extern u32 test_mode;
void playSFMusicEnd(void)
{
//	DWHP83UnMute();
//	sysDelayMs(10);
//	
//	if(flash_music_index == POWER_ON_SOUND)
//		sysDelayMs(500);
//	
//	filter_bypass = FALSE;
//	SPI_CLR_SS0(SPI2);
//	//if(dev_input == BLUETOOTH)
//	//	BT_POWER_ON();

////--iputs("playSFMusicEnd--01.\r\n");  //--zz debug	
//  flg_softmute=0;

//	if(dev_state != DEV_TEST){
//		volume_back = volume_back&~0x80;
//		amp_volume = volume_back;
//		//if(changeInputVolumeUp()== 0)
//			//ampSetVolume(amp_volume);
//			ampSetVolume3(amp_volume);
//	
//		flash_mode = FLASH_CONFIG;
//		if(flash_music_index != PAIRING_SOUND)
//			logEvent((Log_Type)(Flash_audio_track1_finished+flash_music_index));

//	}else{
//		if(test_mode == 1){
//			test_mode = 0;
//			playMusic(DEMO_SOUND);
//		}else
//			flash_mode = FLASH_CONFIG;
//	}
//	//state_color = input_channel_color[dev_input];
	
}

#if(ADPCM_FORMAT == IMA_ADPCM)
/*-------------------------------------------------------------------------
 * @fucname:	 adpcmEncode
 * @brief: 	 use adpcm code to compress the Ldata and Rdata
 * @param: 	 Ldata left channel data, Rdata left channel data
 * @return: 	 store L/R adpcm code a u8 data high 4bit is L, and low 4bit is R
 * @details: 
 *-----------------------------------------------------------------------*/
u8 adpcmEncode(s32 Ldata, s32 Rdata)
{
	s32 diff;
	u32 i;
	u8 code;

	code = 0;
	diff = ((Ldata - pre_Ldata_encode)<<3);	// start encode the left channel
	pre_Ldata_encode = Ldata;
	
	if(diff<0){
		diff = -diff;
		code = 0x80;
	}

	/****  i = diff /adpcm_step_size_table[adpcm_step_size_index_L] ****/
	for(i = 0; i < 8; i++){ 
		if(diff < adpcm_step_size_table[adpcm_step_size_index_L_encode])
			break;
		diff -= adpcm_step_size_table[adpcm_step_size_index_L_encode];
	}
	if(i == 8)
		i = 7;
	//i &= 0x7;	// i can't larger than 7
	
	adpcm_step_size_index_L_encode += adpcm_step_index_table[i];
	if(adpcm_step_size_index_L_encode<0)
		adpcm_step_size_index_L_encode = 0;
	if(adpcm_step_size_index_L_encode > 88)
		adpcm_step_size_index_L_encode = 88;
	
	code |= (i<<4);	// Ldata is encode to 4bit at high 4bit in code
	
	diff = ((Rdata - pre_Rdata_encode)<<3); // start encode the right channel
	pre_Rdata_encode = Rdata;

	if(diff<0){
		diff = -diff;
		code |= 0x08;
	}

	/**** i = diff /adpcm_step_size_table[adpcm_step_size_index_R] ****/
	for(i = 0; i < 8; i++){
		if(diff < adpcm_step_size_table[adpcm_step_size_index_R_encode])
			break;
		diff -= adpcm_step_size_table[adpcm_step_size_index_R_encode];
	}
	if(i == 8)
		i = 7;
//	i &= 0x7; // i can't larger than 7
	
	adpcm_step_size_index_R_encode += adpcm_step_index_table[i];
	if(adpcm_step_size_index_R_encode<0)
		adpcm_step_size_index_R_encode = 0;
	if(adpcm_step_size_index_R_encode > 88)
		adpcm_step_size_index_R_encode = 88;
	
	code |= i;	// Rdata is encode to 4bit at low 4bit in code

	return code;
}


/*-------------------------------------------------------------------------
 * @fucname:	 adpcmDecode
 * @brief: 	 decode the adpcm code and put L channel data in sum_L,
 			 R channel data in sum_R
 * @param: 	 adpcm u8 data, Ldata diff is in high 4bit and Rdata diff is in low 4bit
 * @return: 	 none
 * @details: 	 
 *-----------------------------------------------------------------------*/
void adpcmDecode(u8 data)
{
	u32 step;
	s32 diff;
	s32 sign_bit;
	
/**** decode the left channel data ****/
	step = (((s8)data) >> 4);

	if(step&0x8)
		sign_bit = -1;
	else
		sign_bit = 1;
		
	step &= 0x7;
	
	diff =  adpcm_step_size_table[adpcm_step_size_index_L_decode];
	diff = ((step*diff)>>3) + (diff >> 4);
	pre_Ldata_decode += sign_bit * diff;
	adpcm_step_size_index_L_decode += adpcm_step_index_table[step];
	if(pre_Ldata_decode > 32767)
		pre_Ldata_decode = 32767;
	else if(pre_Ldata_decode < -32767)
		pre_Ldata_decode = -32767;
	if(adpcm_step_size_index_L_decode <0)
		adpcm_step_size_index_L_decode = 0;
	else if (adpcm_step_size_index_L_decode > 88)
		adpcm_step_size_index_L_decode = 88;

/**** decode the right channel data ****/

	step = data & 0xf;

	if(step&0x8)
		sign_bit = -1;
	else
		sign_bit = 1;
		
	step &= 0x7;
	
	diff =  adpcm_step_size_table[adpcm_step_size_index_R_decode];
	diff = ((step*diff)>>3) + (diff >> 4);
	pre_Rdata_decode += sign_bit * diff;
	adpcm_step_size_index_R_decode += adpcm_step_index_table[step];
	if(pre_Rdata_decode > 32767)
		pre_Rdata_decode = 32767;
	else if(pre_Rdata_decode< -32767)
		pre_Rdata_decode = -32767;
	if(adpcm_step_size_index_R_decode <0)
		adpcm_step_size_index_R_decode = 0;
	else if (adpcm_step_size_index_R_decode > 88)
		adpcm_step_size_index_R_decode = 88;
	
}

#elif(ADPCM_FORMAT == MS_ADPCM)

#define predictor_L		(adpcm_step_size_index_L_decode)
#define predictor_R		(adpcm_step_size_index_R_decode)
#define L_sample0			adpcm_step_size_index_L_encode
#define R_sample0			adpcm_step_size_index_R_encode
/*-------------------------------------------------------------------------
 * @fucname:	 adpcmEncode
 * @brief: 	 use adpcm code to compress the Ldata and Rdata
 * @param: 	 Ldata left channel data, Rdata left channel data
 * @return: 	 store L/R adpcm code a u8 data high 4bit is L, and low 4bit is R
 * @details: 
 *-----------------------------------------------------------------------*/
u8 adpcmEncode(s32 Ldata, s32 Rdata)
{
	u8 code;

	code = 0;
	
	return code;
}


/*-------------------------------------------------------------------------
 * @fucname:	 adpcmDecode
 * @brief: 	 decode the adpcm code and put L channel data in sum_L,
 			 R channel data in sum_R
 * @param: 	 adpcm u8 data, Ldata diff is in high 4bit and Rdata diff is in low 4bit
 * @return: 	 none
 * @details: 	 
 *-----------------------------------------------------------------------*/
void adpcmDecode()
{
	static u32 L_delta, R_delta;
	static s32 L_sample1, R_sample1;
	static s16 L_cofe0, L_cofe1, R_cofe0, R_cofe1;
	s8 code;

	if(block_sample_index == 0){ // a new data block start

		SPI_SET_TX_BIT_LEN(SPI2, 16);
		SPI_WRITE_TX0(SPI2, 0xff);                           
		SPI_SET_GO(SPI2); 
		++block_sample_index;
		while(SPI_IS_BUSY(SPI2) != 0); 
//		spiSendWord(0xff, 16);
		predictor_L = SPI2->RX[0];
		SPI_SET_TX_BIT_LEN(SPI2, 32);
		SPI_WRITE_TX0(SPI2, 0xff);                           
		SPI_SET_GO(SPI2); 
		predictor_R = ((u32)predictor_L>>8)<<1;
		predictor_L = ((u32)predictor_L & 0xff)<<1;
		L_cofe0 = cofe_set[predictor_L];
		L_cofe1 = cofe_set[predictor_L+1];
		R_cofe0 = cofe_set[predictor_R];
		R_cofe1 = cofe_set[predictor_R+1];
		while(SPI_IS_BUSY(SPI2) != 0); 
//		spiSendWord(0xff, 32);
		L_delta = SPI2->RX[0];
		SPI_SET_TX_BIT_LEN(SPI2, 32);
		SPI_WRITE_TX0(SPI2, 0xff);                           
		SPI_SET_GO(SPI2); 
		R_delta = L_delta >> 16;
		L_delta = L_delta & 0xffff;
		while(SPI_IS_BUSY(SPI2) != 0); 
//		spiSendWord(0xff, 32);	// get L&R sample0 which stored in data block header
		L_sample0 = SPI2->RX[0];	
		SPI_SET_TX_BIT_LEN(SPI2, 32);
		SPI_WRITE_TX0(SPI2, 0xff);                           
		SPI_SET_GO(SPI2); 
		R_sample0 = L_sample0 >> 16;
		L_sample0 = (L_sample0 << 16)>>16;
		while(SPI_IS_BUSY(SPI2) != 0); 
//		spiSendWord(0xff, 32);	// get L&R sample1 which stored in data block header
		L_sample1 = SPI2->RX[0];
		R_sample1 = L_sample1 >> 16;
		L_sample1 = (L_sample1 << 16)>>16;	
		
		pre_Ldata_decode = L_sample1;	// send L sample0
		pre_Rdata_decode = R_sample1;// send R sample0
		
	}else if(block_sample_index == 1){
	
		++block_sample_index;
			
		pre_Ldata_decode = L_sample0;
		pre_Rdata_decode = R_sample0;
		
	}else{
	
		++block_sample_index;
		if(block_sample_index == samples_per_block){
			block_sample_index = 0;
			data_block_size--;
			file_head.data_block_count--;
		}
		spiSendWord(0xff, 8);
		code = (s8)(SPI2->RX[0]);
		
				/**** decode the left channel data ****/
		pre_Ldata_decode = (L_sample0*L_cofe0 + L_sample1*L_cofe1)>>8;
		pre_Ldata_decode = pre_Ldata_decode + L_delta*(code>>4);
		if(pre_Ldata_decode > ((u16)-1)>>1)
			pre_Ldata_decode =  ((u16)-1)>>1;
		if(pre_Ldata_decode < (s16)(1<<15))
			pre_Ldata_decode = (s16)(1<<15);
		L_sample1 = L_sample0;
		L_sample0 = (s16)pre_Ldata_decode;
		L_delta = (L_delta*AdaptationTable[(u8)code>>4])>>8;
		if(L_delta<16)
			L_delta = 16;
		if(L_delta > ((0x1<<14)-1))
			L_delta = ((0x1<<14)-1);
		
				/**** decode the right channel data ****/
		pre_Rdata_decode = (R_sample0*R_cofe0 + R_sample1*R_cofe1)>>8;
		code = (((s32)(s8)(code<<4)) >> 4);
		pre_Rdata_decode = pre_Rdata_decode + R_delta*code;
		if(pre_Rdata_decode > ((u16)-1)>>1)
			pre_Rdata_decode =  ((u16)-1)>>1;
		if(pre_Rdata_decode < (s16)(1<<15))
			pre_Rdata_decode = (s16)(1<<15);
		R_sample1 = R_sample0;
		R_sample0 = pre_Rdata_decode;
		R_delta = (R_delta*AdaptationTable[code&0xf])>>8;	
		if(R_delta<16)
			R_delta = 16;
		if(R_delta > ((0x1<<14)-1))
			R_delta = ((0x1<<14)-1);
		
	}

}

#undef predictor_L		
#undef predictor_R		
#undef L_sample0			
#undef R_sample0		

#endif

#if(ADPCM_FORMAT == IMA_ADPCM)	
/*-------------------------------------------------------------------------
 * @fucname:	 storeRecodeMusic
 * @brief: 	 get music data from ADC and put Left channel data and 
 			 Right channel data into flash after encode
 * @param: 	 none
 * @return: 	 none
 * @details: 
 *-----------------------------------------------------------------------*/
static __inline void storeRecodeMusic(s32 Ldata, s32 Rdata)
{
	s8 code;

	code = adpcmEncode(Ldata>>16,Rdata>>16);
	
	if(recode_index == 0){
		
		if(flash_data_buf_mux == 1){
			((ADPCMCHUNK*)flash_data_buf1)->Ldata = (Ldata>>16);
			((ADPCMCHUNK*)flash_data_buf1)->Rdata = (Rdata>>16);
			((ADPCMCHUNK*)flash_data_buf1)->Lindex = adpcm_step_size_index_L_encode;
			((ADPCMCHUNK*)flash_data_buf1)->Rindex = adpcm_step_size_index_R_encode;
		}else{
			((ADPCMCHUNK*)flash_data_buf2)->Ldata = (Ldata>>16);
			((ADPCMCHUNK*)flash_data_buf2)->Rdata = (Rdata>>16);
			((ADPCMCHUNK*)flash_data_buf2)->Lindex = adpcm_step_size_index_L_encode;
			((ADPCMCHUNK*)flash_data_buf2)->Rindex = adpcm_step_size_index_R_encode;
		}
		recode_index += 6;
		return;
	}

	
	if(flash_data_buf_mux == 1){

		flash_data_buf1[recode_index++] = code;
		if(recode_index >= 256){
			flash_data_buf_mux = 2;
			flash_write_triggle = 1;
			recode_index &= 0xff;
		}

	}else{
	
		flash_data_buf2[recode_index++] = code;
		if(recode_index >= 256){
			flash_data_buf_mux = 1;
			flash_write_triggle = 1;
			recode_index &= 0xff;
		}

	}
	
}

void ADPCM_TEST(s32 *Ldata, s32 *Rdata)
{
	u8 code;
	static u8 i = 0;
	static u32 pre_freq = 0; 
	static s32 R_predata = 0, L_predata = 0;
	
	if(flash_mode == FLASH_PLAY_MUSIC){
		if( pre_freq == 0){
			pre_freq = 0;
			i++;
			if(i == 32) 
				i = 0;
			code = adpcmEncode((*Ldata)>>16, (*Rdata)>>16);
			if(i == 0){
				pre_Ldata_encode = ((*Ldata)>>16);
				pre_Rdata_encode = ((*Rdata)>>16);

			}

			R_predata = pre_Rdata_decode;
			L_predata = pre_Ldata_decode;	
			adpcmDecode(code);
			if( i == 0 ){
				pre_Ldata_decode = ((*Ldata)>>16);
				pre_Rdata_decode = ((*Rdata)>>16);
			}

		
			*Ldata = (pre_Ldata_decode << 16);
			*Rdata = (pre_Rdata_decode << 16);
		}else{
			pre_freq = 0;
			*Ldata = ((pre_Ldata_decode+L_predata) << 15);
			*Rdata = ((pre_Rdata_decode+R_predata) << 15);
		}
	}
}
#endif
/*-------------------------------------------------------------------------
 * @fucname:	 dealFlashEvent
 * @brief: 	 recode music to flash or play music from flash
 * @param: 	 none
 * @return: 	 none
 * @details: 
 *-----------------------------------------------------------------------*/
extern u32 flash_w;
void dealFlashEvent(s32 *Ldata, s32 *Rdata)
{
	static u32 pre_freq = 0;
	static s32 R_predata = 0, L_predata = 0;

#if(ADPCM_FORMAT == IMA_ADPCM)		
	u16 temp;
	if(flash_mode == FLASH_PLAY_MUSIC && flash_w == 0){
		if( music_size > 0 ){
			if(pre_freq){
				pre_freq = 0;
				*Ldata = ((pre_Ldata_decode+L_predata) << 15);
				*Rdata = ((pre_Rdata_decode+R_predata) << 15);
			}else{
				pre_freq = 1;
				R_predata = pre_Rdata_decode;
				L_predata = pre_Ldata_decode;	
				if(chuck_index != 0 ){
					music_size--;
					chuck_index++;
					spiSendWord(0xff, 8);
					adpcmDecode((u8)(SPI2->RX[0]));
				}else{
					music_size -= 6;
					chuck_index += 6;
					spiSendWord(0xff, 32);
					pre_Ldata_decode = SPI2->RX[0];
					pre_Rdata_decode =(pre_Ldata_decode>>16);
					pre_Ldata_decode = (pre_Ldata_decode<<16)>>16;
					spiSendWord(0xff, 16);
					temp =(u16)(SPI2->RX[0]);
					adpcm_step_size_index_L_decode= temp&0xff;
					adpcm_step_size_index_R_decode= temp>>8;
				}
				*Ldata = ((pre_Ldata_decode) << 16);
				*Rdata = ((pre_Rdata_decode) << 16);
				
			}
		}else{
			SPI_CLR_SS0(SPI2);
			flash_mode = FLASH_CONFIG;
			
			adcKeySubHook = playSFMusicEnd;
		}
	}
	else if(flash_mode == FLASH_RECODE_MUSIC){
		if(pre_freq){
			pre_freq = 0;
			storeRecodeMusic(*Ldata, *Rdata);
		}else
			pre_freq = 1;
	}
#else
	//iputs("dealFlashEvent--03.\r\n");  //--zz debug
	if(flash_mode == FLASH_PLAY_MUSIC){
	//iputs("dealFlashEvent--031.\r\n");  //--zz debug
		if( file_head.data_block_count > 0 ){
			if(pre_freq){
				pre_freq = 0;
				*Ldata = ((pre_Ldata_decode+L_predata) << 15);
				*Rdata = ((pre_Rdata_decode+R_predata) << 15);
			}else{
	//iputs("dealFlashEvent--032.\r\n");  //--zz debug
				pre_freq = 1;
				R_predata = pre_Rdata_decode;
				L_predata = pre_Ldata_decode;	
				adpcmDecode();
	
				*Ldata = ((pre_Ldata_decode) << 16);
				*Rdata = ((pre_Rdata_decode) << 16);
				
			}
		}else{
	//iputs("dealFlashEvent--033.\r\n");  //--zz debug
			SPI_CLR_SS0(SPI2);
			*Ldata = 0;
			*Rdata = 0;
			I2S_CLR_RXFIFO();
			I2S_CLR_TXFIFO();
			flash_mode = FLASH_CONFIG;        //MA 20151020
			
			flg_softmute=0;
			adcKeySubHook = playSFMusicEnd;
		}
	}
	
#endif
}



/*-------------------------------------------------------------------------
 * @fucname:	 writeFlashRecodeData
 * @brief: 	 write recode data to flash
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
#if(ADPCM_FORMAT == IMA_ADPCM)
void writeFlashRecodeData()
{
	if(flash_recode_stop == TRUE){
		ledColorSet(YELLOW_COLOR);
		sysDelayMs(500);
		recodeEnd();
	}
	else if(flash_write_triggle == 1){
			flash_write_triggle = 0;
		//if(flash_data_buf_mux == 2)
			//spiFlashPageWrite(flash_program_address, 256, (u8*)flash_data_buf1);
		//else
			//spiFlashPageWrite(flash_program_address, 256, (u8*)flash_data_buf2);

		flash_program_address += 0x100;

		if(flash_program_address >=  0x100000)
			flash_recode_stop = TRUE;
	}
		
}
#endif



//=========================================================================




//--------------------------read-------------------------


//u32 dwt_readdevid(void)                    //ok  --keep
//{
//    return dwt_read32bitoffsetreg(0x00,0);  //DEV_ID_ID
//}


//u32 dwt_read32bitoffsetreg(u16 regFileID, u16 regOffset)     //ok  --keep
//{
//    u32  regval = 0 ;
//    int     j ;
//    u8   buffer[4] ;

//    dwt_readfromdevice(regFileID,regOffset,4,buffer);  

//    for (j = 3 ; j >= 0 ; j --)
//    {
//        regval = (regval << 8) + buffer[j] ;
//    }
//		
//    return regval;
//} 


//void dwt_readfromdevice( u16 recordNumber, u16 index, u32 length, u8 *buffer )     //ok  --keep
//{
//    u8 header[3] ;  
//    int cnt = 0;  

//    // get header
//    if (index == 0)  
//    {
//        header[cnt++] = (u8) recordNumber ;  
//    }
//    else
//    {
//        header[cnt++] = (u8)(0x40 | recordNumber) ; 

//        if (index <= 127)  
//        {
//            header[cnt++] = (u8) index ;  
//        }
//        else
//        {
//            header[cnt++] = 0x80 | (u8)(index) ; 
//            header[cnt++] =  (u8) (index >> 7) ;  
//        }
//    }

//    readfromspi(cnt, header, length, buffer);   
//} 


//u16 readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
//{

//	int i=0;

//	//decaIrqStatus_t  stat ;
//	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq   //--keep
//	
//	
//	SPI_CLR_SS0(SPI2);	 
//	SPI_DISABLE_REORDER(SPI2); 
//  SPI_SET_SS0(SPI2);
//		

//	for(i=0; i<headerLength; i++)
//	{
//		spiSendWord(headerBuffer[i],8);
//		//readBuffer[0] = SPI2->RX[0];
//	}
//	
//	SPI_ENABLE_REORDER(SPI2);
//	

//	for(i=0; i<readlength; i++)
//	{
//		spiSendWord(0xff,8);
//		readBuffer[i] = SPI2->RX[0];		
//	}

//	//SPIx_CS_GPIO->BSRR = SPIx_CS;
//	SPI_CLR_SS0(SPI2);	

//	//decamutexoff(stat) ;    //--keep

//	return 0;
//} 

void spiwritebyte(u8 ibyte);
u8 spireadbyte(void);
#define NOP10   {	__nop();__nop();__nop();__nop();__nop();  __nop();__nop();__nop();__nop();__nop();}
#define NOP100  {NOP10; NOP10; NOP10; NOP10; NOP10;    NOP10; NOP10; NOP10; NOP10; NOP10;}

u16 readfromspi(u16 headerLength, const u8 *headerBuffer, u32 readlength, u8 *readBuffer)
{
	int i=0;

	//decaIrqStatus_t  stat ;
	//stat = decamutexon() ;      //屏蔽PD3, dw1000 irq   //--keep
	
	GPIO_CLR_PIN(PB5);     //clk low
	GPIO_CLR_PIN(PB4);     //cs open

	NOP100; NOP10;  
	__nop();	

	for(i=0; i<headerLength; i++)
	{
		spiwritebyte(headerBuffer[i]);
	}
	
	for(i=0; i<readlength; i++)
	{
		readBuffer[i] = spireadbyte();		
	}

	NOP100; NOP10; 

	GPIO_CLR_PIN(PB5);     //clk low
	GPIO_SET_PIN(PB4);     //cs close
	//decamutexoff(stat) ;    //--keep

	return 0;
} 

u8 spireadbyte(void)
{
	u8 i;
	u8 value;	

	for(i = 0; i < 8; i++)
	{	 
		value <<= 1;
		GPIO_SET_PIN(PB5);     //clk high	
		//NOP10; 
		
		if(GPIO_GET_PIN(PB6))
		{
			value |= 0x01;
		}
		
		NOP10; 
		GPIO_CLR_PIN(PB5);     //clk low
		NOP10;
  }
	
	return value;
			
}

//void spiFlashRead2(u32 address, u32 count, u8* databuf )
//{
//	u32 index = 0;	
// 	
//	SPI_CLR_SS0(SPI2);	
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_SET_SS0(SPI2);	
//	
//	spiSendWord(address, 8);  //只有主地址 
//	
//	SPI_ENABLE_REORDER(SPI2);		
//	
//	for( index = 0; index < count; index++)
//	{
//		spiSendWord(0xff,8);
//		databuf[index] = SPI2->RX[0];
//	}
//	
//	SPI_CLR_SS0(SPI2);	
//}



//-----------------------write reg -----------------------

//void dwt_writetodevice( u16 recordNumber, u16 index, u32 length, const u8 *buffer )    //ok  --keep
//{
//    u8 header[3] ;  
//    int   cnt = 0;  

//    if (index == 0)  
//    {
//        header[cnt++] = 0x80 | recordNumber ;  
//    }
//    else
//    {
//        header[cnt++] = 0xC0 | recordNumber ; 

//        if (index <= 127)  
//        {
//            header[cnt++] = (u8)index ; 
//        }
//        else
//        {
//            header[cnt++] = 0x80 | (u8)(index) ;  
//            header[cnt++] =  (u8) (index >> 7) ;  
//        }
//    }
//   
//    writetospi(cnt,header,length,buffer);
//} 



//u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer)
//{
//	u16 i=0;
//	volatile u8 itemp=0;

//    //decaIrqStatus_t  stat ;
//    //stat = decamutexon();      //--keep

//		
//		SPI_CLR_SS0(SPI2);	 
//		SPI_DISABLE_REORDER(SPI2); 
//		SPI_SET_SS0(SPI2);	

//		for(i=0; i<headerLength; i++)
//		{
//			spiSendWord(headerBuffer[i],8);
//			itemp = SPI2->RX[0];
//		}	

//    for(i=0; i<bodylength; i++)
//    {

//			spiSendWord(bodyBuffer[i],8);
//			itemp = SPI2->RX[0];					
//	  }

//	  SPI_CLR_SS0(SPI2);			

//    //decamutexoff(stat) ;     //--keep

//    return 0;
//} 


// static __inline void spiSendWord(u32 ulData, u32 ucLength)
//{   
//	SPI_SET_TX_BIT_LEN(SPI2, ucLength);
//	SPI_WRITE_TX0(SPI2, ulData);                           
//	SPI_SET_GO(SPI2); 
//	while(SPI_IS_BUSY(SPI2) != 0); 
//}


//u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer)
//{
//	u16 i=0;
//	//volatile u8 itemp=0;

//    //decaIrqStatus_t  stat ;
//    //stat = decamutexon();      //--keep

//		
//		SPI_CLR_SS0(SPI2);	 
//		SPI_DISABLE_REORDER(SPI2); 
//		SPI_SET_SS0(SPI2);	

//		for(i=0; i<headerLength; i++)
//		{
//			spiSendWord(headerBuffer[i],8);
//			//itemp = SPI2->RX[0];
//		}	

//    for(i=0; i<bodylength; i++)
//    {

//			spiSendWord(bodyBuffer[i],8);
//			//itemp = SPI2->RX[0];					
//	  }

//	  SPI_CLR_SS0(SPI2);			

//    //decamutexoff(stat) ;     //--keep

//    return 0;
//} 

u16 writetospi( u16 headerLength, const u8 *headerBuffer, u32 bodylength, const u8 *bodyBuffer)
{
	uint8_t i;
	
	//stat = decamutexon();      //--keep

	GPIO_CLR_PIN(PB5);     //clk low
	GPIO_CLR_PIN(PB4);     //cs open

	NOP100; NOP10;  
	__nop();

	for(i=0; i<headerLength; i++)
	{
		spiwritebyte(headerBuffer[i]);
	}	

	for(i=0; i<bodylength; i++)
	{

		spiwritebyte(bodyBuffer[i]);
	}

	NOP100; NOP10; 

	GPIO_CLR_PIN(PB5);     //clk low
	GPIO_SET_PIN(PB4);     //cs close
	
	//decamutexoff(stat) ;     //--keep

	return 0;
} 

void spiwritebyte(u8 ibyte)
{
	u8 i;
	
   for(i = 0; i < 8; i++)
   {		 
		  //GPIO_CLR_PIN(PB5);     //clk low
      if(ibyte&0x80)
      {
	       GPIO_SET_PIN(PB6);  //Mo high
      }
      else
	    {
	       GPIO_CLR_PIN(PB6);  //Mo low
      }
			
	  	NOP10;			
	    GPIO_SET_PIN(PB5);     //clk high			
	    ibyte <<= 1;			
	    NOP10;			
	    GPIO_CLR_PIN(PB5);     //clk low
   }

}


//void spiFlashWrite2(u32 address, u32 count, u8* databuf )
//{
//	u32 index = 0;
//	volatile u8 itemp=0;
//	
// 
//	SPI_CLR_SS0(SPI2);	
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_SET_SS0(SPI2);	
//	
//	spiSendWord(address|0x80, 8);  //只有主地址
// 
//	
//	SPI_ENABLE_REORDER(SPI2);	
//	
//	
//	for( index=0; index < count; index++)
//	{
//		spiSendWord(databuf[index],8);
//		itemp = SPI2->RX[0];	
//	}
//	
//	SPI_CLR_SS0(SPI2);
//	
//}


//------------------------------other---------------------------


void reset_DW1000(void)
{
  GPIO_SET_MODE(PB, 9, GPIO_QUASI);  
  //GPIO_SET_MODE(PB, 9, GPIO_OUTPUT);   
	
	GPIO_CLR_PIN(PB9); 
	sysDelayMs(200); 
	GPIO_SET_PIN(PB9); 
	sysDelayMs(50); 
	
  GPIO_SET_MODE(PB, 9, GPIO_INPUT); 
	sysDelayMs(2); 
	
}


void spi_set_rate_low(void)
{
	
//	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
//	SPI_SET_SS0(SPI2);
//	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE0, 32);
//	SPI_SET_MSB_FIRST(SPI2);   //MSB first
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_ENABLE_BCN(SPI2); //Backward compatible
//	SPI_SET_DIVIDER(SPI2, 15); //SPIclk = sysckl/ ( (15+1)*2)  =72 MHz /32 = 2.25 MHZ
}

void spi_set_rate_high(void)
{
//	
//	SPI_SET_SS(SPI2, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
//	SPI_SET_SS0(SPI2);
//	DrvSPI_Init(SPI2, SPI_MASTER, SPI_TYPE0, 32);
//	SPI_SET_MSB_FIRST(SPI2);   //MSB first
//	SPI_DISABLE_REORDER(SPI2);
//	SPI_ENABLE_BCN(SPI2); //Backward compatible
//	SPI_SET_DIVIDER(SPI2, 1); //SPIclk = sysckl/ ( (1+1)*2)  =72 MHz /4 =  18 MHZ	
}



void deca_sleep(u32 time_ms)
{
    sysDelayMs(time_ms);
}

; 
u16 decamutexon(void)           
{
	u16 j = (u16)(NVIC->ISER[0] & (1 << 5));

	if(j) 
  {
		NVIC->ISER[0] &= ~(1 << 5); //disable the external interrupt line
	}
	return j ;    
}


void decamutexoff(u16 j)       
{
	if(j) 
	{                 
		NVIC->ISER[0] |= (1 << 5);;
	}
}

