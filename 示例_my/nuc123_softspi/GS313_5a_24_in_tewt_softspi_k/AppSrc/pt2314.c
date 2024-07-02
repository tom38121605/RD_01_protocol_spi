/****************************************************************************
 * @file:		pt2314.c
 * @version:	V1.00
 * @breif:		pt2314 is used for choosing input channel 
 * @date: 	2013/3/13
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

PT2314_STATU pt2314_status = {I2C_IDLE, 0x00, 0xC0, 0xE0, PT2314_IN1_CODE, 0x77, 0x67};
enum PT2314_FUNC_MODE pt2314_mode;

// enum PT2314_INPUT_CHANNEL{ //--zz
// 	BLUETOOTH = 0,
// 	LINEIN = 1, 	
// 	FLASHIN = 2,
// 	MUTEIN =3,
// 	OPTICAL = 4, 	
// 	COAXIAL = 5 	
// 	
// };
enum PT2314_INPUT_CHANNEL pt2314_input;
enum PT2314_INPUT_CHANNEL pt2314_input_table[15] = 
{
	BLUETOOTH,
	LINEIN,
	FLASHIN
};
// remap for GS234
u8 pt2314_switch_remap[4] = {PT2314_IN1_CODE,PT2314_IN2_CODE,PT2314_IN3_CODE,PT2314_IN4_CODE}; // audio switch code

static i2cPoolType pt2314_data_block;
Timer_ID pt2314_id = TASK_FULL;
Timer_ID pt2314_id_CIC = TASK_FULL; // for change input channel 
u8	pt2314_bass_tbl[12] = {
						0x62,                 //-10dB
						0x63,                 //-8dB
						0x64,                 //-6dB
						0x65,                 //-4dB
						0x66,                 //-2dB
						0x67,                 // 0dB
						0x6e,                 //+2dB
						0x6d,                 //+4dB
						0x6c,                 //+6dB
						0x6b,                 //+8dB
						0x6a,                 //+10dB 
						0x00                 //trap!
	};

u8	pt2314_trieb_tbl[12] = {
						0x72,                 //-10dB    
						0x73,                 //-8dB     
						0x74,                 //-6dB     
						0x75,                 //-4dB     
						0x76,                 //-2dB     
						0x77,                 // 0dB     
						0x7e,                 //+2dB     
						0x7d,                 //+4dB     
						0x7c,                 //+6dB     
						0x7b,                 //+8dB     
						0x7a,                 //+10dB    
						0x00                 //trap!    

	};
extern u32 change_input_state;
extern i2cPoolType se_w58_i2c_block_change_input;
extern u32 BT_MATCH_FLAG;

/*-------------------------------------------------------------------------
 * @fucname:	 pt2314ReleaseI2c
 * @brief: 	 release the i2c data source of pt2314
 * @param: 
 * @return: 
 * @details: 	 release the i2c data source of pt2314
 *-----------------------------------------------------------------------*/
void pt2314ResumeI2c()
{
	pt2314_data_block.index = pt2314_data_block.index_bak;
	pt2314_status.state = I2C_IDLE;
}

/*--void pt2314InputChannelCheck()    //--2015.07.22
{	
	void seW58ChangInputVolumeCheck(void);
	
	if(pt2314_data_block.byte_ptr[0] == (u8)I2C_IDLE){
		//--dev_input = (1+dev_input) > (total_input_channel-1)?0:1+dev_input;
		dev_input = (u32) (1+ (u32)dev_input) > (total_input_channel-1)?(enum PT2314_INPUT_CHANNEL)0:(enum PT2314_INPUT_CHANNEL)(1+dev_input);
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
		if(callDetect() && dev_input == BLUETOOTH)
			state_color = LIGHT_BLUE_COLOR;
		pt2314_input = pt2314_input_table[dev_input];
		unMute();
		if(dev_input == BLUETOOTH){
			btStartUp();

		}else{
			btShutDown();
	
		}
		changeInputVolumeUp();
		return;
	}
	else if(pt2314_data_block.byte_ptr[0] == (u8)I2C_BUSY){
		startATask(pt2314_id_CIC, 2, 0, pt2314InputChannelCheck); //i2c bus is used and wait 2ms
		return;	
	}
	else{ // i2c error
		changeInputChannel(); //i2c bus is used and wait 2ms
		return;
	}
}
*/

void sel441clk()
{
	//--u32 status;

}

void sel48clk()
{
	//--u32 status;
	

}

/*-------------------------------------------------------------------------
 * @fucname:	 changeInputChannel
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
extern u32 ccount;
/*--void changeInputChannel()  //--2015.07.22
{
	//--static u32 default_input_volume;
	ampMute();
	dev_input = (u32)(1+(u32)dev_input) > (total_input_channel-1)?(enum PT2314_INPUT_CHANNEL)0:(enum PT2314_INPUT_CHANNEL)(1+dev_input);
	if(state_color == LIGHT_BLUE_COLOR){
		dev_input = BLUETOOTH;
	}else{
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
	}
	
	if(dev_input == BLUETOOTH){
		sysDelayMs(100);
		btStartUp();
		logEvent(BLUETOOTH_SOURE_IN);
		logEvent(bt_pairing);
		sysDelayMs(50);
		GPIO_SET_PIN(PA10);
		sysDelayMs(50);
		ccount = 0;
//iputs("PAIRING_SOUND--02.\r\n");  //--zz debug
		playMusic(PAIRING_SOUND);
	}else{
		//btShutDown();
		logEvent(LINEIN_SOURE_IN); 
		logEvent(bt_power_down);
		sysDelayMs(50);
		GPIO_CLR_PIN(PA10);
		BT_MATCH_FLAG = 0;
		playMusic(DISCONECTED_SOUND);
	}
	unMute();
	sysDelayMs(20);
	ledColorSet(state_color);
	if(callDetect() && dev_input == BLUETOOTH)
		state_color = LIGHT_BLUE_COLOR;
	
	changeInputVolumeUp();
}
*/


/*-------------------------------------------------------------------------
 * @fucname:	 pt2314ResumeCheck
 * @brief: 	 check whether i2c transmisson is ok
 * @param: 	 none
 * @return: 	 none
 * @details: 	 check whether i2c transmisson is ok
 *-----------------------------------------------------------------------*/
void pt2314ResumeCheck()
{

}


/*--void pt2314DataResume()   //--2015.07.22
{
	if(DECT_NFC())
		dev_input = BLUETOOTH;
		
	state_color = input_channel_color[dev_input];
	if(dev_input == BLUETOOTH)
	{
		sysDelayMs(100);
		//btStartUp();
		GPIO_SET_PIN(PA10);
		logEvent(BLUETOOTH_SOURE_IN);
		logEvent(bt_pairing);		
	}
	else
	{
		//btShutDown();
		GPIO_CLR_PIN(PA10);
		logEvent(bt_power_down);
		logEvent(LINEIN_SOURE_IN);
	}
	
	if(dev_state == DEV_TEST)
		ledColorSet(YELLOW_COLOR);
	else
		ledColorSet(state_color);
	
}
*/

/*-------------------------------------------------------------------------
 * @fucname:	 pt2314Resume
 * @brief: 	init the pt2314 to pt2314_status indicated
 * @param: 	none
 * @return: 	none
 * @details: 	init the pt2314 to pt2314_status indicated
 *
 * @note: 	!!! when call this function be sure i2c bus is not busy
 *-----------------------------------------------------------------------*/
void pt2314Init()
{
	
	//GPIO_SET_MODE(AUDIO_DET_PORT, AUDIO_DET_PIN, GPIO_INPUT);
	//GPIO_SET_MODE(SPDIF_SEL_PORT, SPDIF_SEl_PIN, GPIO_OUTPUT); // opt/cax selection pin output enable

	pt2314_id = getATask();
	pt2314_id_CIC = getATask();
}

