/****************************************************************************
 * @file:		i2s.c
 * @version:	V1.00
 * @breif:		i2s source file
 * @date: 	2013.4.8
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/


#include "sysinc.h"
#include "filter.h"

u8 flg_playing  =0;

//=================================
	static u32 L_delta, R_delta;	
	static u32 L_delta_old, R_delta_old;	
  static u32 pre_freq = 0;
	
	u8 codex;
	u8 codey;
	s32 diff;
	s32 newsamplex;
	s32 newsampley;
	
	u32 ustep;
	u8 utemp;
	
u32 read_address = 0;

//==========================
	

u32 imusicdatalen=0;
u32 idatasum=0;	
u32 iDataCount=0;
u32 inowaddress =0;

s16 sample0x;
s16 sample0y;

s32 presamplex;
s32 presampley;
s8  index0_x;
s8  index0_y;
s8 newindex_x;
s8 newindex_y;

extern s32 i2s_rxbuf[];
extern s32 i2s_txbuf[];
extern u32 filter_bypass;
extern FUNC_Timer_Task adcKeySubHook;

u8 MUSIC_FLASH_SIZE = 43;     //ma 20150615  add MUSIC_FLASH_SIZE FOR FLASH MUSIC 
EFLASH_MODE flash_mode = FLASH_CONFIG;
u32 flash_music_index = 0;
S32 music_size = 0;
SFLASH_INFO sflash_info_cache;
s32 audio_detect_L = 0,audio_detect_R = 0;
u32 flash_program_address = 0;
u32 flash_data_buf_mux = 1;
u8 flash_status = 0;
u32 flash_write_triggle = 0;
u32 flash_recode_stop = FALSE;

static s8 index_adjust[16]={
    -1,-1,-1,-1,2,4,6,8,
    -1,-1,-1,-1,2,4,6,8,
};

static u16 steptab[89]={
    7,8,9,10,11,12,13,14,16,17,
    19,21,23,25,28,31,34,37,41,45,
    50,55,60,66,73,80,88,97,107,118,
    130,143,157,173,190,209,230,253,279,307,
    337,371,408,449,494,544,598,658,724,796,
    876,963,1060,1166,1282,1411,1552,1707,1878,2066,
    2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,
    5894,6484,7132,7845,8630,9493,10442,11487,12635,13899,
    15289,16818,18500,20350,22385,24623,27086,29794,32767
};


/*****add play_music_flag****/
u8 play_music_flag = 0;     //ma 20150616
/*****add version number record****/   //MA 20150615 
u8 BT_VERSION_NUMBER[4]    = {'0','.','0','0'};
u8 MCU_VERSION_NUMBER[4]   = {'6','.','6','8'};      //ma 20151013 SVN 6.71
u8 WL_VERSION_NUMBER[4]    = {'0','.','0','0'};
u8 FLASH_VERSION_NUMBER[4] = {'1','.','1','2'};
u8 BT_name_block[25][30] ={
	{"AT+SUFFIX=\"LIVING ROOM\""},          {"AT+SUFFIX=\"KITCHEN\""},           {"AT+SUFFIX=\"DINING ROOM\""},        {"AT+SUFFIX=\"PARTY ROOM\""},   
	{"AT+SUFFIX=\"BEDROOM 1\""},						{"AT+SUFFIX=\"BEDROOM 2\""},         {"AT+SUFFIX=\"PATIO\""},              {"AT+SUFFIX=\"OFFICE\""},              
	{"AT+SUFFIX=\"SALON\""},            		{"AT+SUFFIX=\"CUISINE\""},				   {"AT+SUFFIX=\"SALLE A MANGER\""},     {"AT+SUFFIX=\"SALLE DE JEUX\""},     
	{"AT+SUFFIX=\"CHAMBRE 1\""},            {"AT+SUFFIX=\"CHAMBRE 2\""},         {"AT+SUFFIX=\"PATIO\""},			         {"AT+SUFFIX=\"BUREAU\""},               
	{"AT+SUFFIX=\"WOHNZIMMER\""},           {"AT+SUFFIX=\"KUECHE\""},            {"AT+SUFFIX=\"ESSZIMMER\""},        	 {"AT+SUFFIX=\"PARTYRAUM\""},
	{"AT+SUFFIX=\"SCHLAFZIMMER 1\""},       {"AT+SUFFIX=\"SCHLAFZIMMER 2\""},    {"AT+SUFFIX=\"TERRASSE\""},           {"AT+SUFFIX=\"BUERO\""},  
	{"AT+SUFFIX=\" \""},
};
// static s32 recode_index = 0; //used in i2s 48k interrupt       //20150728  del static
s32 recode_index = 0;     //20150728  del static
// static s32 pre_Rdata_encode = 0, pre_Ldata_encode = 0; // used in adpcm encode  //2015728 del static 
s32 pre_Rdata_encode = 0, pre_Ldata_encode = 0; // used in adpcm encode     //2015728 del static 
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
// static u8 chuck_index = 0;         //ma 20150728   del static
 u8 chuck_index = 0;                   //ma 20150728   del static
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
	I2S_CLR_RXFIFO();
	I2S_ENABLE_RX();
#if(DMAI2S == TRUE)
	I2S_ENABLE_RXDMA();
	PDMA_TRG_ENABLE(PDMA1);
	filterInit();
#else
	I2S_CLR_TXFIFO();
	I2S_ENABLE_TX();
	DrvI2S_EnableInt(I2S_INT_RXTH); 
	NVIC_SetPriority(I2S_IRQn,0);
#endif
	
	//GPIO_SET_MODE(PB,13,GPIO_OUTPUT);
	//GPIO_SET_MODE(PB,8,GPIO_INPUT);
}


/*-------------------------------------------------------------------------
 * @fucname:	 SPI1_IRQHandler
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void SPI1_IRQHandler (void)
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
		databuf[index] = SPI1->RX[0];
	}
	SPI_CLR_SS0(SPI1);

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
	
	if( length == 0 ||(address&0xff) || length > 256)
		return FALSE;
	
	SF_CMD_PAGEWRB(address&(~0xff));
		
	for(index = 0; index < length; index++)
		spiSendWord(databuf[index],8);	 //write the data of this page
	
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
extern u32 volume_back;
void spiFlashInit()
{
	GPIO_SPI1_SS0_PC8();
	GPIO_SPI1_CLK_PC9();
	GPIO_SPI1_MOSI0_PC11();
	GPIO_SPI1_MISO0_PC10();
	SPI_SET_SS(SPI1, SPI_SS_LOW_TRIGGER, SPI_SW_SS);
	SPI_SET_SS0(SPI1);
	DrvSPI_Init(SPI1, SPI_MASTER, SPI_TYPE2, 32);
	SPI_SET_MSB_FIRST(SPI1);   //MSB first
	SPI_DISABLE_REORDER(SPI1);
	SPI_ENABLE_BCN(SPI1); //Backward compatible
	SPI_SET_DIVIDER(SPI1, 1); //SPIclk = 18 MHZ
	
	spiFlashRead(0, sizeof(sflash_info_cache), (u8*)&sflash_info_cache );
	if(sflash_info_cache.falsh_sn != 0x4E534C47)
		sflash_info_cache.info.count = 0;		// the flash don't have music
	volume_back = 0x00;
	
	flash_mode = FLASH_CONFIG;
	iDataCount=0;

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
// void readWaveHeaderInfo(u32 start_address)
// {
// 	u32 i;

// 	SF_CMD_READ(start_address);
// 	for(i = 0; i < sizeof(file_head); i++){
// 		spiSendWord(0xff, 8);
// 		((u8*)&file_head)[i] = SPI1->RX[0];
// 	}
// 	
// 	samples_per_block = (file_head.data_block_size -14) +2;
// 	data_block_size = file_head.data_block_size;

// 	for(i = 0; i < 20; i++){
// 		cofe_set[i] = file_head.cofe_sets[i];
// 	}
// 	//start_address +=  sizeof(file_head);
// 	//SF_CMD_READ(start_address);
// }

void readWaveHeaderInfo(u32 start_address)
{	
	SF_CMD_READ(start_address);
	
	spiSendWord(0xff, 32);
	imusicdatalen =SPI1->RX[0];
	
  //idatasum=imusicdatalen/4;	
  idatasum=imusicdatalen;	
	
//uartdecodeH32(imusicdatalen,"  imusicdatalen: ");   //--zz debug  	
	
	//get sample0x
	spiSendWord(0xff, 16);
	sample0x =SPI1->RX[0];
	
	//get presamplex
	presamplex=sample0x;
// 	if (presamplex>32787)
// 		presamplex=presamplex-65536;
	
	//get sample0y	
	spiSendWord(0xff, 16);
	sample0y =SPI1->RX[0];
	
	//get presampley
	presampley=sample0y;
// 	if (presampley>32787)
// 		presampley=presampley-65536;
	
	//get index0_x
	spiSendWord(0xff, 8);
	index0_x =SPI1->RX[0];
	
	if (index0_x<0) index0_x=0;
	if (index0_x>88) index0_x=88;
	newindex_x=index0_x;

	//get index0_y
	spiSendWord(0xff, 8);
	index0_y =SPI1->RX[0];
	
	if (index0_y<0) index0_y=0;
	if (index0_y>88) index0_y=88;
	newindex_y=index0_y;
	
	
	//SPI_CLR_SS0(SPI1); //--zz
	
	//inowaddress=start_address+4;  //0x10
	inowaddress=start_address+12;  //0x10	
	SF_CMD_READ(inowaddress);	
	
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
U32 volume_back = 0;
void playMusic(s32 index)
{
	//u32 read_address = 0;
	u32 v;	

	//if ((flash_mode == FLASH_PLAY_MUSIC) && (index!=POWER_OFF_SOUND ))   return;  
	if (flash_mode == FLASH_PLAY_MUSIC)   
	{
		if (index==POWER_OFF_SOUND ) 
		{
			iDataCount=0;
      flash_mode = FLASH_CONFIG;   	
			SPI_CLR_SS0(SPI2);
		}
		else	
			return;  
	}
		

	flash_music_index = index;
	if(flash_music_index >= sflash_info_cache.info.count)
		return;
  soft_mute_flag = 1;        //ma 20150703  set soft mute flag
	ampMute();   //MA 20150618
	
// 		mute();  //MA 20150618
// 	sysDelayMs(100);
	
// 	flg_playing=1;  //--zz
	
	if(dev_state != DEV_TEST)
	{
		if(!(volume_back&0x80))
			volume_back = amp_volume | 0x80;
		
    /****change the power on volume*******/
		/*****20150702	************/	
		if(index == POWER_ON_SOUND )                                    //ma 20150707   change the defualt power on sound volume 
			v = 24;   //16;            //ma 20150702   change 23 to  18 
		else if(index == POWER_OFF_SOUND)                               //ma 20150707   change the defualt power off sound volume 
			v = 24;   //18;
		else if(index == SWITCH_SOUND)
			v= 14;
		else if(index == NEW_BT_PAIRING_SOUND)   //ma 20150901   set bt pairing sound default volume
			v= 26;
		else if(index == BATTERY_LOW_SOUND)    //ma 20150702   change default to  23  
			v= 18;             //ma  20150703  change 23 to 31  to 18 flash v1.05
		else
			v = 23;            //ma 20150707   the defualt sound volume   no change 

//test		
v = 20;  //--zz test
		
		
		amp_volume = v;
		ampSetVolume(v);
	}
 
	flash_mode = FLASH_CONFIG; 

	//Dwhp83Standby();
	DWHP83Mute();
	sysDelayMs(30);
	
	adpcmDataInit();

	read_address = sflash_info_cache.info.address[index];
	//music_size = sflash_info_cache.info.address[index+1] -read_address;
	
uartdecode(read_address,"  read_address: ");   //--zz debug  
	
//	if(++flash_music_index >= sflash_info_cache.info.count)
//		flash_music_index = 0; 
	
#if(ADPCM_FORMAT == MS_ADPCM)
	readWaveHeaderInfo(read_address);
#else
	SF_CMD_READ(read_address);
#endif

	filter_bypass = TRUE;
	adcKeySubHook = NULL;
	ampunMute();      //MA  20150618
	sysDelayMs(20);   //MA  20150618
	
	
// 	unMute();       //MA  20150618
// 	sysDelayMs(20); //MA 20150618
	flash_mode = FLASH_PLAY_MUSIC;
	flg_playing=1;  //--zz
	//logEvent(Flash_audio_track1_started+flash_music_index);	
	
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
	flash_mode = FLASH_CONFIG;
	flash_recode_stop = FALSE; // the flash is start to be written
	flash_program_address = 0x100;
	flash_write_triggle = 0;

/*----------- adpcm data init -----------*/
	adpcmDataInit();

/*------------ erase the whole flash  -------------*/
	SF_BULK_ERASE();
	flash_status = 1;
	ledColorSet(PURPLE_COLOR);
	
	while(flash_status&1){
		sysDelayMs(500);
		ledColorSet(PURPLE_COLOR); //during  erasing led is purple color
		SF_GET_STATUS();
	}
	
	ledColorSet(GREEN_COLOR);	// show green data when flash erase is done
	
	/*
	I2S_DISABLE_MCK();
	APBCLK_DISABLE(CLK_I2S);
	I2SClkInit(RECODE_AUDIO_RATE);	// init i2s
	i2sStart();
	*/
	sysDelayMs(1000);
	filter_bypass = TRUE;
	flash_mode = FLASH_RECODE_MUSIC;
}


void recodeEnd(void)
{	
	init_spiflash_info();
	SPI_CLR_SS0(SPI1);
	flash_mode = FLASH_CONFIG;
	filter_bypass = FALSE;
}


void playSFMusicEnd(void)
{
	DWHP83UnMute();
	sysDelayMs(10);
	
// 	if(flash_music_index == POWER_ON_SOUND)  //--zz
// 		sysDelayMs(500);
	
	filter_bypass = FALSE;
	SPI_CLR_SS0(SPI1);
	//if(dev_input == BLUETOOTH)
	//	BT_POWER_ON();

	if(dev_state != DEV_TEST)
	{
		volume_back = volume_back&~0x80;
		amp_volume = volume_back;
		
		//if(changeInputVolumeUp()== 0)
			ampSetVolume(amp_volume);
	
		flash_mode = FLASH_CONFIG;
		//logEvent(Flash_audio_track1_finished+flash_music_index);
	}
	else
	{
		flash_mode = FLASH_CONFIG;
	}
	
	//flg_softmute=0;  //--zz
	flg_playing=0;   //--zz
	
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

		SPI_SET_TX_BIT_LEN(SPI1, 16);
		SPI_WRITE_TX0(SPI1, 0xff);                           
		SPI_SET_GO(SPI1); 
		++block_sample_index;
		while(SPI_IS_BUSY(SPI1) != 0); 
//		spiSendWord(0xff, 16);
		predictor_L = SPI1->RX[0];
		SPI_SET_TX_BIT_LEN(SPI1, 32);
		SPI_WRITE_TX0(SPI1, 0xff);                           
		SPI_SET_GO(SPI1); 
		predictor_R = ((u32)predictor_L>>8)<<1;
		predictor_L = ((u32)predictor_L & 0xff)<<1;
		L_cofe0 = cofe_set[predictor_L];
		L_cofe1 = cofe_set[predictor_L+1];
		R_cofe0 = cofe_set[predictor_R];
		R_cofe1 = cofe_set[predictor_R+1];
		while(SPI_IS_BUSY(SPI1) != 0); 
//		spiSendWord(0xff, 32);
		L_delta = SPI1->RX[0];
		SPI_SET_TX_BIT_LEN(SPI1, 32);
		SPI_WRITE_TX0(SPI1, 0xff);                           
		SPI_SET_GO(SPI1); 
		R_delta = L_delta >> 16;
		L_delta = L_delta & 0xffff;
		while(SPI_IS_BUSY(SPI1) != 0); 
//		spiSendWord(0xff, 32);	// get L&R sample0 which stored in data block header
		L_sample0 = SPI1->RX[0];	
		SPI_SET_TX_BIT_LEN(SPI1, 32);
		SPI_WRITE_TX0(SPI1, 0xff);                           
		SPI_SET_GO(SPI1); 
		R_sample0 = L_sample0 >> 16;
		L_sample0 = (L_sample0 << 16)>>16;
		while(SPI_IS_BUSY(SPI1) != 0); 
//		spiSendWord(0xff, 32);	// get L&R sample1 which stored in data block header
		L_sample1 = SPI1->RX[0];
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
		if(block_sample_index == samples_per_block)
		{
			block_sample_index = 0;
			data_block_size--;
			file_head.data_block_count--;
		}
		spiSendWord(0xff, 8);
		code = (s8)(SPI1->RX[0]);
		
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


void dealFlashEvent(s32 *Ldata, s32 *Rdata)
{
// 	static u32 L_delta, R_delta;	
// 	static u32 L_delta_old, R_delta_old;	
//   static u32 pre_freq = 0;
// 	
// 	u8 codex;
// 	u8 codey;
// 	s32 diff;
// 	s32 newsamplex;
// 	s32 newsampley;
// 	
// 	u32 ustep;
// 	u8 utemp;
	
//   if(flash_mode == FLASH_PLAY_MUSIC)
// 	{

    if (iDataCount < imusicdatalen)
    //if (iDataCount < idatasum)
		{			
 			//inowaddress=0x10;
 			//--SF_CMD_READ(inowaddress); //-- to remove
			
			if (pre_freq==0)
			{			
				pre_freq=1;
				
				spiSendWord(0xff, 8);    			
				utemp = SPI1->RX[0]; 			
				
				inowaddress +=1;	
				

 				//L_delta = utemp & 0xffff;            //low
 				//R_delta = (utemp >> 16) & 0xffff;    //high
				
				//--decode left--
				codex = utemp & 0x0f;	

        ustep =	steptab[newindex_x];	
				
				diff= ustep>>3;
				if (codex & 4)  diff += ustep;
				if (codex & 2)  diff += ustep>>1;
				if (codex & 1)  diff += ustep>>2;
	
				if (codex & 8)  diff = -diff;
				
				newsamplex = presamplex + diff;
				if (newsamplex>32767)  newsamplex=32767;
				if (newsamplex<-32768)  newsamplex=-32768;
				
				presamplex = newsamplex;
				
				
				//write to 25p80   --x
				//......								
					
				
				newindex_x = newindex_x + index_adjust[codex];
				if (newindex_x<0) newindex_x=0;
				if (newindex_x>88) newindex_x=88;				
				
				
				//--decode right--
				codey = (utemp>>4) & 0x0f;
				
				ustep =	steptab[newindex_y];	

				
				diff= ustep>>3;
				if (codey & 4)  diff += ustep;
				if (codey & 2)  diff += ustep>>1;
				if (codey & 1)  diff += ustep>>2;
	
				if (codey & 8)  diff = -diff;
				
				newsampley = presampley + diff;
				if (newsampley>32767)  newsampley=32767;
				if (newsampley<-32768)  newsampley=-32768;


				//write to 25p80   --y
				//......


				presampley = newsampley;
				
				newindex_y = newindex_y + index_adjust[codey];
				if (newindex_y<0) newindex_y=0;
				if (newindex_y>88) newindex_y=88;
				
				
				//--output newsample--
				L_delta = (u32)newsamplex<<16;   //16
				R_delta = (u32)newsampley<<16;   //16
				
				*Ldata =(s32)(L_delta);
				*Rdata =(s32)(R_delta);
				
				L_delta_old = L_delta;
				R_delta_old = R_delta;
				
				iDataCount++;	
			}			

      else
			{				
				pre_freq=0;
				
				*Ldata =(s32)(L_delta_old);
				*Rdata =(s32)(R_delta_old);				
			}
			
// 			//test to remove voice for debug
// 			*Ldata = 0;   
// 			*Rdata = 0;   
			
    }		
		else   
		{			
//uartdecodeH32(iDataCount,"  iDataCount: ");   //--zz debug  
			
			iDataCount=0;
			
			SPI_CLR_SS0(SPI1); 
			*Ldata = 0;   
			*Rdata = 0;      
			I2S_CLR_RXFIFO();                
			I2S_CLR_TXFIFO();   

      flash_mode = FLASH_CONFIG;   	
      flg_playing=0;			
       
			//flg_softmute=0;
			adcKeySubHook = playSFMusicEnd; 
		}
		
	//}
}


/*-------------------------------------------------------------------------
 * @fucname:	 writeFlashRecodeData
 * @brief: 	 write recode data to flash
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void writeFlashRecodeData()
{
	if(flash_recode_stop == TRUE){
		ledColorSet(PURPLE_COLOR);
		sysDelayMs(500);
		recodeEnd();
	}
	else if(flash_write_triggle == 1){
			flash_write_triggle = 0;
		if(flash_data_buf_mux == 2)
			spiFlashPageWrite(flash_program_address, 256, (u8*)flash_data_buf1);
		else
			spiFlashPageWrite(flash_program_address, 256, (u8*)flash_data_buf2);

		flash_program_address += 0x100;

		if(flash_program_address >=  0x100000)
			flash_recode_stop = TRUE;
	}
		
}


/*******play version sound********/    //ma 20150615
void version_sound_play()
{
// 	u8 i,sound_number;
// 	static u8 play_music_flag = 0;
	/******MCU VERSION PLAY****/
// 	playMusic(FIRMWARE_VERSION_SOUND);
// 	sysDelayMs(3000);
// 	for(i =0;i<4;i++)
// 	{
// 		if(MCU_VERSION_NUMBER[i] == '.')
// 			playMusic(DOT_SOUND);
// 		else
// 		{
// 			sound_number = MCU_VERSION_NUMBER[i] - '0';
// 			playMusic(sound_number+NUMBER_0);
// 		}
// 		sysDelayMs(2000);
// 	}
// 	sysDelayMs(500);
// 	/******BT VERSION PLAY****/
// 	ledColorSet(BLUE_COLOR);
// 	for(i =0;i<4;i++)
// 	{
// 		if(BT_VERSION_NUMBER[i] == '.')
// 			playMusic(DOT_SOUND);
// 		else
// 		{
// 			sound_number = BT_VERSION_NUMBER[i] - '0';
// 			playMusic(sound_number+NUMBER_0);
// 		}
// 		sysDelayMs(1200);
// 	}
// 	sysDelayMs(500);
// 	/******WL VERSION PLAY****/
// 	ledColorSet(PURPLE_COLOR);	
// 	for(i =0;i<4;i++)
// 	{
// 		if(WL_VERSION_NUMBER[i] == '.')
// 			playMusic(DOT_SOUND);
// 		else
// 		{
// 			sound_number = WL_VERSION_NUMBER[i] - '0';
// 			playMusic(sound_number+NUMBER_0);
// 		}
// 		sysDelayMs(1200);
// 	}
// 	sysDelayMs(500);
// 	/******FLASH VERSION PLAY****/
// 	ledColorSet(YELLOW_COLOR);	
// 	for(i =0;i<4;i++)
// 	{
// 		if(FLASH_VERSION_NUMBER[i] == '.')
// 			playMusic(DOT_SOUND);
// 		else
// 		{
// 			sound_number = FLASH_VERSION_NUMBER[i] - '0';
// 			playMusic(sound_number+NUMBER_0);
// 		}
// 		sysDelayMs(1200);
// 	}
}
/*******play all music sound for test*****/
void play_all_sound()
{
// 	u8 i;
// 	
// 	for(i=0;i< MUSIC_FLASH_SIZE;i++)
// 	{
// 		playMusic(i);
// 		sysDelayMs(500);
// 	}
}
