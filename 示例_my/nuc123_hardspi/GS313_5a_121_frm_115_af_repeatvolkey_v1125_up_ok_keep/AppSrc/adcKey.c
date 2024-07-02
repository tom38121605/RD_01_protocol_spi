/****************************************************************************
 * @file:		adcKey.c
 * @version:	V1.00	
 * @breif:	adckey source file	
 * @date: 	2013/2/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define AUDIO_THRESHOLD		0x10
#define PRESS_SOUND_MODE		1   // 1: EVERY  0: ONCE
#define PRESS_INTERNEL_TIME		16  // TIME = *40ms

#define PRESS_SENSBILITY		21   // the valu smaller  more sensitive
//#define PRESS_SENSBILITY		19   // the valu smaller  more sensitive

//--#define cap_config_num 48
//#define cap_config_num 33  //46
#define cap_config_num 46  //46

u32 audio_quiet_time = 0; 
bool flg_ampdown = 0;

extern u32 f_test;


// button that press down without relax, adc_key_word will 
// record once untill relax the button
enum KEYWORD key_word = NONE_KEY; 

// button that press down without relax,key_word_continue
// will record per configuration interval, e.g record per 100ms
enum KEYWORD key_word_continue = NONE_KEY;	// continue key press
Timer_ID adc_key_task = TASK_FULL;
static u16 key_bak = 0,key_irq_index = 0;		// key value for adc irq
static u16 key_adc_value = 0;
//static u16 key_buf_lrf[MAX_LRF_KEYS] = {NONE_KEY};	// store the keys for LRF
enum KEYWORD volatile key_buf = NONE_KEY;				// the key after LRF
enum KEYWORD key_buf_adc = NONE_KEY;
u16 u16ADCBuf = 0;	// used for pdma destAddr buffer of adc value

u32 auto_standy_delay_minunte = 5;
u32 battery_volage = 0;
u32 dc_time_stamp = 0;

volatile static u32 bt_volage = 0; 
volatile static u32 battery_cc = 0;  //--, bt_count = 120, bt_vv = 0

u32 battery_level = 0; 
u32 battery_level_old = 0; 
u32 battery_level2 = 0; 
u32 bt_volage_adc;

u32 inew_volage=0;
u8 flg_batterychk=0;
u8 flg_softmute=0;


		volatile  uint8_t   CAP_cfg[cap_config_num][2] =   
		{   //  Address     Value
					
						{   0x00,       0x00    },      // Main Control    0x00-->1, 0xc0-->8
						{   0x1F,       0x6F    },      // Sensitivity Control
						{   0x20,       0x20    },      // General Configuration
						{   0x21,       0xFF    },      // Sensor Input Enable
						{   0x22,       0xA4    },      // Sensor Input Configuration
						{   0x23,       0x07    },      // Sensor Input Configuration 2
						{   0x24,       0x39    },      // Averaging and Sampling Config
						{   0x26,       0x00    },      // Calibration Activate
						{   0x27,       0xFF    },      // Interrupt Enable
						{   0x28,       0xFF    },      // Repeat Rate Enable
						{   0x2A,       0x00    },      // Multiple Touch Configuration
						{   0x2B,       0x00    },      // Multiple Touch Pattern Config
						{   0x2D,       0xFF    },      // Multiple Touch Pattern
						{   0x2F,       0x8A    },      // Recalibration Configuration
						{   0x30,       0x40    },      // Sensor Input 1 Threshold
						{   0x31,       0x40    },      // Sensor Input 2 Threshold
						{   0x32,       0x40    },      // Sensor Input 3 Threshold
						{   0x33,       0x40    },      // Sensor Input 4 Threshold
						{   0x34,       0x40    },      // Sensor Input 5 Threshold
						{   0x35,       0x40    },      // Sensor Input 6 Threshold
						{   0x36,       0x40    },      // Sensor Input 7 Threshold
						{   0x37,       0x40    },      // Sensor Input 8 Threshold
						{   0x38,       0x01    },      // Sensor Input Noise Threshold
						{   0x40,       0x01    },      // Standby Channel
						{   0x41,       0x39    },      // Standby Configuration
						{   0x42,       0x02    },      // Standby Sensitivity
						{   0x43,       0x40    },      // Standby Threshold
						{   0x44,       0x40    },      // Configuration 2
						{   0x71,       0x00    },      // LED Output Type
						{   0x72,       0x00    },      // LED Sensor Input Linking
						{   0x73,       0x00    },      // LED Polarity
						{   0x74,       0x00    },      // LED Output Control
						{   0x77,       0x00    },      // LED Linked Transition Control
						{   0x79,       0x00    },      // LED Mirror Control
						{   0x81,       0x00    },      // LED Behavior 1
						{   0x82,       0x00    },      // LED Behavior 2
						{   0x84,       0x20    },      // LED Pulse 1 Period
						{   0x85,       0x14    },      // LED Pulse 2 Period
						{   0x86,       0x5D    },      // LED Breathe Period
						{   0x88,       0x04    },      // LED Config
						{   0x90,       0xF0    },      // LED Pulse 1 Duty Cycle
						{   0x91,       0xF0    },      // LED Pulse 2 Duty Cycle
						{   0x92,       0xF0    },      // LED Breathe Duty Cycle
						{   0x93,       0xF0    },      // LED Direct Duty Cycle
						{   0x94,       0x00    },      // LED Direct Ramp Rates
						{   0x95,       0x00    }       // LED Off Delay	
			
		};


/*-------------------------------------------------------------------------
 * @fucname:	 AdcInit
 * @brief: 	 adc clk init 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 adc clk init
 *-----------------------------------------------------------------------*/
void adcKeyClkInit()
{
	SYS_RESET_IP2(SYS_RST_ADC);
	SET_ADC_CLKSRC(CLK_ADC_HXT);
	SET_ADC_CLKDIV(ADC_CLK_PRESCALE);
	APBCLK_ENABLE(CLK_ADC);
	
}
		



/*-------------------------------------------------------------------------
 * @fucname:	 readRH6010
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
static i2cPoolType touch_i2c_block;
static volatile u8 touch_buf[16];
u8 readCAP8298(u32 address) 
{  
	initI2cDataBlock(	&touch_i2c_block,  
					0x50,  
					I2C_RD,  
					2, 
					1,   
					(u8*)&touch_buf[0] 
				);  
	
	touch_buf[1] = address&0xff; 
	touch_buf[2] = 0x0; 
	touch_buf[0] = I2C_BUSY;	 

	addI2cChain(&touch_i2c_block, 0); 
	while(touch_buf[0] == I2C_BUSY) 
			; 
	return touch_buf[2]; 
	
}

u8 capack(void)
{
	return touch_buf[0];
}

u32 readCAPBlock(u32 address, u32 count)
{
	initI2cDataBlock(	&touch_i2c_block, 
					0x50, 
					I2C_RD, 
					1+count,
					1,
					(u8*)&touch_buf[0]
				);
	
	touch_buf[1] = address&0xff;;
	touch_buf[2] = 0x0;
	touch_buf[0] = I2C_BUSY;
	addI2cChain(&touch_i2c_block, 0);
	while(touch_buf[0] == I2C_BUSY)
			;
	return touch_buf[0] == I2C_IDLE;
}

u32 writeCAP8298(u32 data, u32 address)
{
	initI2cDataBlock(	&touch_i2c_block, 
					0x50, 
					I2C_WR, 
					2, 
					1, 
					(u8*)&touch_buf[0]
			);
	
	touch_buf[1] = address&0xff;
	touch_buf[2] = (u8)data;
	touch_buf[0] = I2C_BUSY;
	

	addI2cChain(&touch_i2c_block, 0);
	while(touch_buf[0] == I2C_BUSY)
		;
	return (touch_buf[0] == I2C_IDLE)
		;
}


u32 writeCAP8298Block(volatile u8 *buf, u32 count)
{
	initI2cDataBlock(	&touch_i2c_block, 
					0x50, 
					I2C_WR, 
					count+1,
					1,
					(u8*)&buf[0]
			);
	
	buf[0] = I2C_BUSY;
	addI2cChain(&touch_i2c_block, 0);
	while(buf[0] == I2C_BUSY)
			;
	
	return (buf[0] == I2C_IDLE);
}


/*--void touchKeyconfig(void)
{
	//--u32 i;

	writeCAP8298(0x01,0xc9);
	sysDelayMs(30);
	writeCAP8298(PRESS_INTERNEL_TIME,0xcc);
	sysDelayMs(30);
	writeCAP8298(PRESS_SOUND_MODE,0xc4);
	sysDelayMs(30);
	writeCAP8298(PRESS_SENSBILITY,0xCA);
	
	
	//for(i = 0; i < cap_config_num; i++)
	//	writeCAP8298(CAP_cfg[i][1],CAP_cfg[i][0]);
	//writeCAP8298(0xff,0x21);

}
*/

void touchKeyconfig()
{
	u32 i;
	
	for(i = 0; i < cap_config_num; i++)
		writeCAP8298(CAP_cfg[i][1],CAP_cfg[i][0]);
	
	//writeCAP8298(0xff,0x21);
}

void capIntEnable(void)
{
	//writeCAP8298(0xef,0x27);
}

void capIntDisable(void)
{
	//writeCAP8298(0xfb,0x28);
	//writeCAP8298(0X66,0X61);
	//writeCAP8298(0x04,0x27);
	//writeCAP8298( readCAP8298(0)&0xfe, 0x00);
	writeCAP8298(0x00,0xc9);
	sysDelayMs(50);
}

void capIntDisable1(void)
{
	writeCAP8298(0x02,0xc9);
	sysDelayMs(50);
}

void capPinEnable(u8 msk)
{
	writeCAP8298(msk,0x21);
}

void resetCap(void)
{
	//--U32 i = 0;
	writeCAP8298(0xAA,0xCE);	
}


//#define PWIN_SIZE	4

static s8 CAP_sensorDeltas[8];
u32 capSliderPostion()
{
	static u32 sliderPostion, segment;
	static u8 maxIndex,indexMinusOne,indexPlusOne;
	s32  sum, i, temp;

	temp = 0; 
	sum = 0; 
	maxIndex = 0; 
	 //Find the sensor on the slider which has the largest delta value,  
	 //and set negative delta to 0 
	for(i = 0; i < 6; i++) 
	{
		if(CAP_sensorDeltas[i] < 5)
			CAP_sensorDeltas[i] = 0;
		if(CAP_sensorDeltas[i] > temp)
		{
			maxIndex = i;
			temp = CAP_sensorDeltas[i];
		}
		sum	+= CAP_sensorDeltas[i];
	}

	if(sum > ((temp*5)>>1) || temp < 10)
		return 0xff; 
	
	//  Decode the position 
	if(maxIndex == 0) 
	{ 
		indexMinusOne = 5; 
		indexPlusOne = 1; 
		if(CAP_sensorDeltas[indexPlusOne] > CAP_sensorDeltas[indexMinusOne]) 
		{ 
			indexMinusOne = 0; 
			segment = 0; 
		} 
		else 
		{ 
			indexPlusOne = 0; 
			indexMinusOne = 5; 
			segment = 5; 
		} 
	}
	else if(maxIndex == 5)
	{
		indexMinusOne = 4;
		indexPlusOne = 0;
		if(CAP_sensorDeltas[indexPlusOne] > CAP_sensorDeltas[indexMinusOne])
		{
			indexMinusOne = 5;
			segment = 5;
		}
		else
		{
			indexPlusOne = 5;
			indexMinusOne = 4;
			segment = 4;
		}
	}
	else
	{
		indexMinusOne = maxIndex -1;
		indexPlusOne = maxIndex + 1;
		if(CAP_sensorDeltas[indexPlusOne] > CAP_sensorDeltas[indexMinusOne]){
			indexMinusOne = maxIndex;
			segment = maxIndex;
		}
		else
		{
			indexPlusOne = maxIndex;
			indexMinusOne = maxIndex-1;
			segment = indexMinusOne;
		}
	}

	sliderPostion = CAP_sensorDeltas[indexPlusOne]*20;
	sliderPostion /= (CAP_sensorDeltas[indexPlusOne] + CAP_sensorDeltas[indexMinusOne]);

	
	return segment*20 + sliderPostion;

}


/*-------------------------------------------------------------------------
 * @fucname:	 readKeyContinue
 * @brief: 	 get pressed key's value in configuration interval
 * @param: 	 none
 * @return: 	 the key_word_continue value
 * @details: 	 return key_word_continue value, can de-bouncd pulse < 60ms = scan rate 
 *-----------------------------------------------------------------------*/
////volatile static u32 delaycc = 0;
enum KEYWORD key_f;
//enum KEYWORD adcReadKeyContinue()
//{
//	static s32 kwcdc = 0; //key word continue delay counter
//	static enum KEYWORD key_c_r = NONE_KEY;
//	key_word_continue = NONE_KEY;
//	
//	if(key_word != NONE_KEY)
//	{
//		
//uartdecode(key_word,"  key_word: ");   //--zz debug  		
//		
//		kwcdc = KEY_CONTINUE_START;
//	}
//	else if (++kwcdc < KEY_CONTINUE_COUNTER)
//	{
//		key_c_r = key_f;
//	}
//	else
//	{
//		kwcdc = 0;
//		
//		if(key_c_r == key_f  /*&& delaycc > 3*/)
//		{
//			key_word_continue = key_f;	
//			
//uartdecode(key_word_continue,"  key_word_continue: ");   //--zz debug  		
//			
//		}
//	}
//	
//	return key_word_continue;
//}

enum KEYWORD adcReadKeyContinue()
{
	static s32 kwcdc = 0; //key word continue delay counter
	static enum KEYWORD key_c_r = NONE_KEY;
	key_word_continue = NONE_KEY;
	
	if(key_word != NONE_KEY){
		kwcdc = KEY_CONTINUE_START;
	}else if (++kwcdc < KEY_CONTINUE_COUNTER){
		key_c_r = key_buf;
	}else{
		kwcdc = 0;
		if(key_c_r == key_buf)
			key_word_continue = key_buf;	
	}

//	if(key_buf == CHANNEL_SWITCH_KEY)
//		key_word_continue = CHANNEL_SWITCH_KEY;
	
	return key_word_continue;
}

//enum KEYWORD adcReadKeyContinue()
//{
//	static s32 kwcdc = 0; //key word continue delay counter
//	static enum KEYWORD key_c_r = NONE_KEY;
//	key_word_continue = NONE_KEY;
//	
//	if(key_word == NONE_KEY) 
//	{
//		kwcdc = 0;
//		return key_word_continue;
//	}	
//	
//	
//	if (++kwcdc < (KEY_CONTINUE_COUNTER + 6) )
//	{
//		key_c_r = key_buf;
//	}
//	else
//	{
//		kwcdc = 0;
//		if(key_c_r == key_buf)
//			key_word_continue = key_buf;	
//	}

////	if(key_buf == CHANNEL_SWITCH_KEY)
////		key_word_continue = CHANNEL_SWITCH_KEY;
//	
//	return key_word_continue;
//}

void keyRst()
{
	key_word = key_f = NONE_KEY;
}


/*-------------------------------------------------------------------------
 * @fucname:	 devTaskDealKey
 * @brief: 	 do test key 
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 press a key to turn the lcd to corresponding color 
 *-----------------------------------------------------------------------*/
void devTestDealAdcKey()
{
/*--	dealTestKey(key_word);
	dealTestKeyContinue(key_word_continue);
	*/
}


/*-------------------------------------------------------------------------
 * @fucname:	 devStandbyDealAdcKey
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void devStandbyDealAdcKey()
{
	delStandbytkey(key_word);
}

/*--void uartdecode(u32 x, u8* str)        //--2015.07.02
 {
	u8 buf[16];
	u32 index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	iputs(str);
	iputs(buf);
	iputs("\r\n");
 }
*/

 void uartdecode2(u32 x, u8* str)
 {
	u8 buf[16];
	u32 index;
 
	index = itoa(x, &buf[0]);
	for(; index<6; index++)
		buf[index] = ' ';
	buf[index++] = 0;
	iputs(str);
	iputs(buf);
	//iputs("\r\n");
 }
 

u8 touchClr(void)
 {
 	/*--if(dev_state != DEV_TEST)
 		return readCAP8298(0xc1);
	else
		return 1;
  */
	 
	 return 100;  //zz--debug
	 
 } 
	 

static u8 key_touch;
static u8 nfc_f;
U32  NFC_DECT(void)
{
	if(nfc_f)
	{
		nfc_f = 0;
		return 1;
	}
	else
		return 0;
}


s32 amp_Mflag = -1;
void touchKeyRemap(void)
{
	//--static u32 error_c = 0;
	//--static u32 position;
	//--u8 value;
	//--u32 i, volume, cv;

	key_f = NONE_KEY;
	nfc_f = 0;

	//key_touch = readCAP8298(0xc0);  //cap8298
	key_touch = readCAP8298(0x3);     //cap1289
	sysDelayMs(5);

	readCAP8298(0xc0);
	
	uartdecode(key_touch," key ");

/*--	if(key_touch==0x07)
	{
		flg_softmute=1;
		
		if(flg_ampdown == 1)
		{
			flg_ampdown = 0;
			iputs("amp on\r\n");
 			ampPowerUp();	
		}
		playMusic(DD_SOUND);
		if(amp_Mflag < 20)
			amp_Mflag = 80;
		else
			amp_Mflag = 65;
		
		audio_quiet_time=0;
		cclsllt=0;		
	}
	else if(key_touch==0x01)
	{
		key_f = POWER_KEY; 
		//--uartdecode(0x01,"powerkey");
		
		if(flg_ampdown == 1)
		{
			flg_ampdown = 0;
			iputs("amp on\r\n");
 			ampPowerUp();	
		}	    		
		
	}
	else if(key_touch == 0x02)
	{
			key_f = FWD_KEY;
			amp_Mflag = 42;
			//--uartdecode(0x02,"FWD_KEY");
	}
	else if(key_touch == 0x03)
	{
			key_f = SOUND_CLOSE_KEY;
			amp_Mflag = 42;
			//--uartdecode(0x03,"SOUND_CLOSE_KEY");
	}
	else if(key_touch == 0x04)
	{
			key_f = SIRI_KEY;
			amp_Mflag = 42;
			//--uartdecode(0x04,"SIRI_KEY");
	}
	else if(key_touch == 0x05)
	{
			key_f = REW_KEY;
			amp_Mflag = 42;
			//--uartdecode(0x05,"REW_KEY");
	}
	else if(key_touch == 0x06)
	{
			nfc_f = 1;
			//--uartdecode(0x06,"NFC");
	}
	else
	{	
		cap_v = (s8)readCAP8298(0xc5);

		uartdecode(cap_v," v ");

		if(vol_c == 0)
		{
			vol_c = 1;
			vol_cc = 1;
		}


		if(cap_v < 0)
		{
			if(amp_volume > 2)
			{
				amp_volume -= 2;
			}
			else
				amp_volume = 0;
		
//iputs("ampSetVol--A16\r\n");    //--zz debug		
			ampSetVolume(amp_volume);
			iputs("-\r\n");
		}
		else if(cap_v > 0)
		{
			if(amp_volume < 30)
			{
				amp_volume += 2; 
			}
			else
				amp_volume = 31;
		
//iputs("ampSetVol--A17\r\n");    //--zz debug		
			ampSetVolume(amp_volume);
			iputs("+\r\n");
		}
		return;
	}
	*/
   
	
}

/*--u32 volume_send = 0;
static void  _sendVolume()
{
	volatile static u32 vvv = 0;
	
	if(vol_c == 1)
	{
		if(++vol_cc > 10 || abs(volume_send - amp_volume) > 8)
		{
			vol_cc = 0;
			if(volume_send != amp_volume)
			{
				btSetVolume(amp_volume);
				volume_send = amp_volume;
			}
			vol_c = 0;
			vvv = 0;
		}
		
	}
		
}
*/

/*-------------------------------------------------------------------------
 * @fucname:	 readKey
 * @brief: 	 return key_word value once in which case the button pressed down without relax
 * @param: 	 none
 * @return: 	 return key_word value
 * @details: 	 return key_word value de-bounce by compared twice if equal
 			 can de-bouncd pulse < 60ms = scan rate 
 *-----------------------------------------------------------------------*/
//enum KEYWORD adcReadKey(void)
//{	
//	//static enum KEYWORD key_word_r;
//	//static enum KEYWORD key_word_bak = NONE_KEY;	// last key_word
//	//static u32 ccount = 0, cap_count = 0;


//		//--_sendVolume();
//		
//		/*--if(amp_Mflag > 0) 
//		{
//			amp_Mflag--;
//			if(amp_Mflag == 40)
//			{
//				flash_mode = FLASH_CONFIG;
//				iputs("FLASH_CONFIG\r\n");
//			}
//		}
//		else if(amp_Mflag == 0)
//		{
//			unMute();
//			//iputs("dd_mute_off\r\n");
//			sysDelayMs(20);
//		
////--iputs("ampSetVol--A18\r\n");    //--zz debug		
//			ampSetVolume(amp_volume);
//			amp_Mflag = -1;	
//			//ampSetVolume(amp_volume);
//		}
//		*/
//		
//		/*if(!GPIO_GET_PIN(PC9))
//				ccount = 1;
//		
//		if(ccount)
//		{ 
//			ccount--;
//			touchKeyRemap();
//			cap_count = 0;
//		}
//		else
//		{
//			key_f = NONE_KEY;
//			if(++cap_count > 200)
//			{
//				cap_count = 0;
//				readCAP8298(0xC0);
//				if(capack() == I2C_ERROR)
//				{ 
//					iputs("i2c tp error\r\n"); 
//					resetCap();
//					sysDelayMs(300);
//					touchKeyconfig();
//					capIntEnable();
//				}				
//			}	
//			
//		}
//		
//		if(key_f != NONE_KEY && key_buf == NONE_KEY && !GPIO_GET_PIN(PC13))
//			key_word = key_f;
//		else{
//			if(GPIO_GET_PIN(PC13))
//				key_buf = PAIR_KEY;

//	 		key_f = key_buf;
//			
//			if(key_f == NONE_KEY)
//			{
//				key_word = NONE_KEY;
//				key_word_bak = NONE_KEY;
//				key_word_r = NONE_KEY;
//				delaycc = 0;
//			}else if (key_word_r == key_f)
//				key_word = NONE_KEY;
//			else if(key_word_bak == NONE_KEY){
//				key_word_bak = key_f;
//			 	key_word_r =key_word = NONE_KEY;
//			}else if(key_word_bak == key_f){
//				//delaycc = 4;
//	 	 		key_word_r = key_word = key_f;
//			}else{
//				//key_word_bak = key_f;
//			 	key_word_r = key_word = NONE_KEY;
//			}
//		}
//    */		
//		
//		return key_word;
//		
//}

enum KEYWORD adcReadKey()
{	
	static enum KEYWORD key_word_r;
	static enum KEYWORD key_word_bak = NONE_KEY;	// last key_word
	//static u32 key_flag_count = 0;	
		
		if(key_buf == NONE_KEY)
		{
			key_word = NONE_KEY;
			key_word_bak = NONE_KEY;
			key_word_r = NONE_KEY;
		}
		else if (key_word_r == key_buf)
		{			
			key_word = NONE_KEY;
		}
		else if(key_word_bak == NONE_KEY)
		{	
			key_word_bak = key_buf;
		 	key_word_r =key_word = NONE_KEY;
		}
		else if(key_word_bak == key_buf)
		{			
      key_word_r = key_word = key_buf;
			
//uartdecode(key_buf,"  key_word: ");   //--zz debug  
		}
		else
		{		
			key_word_bak = key_buf;
		 	key_word_r = key_word = NONE_KEY;
		}		
		
// 	if(GPIO_GET_PIN(PC8))   //POWER_KEY_PORT_PIN
// 		key_word = POWER_KEY;		
		
//--uartdecode(key_word,"  key_word: ");   //--zz debug  
		
		return key_word;  		
}


/*-------------------------------------------------------------------------
 * @fucname:	 devStartDealKey
 * @brief: 	deal the key event in dev start state
 * @param: 	none
 * @return: 	none
 * @details: 	deal the key event in dev start state
 *-----------------------------------------------------------------------*/
extern u32 BT_MATCH_FLAG;
extern Timer_ID pt2314_id;
extern void btCheckDone(void);
void devStartDealAdcKey(void)
{
	//--static u32 k_counter = 0, k_counter1 = 0;
	static u32 k_counter = 0;   //--2015.07.02
	
	delStartkey(key_word);
	 
	if(key_word_continue == INC_KEY || key_word_continue == DEC_KEY)    //--2015.07.02
	{        
		if(++k_counter >= INC_KEY_COUNTER)
		{
			delStartkey(key_word_continue);
			k_counter = 0;
		}
	}	
	

//	if(key_word == PAIR_KEY){
//		playMusic(PAIRING_SOUND);
//	}

}


/*--	u32 audioDetect(void)   //--2015.07.02
{
u32 audio_threshold;    

	//if(amp_volume < 26)
	//	audio_threshold = AUDIO_THRESHOLD;
	//else if(amp_volume < 29)
		audio_threshold = AUDIO_THRESHOLD+(AUDIO_THRESHOLD>>1);
	//else
	//	audio_threshold = (AUDIO_THRESHOLD<<2);

	if(dev_input == BLUETOOTH && BT_MATCH_FLAG == 0)
		audio_threshold = AUDIO_THRESHOLD*2;
	
	if( (audio_L>>10) > audio_threshold ||(audio_R>>10) > audio_threshold)
		return TRUE;
	else
		return FALSE;
	
	
}
*/
	
/*-------------------------------------------------------------------------
 * @fucname:	 adcKeyHook
 * @brief: 	 audio input detect
 * @param: 	 none
 * @return: 	 none
 * @details: 	 audio input detect
 *-----------------------------------------------------------------------*/
u32 standby_flag;
u32 liocx = 0;
extern u32 shut_key;
//--static u32 shut_down_timer = 0;        //--2015.07.02
u8 time_out = 0;
adcKeyHook()
{
	
	/*--time_out = 0;     //--2015.07.02

	if(DECT_DC())
	{
		audio_quiet_time=0;
		cclsllt=0;
		
	}	
	
	auto_standy_delay_minunte = 30;

	if(audioDetect())
	{
		audio_quiet_time = 0; // audio is inputing
		shut_down_timer = 0;
		
		if(liocx != 0X55AA)
		{
			liocx = 0X55AA;
			ampPowerUp();
			iputs("amp on\r\n");
		}
		
	}
	else 
	{		
		auto_standy_delay_minunte=30;  //--zz
		

		if(audio_quiet_time >= (auto_standy_delay_minunte*3240000) )  //--270*200*60
		{
			iputs("no audio for 10/30 min\r\n");
			time_out = 1;
			capIntDisable1(); 
			if(twslinklst() == 4 || btlinklst() == 4)     //--BOTH LINK LOST
				;
			else
			{
				btShutDown1();  
				enterStandby();
			}
			audio_quiet_time = 0;
			//shut_down_timer = 0;
		 } 
		
		if(++shut_down_timer > 2000)
		{
			shut_down_timer = 0; // audio is inputing
			if(liocx != 0xccdd)
			{
				liocx = 0xccdd;
				iputs("amp down\r\n");
				ampPowerDown();
				flg_ampdown = 1;
				sysDelayMs(20);
			}
			
		}
		
	}
	audio_R = audio_L = 0;
*/

}


/*-------------------------------------------------------------------------
 * @fucname:	 keyScan 
 * @brief: 	 scan the key and get key value if has 
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get key_word value with  and key_word_continue value
 *-----------------------------------------------------------------------*/
FUNC_Timer_Task adcKeySubHook;
extern u8 battery_f;
void adckeyScan(void)
{
	
	/*
	if((channel_dec == CHANNEL_AUX_KEY && dev_input == BLUETOOTH) ||
	   (channel_dec == CHANNEL_BT_KEY && dev_input == LINEIN)){
	   	BAT_STATE_DECT_DELAY_COUNT = 0; 
		led_battery_flash = 0;
		stopATask(pt2314_id);
		GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
		changeInputVolumeDown(INPUT_CHANGE_STATE);
	}
	*/
	
//iputs("adckeyScan\r\n");	
	
	key_word = adcReadKey();
	key_word_continue = adcReadKeyContinue();
	
//uartdecode(key_word,"  key_word: ");   //--zz debug  			
	
	
	switch(dev_state)   //--2015.07.02
	{		
	// the dev_state is init to DEV_START and if the CHANNEL3_KEY pressed at  the dev is power on
	// qithin DEV_START_TIME then the dev state will change to DEV_TEST
		case DEV_START:
			devStartDealAdcKey();
			break;
		case DEV_TEST:
			iputs("ts");
			devTestDealAdcKey();	// if dev is in DEV_TEST STATE do it
			break;
		case DEV_STANDBY:
			//--iputs("ss");
			devStandbyDealAdcKey();
			break;
		default:
			iputs("sf");
			break;
	}
	
	
	/*--if(dev_state != DEV_STANDBY)
		adcKeyHook();
	else
	{
		if(!DECT_DC()&&dev_state==DEV_STANDBY)
			if(standby_flag == 1)
			{
				if(++shut_down_timer > 60*1000*2)
					doPowerOff();
			}
			else
				shut_down_timer = 0;
	}
	
	if(adcKeySubHook){
		adcKeySubHook();
		adcKeySubHook = NULL;
	}

	if(dev_state == DEV_STANDBY && !DECT_DC())
	{
		// if didn't dect DC and it is in DEV_STANDBY, then turn off the power
		doPowerOff();
	}
	*/
	
	startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
}


/*--extern void showVersion(U32 I);       //--2015.07.02
void devTest(void)
{
	static u32 dev_test_flag = 0;
	static u32 counter;

	if(!DECT_DC())
	{
		dev_test_flag = counter = systick_counter;
		dc_time_stamp = dev_test_flag;
		while(1)
		{
			if(	systick_counter < DEV_TEST_TIMER &&
		   		key_buf == STEREO_KEY &&
		  		GPIO_GET_PIN(PC13))
			{
				// press the inc_key and power key more than 3 sec when power on
				if(systick_counter > counter)
				{
					counter = systick_counter;
					if(++dev_test_flag > 1500 + dc_time_stamp )
						showVersion(0);
				}
			}
			else
				if(systick_counter > counter)
				{
					counter = systick_counter;
					if( (systick_counter - dev_test_flag) > 200 )
						break;
				}
				
		}
		
	}
	
}
*/

void adcKeyInit()       
{
	ADC_KEY_ENABLE;		 // OPEN KEY PIN ADC FUNCTION
	
	//--GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN, GPIO_INPUT);	
	//--GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, ADC_KEY_PIN);
	
	GPIO_SET_MODE(ADC_KEY_PORT,BATTERY_V_DECT_PIN,GPIO_INPUT);
	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, BATTERY_V_DECT_PIN);
	
	ADC_SET_MODE(ADC_SINGLE_SCAN);   //SINGLE SCAN MODE
	ADC_CH_ENABLE(ADC_KEY_MSK);  
	ADC_CH_ENABLE(0x180);  
	ADC_CONVERTER_ENABLE();  
	
}

//void adcKeyInit()
//{	
//	
//iputs("adcKey Init in\r\n");
//	
//	ADC_KEY_ENABLE;		 // OPEN KEY PIN ADC FUNCTION
//	
//	GPIO_SET_MODE(ADC_KEY_PORT, ADC_KEY_PIN, GPIO_INPUT);
//	GPIO_SET_MODE(ADC_KEY_PORT,BATTERY_V_DECT_PIN,GPIO_INPUT);
//	//GPIO_SET_MODE(ADC_KEY_PORT,1,GPIO_INPUT);
//	
//	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, ADC_KEY_PIN);
//	GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, BATTERY_V_DECT_PIN);
//	//GPIO_DISABLE_DIGITAL_PATH(ADC_KEY_PORT, 1);
//	
//	ADC_SET_MODE(ADC_SINGLE_SCAN); // SINGLE SCAN MODE
//	ADC_CH_ENABLE(ADC_KEY_MSK);	
//	
//	ADC_CONVERTER_ENABLE(); 	
//}



Timer_ID appkey_id;

void adckeyTaskInit()
{	
	adcKeyInit();	   
	
//--iputs("adckeyTaskInit -- 01\r\n");	

	
#if(ADC_KEY_PDMA_EN == FALSE)
	NVIC_SetPriority (ADC_IRQn, ADC_INT_PRIORITY); 
	DrvADC_EnableInt();
#endif
	
	battery_cc = 0;
	adc_key_task = getATask();
	//appkey_id = getATask();
	
 	if(adc_key_task != TASK_FULL);
	{
//--iputs("adckeyTaskInit -- 02\r\n");	
		startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
	}
	
}
//void adckeyTaskInit()
//{
//	
//iputs("adckeyTask Init in\r\n");
//	
//	adcKeyInit();
//#if(ADC_KEY_PDMA_EN == FALSE)
//	NVIC_SetPriority (ADC_IRQn, ADC_INT_PRIORITY); 
//	DrvADC_EnableInt();
//#endif
//	
//	adc_key_task = getATask();
//	if(adc_key_task != TASK_FULL);
//		startATask(adc_key_task, ADC_KEY_SCANF_RATE, 0, adckeyScan);
//}

	

/*-------------------------------------------------------------------------
 * @fucname:	 adcKeyRemap
 * @brief: 	 remap the key
 * @param: 	 adc key
 * @return: 	 none
 * @details: 	 remap the adc key and store in the key_buf, can de-bounce pulse < 8ms
 *-----------------------------------------------------------------------*/
void adcKeyRemap(u16 adcBuf)
{
	// if has a pulse do lrf 
	if(((adcBuf > key_bak)  ? adcBuf - key_bak :  key_bak - adcBuf) > 115)
	{
		key_adc_value = key_irq_index = 0;
		key_bak = adcBuf;
		return;
	}
	else 
		key_adc_value += adcBuf;// get adc value
		
	key_bak = adcBuf;
	
	if(++key_irq_index ==  (1<<MAX_LRF_KEYS))
	{	 //remap the adc key 
		key_irq_index = 0;
		key_adc_value >>= MAX_LRF_KEYS;

		if(key_adc_value > 1000 )
			key_buf = NONE_KEY;
		else if(key_adc_value > 850)	 // the min pulse hight 115 = 1000 - 895 , use in lrf
			key_buf = NONE_KEY;
		else if (key_adc_value > 750)
			key_buf = MULTIROOM_KEY;	// adc_EN, the last risistance,when press the v is max
		else if (key_adc_value > 650)
			key_buf = NONE_KEY;
		else if (key_adc_value > 550)
			key_buf = STEREO_KEY;
		else
			key_buf = NONE_KEY;	
		
//		key_adc_value = 0;
//		key_buf_adc = key_buf;
		
		
		key_buf_adc = key_buf;	
		
		if(!GPIO_GET_PIN(POWER_KEY_PORT_PIN))   //PC8 
		{
			key_buf = POWER_KEY;	
			//--uartdecode(key_buf,"  key_buf: ");   //--zz debug  
		}

		if(!GPIO_GET_PIN(DEC_KEY_PORT_PIN))   //PC9
			key_buf = DEC_KEY;	
		
		if(!GPIO_GET_PIN(INC_KEY_PORT_PIN))   //PC10
			key_buf = INC_KEY;	

		
		key_adc_value = 0;		
	}
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 ADC_IRQHandler
 * @brief: 	 do LRF of adc key and put the pressed key's value in key_buf
 * @param: 	 none
 * @return: 	 none
 * @details: 	 do lrf to adc key and put the pressed key's value in key_buf which has LRF
 *-----------------------------------------------------------------------*/
// extern u32 motor_cc;
// extern u32 motor_direction;
// extern u32 bt_ck;
// extern u32 bt_cycle;
// extern  u32 bvb;
// //--u32 volatile bt_avmx = 0;

// 				//  0     10                                                100
// u32 bat_vtbl_0[] = { 708,715,742,749,753,761,772,786,798,815,817};
// 				//  0     10                                                100
// u32 bat_vtbl_1[] = { 728,750,765,770,774,780,790,799,805,826,830};

// //--static s32 lpf_data[1024];


//--static u32  sum_lpf = 0;  //--lpf_index = 0,
u32 lpf_delt = 0;
extern s32 lpf_out;


static i2cPoolType eeprom_i2c_block;
static volatile u8 eeprom_i2c_buf[3];

u8 readeeprom(u32 address)
{
  initI2cDataBlock( &eeprom_i2c_block, 
          0xA0, 
          I2C_RD, 
          2,
          1,
          (u8*)&eeprom_i2c_buf[0]
        );
  
  eeprom_i2c_buf[1] = address&0xff;;
  eeprom_i2c_buf[2] = 0x0;
  eeprom_i2c_buf[0] = I2C_BUSY;

  addI2cChain(&eeprom_i2c_block, 0);
  while(eeprom_i2c_buf[0] == I2C_BUSY)
      ;
  return eeprom_i2c_buf[2];	
}


u32 writeeeprom(u32 data, u32 address)
{
//iputs("weeprom--01.\r\n");  //--zz debug
  initI2cDataBlock(  &eeprom_i2c_block, 
          0xA0, 
          I2C_WR, 
          2, 
          1, 
          (u8*)&eeprom_i2c_buf[0]
      );
  
  eeprom_i2c_buf[1] = address&0xff;
  eeprom_i2c_buf[2] = (u8)data;
  eeprom_i2c_buf[0] = I2C_BUSY;
  

  addI2cChain(&eeprom_i2c_block, 0);
//iputs("weeprom--02.\r\n");  //--zz debug
  while(eeprom_i2c_buf[0] == I2C_BUSY)
    ;
  return (eeprom_i2c_buf[0] == I2C_IDLE)
    ;
}

u8 Key_Scan(u8 mode)
{   
	static u8 f_keyup=1;      
	
	//if(mode) f_keyup=1;       
	
	if( f_keyup&&( GPIO_GET_PIN(PD0) ==0) )
	{
		f_keyup=0;  
		
		sysDelayMs(10);           
		if( GPIO_GET_PIN(PD0)==0) return 1;     
	}
	else if( GPIO_GET_PIN(PD0)==1)
		f_keyup=1;       
	
	 return 0;                
	
}


//=============================================

static i2cPoolType bq_i2c_block;
static volatile u8 bq_i2c_buf[3];

u8 readbq(u32 address)
{
  initI2cDataBlock( &bq_i2c_block, 
          0xD6, 
          I2C_RD, 
          2,
          1,
          (u8*)&bq_i2c_buf[0]
        );
  
  bq_i2c_buf[1] = address&0xff;;
  bq_i2c_buf[2] = 0x0;
  bq_i2c_buf[0] = I2C_BUSY;

  addI2cChain(&bq_i2c_block, 0);
  while(bq_i2c_buf[0] == I2C_BUSY)
      ;
  return bq_i2c_buf[2];	
}


u32 writebq(u32 data, u32 address)
{
//iputs("weeprom--01.\r\n");  //--zz debug
  initI2cDataBlock(  &bq_i2c_block, 
          0xD6, 
          I2C_WR, 
          2, 
          1, 
          (u8*)&bq_i2c_buf[0]
      );
  
  bq_i2c_buf[1] = address&0xff;
  bq_i2c_buf[2] = (u8)data;
  bq_i2c_buf[0] = I2C_BUSY;
  

  addI2cChain(&bq_i2c_block, 0);
//iputs("weeprom--02.\r\n");  //--zz debug
  while(bq_i2c_buf[0] == I2C_BUSY)
    ;
  return (bq_i2c_buf[0] == I2C_IDLE)
    ;
}



/*-------------------------------------------------------------------------
 * @fucname:	 ADC_IRQHandler
 * @brief: 	 do LRF of adc key and put the pressed key's value in key_buf
 * @param: 	 none
 * @return: 	 none
 * @details: 	 do lrf to adc key and put the pressed key's value in key_buf which has LRF
 *-----------------------------------------------------------------------*/
#define base_voltage  1250
void ADC_IRQHandler()
{	
	u16 key_current=0;
	static u32 iadccount=0;
	static u32 baseband_adc=0;
	static u32 batt_adc=0;
		
	
	ADC_CLR_IF(ADC_CONV_DONE_IF);  // clean adc int flag

		
	baseband_adc  += ADC_GET_RSLT(7); 
	batt_adc  += ADC_GET_RSLT(BATTERY_V_DECT_PIN); 

	if (iadccount++>= (1<<10) )
	{
		iadccount=0;
		
		baseband_adc >>=10;
		batt_adc >>=10;
		
		bt_volage_adc = batt_adc * base_voltage / baseband_adc;
		

		//uartdecode(bt_volage_adc,"  bt_volage_adc: ");   //--zz debug  
    //iputs("\r\n");		
		
	} 		
	
	adcKeyRemap(key_current);			
	
}



