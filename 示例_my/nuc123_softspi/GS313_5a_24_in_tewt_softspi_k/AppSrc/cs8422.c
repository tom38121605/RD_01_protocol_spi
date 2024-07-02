/****************************************************************************
 * @file:		cs8422.c
 * @version:	V1.00
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

enum CS8422_INPUT_CHANNEL cs8422_input;
// enum CS8422_INPUT_CHANNEL cs8422_input_table[] = {
// 												BLUETOOTH,
// 												AUX, 	
// 												OPTICAL, 	
// 												USERDEF 	
// 										};


CS8422_CMDT cs8422_status;
static i2cPoolType cs8422_i2c_block;
Timer_ID cs8422_id = TASK_FULL;
Timer_ID sew58_id_CIC = TASK_FULL; // for change input channel 


/*-------------------------------------------------------------------------
 * @fucname:	 cs8422SendCmd
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void cs8422SendCmd(u8 reg, u8 data)
{
	initI2cDataBlock(	&cs8422_i2c_block, 
					CS8422_ADDR, 
					I2C_WR, 
					2,
					1,
					(u8*)&cs8422_status
				);
	
	cs8422_status.common.cmd = reg;
	cs8422_status.common.data = data;
	cs8422_status.state = I2C_BUSY;
	addI2cChain(&cs8422_i2c_block, 0);
	while(cs8422_status.state == I2C_BUSY)
			;
// 	SYS_UnlockReg();
// 	WDT_TIMER_RESET();   //Clear WDT timer
// 	SYS_LockReg();
}

// u8 cs8422GetCmd(u8 reg)   //--zz
// {
// 	initI2cDataBlock(	&cs8422_i2c_block, 
// 					CS8422_ADDR, 
// 					I2C_RD, 
// 					2,
// 					1,
// 					(u8*)&cs8422_status
// 				);
// 	
// 	cs8422_status.common.cmd = reg;
// 	cs8422_status.common.data = 0;
// 	cs8422_status.state = I2C_BUSY;
// 	addI2cChain(&cs8422_i2c_block, 0);
// 	while(cs8422_status.state == I2C_BUSY)
// 			;
// // 	SYS_UnlockReg();
// // 	WDT_TIMER_RESET();   //Clear WDT timer
// // 	SYS_LockReg();
// 	
// 	return cs8422_status.common.data;
// }

void cs8422SRCSourceSDIN()
{
	cs8422SendCmd(0x0A,0x00);
}


void cs8422SRCSourceAES3()
{
	cs8422SendCmd(0x0A,0x02);
}

void cs8422AESRX2()
{
	cs8422SendCmd(0x03,0xd4);
}

// void cs8422I2sLeftJust()  //--zz
// {
// 	cs8422SendCmd(0x0B,0x00);
// }

// void cs8422I2sStand()  //--zz
// {
// 	cs8422SendCmd(0x0B,0x08);
// }


// void cs8422AESRX3()  //--zz
// {
// 	cs8422SendCmd(0x03,0xfc);
// }


extern s32 sum_H;
extern u16 mu_addr;
void getMuAddr()  //--zz
{ 
//	u32 i;    //ma 20150629  

	CHOOSE_AUX_INPUT();
	cs8422SRCSourceSDIN();
	sysDelayMs(1500); 
	mu_addr = (sum_H>>8);
	if(mu_addr < 8){
		mu_addr = ((mu_addr+3)<<8);
	}

} 


 extern U32 R_C, B_C, G_C;
// extern mute();     //ma 20150624
extern u32 BT_MATCH_FLAG;
void cs8422InitAllConf(void)
{
	cs8422SendCmd(0x02,0x0);// cs8422 work normal
	cs8422SendCmd(0x03,0xc4);//cs84222 input RX2, input digital referenced to VA
	cs8422SendCmd(0x04,0x04);
	cs8422SendCmd(0x08,0x40);//set SRC/SAO MCLK be XTI
	cs8422SendCmd(0x09,0x08);//set SRC SOFT OPEN
	cs8422SendCmd(0x0A,0x00);//SDO1 USE SRC, and SDO1 UNMUTE, and SRC SOURCE IS SDIN
	cs8422SendCmd(0x0B,0x08);//SDIN is I2S data format
	cs8422SendCmd(0x0C,0x04);//SDO1 is I2S data formate and disable TDM


	getMuAddr();
	if(dev_input == BLUETOOTH){ 
		sysDelayMs(20);
		CHOOSE_BT_INPUT();
		cs8422SRCSourceSDIN(); 
	}else if(dev_input == AUX){
		//--btShutDown();
		sysDelayMs(20);
		BT_MATCH_FLAG = 0;
		//btShutDown();
		CHOOSE_AUX_INPUT();
		cs8422SRCSourceSDIN();
	}else if(dev_input == OPTICAL){
		//--btShutDown();
		sysDelayMs(20);
		BT_MATCH_FLAG = 0;
		//btShutDown(); 
		cs8422AESRX2();
		cs8422SRCSourceAES3();
	}else if(dev_input == USERDEF){
		//--btShutDown();
		sysDelayMs(20);
		BT_MATCH_FLAG = 0;

		Dwhp83CuMuDetect();
		if(Dwhp83_CuMu == 0X55AA){
			Dwhp83Slave();

		}
		
		CHOOSE_AUX_INPUT();
		CHOOSE_UNIFI_INPUT();
		cs8422SRCSourceSDIN();
		
	}
	//--state_color = input_channel_color[dev_input];
	//--ledColorSet(state_color);
	
}


u32 DETECT_USER_MODULE()
{	
	if(Dwhp83CuMuDetect()==0)
		return 0;
	else
		return 1;
// 	return 0;
}

u8 last_user_module = 4;       //ma 20150602
extern u8 COCO_KEY_BEFORE;    //ma 20150602
enum CS8422_INPUT_CHANNEL getNextInput()
{
// 	u8 coco_lost[3] = {0x00,0x00,0x00};
	if(DETECT_USER_MODULE()){
		last_user_module = total_input_channel;
		total_input_channel = 4;
// 		if(last_user_module != total_input_channel)
// 		{
// 		}
		switch(dev_input){
			case BLUETOOTH:
				return dev_input = USERDEF;      //MA 20150629  return enum
			case USERDEF:
				return dev_input = AUX;
			case AUX:
				return dev_input = OPTICAL;
			case OPTICAL:
				return dev_input = BLUETOOTH;
			default:
				break;
		};
	}else{
		last_user_module = total_input_channel;
		total_input_channel = 3;
		switch(dev_input){
			case BLUETOOTH:
				return dev_input = AUX;
			case AUX:
				return dev_input = OPTICAL;
			case OPTICAL:
				return dev_input = BLUETOOTH;
			default:
				dev_input = AUX;     //ma 20150731  if do not detect AUX
				break;
		}
	}

	return BLUETOOTH;      //ma 20150629  
}


void InputChannelCheck()
{	
	void seW58ChangInputVolumeCheck(void);
	
	if(cs8422_i2c_block.byte_ptr[0] == (u8)I2C_IDLE){
		
		return;
	}
	else if(cs8422_i2c_block.byte_ptr[0] == (u8)I2C_BUSY){
		startATask(sew58_id_CIC, 2, 0, InputChannelCheck); //i2c bus is used and wait 2ms
		return;	
	}
	else{ // i2c error
		changeInputChannel(); //i2c bus is used and wait 2ms
		return;
	}

}


extern u32 cc_blue_p;
void changeInputChannel()
{	
	//--mute();
	sysDelayMs(50);  
	
	if(dev_input == BLUETOOTH)
	{
		//--bt_paring_flag = 0;    //ma 20150528
		NFC_flag = 0;
		//--BT_OPEN();            //ma 20150526
		//--btStartUp();        //ma 20150520    BT+ON
		CHOOSE_BT_INPUT();
		cs8422SRCSourceSDIN();
		//--bt_power_flag = 0;        //MA 20150608    RESET  bt power flag 

	}
	else if(dev_input == AUX)
	{
//-- 		if(!bt_power_flag)           //ma 20150526   power off ,set bt power flag
// 		{	
// 			BT_OFF();            //ma 20150526			
// 			btShutDown();         //ma 20150520    BT+OFF
// 			sysDelayMs(20);
// 		}
		BT_MATCH_FLAG = 0;
		//btShutDown();
		CHOOSE_AUX_INPUT();
		cs8422SRCSourceSDIN();
		
//-- 		if(bt_change_name_mode)    
// 		{
// 				bt_change_name_mode = 0;
// 		}


	}
	else if(dev_input == OPTICAL)
	{

		BT_MATCH_FLAG = 0;
		//btShutDown(); 
		cs8422AESRX2();
		cs8422SRCSourceAES3();


	}
	else if(dev_input == USERDEF)
	{
//-- 		if(!bt_power_flag)           //ma 20150608  power off ,set bt power flag
// 		{	
// 			BT_OFF();      //ma 20150526
// 			btShutDown();    //ma 20150520    BT+OFF
// 			//btShutDown();
// 			sysDelayMs(100);   //ma 20150525  add some time to bt shutdown
// 			bt_power_flag =1;           //ma 20150526   power off ,set bt power flag
// 		}
		BT_MATCH_FLAG = 0;		
		Dwhp83CuMuDetect();
		if(Dwhp83_CuMu == 0X55AA)   // CU --zz 
		{
			if(!SB_flag)   //ma 20150515  no sb play         //ma 20150708   sb flag  no rx auto ly
			{
				Dwhp83Slave();     //ma 20150514  SB mode only CU
			}

		}

		CHOOSE_AUX_INPUT();     
		CHOOSE_UNIFI_INPUT();    
		cs8422SRCSourceSDIN();  		
	}
	
	//--state_color = input_channel_color[dev_input];
	//--ledColorSet(state_color);     //ma 20150606

	unMute();
	changeInputVolumeUp();
	DWHP83UnMute();     //ma 20150713
	
}


void cs8422Init()
{
	cs8422_id = getATask();
	sew58_id_CIC = getATask();
}


