/****************************************************************************
 * @file:		seW58.c
 * @version:	V1.00
 * @breif:		SE_W58_II control sourcefile
 * @date: 	2013/3/15
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

u32	w3d_volume = 31;
u32 	w3d_volume_back = 0;
u8	w3d_volume_lock = FALSE;
u8	w3d_lock = FALSE;
u8	w3d_volume_tbl[SE_MAX_VOLUME] = {
										0x00,
										0x01,
										0x02,
										0x03,
										0x04,
										0x05,
										0x06,
										0x07,
										0x08,
										0x09,
										0x0a,
										0x0c,
										0x0d,
										0x0e,
										0x0f,
										0x10,
										0x13,
										0x17,
										0x1b,
										0x20,
										0x26,
										0x2d,
										0x36,
										0x40,
										0x4c,
										0x5a,
										0x6c,
										0x80,
										0x98,
										0xb5,
										0xd7,
										0xff
									};

u8	w3d_sub_tbl[] = {
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xff,
					0xef,
					0xdf,
					0xcf,
					0xbf,
					0x80
				};

typedef struct {
	enum I2C_STATE state;
	W3DDATTYPE Volume;
	W3DDATTYPE Ambiance;
	W3DDATTYPE Balance;
	W3DDATTYPE Clarity;
	W3DDATTYPE SubOutGain;
	W3DDATTYPE Bypass;
	W3DDATTYPE BypassGain;
	W3DDATTYPE Mute;
	W3DDATTYPE ChannelCheck;
	W3DDATTYPE Status1func;
	W3DDATTYPE SurroundGain;
	W3DDATTYPE CenterGain;
}SE_W58_STRUCT; 
SE_W58_STRUCT se_w3d_status;

static u32 vlmtbl_index = 0;
static u8 w3d_cmd;
static FUNC_Timer_Task seW58send = NULL;
W3D_DATA_CELL w3d_volume_change = {I2C_IDLE,{w3d_cmd_Volume, 0}};
Timer_ID w58_id = TASK_FULL;
static i2cPoolType se_w58_i2c_block; // for seW58 set/get status and adc/dec volume
i2cPoolType se_w58_i2c_block_change_input; //for change input dec/adc volume

// indicate change_input_channel is in volume dowm(1) or up(2) ,
// and 0 means not in change input channel state
// 3 means turn down the volume before power off
// 4 means turn down the volume and don't power off
enum W3D_STATE change_input_state = VOLUME_UP_STATE;	


extern Timer_ID sew58_id_CIC;
extern u8 pt2314_switch_remap[];

/*-------------------------------------------------------------------------
 * @fucname:	 seW58SendCheck
 * @brief: 	 check whethe i2c data has finished
 * @param: 	 none
 * @return: 	 none
 * @details: 	 check whether i2c data has finished
 *-----------------------------------------------------------------------*/
void seW58SendCheck()
{
	if(se_w3d_status.state == I2C_IDLE){
		stopATask(w58_id);
	}else if(se_w3d_status.state == I2C_BUSY)
		startATask(w58_id, 2, 0, seW58SendCheck);
	else if(se_w3d_status.state == I2C_ERROR)
		if(seW58send)
			seW58send();
}

/*-------------------------------------------------------------------------
 * @fucname:	 pt2314ReleaseI2c
 * @brief: 	 release the i2c data source of pt2314
 * @param: 
 * @return: 
 * @details: 	 release the i2c data source of pt2314
 *-----------------------------------------------------------------------*/
void  seW58ResumeI2c()
{
	se_w58_i2c_block.index = se_w58_i2c_block.index_bak;
	se_w3d_status.state = I2C_IDLE;
	w3d_volume_lock = FALSE;
	w3d_lock = FALSE;
	
}


/*-------------------------------------------------------------------------
 * @fucname:	 seW58Getstatus
 * @brief: 	 get se_w58_II's data
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get se_w58_II's data
 *-----------------------------------------------------------------------*/
void seW58Getstatus()
{
	if(se_w3d_status.state == I2C_BUSY){
		startATask(w58_id, 2, 0, seW58Getstatus); //wait 2ms 
		return;
	}	
	
	initI2cDataBlock(	&se_w58_i2c_block, 
					SE_W58_II_ADDR, 
					I2C_RD, 
					sizeof(se_w3d_status) - 1,
					1,
					(u8*)&se_w3d_status
				);
	
	se_w3d_status.state = I2C_BUSY;
	addI2cChain(&se_w58_i2c_block, 0);
	seW58send = seW58Getstatus;
	if(w58_id != TASK_FULL)
		startATask(w58_id, 3, 0, seW58SendCheck);
}

/*-------------------------------------------------------------------------
 * @fucname:	 seW58Sendstatus
 * @brief: 	 set se_w58_II's data
 * @param: 	 none
 * @return: 	 none
 * @details: 	 set se_w58_II's data
 *-----------------------------------------------------------------------*/
static void _seW58SendAllstatus()
{
	if(se_w3d_status.state == I2C_BUSY ){
		startATask(w58_id, 2, 0, _seW58SendAllstatus); //wait 2ms 
		return;
	}	

	if(++w3d_cmd > 11){
		w3d_lock = FALSE;
		return;
	}
		
	se_w58_i2c_block.index =  (w3d_cmd<<1)+1;
	se_w58_i2c_block.index_bak =  (w3d_cmd<<1)+1;
	se_w58_i2c_block.bcount =  (w3d_cmd<<1)+2;

	se_w3d_status.state = I2C_BUSY;
	addI2cChain(&se_w58_i2c_block, 0);
	
	if(w58_id != TASK_FULL)
		startATask(w58_id, 1, 0, _seW58SendAllstatus);
}


void  seW58SendAllStatus()
{
	w3d_cmd = 0;
	w3d_lock = TRUE;
	
	initI2cDataBlock(	&se_w58_i2c_block, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,// sizeof(se_w3d_status) - 1
					1,
					(u8*)&se_w3d_status
				);
	
	addI2cChain(&se_w58_i2c_block, 0);
	
	_seW58SendAllstatus();
}
/*-------------------------------------------------------------------------
 * @fucname:	 seW58sendCmd
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void seW58sendCmd()
{
	if(w3d_lock == TRUE)
		return;
	
	if(se_w3d_status.state == I2C_BUSY ){
		startATask(w58_id, 2, 0, seW58sendCmd); //wait 2ms 
		return;
	}	
	
	initI2cDataBlock(	&se_w58_i2c_block, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					(w3d_cmd<<1)+2,// sizeof(se_w3d_status) - 1
					(w3d_cmd<<1)+1,
					(u8*)&se_w3d_status
				);
	
	se_w3d_status.state = I2C_BUSY;
	addI2cChain(&se_w58_i2c_block, 0);
	//seW58send = seW58sendCmd;
	
	//if(w58_id != TASK_FULL)
		//startATask(w58_id, 2, 0, seW58SendCheck);
}


/*-------------------------------------------------------------------------
 * @fucname:	 seW58SetAmbiance
 * @brief: 	 Set Ambiance
 * @param: 
 * @return: 
 * @details:     Set Ambiance
 *-----------------------------------------------------------------------*/
void seW58SetAmbiance(u8 amblance)
{
	se_w3d_status.Ambiance.data = amblance;
	w3d_cmd = w3d_cmd_Ambiance;
}


void seW58SetBalance(u8 balance)
{
	se_w3d_status.Balance.data = balance;
	w3d_cmd = w3d_cmd_Balance;
}

void seW58SetClarity(u8 clarity)
{
	se_w3d_status.Clarity.data = clarity;
	w3d_cmd = w3d_cmd_Clarity;
}

void seW58SetSubOutGain(u8 sub_out_gain)
{
	se_w3d_status.SubOutGain.data = sub_out_gain;
	w3d_cmd = w3d_cmd_SubOutGain;
}

void seW58SetBypass(u8 bypass)
{
	se_w3d_status.Bypass.data = (bypass)&1;
	w3d_cmd = w3d_cmd_Bypass;
} 

void seW58SetBypassGain(u8 by_pass_gain)
{
	se_w3d_status.BypassGain.data = by_pass_gain;
	w3d_cmd = w3d_cmd_BypassGain;
}

void seW58SetMute(u8 mute)
{
	se_w3d_status.Mute.data = mute;
	w3d_cmd = w3d_cmd_Mute;
}

void seW58SetChannelCheck(u8 channel_check)
{
	se_w3d_status.ChannelCheck.data = channel_check;
	w3d_cmd = w3d_cmd_ChannelCheck;
}

void seW58SetStatus1func(u8 status1_func)
{
	se_w3d_status.Status1func.data = status1_func;
	w3d_cmd = w3d_cmd_Status1func;
}


void seW58SetSurroundGain(u8 surround_gain)
{
	se_w3d_status.SurroundGain.data = surround_gain;
	w3d_cmd = w3d_cmd_SurroundGain;
}


void seW58SetVolume(u8 volume)
{
	se_w3d_status.Volume.data = w3d_volume_tbl[volume];
	w3d_cmd = w3d_cmd_Volume;
}

void seW58DefaultVolume()                     //ma 20150713
{
	se_w3d_status.Volume.data = SE_W58_DEFAULT_VOLUME;
	w3d_cmd = w3d_cmd_Volume;
}
u32  ismute()
{
	return se_w3d_status.Mute.data;
}

/*-------------------------------------------------------------------------
 * @fucname:	 w3dSwitch
 * @brief: 	 open or close 3d
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void w3dSwitch()
{
	if(w3d_lock == FALSE){
		changeInputVolumeDown(W3D_SWITCH_STATE);
	
		if(se_w3d_status.Bypass.data == 0)
			GPIO_SET_PIN(PC8);
		else
			GPIO_CLR_PIN(PC8);
	}
}


/*-------------------------------------------------------------------------
 * @fucname:	 testOutputSpeakers
 * @brief: 	 output the speakers one by one 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void testOutputSpeakers() 
{
	if(w3d_lock == FALSE){
		seW58SetChannelCheck(channelspeaker[test_channel]);
		seW58sendCmd();
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 w3d_mute
 * @brief: 	 mute or open the se_w58_ii
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void w3d_mute()
{
	if(w3d_lock == FALSE)
		if(se_w3d_status.Mute.data == 0){
			changeInputVolumeDown(MUTE_STATE);
		}else{
			changeInputVolumeDown(UN_MUTE_STATE);
		}

}

u32 isMute()
{
	return se_w3d_status.Mute.data;
}

/*-------------------------------------------------------------------------
 * @fucname:	 seW58Init
 * @brief: 	 get se_w58_II task id
 * @param: 	 none
 * @return: 	 none
 * @details: 	 get se_w58_II task id
 *-----------------------------------------------------------------------*/
void seW58Init()
{
	w58_id = getATask();
	
	se_w3d_status.state = I2C_IDLE;
	se_w3d_status.Volume.cmd = w3d_cmd_Volume;
	se_w3d_status.Ambiance.cmd = w3d_cmd_Ambiance;
	se_w3d_status.Balance.cmd = w3d_cmd_Balance;
	se_w3d_status.Clarity.cmd = w3d_cmd_Clarity;
	se_w3d_status.SubOutGain.cmd = w3d_cmd_SubOutGain;
	se_w3d_status.Bypass.cmd = w3d_cmd_Bypass;
	se_w3d_status.BypassGain.cmd = w3d_cmd_BypassGain;
	se_w3d_status.Mute.cmd = w3d_cmd_Mute;
	se_w3d_status.ChannelCheck.cmd = w3d_cmd_ChannelCheck;
	se_w3d_status.Status1func.cmd = w3d_cmd_Status1func;
	se_w3d_status.SurroundGain.cmd = w3d_cmd_SurroundGain;
	se_w3d_status.CenterGain.cmd = w3d_cmd_CenterGain;
}

/*-------------------------------------------------------------------------
 * @fucname:	 seW58VolumeCheck
 * @brief: 	 check whethe i2c data has finished
 * @param: 	 none
 * @return: 	 none
 * @details: 	 check whether i2c data has finished
 *-----------------------------------------------------------------------*/
void seW58VolumeCheck()
{
	if(se_w3d_status.state == I2C_IDLE){
		if(se_w3d_status.Volume.data > w3d_volume_tbl[w3d_volume]){
			se_w3d_status.Volume.data--;
			addI2cChain(&se_w58_i2c_block, 0);
			startATask(w58_id, 2, 0, seW58VolumeCheck);
		}else if(se_w3d_status.Volume.data < w3d_volume_tbl[w3d_volume]){
			se_w3d_status.Volume.data++;
			addI2cChain(&se_w58_i2c_block, 0);
			startATask(w58_id, 2, 0, seW58VolumeCheck);
		}else{ // equal 
			seW58SetSubOutGain(w3d_sub_tbl[w3d_volume]);
			seW58sendCmd();
			led_flash = 4;		// DARK the led 4*30ms	
		}
	}else if(se_w3d_status.state == I2C_BUSY)
		startATask(w58_id, 1, 0, seW58VolumeCheck);
	else if(se_w3d_status.state == I2C_ERROR){
		
		if(se_w3d_status.Volume.data > w3d_volume_tbl[w3d_volume]){
			addI2cChain(&se_w58_i2c_block, 0);
			startATask(w58_id, 1, 0, seW58VolumeCheck);
		}else if(se_w3d_status.Volume.data < w3d_volume_tbl[w3d_volume]){
			addI2cChain(&se_w58_i2c_block, 0);
			startATask(w58_id, 1, 0, seW58VolumeCheck);
		}
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 seW58DecVolume
 * @brief: 	 dec volume by control seW58
 * @param: 	 none
 * @return: 	 none
 * @details: 	 dec volume by control seW58
 *-----------------------------------------------------------------------*/
void seW58DecVolume()
{
	u8 volume;

	if(w3d_volume_lock || w3d_lock)
		return;
	
	if(se_w3d_status.state == I2C_BUSY){
		startATask(w58_id, 2, 0, seW58DecVolume); //wait 2ms 
		return;
	}	

	if(se_w3d_status.Mute.data == 1){
		w3d_mute();
		se_w3d_status.Volume.data = w3d_volume_tbl[w3d_volume];
		return;
	}
		
	if(w3d_volume > 0){
		volume = w3d_volume_tbl[w3d_volume];
		w3d_volume--;
	}else
		return;
	
	initI2cDataBlock(	&se_w58_i2c_block, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&se_w3d_status
				);
	
	se_w3d_status.state = I2C_BUSY;
	se_w3d_status.Volume.cmd = w3d_cmd_Volume;
	se_w3d_status.Volume.data = --volume;
	
	addI2cChain(&se_w58_i2c_block, 0);
	
	if(w58_id != TASK_FULL)
		startATask(w58_id, 2, 0, seW58VolumeCheck);
	
}

/*-------------------------------------------------------------------------
 * @fucname:	 seW58AddVolume
 * @brief: 	 add volume by control seW58
 * @param: 	 none
 * @return: 	 none
 * @details: 	 add volume by control seW58
 *-----------------------------------------------------------------------*/
void seW58AddVolume()
{
	u8 volume;

	if(w3d_volume_lock || w3d_lock)
		return;
	
	if(se_w3d_status.state == I2C_BUSY){
		startATask(w58_id, 2, 0, seW58AddVolume); //wait 2ms 
		return;
	}	

	
	if(se_w3d_status.Mute.data == 1){
		w3d_mute();
		se_w3d_status.Volume.data = w3d_volume_tbl[w3d_volume];
		return;
	}
	
	if(w3d_volume < SE_MAX_VOLUME -1){
		volume = w3d_volume_tbl[w3d_volume];
		w3d_volume++;
	}else
		return;

	
	initI2cDataBlock(	&se_w58_i2c_block, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&se_w3d_status
				);
	
	se_w3d_status.state = I2C_BUSY;
	se_w3d_status.Volume.cmd = w3d_cmd_Volume;
	se_w3d_status.Volume.data = ++volume;
	
	addI2cChain(&se_w58_i2c_block, 0);
	
	if(w58_id != TASK_FULL)
		startATask(w58_id, 1, 0, seW58VolumeCheck);
	
}



static __inline void turnDownVolume()
{
	if(w3d_volume_change.state == I2C_IDLE){
		
		for(; vlmtbl_index > 0; vlmtbl_index-- ){

			if(w3d_volume_change.w3d_data.data > w3d_volume_tbl[vlmtbl_index-1]
				&& w3d_volume_change.w3d_data.data <= w3d_volume_tbl[vlmtbl_index ]
			){
				w3d_volume_change.w3d_data.data = w3d_volume_tbl[vlmtbl_index-1];
				break;
			}		
			if(vlmtbl_index == 1)
				vlmtbl_index = SE_MAX_VOLUME;
		}
		if(vlmtbl_index <= 1)
				vlmtbl_index = SE_MAX_VOLUME-1;
		addI2cChain(&se_w58_i2c_block_change_input, 1);
	}
	startATask(sew58_id_CIC, 5, 0, seW58ChangInputVolumeCheck);
}

static __inline void turnUpVolume()
{
	if(w3d_volume_change.state == I2C_IDLE){
		
		if(vlmtbl_index >= SE_MAX_VOLUME-1)
				vlmtbl_index = 0;
		
		for(; vlmtbl_index < SE_MAX_VOLUME-1; vlmtbl_index++ ){
			if(w3d_volume_change.w3d_data.data >= w3d_volume_tbl[vlmtbl_index]
				&& w3d_volume_change.w3d_data.data < w3d_volume_tbl[vlmtbl_index +1]){
				w3d_volume_change.w3d_data.data = w3d_volume_tbl[vlmtbl_index+1];
				break;
			}		

		}
		
		addI2cChain(&se_w58_i2c_block_change_input, 1);
	}
	startATask(sew58_id_CIC, 5, 0, seW58ChangInputVolumeCheck);
}

void seW58FlasfLed()
{
	led_flash = 12;
	startATask(w58_id, 720, 0, seW58FlasfLed);
}

void stopFlashLed()
{
	stopATask(w58_id);
}

void unMute()
{
	seW58SetMute(0);
	seW58sendCmd();
	sysDelayMs(3); // wait for mute cmd sent 
	stopFlashLed();
}


void mute()
{
	seW58SetMute(1);
	seW58sendCmd();
	sysDelayMs(3); // wait for mute cmd sent 
}


/*-------------------------------------------------------------------------
 * @fucname:	 seW58ChangInputVolumeCheck
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void seW58ChangInputVolumeCheck()
{
	if(w3d_volume_change.state == I2C_IDLE){

		switch(change_input_state){

			case INPUT_CHANGE_STATE:

				if(w3d_volume_change.w3d_data.data == 0 ||
					se_w3d_status.Mute.data == 1){
						ampSetVolume(amp_volume);
						w3d_volume_change.w3d_data.data = 0;
						startATask(sew58_id_CIC, 2, 0, changeInputChannel);
	
				}
				else{
					turnDownVolume();
				}
				
				break;

			case VOLUME_UP_STATE:
				
				if(w3d_volume_change.w3d_data.data < w3d_volume_tbl[w3d_volume]){
					turnUpVolume();
				}else{
					stopATask(sew58_id_CIC);
					seW58SetSubOutGain(w3d_sub_tbl[w3d_volume]);
					seW58sendCmd();
					w3d_volume_change.w3d_data.data = 0;
					w3d_volume_lock = FALSE;
					change_input_state = W3D_IDLE_STATE;
				}
				
				break;
				
			case VOLUME_DOWN_STATE:
				
				 if(w3d_volume_change.w3d_data.data > w3d_volume_tbl[w3d_volume]){
					turnDownVolume();
				}else{
					stopATask(sew58_id_CIC);
					seW58SetSubOutGain(w3d_sub_tbl[w3d_volume]);
					seW58sendCmd();
					w3d_volume_change.w3d_data.data = 0;
					w3d_volume_lock = FALSE;
					change_input_state = W3D_IDLE_STATE;
				}
				
				break;

			case VOLUME_RECOVER:
			case POWER_OFF_STATE:
			case VOLUME_DOWN_UP:
				if(w3d_volume_change.w3d_data.data == 0){
					if(change_input_state == POWER_OFF_STATE){
						seW58SetMute(1);
						seW58sendCmd();
						powerOff();
					}else{
						ampSetVolume(amp_volume);
						change_input_state = VOLUME_UP_STATE;
						turnUpVolume();
					}
					w3d_volume_lock = FALSE;
					//change_input_state = W3D_IDLE_STATE;
					
				}else {
					turnDownVolume();
				}
				
				break;
			
				
			case MUTE_STATE:

				if(w3d_volume_change.w3d_data.data == 0){
					
					w3d_lock = FALSE;
					seW58SetSubOutGain(w3d_sub_tbl[0]);
					seW58sendCmd();
					sysDelayMs(3); // wait for subgain cmd sent 
					seW58SetMute(1);
					seW58sendCmd();
					
					w3d_volume_lock = FALSE;
					startATask(w58_id, 2, 0, seW58FlasfLed);
					change_input_state = W3D_IDLE_STATE;
				}
				else{
					w3d_lock = TRUE;
					turnDownVolume();
				}

				break;

			case UN_MUTE_STATE:

				if(w3d_volume_change.w3d_data.data == 0){
					w3d_lock = FALSE;
					seW58SetMute(0);
					seW58sendCmd();	 
					w3d_lock = TRUE; // to avoid quick switch between MUTE_STATE and UN_MUTE_STATE
					sysDelayMs(2);	// wait for mute cmd sent 
					turnUpVolume();
				}else if(w3d_volume_change.w3d_data.data < w3d_volume_tbl[w3d_volume]){
					turnUpVolume();
				}else{
					stopATask(w58_id);
					w3d_lock = FALSE;
					seW58SetSubOutGain(w3d_sub_tbl[w3d_volume]);
					seW58sendCmd();
					sysDelayMs(3); // wait for subgain cmd sent 
					w3d_volume_change.w3d_data.data = 0;
					w3d_volume_lock = FALSE;
					change_input_state = W3D_IDLE_STATE;
				}
				
				break;
				
			case W3D_SWITCH_STATE:

				if(w3d_volume_change.w3d_data.data == 0 ||
					se_w3d_status.Mute.data == 1){
					//led_flash = 4; // flash the led
					w3d_lock = FALSE;
					w3d_volume_change.w3d_data.data = 0;
					seW58SetBypass(~se_w3d_status.Bypass.data);
					seW58sendCmd();
					sysDelayMs(3); // wait for mute cmd sent 	
					unMute();
					if(se_w3d_status.Mute.data == 0){
						change_input_state = VOLUME_UP_STATE;
						startATask(sew58_id_CIC, 3, 0, seW58ChangInputVolumeCheck);
					}
				}
				else{
					w3d_lock = TRUE;
					turnDownVolume();
				}

				break;
				
			default:
				break;
				
		}
	}
	else if(w3d_volume_change.state == I2C_BUSY)
		startATask(sew58_id_CIC, 1, 0, seW58ChangInputVolumeCheck);
	else{
		initI2cDataBlock(	&se_w58_i2c_block_change_input, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&w3d_volume_change
				);
		addI2cChain(&se_w58_i2c_block_change_input, 0);
		startATask(sew58_id_CIC, 2, 0, seW58ChangInputVolumeCheck);
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 getChangeVolume
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void getChangeVolume()
{	
	switch(change_input_state){
		case INPUT_CHANGE_STATE: 
		case POWER_OFF_STATE:
		case W3D_SWITCH_STATE:
		case MUTE_STATE:
		case	VOLUME_DOWN_UP:
		case VOLUME_RECOVER:
			if(w3d_volume_change.w3d_data.data)
				return;
			else{
				w3d_volume_change.w3d_data.data = w3d_volume_tbl[w3d_volume];
				w3d_volume_lock = TRUE;
			}
			break;
		default:
			break;
	}
}

/*-------------------------------------------------------------------------
 * @fucname:	 changeInputVolumeDown
 * @brief: 	 dec the volume to lowesr before change input 
 * @param: 	 u32 state
 * @return: 	 none 
 * @details: 	 dec the volume to lowesr before change input 
 *-----------------------------------------------------------------------*/
void changeInputVolumeDown(enum W3D_STATE state)
{
	change_input_state = state;

	getChangeVolume();
	
	initI2cDataBlock(	&se_w58_i2c_block_change_input, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&w3d_volume_change
				);

	startATask(sew58_id_CIC, 2, 0, seW58ChangInputVolumeCheck);
}

/*-------------------------------------------------------------------------
 * @fucname:	 changeInputVolumeUp
 * @brief: 	 add the volume to lowesr after change input 
 * @param: 	 none
 * @return: 	 none 
 * @details: 	 add the volume to lowesr after change input 
 *-----------------------------------------------------------------------*/
void changeInputVolumeUp()
{
	change_input_state = VOLUME_UP_STATE;
	initI2cDataBlock(	&se_w58_i2c_block_change_input, 
					SE_W58_II_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&w3d_volume_change
				);
	
	seW58ChangInputVolumeCheck();
}


