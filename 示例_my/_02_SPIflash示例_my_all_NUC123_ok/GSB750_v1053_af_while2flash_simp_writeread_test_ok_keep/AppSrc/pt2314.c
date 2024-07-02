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
enum PT2314_INPUT_CHANNEL pt2314_input;
enum PT2314_INPUT_CHANNEL pt2314_input_table[15] = {
													BLUETOOTH,
													LINEIN,
													WIRELESS,
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
/*-------------------------------------------------------------------------
 * @fucname:	 checkInputChannel
 * @brief: 	 check whether chang input channel is well done
 * @param: 	 NONE
 * @return: 	 NONE
 * @details:  	 check whether chang input channel is well done then change led color and dev_input
 *-----------------------------------------------------------------------*/
void pt2314InputChannelCheck()
{	
	void seW58ChangInputVolumeCheck(void);
	
	if(pt2314_data_block.byte_ptr[0] == (u8)I2C_IDLE){
		dev_input =(enum PT2314_INPUT_CHANNEL)((1+dev_input) > (total_input_channel-1)?0:1+dev_input);
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
		if(DECT_CALL() && dev_input == BLUETOOTH)
			state_color = LIGHT_BLUE_COLOR;
		pt2314_input = pt2314_input_table[dev_input];
		unMute();
		if(dev_input == BLUETOOTH){
			BT_POWER_ON();

		}else{
			BT_POWER_OFF();
	
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


u32 bt_check_flag = 0;
void btCheckDone(void)
{
	bt_check_flag = 0;
// 	GPIO_CLR_PIN(BT_HANG_UP_PORT_PIN);
	stopATask(pt2314_id);
} 

/*-------------------------------------------------------------------------
 * @fucname:	 pt2314ResumeCheck
 * @brief: 	 check whether i2c transmisson is ok
 * @param: 	 none
 * @return: 	 none
 * @details: 	 check whether i2c transmisson is ok
 *-----------------------------------------------------------------------*/
static u32 stamp = 0;
void btCheck(void)
{
	if(BT_MATCH_FLAG == 1){
		stopATask(pt2314_id);
		return;
	}else if(systick_counter - stamp > 5000 ){
		bt_check_flag = 1;
// 		GPIO_SET_PIN(BT_HANG_UP_PORT_PIN);
		startATask(pt2314_id, 1500, 0, btCheckDone);
		return;
	}
	startATask(pt2314_id, 10, 0, btCheck);
}	

extern u32 ccount;
extern volatile u32 Dwhp83_CuMu;
extern void dwph83Check(void);

/*-------------------------------------------------------------------------
 * @fucname:	 changeInputChannel
 * @brief: 	 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void changeInputChannel()
{
// 	static u32 default_input_volume;
	u32 nfc = 0;
	ampMute();

	if(DECT_NFC()){
		dev_input = BLUETOOTH;
		stamp = systick_counter;
		nfc = 1;
		bt_check_flag = 0;
		btCheck();
	}else
		dev_input =(enum PT2314_INPUT_CHANNEL)((1+dev_input) > (total_input_channel-1)?0:1+dev_input);
		
	state_color = input_channel_color[dev_input];
	ledColorSet(state_color);

	if(dev_input ==  WIRELESS){
		BT_POWER_OFF();
		sysDelayMs(150);
		BT_POWER_ON();
		sysDelayMs(200);
		BT_POWER_OFF();
		GPIO_CLR_PIN(WIRELESS_BT_SEL_PORT_PIN);
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
// 		playMusic(DISCONECTED_SOUND);
		Dwhp83_CuMu = 0x55aa;
		Dwhp83CuMuDetect();
		if(Dwhp83_CuMu == 0x55aa){
			Dwhp83Slave();
			sysDelayMs(200);
			startATask(pt2314_id, 2500, 0, dwph83Check);
		}//else
		//	Dwhp83EnterPairing();
		
		WB_paired_flag = 0;
	}else{
		stopATask(pt2314_id);
		GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
		//Dwhp83Standby();
		if(dev_input == BLUETOOTH){
			BT_POWER_ON();
			sysDelayMs(300);
			logEvent(BLUETOOTH_SOURE_IN);
			logEvent(bt_pairing);
			GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
			ccount = 0;
// 			playMusic(PAIRING_SOUND);
			if(nfc == 0){
				bt_check_flag = 1;
// 				GPIO_SET_PIN(BT_HANG_UP_PORT_PIN);
				startATask(pt2314_id, 1500, 0, btCheckDone);
			}
		}else{
// 			playMusic(DISCONECTED_SOUND);
			BT_POWER_OFF();
			sysDelayMs(150);
			BT_POWER_ON();
			sysDelayMs(200);
			BT_POWER_OFF();
			//logEvent(LINEIN_SOURE_IN);
			//logEvent(bt_power_down);
			GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
			BT_MATCH_FLAG = 0;
			WB_paired_flag = 0;
		}
	}
	sysDelayMs(5);
	unMute();
	ledColorSet(state_color);
	if(DECT_CALL() && dev_input == BLUETOOTH)
		state_color = LIGHT_BLUE_COLOR;
	
	changeInputVolumeUp();
}



/*-------------------------------------------------------------------------
 * @fucname:	 pt2314DataResume
 * @brief: 	 resume the state of the pt2314
 * @param: 	 none
 * @return: 	 none
 * @details: 	resume the state of the pt2314
 *-----------------------------------------------------------------------*/
u32 nfc_start = 0;
void pt2314DataResume()
{
	u32 nfc = 0;
	
	if(DECT_NFC() || nfc_start == 1){
		dev_input = BLUETOOTH;
		stamp = systick_counter;
		nfc = 1;
		nfc_start = 0;
		bt_check_flag = 0;
		btCheck();
	}
	 
	state_color = input_channel_color[dev_input];
	if(dev_input ==  WIRELESS){
		BT_POWER_OFF();
		sysDelayMs(300);                           //MA 20151130  change 3000ms to 300ms  when power on wireless mode 
		GPIO_CLR_PIN(WIRELESS_BT_SEL_PORT_PIN);
		//Dwhp83EnterPairing();
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
		startATask(pt2314_id, 10, 0, dwph83Check);
		
	}else{
		stopATask(pt2314_id);
		GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
		if(dev_input == BLUETOOTH){
// 			BT_POWER_ON();
// 			sysDelayMs(300);
			//BT_POWER_OFF();
			GPIO_SET_PIN(BT_AUX_CTRL_PORT_PIN);
			logEvent(BLUETOOTH_SOURE_IN);
			logEvent(bt_pairing);
			if(nfc == 0){
				bt_check_flag = 1; 
// 				GPIO_SET_PIN(BT_HANG_UP_PORT_PIN);
				startATask(pt2314_id, 1500, 0, btCheckDone);
			}
		}else{
			//BT_POWER_ON();
			//sysDelayMs(800);
			BT_POWER_OFF();
			//sysDelayMs(200);
			GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
			logEvent(bt_power_down);
			logEvent(LINEIN_SOURE_IN);
		}
	}
	if(dev_state == DEV_TEST)
		ledColorSet(SOE_COLOR);
	else
		ledColorSet(state_color);
}


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




