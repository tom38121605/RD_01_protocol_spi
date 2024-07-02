/****************************************************************************
 * @file:		appkey.c
 * @version:	V1.00
 * @breif:	do key event which was sent by ir_key or adc_key 
 * @date: 	2013/3/01
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

extern u32 BT_MATCH_FLAG;
extern u32 volume_key_delay;
extern u32 flash_music_index;
Timer_ID appkey_id;
u32 dc_auto_down_flag = 0;
u8 bt_disable_reconnect = 1;      //ma 20150520   default  no btreconect
u8 SB_flag = 0x00;
// u8 SB_play_flag = 0;             //ma 20150515 add  SB play flag
u8 bt_power_flag = 1;             //ma 20150526   add bt power flag to enable pairing
u8 bt_paring_flag =0;           //ma 20150527 
extern u8 COCO_KEY_BEFORE;    //ma 20150602
extern u32 cc_blue_p;         //ma 20150609 

u8 TX_sound_flag = 0;     //ma 20150817 add  master transmission sound 
/*-------------------------------------------------------------------------
 * @fucname:	 delStandbytkey
 * @brief: 	 deal key event when dev is in standby mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in standby mode 
 *-----------------------------------------------------------------------*/
extern enum KEYWORD key_buf;
void delStandbytkey(enum KEYWORD keyword)
{
	/*****add counter to 1s power on ****/
// 	static u32 standby_key_count = 0;    //ma 20150702   



			switch(keyword){
				case POWER_KEY:
					ir_key_flag   =0;                 //ma 20150728 
					outStandby();                   //ma 20150727  power key can power on any time 
				break;
				case CHANNEL_SWITCH_KEY: 
				case INC_KEY:
				case DEC_KEY:
				case MULTIROOM_KEY:
					if((dc_auto_down_flag == 0)&&(!ir_key_flag))    //ma 201507 28  if not ir key 
					{
 						ir_key_flag = 0;              //ma 20150728   
						outStandby();                //ma 20150727  other keys can power on in no dc auto down state
					}
					else if(ir_key_flag)
						ir_key_flag = 0;
					break;
				default:
					break;
			}


}

static i2cPoolType eeprom_i2c_block;         //ma 20150612   add eeprom store 
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
  while(eeprom_i2c_buf[0] == I2C_BUSY)
    ;
  return (eeprom_i2c_buf[0] == I2C_IDLE)
    ;
}
void setConfig()
{
// 	u32 devce_config = dwhp83_type;                //ma  20150513  lower Byte 
// 	u32 device_config2 = 0x50000000;                    //ma  20150513  add  SB_config to eeprom
	u8 dev_input_number = 0;     //ma 20150630  add dev_input_number to remember 
	if(dev_input == BLUETOOTH)
		dev_input_number = 0;
	else if(dev_input == USERDEF)
		dev_input_number = 1;
	else if(dev_input == AUX)
		dev_input_number = 2;
	else if(dev_input == OPTICAL)
		dev_input_number = 3;
	
	writeeeprom(0x5A,0x01);
  sysDelayMs(30);
	writeeeprom(dwhp83_type,0x02);
  sysDelayMs(30);
	writeeeprom(amp_volume,0x03);
  sysDelayMs(30);
	writeeeprom(dev_input_number,0x04);
  sysDelayMs(30);
	writeeeprom(dc_auto_down_flag,0x05);
  sysDelayMs(30);
	writeeeprom(SB_flag,0x06);
  sysDelayMs(30);
	writeeeprom(bt_hfp_flag,0x07);
  sysDelayMs(30);

	I2S_DISABLE_INT(I2S_INT_RXTH);
	I2S_CLR_RXFIFO();
	I2S_CLR_TXFIFO();
	i2sStart(); 
	
}

void getConfig()
{
// 	u32 k,q;         //ma 20150513 add q  for sb mode
 	u8 k;	
	u8 dev_input_number = 0;     //ma 20150630  add dev_input_number to remember 
	k= readeeprom(0x01);
	sysDelayMs(5);
	if(k != 0x5A)
	{
		dwhp83_type = 0;
		amp_volume =	24;
		dev_input_number = 0;
		dc_auto_down_flag = 0;
		SB_flag = 0x00;
		bt_hfp_flag = 0;
	}
	else
	{
		dwhp83_type = readeeprom(0x02); sysDelayMs(5);
		amp_volume = readeeprom(0x03); sysDelayMs(5);
		dev_input_number = readeeprom(0x04); sysDelayMs(5);
		dc_auto_down_flag = readeeprom(0x05); sysDelayMs(5);
		SB_flag = readeeprom(0x06); sysDelayMs(5);
		bt_hfp_flag = readeeprom(0x07); sysDelayMs(5);
	}
	
	if(dev_input_number == 0)
		dev_input = BLUETOOTH;
	else if(dev_input_number == 1)
		dev_input = USERDEF;
	else if(dev_input_number == 2)
		dev_input = AUX;
	else if(dev_input_number == 3)
		dev_input = OPTICAL;
}


void asCenter()
{	
// 	mute();
// 	sysDelayMs(100);
	dwhp83_type = VOLUME_C;
// 	setConfig();
	writeeeprom(dwhp83_type,0x02);
  sysDelayMs(30);
// 	sysDelayMs(100);               //ma 20150825  del useless delay
// 	unMute();        
}


void asStreao()
{
	mute();
	sysDelayMs(100);
	dwhp83_type = VOLUME_STREO;
// 	setConfig();
	writeeeprom(dwhp83_type,0x02);
  sysDelayMs(30);
	sysDelayMs(100);             
	unMute();
}

/*-------------------------------------------------------------------------
 * @fucname:	 delStartkey
 * @brief: 	 deal key event when dev is in start mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in start mode 
 *-----------------------------------------------------------------------*/
//  extern u32 Dwhp83_flag;      //20150612
extern DWHP83_STATE dwhp83_status;
extern volatile u32 Dwhp83_CuMu;
extern enum KEYWORD key_buf_adc;
extern u32  ismute(void);
u8 cm = 0;
// u8 mute_back_volume = 0;          //ma 20150825 cut useless value 
// u8 set_cu_flag = 0;       //ma 20150714       //MA 20150813
// u8 flash_music_test = 1;   //ma 20150615     //ma 20150901
u8 mic_mute_flag = 0;    //ma 20150528

extern u16 mu_addr;               //ma 20150716
enum CS8422_INPUT_CHANNEL dev_input_before_change = BLUETOOTH;     //ma 20150708 
/********change channal set color  at once  and  after 1sec handle*********/
u8 change_channal_flag = 0;     //ma 20150708  add change_channal_flag 
// u8 message_test_flag = 1;    //ma 201500710  
u8 play_key_flag = 0;          //ma 20150714  
u8 soundbar_multiroom_flag = 0;         //ma 20150717
// u8  tx_rx_test_mode = 1;             //ma 20150716       add tx rx test mode  to send message to TX to change to RX   just for testing 
// u8 HFP_factory_test_flag = 1;           //ma 20150724
u8 Core_tx_flag = 0;              //ma 20150813  add Core tx flag mute time 

void delStartkey(enum KEYWORD keyword)
{
// 	static u32 cp = 49;  //ma 20150629  add cp for test flash 
	static u32 cc = 0, ca = 0, cd = 0, ch = 0, cn = 0,cbt = 6,cf = 9; //ma 20150527  add ch for HFP_flag    //ma 20150616  add  bt name change add
	   //ma 20150616 add cbt  for  bt name change count     //ma 20150617   add cf  for factory mode 
// 	u8 coco_sb[3] = {0x05,0x05,0x05};       //ma 20150512 SB   mode  COCO change     //ma 20150825  del useless value 
	u32 i = 0;
	if(keyword != NONE_KEY){
		play_key_flag = 1;      //ma 20150714
		BAT_STATE_DECT_DELAY_COUNT = 0;
// 		led_battery_flash = 0;              //ma 20150825  dummy useless value 
// 		if(ismute())                             //MA 20150929 not anykey can unmute 
// 			w3d_mute();
	}
	
	switch(keyword){
		
		/******lab  mode krey*******/       //ma 20150720  
	  	case LAB_BT_MODE:                    //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
				if(dev_input != BLUETOOTH)     //ma 20150803   if state is not at bluetooth mode ,change it 
				{
					dev_input = BLUETOOTH;
					changeInputVolumeDown(INPUT_CHANGE_STATE);
					state_color = input_channel_color[dev_input];
					ledColorSet(state_color);
				}
				if(test_channel != 5)
				{
					test_channel = 5;
					testOutputSpeakers();
				}
// 				led_flash = 4;                  //ma 20150923 do not flash  when factory mode LAB_key  pressed                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
				break;
	
			case LAB_BT_PAIRING_MODE:                  //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
				if(dev_input != BLUETOOTH)         //ma 20150803   if state is not at bluetooth mode ,change it 
				{
					dev_input = BLUETOOTH;
					changeInputVolumeDown(INPUT_CHANGE_STATE);
					state_color = input_channel_color[dev_input];
					ledColorSet(state_color);
				}
				bt_dis_last_connect();    //ma 20150527
				sysDelayMs(20);
				btPairing();
				sysDelayMs(20);
				if(test_channel != 5)
				{
					test_channel = 5;
					testOutputSpeakers();
				}
// 				led_flash = 4;            //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_OPTICAL_MODE:                         //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
				if(dev_input != OPTICAL)                    //ma 20150803   if state is not at OPTICAL mode ,change it 
				{
					dev_input = OPTICAL;
					changeInputVolumeDown(INPUT_CHANGE_STATE);
					state_color = input_channel_color[dev_input];
					ledColorSet(state_color);
				}
				if(test_channel != 5)
				{
					test_channel = 5;
					testOutputSpeakers();
				}
// 				led_flash = 4;                //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_LINE_IN_MODE:                             //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
				if(dev_input != AUX)                         //ma 20150803   if state is not at AUX mode ,change it
				{
					dev_input = AUX;
					changeInputVolumeDown(INPUT_CHANGE_STATE);
					state_color = input_channel_color[dev_input];
					ledColorSet(state_color);
				}
				if(test_channel != 5)
				{
					test_channel = 5;
					testOutputSpeakers();
				}
// 				led_flash = 4;                 //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_NET_WORK_RX_MODE:                         //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
				if(dev_input != USERDEF)
				{
					dev_input = USERDEF;
					changeInputVolumeDown(INPUT_CHANGE_STATE);
					state_color = input_channel_color[dev_input];
					ledColorSet(state_color);
				}
				if(Dwhp83CuMuDetect())      //ma 20150803 
				{
					if(Dwhp83_CuMu == !MU)
					{
						changetoWirelessSlave();
						BT_OFF();     					 	//ma 20150731   DWHP83 RX MODE, turn off bt    
						btShutDown();    					//ma 20150731   DWHP83 RX MODE, turn off bt   
						sysDelayMs(2000);
					}
				}
				if(test_channel != 5)
				{
					test_channel = 5;
					testOutputSpeakers();
				}
// 				led_flash = 4;                    //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_VOL_MAX:                                 //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
        amp_volume = 31;
// 				led_flash = 4;                        //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				ampSetVolume(amp_volume);
				DWP83CuCV();
				if( dwhp83_type != VOLUME_STREO )
					Dwhp83_flag |= 0x2;
				break;
			case LAB_SPEAK_1:                             //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
// 				ledColorSet(WHITE_COLOR);             //ma 20150803  it is not necessary
				test_channel = 3;
				testOutputSpeakers();
// 				led_flash = 4;                    //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_SPEAK_2:                              //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
//         ledColorSet(WHITE_COLOR);            //ma 20150803  it is not necessary
				test_channel = 1;
				testOutputSpeakers();
// 				led_flash = 4;                    //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_SPEAK_3:                                   //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
//         ledColorSet(WHITE_COLOR);             //ma 20150803  it is not necessary
				test_channel = 2;
				testOutputSpeakers();
// 				led_flash = 4;                          //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_SPEAK_4:                               //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
//         ledColorSet(WHITE_COLOR);             //ma 20150803  it is not necessary
				test_channel = 0;
				testOutputSpeakers();
// 				led_flash = 4;                           //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_SPEAK_5:                                //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
//         ledColorSet(WHITE_COLOR);             //ma 20150803  it is not necessary
				test_channel = 4;
				testOutputSpeakers();
// 				led_flash = 4;                             //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
			case LAB_SPEAK_ALL:                                 //MA 20150817
				if(!factory_mode_flag)            //ma 20150928   only in factory mode do this 
					return;
//         ledColorSet(WHITE_COLOR);              //ma 20150803  it is not necessary
				test_channel = 5;
				testOutputSpeakers();
// 				led_flash = 4;                             //ma 20150923 do not flash  when factory mode LAB_key  pressed   
				break;
		
		/*******lab test key  end *****/
		
		case FACTORY_KEY_1:                                    //MA 20150716    left
			
		
		if(ismute())                             //MA 20150929 not anykey can unmute 
		{
			w3d_mute();
			return;
		}
// 		/*******TX_RX_test *********/
// 		if(tx_rx_test_mode)			            //ma 20150716   
// 		{
// 			 led_flash = 5;
// 			DWHP83SendCmd(0x02,0,mu_addr);
// 			return ;
// 		}
		/*******test end **********/
							/*****factory mode *****///ma 20150617
					if(factory_mode_flag)
					{
						if(cf >0)
						{
							cf--;
						}
						else if(cf == 0)
							cf = 8;

						if(0 == cf)
						{
							ledColorSet(GREEN_COLOR);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						else if(1 == cf)
						{
							ledColorSet(BLUE_COLOR);
							bt_version_read();
							sysDelayMs(100);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						else if(2 == cf)
						{
							ledColorSet(PURPLE_COLOR);
							if(Dwhp83CuMuDetect())
							{
								Dwhp83ReadVersion();      //ma 20150625
								sysDelayMs(100);
							}
							if(WL_VERSION_NUMBER[0] == 0)
								play_music_flag = 0;
							else	play_music_flag = cf *10 +1;                //ma 20150708  
						
							key_time_count = 0;
						}
						else if(3 == cf)
						{
							ledColorSet(YELLOW_COLOR);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						if(4 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 3;
							testOutputSpeakers();
							play_music_flag = 0x82;
							key_time_count  =0;       //ma 20150617

						}
						else if(5 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 1;
							testOutputSpeakers();
							play_music_flag = 0x84;
							key_time_count  =0;       //ma 20150617
						}
						else if(6 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 2;
							testOutputSpeakers();
							play_music_flag = 0x88;
							key_time_count  =0;       //ma 20150617
						}
						else if(7 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 0;
							testOutputSpeakers();
							play_music_flag = 0x90;
							key_time_count  =0;       //ma 20150617
						}
						else if(8 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 4;
							testOutputSpeakers();
							play_music_flag = 0xc0;
							key_time_count  =0;       //ma 20150617
						}
						else
						{
							test_channel = 5;
							testOutputSpeakers();
							sysDelayMs(100);
						}

// 						key_time_count  =0;       //ma 20150617
// 						return;
					}


				break;
		case FACTORY_KEY_2:                                                //MA 20150716    left
// 				if(message_test_flag)
// 				{
// 					TX_SB_alive();
// 					return;
// 				}
// 							/*******TX_RX_test *********/
// 					if(tx_rx_test_mode)			            //ma 20150716   
// 					{
// 							Dwhp83Master();
// 							Dwhp83_flag = 0;
// 							led_flash = 5;
// 							sysDelayMs(1000);  
// 						return ;
// 					}
				if(ismute())                             //MA 20150929 not anykey can unmute 
				{
					w3d_mute();
					return;
				}
					/*****factory mode *****///ma 20150617
					if(factory_mode_flag)
					{
						if(cf <8)
						{
							cf++;
						}
						else if(cf == 8)
							cf = 0;
						else cf = 0;
						if(0 == cf)
						{
							ledColorSet(GREEN_COLOR);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						else if(1 == cf)
						{
							ledColorSet(BLUE_COLOR);
							bt_version_read();
							sysDelayMs(100);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						else if(2 == cf)
						{
							ledColorSet(PURPLE_COLOR);
							if(Dwhp83CuMuDetect())
							{
								Dwhp83ReadVersion();      //ma 20150625
								sysDelayMs(100);
							}
							if(WL_VERSION_NUMBER[0] == 0)
								play_music_flag = 0;
							else	play_music_flag = cf *10 +1;        // ma 20150703   play_music_flag = 21
						}
						else if(3 == cf)
						{
							ledColorSet(YELLOW_COLOR);
							play_music_flag = cf *10 +1;
							key_time_count = 0;
						}
						if(4 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 3;
							testOutputSpeakers();
							play_music_flag = 0x82;
							key_time_count  =0;       //ma 20150617

						}
						else if(5 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 1;
							testOutputSpeakers();
							play_music_flag = 0x84;
							key_time_count  =0;       //ma 20150617
						}
						else if(6 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 2;
							testOutputSpeakers();
							play_music_flag = 0x88;
							key_time_count  =0;       //ma 20150617
						}
						else if(7 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 0;
							testOutputSpeakers();
							play_music_flag = 0x90;
							key_time_count  =0;       //ma 20150617
						}
						else if(8 == cf)
						{
							ledColorSet(WHITE_COLOR);
							test_channel = 4;
							testOutputSpeakers();
							play_music_flag = 0xc0;
							key_time_count  =0;       //ma 20150617
						}
						else
						{
							test_channel = 5;
							testOutputSpeakers();
							sysDelayMs(100);
						}
// 						key_time_count  =0;       //ma 20150617
// 						return;
					}
				break;
// 		case FACTORY_KEY_3:
// 			playMusic(NUMBER_3);
// 			sysDelayMs(500);
// 				break;
// 		case FACTORY_KEY_4:
// 			playMusic(NUMBER_4);
// 			sysDelayMs(500);
// 				break;
		case NONE_KEY:
					//if(cc++ >16)
					//	stopAmpFlash();
					break;
		case INC_KEY:
// 			if(flash_music_test)    //ma 20150901  test sound 
// 			{
// 				led_flash = 4;
// 				playMusic(TX_MULTIROOM_SOUND);
// 				return;
// 			}
// 			if(flash_music_test)
// 			{
// 				if(cp<49)
// 					cp++;
// 				else if(cp == 49)
// 					cp = 0;
// 				if(cp<TOTAL_MUSIC_COUNT)
// 				{

// 					playMusic(cp);

// 					
// 				}
//  				led_flash = 4;  
// 				return;
// 			}
		
// 			if(HFP_factory_test_flag)
// 			{
// 				led_flash =4;
// 				NFC_flag= 1;
// 				btEnaleHFP();    //ma 20150724
// 				return;
// 			}
			if(ismute())                             //MA 20150929 not anykey can unmute 
			{
				w3d_mute();
				return;
			}
			if(factory_mode_flag)
			{

							return;
// 					}

			}
					/*****bt_change_name_mode*****///ma 20150616
					if(bt_change_name_mode)
					{
						if(cbt<7)
							cbt = 7;
						else if(cbt<30)
						{
							cbt++;
						}
						else if(cbt == 30)
							cbt = 47;
						else if(cbt < 47)
							cbt = 30;
						else if(cbt == 47)
							cbt = 7;
						else if(cbt >47)
							cbt = 47;
						if(cbt<31&&(cbt>6)||cbt ==47)
						{
							playMusic(cbt);
// 							sysDelayMs(500);							
						}
						return;
					}
					ampAddVolume();
// 					if(SB_flag&&dev_input == USERDEF)                          //MA 20150731  VOLUME to sub does not work when rx
// 						SB_volume_trans();    //ma 20150604   SB_volume_trans
					break;
		case DEC_KEY:

// 			if(flash_music_test)  //ma 20150901 test sound
// 			{
// 				led_flash = 4;           
// 				playMusic(NEW_BT_PAIRING_SOUND);
// 				return;
// 			}
// 			if(flash_music_test)
// 			{
// 				if(cp >0)
// 					cp--;
// 				else if(cp == 0)
// 					cp = 49;
// 				if(cp<TOTAL_MUSIC_COUNT)
// 				{
// 					playMusic(cp);

// 				}
// 				led_flash = 4;  
// 				return;
// 			}
		
// 			if(HFP_factory_test_flag)
// 			{
// 				btDisableHFP();    //ma 20150724
// 				led_flash =4;
// 				NFC_flag = 1;
// 				return;
// 			}
			if(ismute())                             //MA 20150929 not anykey can unmute 
			{
				w3d_mute();
				return;
			}
			if(factory_mode_flag)
			{

							return;
// 					}

			}
					/*****bt_change_name_mode*****///ma 20150616
					if(bt_change_name_mode)
					{
						if(cbt >47)
							cbt =47;
						else if(cbt == 47)
							cbt = 30;
						else if(cbt>30)
							cbt = 30;
						else if(cbt>7&&cbt <31)
						{
							cbt--;
						}
						else if(cbt < 7)
							cbt = 7;
						else if(cbt == 7)
							cbt = 47;
						if(cbt<31&&(cbt>6)||cbt ==47)
						{
							playMusic(cbt);
							sysDelayMs(500);							
						}
						return;
					}
					ampDecVolume();
// 					if(SB_flag&&dev_input == USERDEF)                              //MA 20150731  VOLUME to sub does not work when rx
// 					SB_volume_trans();    //ma 20150604   SB_volume_trans
					break;
		case MULTIROOM_KEY:
			if(ismute())                             //MA 20150929 not anykey can unmute 
			{
				w3d_mute();
				return;
			}
// 			/*****factory mode *****///ma 20150617
		
			if(factory_mode_flag)
			{

							return;
// 					}

			}
				/*****bt_change_name_mode*****///ma 20150616
					if(bt_change_name_mode)    
					{
							ledColorSet(GREEN_COLOR);
							playMusic(PAIRED_SOUND);
							sysDelayMs(500);
							ledColorSet(state_color);
							bt_change_name_mode = 0;
							if((cbt<31)&&(cbt>6)||(cbt ==47))            //ma 20150703  add core sound                     
							{
								if(cbt == 47)
									bt_name_send(BT_name_block[24]);
								else
									bt_name_send(BT_name_block[cbt-7]);
							}
// 							bt_name_send(BT_name_block[10]);
							sysDelayMs(1000);  
							cbt = 6;
							bt_dis_last_connect();    //ma 20150527
							sysDelayMs(20);
							btPairing();
							sysDelayMs(20);
							bt_paring_flag = 1;		   // ma 20150527
// 							BT_OFF();            //ma 20150526	
// 							sysDelayMs(500);
// 							btShutDown();         //ma 20150520    BT+OFF
// 							sysDelayMs(6000);  
// 							BT_OPEN();            //ma 20150526
// 							sysDelayMs(500);
// 							btStartUp();        //ma 20150520    BT+ON
// 							sysDelayMs(4000); 
							return;
					}
// 										/*****bt_change_name_mode*****///ma 20150616              //ma 20150929  delete useless function
//         if(ismute())           //ma 20150902      if mute ,unmute
// 				{
// 					ampAddVolume();
// 					return;
// 				}
				if(Dwhp83CuMuDetect() == 0)
					return;
	
				ledColorSet(state_color);
// 				while(key_buf == MULTIROOM_KEY){                                 //ma 20150807
// 							sysDelayMs(20);
// 							if(++i > 100){  					//ma 20150512  send message to EXT-mcu      //ma 20150605 disable long press multiroom key
// // 								version_sound_play();    //MA 20150616
// // 								play_music_flag =17;    //ma 20150618 
// // 								key_time_count  =0;       //ma 20150617
// 							}
// 				}

			if(i < 100 && dev_input != USERDEF){
				
        TX_sound_flag  = 1;    //ma 20150901 set play multiroom sound flag    
				if(SB_flag)
				{

// 							if(SB_play_flag == 1)               //ma  20150602
						
// 							{
						ledColorSet(PURPLE_COLOR);						//ma 20150513        //MA 20150602     //MA 20150605  change to UI color
// 											playMusic(PAIRED_SOUND);							//ma 20150513
						
// 								led_flash = 4;             //ma 20150902     del useless function 
					      playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								sysDelayMs(500);	                              //ma 20150604
								ledColorSet(state_color);                    //ma 20150602	
								sysDelayMs(500);						
					if(change_channal_counter == 0)         //ma 20150513 AUX state and SB MODE 
					{						
// 						if(isDwhp83Linked()){        //ma 20150702  if linked do not multiroom   ma 20150703 sounbar link /no soundbar link     //ma20150703 if linked 
							if(soundbar_coco_check()||(!isDwhp83Linked()))        //ma 20150703  if  sounbar coco link multiroom to 
							{
											change_to_local_coco();    //ma 20150515   SB mode  tell SUB  change to local  COCO    0x22
											sysDelayMs(100);
											DWHP83Mute();
											set_local_coco();              //ma 20150807

											sysDelayMs(1000);        //ma 20150514  wait for the current CU change to MU
                      Dwhp83Slave();
// 								      sysDelayMs(1000);
// 											if(Dwhp83CuMuDetect())
// 											{
// 												if(Dwhp83_CuMu == !MU)
// 													sysDelayMs(1000);
// 												else if(Dwhp83CuMuDetect())
// 												{
// 													if(Dwhp83_CuMu == !MU)
// 														sysDelayMs(1000);
// 													else if(Dwhp83CuMuDetect())
// 													{
// 															if(Dwhp83_CuMu == !MU)
// 																sysDelayMs(1000);
// 													}
// 												}
// 											}
											DWP83SpiritTask();											
// 											DWHP83UnMute();
											TX_flag = 1;
										  playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								}

				}
				else
				{
					
					soundbar_multiroom_flag = 1;     //ma 20150717  
				}						
			}
			else
			{
// 				Dwhp83GetStatus();
// 				if(dwhp83_status.link != 0)
// 				{
					Dwhp83CuMuDetect();     //MA 20150902
					if(Dwhp83_CuMu == CU)
					{
						 TX_sound_flag  = 0; 
						 return;
					}
					tx_linked_flag = 0;      //ma  20150906  clear tx linked flag   
//  				}
						playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
						ledColorSet(PURPLE_COLOR);				//MA 20150605  change to UI color
// 						led_flash = 4;                     //ma 20150605       ma 20150902  del useless function 
						sysDelayMs(500);	                              //ma 20150605
						ledColorSet(state_color);                    //ma 20150602
						if(Dwhp83_CuMu == MU){
							Core_tx_flag = 1;     //ma 20150813  set core tx flag 
							Dwhp83GetStatus();
							if(dwhp83_status.link == 0 || !isDwhp83Linked()){
// 								MU_tell_CU_change();     //ma 20150514  local the-one want to be CU ,he tell the current CU change to MU 
// 								sysDelayMs(1000);        //ma 20150514  wait for the current CU change to MU
								Dwhp83Master();          
								sysDelayMs(2000);
								playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								//Dwhp83EnterPairing();
// 								playMusic(PAIRED_SOUND);                 //ma 20150606  no sound to indicate 
							}else{
// 								set_cu_flag = 1;                             //ma 20150714	     //MA 20150813
								DWP83SpiritTask();								             //ma 20150717
								sysDelayMs(1000);
 								Dwhp83_flag |= 0x1;
								DWP83SpiritTask();                           //ma 20150717
								sysDelayMs(1000);                          //ma 20150715
// 							DWHP83SendCmd(0x02,0,mu_addr);                    //ma 20150716
// 								sysDelayMs(1000);
								Dwhp83Master();
								Dwhp83_flag = 0;
								playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
								sysDelayMs(2000);                          // ma  20150715 
								DWP83SpiritTask();                           //ma 20150717
								playMusic(TX_MULTIROOM_SOUND);        //ma 20150901 
							}
// 						DWHP83UnMute();    //ma 20150713
						DWHP83Mute();        //ma 20150813							
						}else{
// 							if((dwhp83_status.link == 1)&& isDwhp83Linked())
// 							{    //ma 20150717
								sysDelayMs(1000);                                            //ma 20150825 del useless function and add 1 second delay here 
// 							}
// 							else 
// 							{
// 								Dwhp83Slave();
// 								DWP83SpiritTask();								             //ma 20150717
// 								sysDelayMs(1000);
//  								Dwhp83_flag |= 0x1;
// 								DWP83SpiritTask();                           //ma 20150717
// 								sysDelayMs(1000);                          //ma 20150715
// // 							DWHP83SendCmd(0x02,0,mu_addr);                    //ma 20150716
// // 								sysDelayMs(1000);
// 								Dwhp83Master();
// 								Dwhp83_flag = 0;
// 								sysDelayMs(2000);                          // ma  20150715 
// 								DWP83SpiritTask();                           //ma 20150717
// 							}
// 							//Dwhp83EnterPairing();
						}
						ledColorSet(state_color);
				}
			}
				break;
		case CHANNEL_SWITCH_KEY: 
					if(ismute())                             //MA 20150929 not anykey can unmute 
					{
						w3d_mute();
						return;
					}
// 					/*****factory mode *****///ma 20150617
					if(factory_mode_flag)
					{
// 						factory_mode_flag = 0;
// 						cf =0;
						return;
					}
// 					/*****bt_change_name_mode*****///ma 20150617
					if(bt_change_name_mode)    
					{
// 							bt_change_name_mode = 0;
							cbt = 7;             //MA 20150707
					}
// 					if(ismute())           //ma 20150902      if mute ,unmute     //ma 20150929 delete useless function
// 					{
// 						ampAddVolume();
// 						return;
// 					}
					if(state_color == LIGHT_BLUE_COLOR)
						bt_hang_up_signal = TRUE;
					else {
						if(state_color == BLUETOOTH_COLOR)
						{
							ledColorSet(state_color);      //MA 20150906
							while(key_buf == CHANNEL_SWITCH_KEY){
								sysDelayMs(20);
								if(++i > 250){                        //soure 2sec paring to 5sec
	//								bt_disable_reconnect = 1;          //ma 20150520  cut  reconnect 
	// 								btForgetAll();    //ma 20150527
	// 								sysDelayMs(20);
									cc_blue_p = 45;         //ma 20150907        set  45 
									bt_paring_flag = 1;		   // ma 20150527	
									NFC_flag =1;
									btPairing();
									sysDelayMs(20);            //ma 20150805 
	// 								led_flash = 4;

									
									break;
								}
							}
						}
						if(i < 100){
							cc_blue_p = 45;         //ma 20150907      set 45 
							if(change_channal_counter == 0)              //ma 20150708  record the channale before change
							{
								dev_input_before_change = dev_input;   //ma 20150708  set  dev_input_before_change
							}
							dev_input = getNextInput();
							changeInputVolumeDown(INPUT_CHANGE_STATE);
							/****change source indicate state color at once*****/   //ma 201500606
							state_color = input_channel_color[dev_input];
							ledColorSet(state_color);
// 							setConfig();              //ma 20150715
// 							sysDelayAMs(100);
						}
					}
					change_channal_flag = 1;          // ma 20150708  set  channal flag 
// 					TX_sounder_counter = 0;           //ma 20150902 
					TX_sound_flag = 0;                //ma 20150902 
					change_channal_counter = 0;       // ma 20150708  clr  change channal counter 
					break;
		case SOUND_CLOSE_KEY:
				/*****callin .dis/enable mic****/
					if(callDetect())			//ma 20150529
					{
						if(cm) cm= 0;
						else cm =1;
						if(cm == 0) 
						{
							btEnableMic(); //ma 20150508  mute mic 
							sysDelayAMs(20);
							mic_mute_flag = 0;    // ma 20150527 
						}
						if(cm == 1) 
						{
							btDisableMic();
							sysDelayAMs(20);
							mic_mute_flag = 1;     // ma 20150527   
						}
						break;
					}
					volume_key_delay = BT_MATCHING_DELAY;
					
					if(w3d_lock == FALSE){
						if(ismute() == 0){
							DWP83CuCV0();
						}else{
							DWP83CuCV();
						}
					}
					w3d_mute();
					
					break;
		case POWER_KEY:
					cc = 0;
					ca = 0;
					cd = 0;
					ch = 0;      // ma 20150527  set ch
					cn = 0;       //ma 20150616  add  bt name change add
					ledColorSet(state_color);
					while(GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
						sysDelayMs(20);
						cc++;
						
						if(key_buf_adc == INC_KEY)
							ca++;
						else
							ca = 0;
						
						if(key_buf_adc == DEC_KEY)
							cd++;
						else
							cd = 0;
						
						/***********CHANNEL_SWITCH_KEY++******/
						if(key_buf_adc == CHANNEL_SWITCH_KEY)
							ch++;
						else
							ch = 0;
						/***********MULTIROOM_KEY++******/  //ma 20150616  add  bt name change add
						if(key_buf_adc == MULTIROOM_KEY)  
							cn++;
						else
							cn = 0;
						if(cn > 240){
							if(bt_change_name_mode == 0)
							{
								bt_change_name_mode =1;
								ledColorSet(GREEN_COLOR);
								playMusic(PAIRED_SOUND);
								sysDelayAMs(500);
							}
							return;
						}
						
						if(ca > 240){
							SB_flag = 1;                //ma 20150602
							asCenter();                  //ma 20150514               
							ledColorSet(GREEN_COLOR);
							/*******need to add paired sound  when SB mode open******/
							playMusic(PAIRED_SOUND);     //ma 20150702 
							
							setConfig();              //ma 20150602
							sysDelayAMs(500);
// 							if(SB_flag == 0x00)     //ma 20150513 linked, the-one send to SUB  to  enable SB mode 
//  							{
// 								DWHP83_enable_sb();   //ma 20150514  after local the-one linked SUB ,the former tell the latter to have SB mode 
// 								sysDelayAMs(1000);			//ma 20150514  maybe 1000ms is more suitble					
// 							}
							
							/******no need  to change coco  or set CU  at once   if RX********/							 
							if(Dwhp83CuMuDetect())        //MA 20150702  when RX do not change coco at once 
							{
								if(dev_input != USERDEF)
								{
										/*****reset soundbar coco***/
											set_soundbar_coco();   //ma 20150825   set soundbar coco here 
								}
							}
							return;
						}

						if(cd > 240){
							
							asStreao();
							ledColorSet(RED_COLOR);
							/********need  to add disconected sound  when  SB mode is off****/
							playMusic(DISCONECTED_SOUND);     //ma 20150702 


							sysDelayAMs(500);
							
							if(SB_flag == 0x01)
							{
								SB_flag = 0x00;            //ma 20150513  exit SB mode
// 								SB_play_flag = 0;      //ma 20150515 reset SB_play_flag
// 								change_to_local_coco(); 	//ma 20150513   SB mode  tell SUB  chaneg to local  COCO    //ma 20150602
								sysDelayMs(100); 
								/*****reset local coco***/
								set_local_coco();     //ma 20150825   set  local  coco 
								
 								setConfig();                    //ma 20150514  send it to eeprom    //ma 20150602
								sysDelayAMs(500);
							}
							Dwhp83CuMuDetect();
							if(dev_input == USERDEF)                                   //ma 20150602
							
							{
								if(Dwhp83_CuMu == MU)        //ma 20150514 if  local the-one is MU to tell the current CU to tell SUB  change COCO and enter SB mode 
								{
									DWHP83toEXT_MCU();     //ma 20150513  change funcion to AUE   0x55
									sysDelayMs(100);
								}
							}
							if(dev_input == AUX)            //ma 20150603
							{
								if(Dwhp83_CuMu == CU)        //ma 20150514 if  local the-one is MU to tell the current CU to tell SUB  change COCO and enter SB mode 
								{
									DWHP83toSUB();           //ma 20150512  CU send massage to SUB  0x66
									sysDelayMs(100);
								}
							}
							return;
						}

						if(cc > 500){
							if(dc_auto_down_flag == 0){    // ma 20150508   disable  DC auto power off //20150612
								ledColorSet(GREEN_COLOR);
								playMusic(PAIRED_SOUND);     // ma  20150706  change the default  mode  power off  any key can    power on 

								dc_auto_down_flag = 5;
							}else{
								ledColorSet(RED_COLOR);
								playMusic(DISCONECTED_SOUND);
								dc_auto_down_flag = 0;
							}
								writeeeprom(dc_auto_down_flag,0x05);
// 									sysDelayMs(30);
							sysDelayAMs(500);
							return;
						}
						/***********CHANNEL_SWITCH_KEY 5sec******/
						if(ch >250)       //ma 20150527  20ms *250 = 5sec    need bt restart
						{
							if(bt_hfp_flag == 1)
							{
								ledColorSet(RED_COLOR);
								playMusic(DISCONECTED_SOUND);
								bt_hfp_flag = 0;
								btDisableHFP();
							}
							else
							{
								ledColorSet(GREEN_COLOR);
								playMusic(PAIRED_SOUND);
								bt_hfp_flag = 1;
								btEnaleHFP();
							}
// 							setConfig();        //ma 201500527  add setconfig
							writeeeprom(bt_hfp_flag,0x07);
							sysDelayAMs(500);    
							return;
						}
					}
					
					if(ismute())           //ma 20150902      if mute ,unmute
					{
						ampAddVolume();
					}

					BT_MATCH_FLAG  =0 ;     //ma 20150707   set  bt match flag 
					
					/********resume factory setting*******/
					if(factory_mode_flag)
					{

						dwhp83_type = VOLUME_STREO;
						amp_volume = 25;
						dc_auto_down_flag =0;
						SB_flag = 0;
						bt_hfp_flag = 0;
						dev_input = BLUETOOTH;
						btStartUp();
						sysDelayAMs(100);
						btForgetAll(); 
						sysDelayAMs(20);			   //ma 20150805 			
					}
					/******factory setting end ****/
					
					setConfig();           //ma 20150706    before power ff ,store 
					sysDelayAMs(100);
// 					enterStandby();
					
							/*********add enable /disable adc key power on function******/
// 					state_color = RED_COLOR;
					if(DECT_DC())
					{
						if(dc_auto_down_flag == 5)        //ma 20150629  
						{
							GPIO_SET_PIN(BLUE_PIN);	     //MA 20150706   close led 
							GPIO_SET_PIN(GREEN_PIN);
							GPIO_CLR_PIN(RED_PIN);
// 							DrvTIMER_DisableInt(TIMER0);
						 sleep_flag =1;       // ma 20150706  sleep flag      
						 sleep();
						}
						else if(dc_auto_down_flag == 0)        //ma 20150706 
						{							
							enterStandby();
							sleep_flag =0;   
						}
					}
					else
					{						
						enterStandby();        //ma 20150706 
						sleep_flag =0;  
					}
					
					break;
			
		default:
					break;
	}	
}


/*-------------------------------------------------------------------------
 * @fucname:	 delStartkeyContinue
 * @brief: 	 deal continue key event when dev is in start mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal continue key event when dev is in start mode 
 *-----------------------------------------------------------------------*/
void delStartkeyContinue(enum KEYWORD keyword)
{

}


