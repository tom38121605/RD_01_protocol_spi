/****************************************************************************
 * @file:		appkey.c
 * @version:	V1.00
 * @breif:	do key event which was sent by ir_key or adc_key 
 * @date: 	2013/3/01
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"

extern u32 BT_MATCH_FLAG;
extern u32 volume_key_delay;
extern FUNC_Timer_Task adcKeySubHook;
extern void playSFMusicEnd(void);
extern enum KEYWORD key_buf;
extern s8 tws_out;

u8 SB_flag = 0x00;

/*-------------------------------------------------------------------------
 * @fucname:	 delStandbytkey
 * @brief: 	 deal key event when dev is in standby mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in standby mode 
 *-----------------------------------------------------------------------*/
void delStandbytkey(enum KEYWORD keyword)
{
/*--	u32 i = 0;
	
	switch(keyword)
	{
		case PAIR_KEY:
				ledColorSet(state_color);
				while(GPIO_GET_PIN(PC13))
				{
					sysDelayMs(20);
					if(++i > 100)
					{
						set_p();
						playMusic(DD_SOUND);
						outStandby();
						break;
					}
				}
				break;
				
		case POWER_KEY:

			capIntEnable();
			outStandby();
			break;
		
		default:
			break;
	}
	*/
}



u8 hidden_type = 0xa0;
u8 battery_f = 0x00;
u8 auto_play = 0x55;
void setConfig(void)
{
/*--	writeeeprom(0x5A,0x01);
  sysDelayMs(30);
	writeeeprom(auto_play,0x02);
  sysDelayMs(30);
	writeeeprom(battery_f,0x03);
  sysDelayMs(30);
	writeeeprom(hidden_type,0x04);
  sysDelayMs(30);

	
	I2S_DISABLE_INT(I2S_INT_RXTH);
	I2S_CLR_RXFIFO();
	I2S_CLR_TXFIFO();
	i2sStart(); 
	*/
}

void getDevCofig(void)
{
/*	u8 k;	

	k= readeeprom(0x01);
	sysDelayMs(5);
	if(k != 0x5A)
	{
		auto_play = 0x55;
		battery_f = 0xff;
		hidden_type = 0XA0;
	}
	else
	{
		auto_play = readeeprom(0x02); sysDelayMs(5);
		battery_f = readeeprom(0x03); sysDelayMs(5);
		hidden_type = readeeprom(0x04); sysDelayMs(5);
	}
	
*/	
	
}


 void playBTname(u8 v)
 {
/*	if(v == 0xa0){
		playMusic(HIDDENRADIO_SOUND);
	}else if(v == 0xa1){
		playMusic(BEDROOM_SOUND);
	}else if(v == 0xa2){
		playMusic(BATHROOM_SOUND);
	}else if(v == 0xa3){
		playMusic(KITCHEN_SOUND);
	}else if(v == 0xa4){
		playMusic(LIVING_ROOM_SOUND);
	}else if(v == 0xa5){
		playMusic(LEFT_CHANNEL_SOUND);
	}else if(v == 0xa6){
		playMusic(RIGHT_CHANNEL_SOUND);
	}else if(v == 0xa7){
		playMusic(AUTO_PLAY_ACTIVATE_SOUND);
	}else if(v == 0xa8){
		playMusic(AUTO_PLAY_DEACTIVATE_SOUND);
	}
	*/
 }
 
/*-------------------------------------------------------------------------
 * @fucname:	 delStartkey
 * @brief: 	 deal key event when dev is in start mode
 * @param: 	 keyword
 * @return: 	 none
 * @details: 	 deal key event when dev is in start mode 
 *-----------------------------------------------------------------------*/
extern u32 power_delay;
extern U32 pairing_key_flag;
extern u32 ccf;
extern enum KEYWORD key_buf_adc;
extern u32 volume_send;

u32 tws_f = 0;
void delStartkey(enum KEYWORD keyword)
{
	/*--static u32 tws_o = 0;
	u32 kkk = 0;
	U32 sck = 0, mck = 0;
	u32 kcm = 0;
	u8 hidden_type_= hidden_type;
	enum KEYWORD  key_bak = NONE_KEY, key_w = NONE_KEY;
	*/


	
	if(keyword != NONE_KEY )
	{
		/*BAT_STATE_DECT_DELAY_COUNT = 0; // battery power dectect delay
		led_battery_flash = 0;          // don't show battery information while a key pressed
		
		if(flg_ampdown == 1)
		{
			flg_ampdown = 0;
			iputs("amp on\r\n");
			ampPowerUp();	
		}				
		*/
		
//uartdecode(keyword,"  keyword2: ");   //--zz debug  
		
		;
	}	
	
	switch(keyword)
	{
		case NONE_KEY:
					break;
		
		case POWER_KEY:
          iputs("POWER KEY in\r\n");  //--zz debug   
		
					break;			

		case INC_KEY:
          iputs("INC KEY in\r\n");  //--zz debug   
		
					break;			

		case DEC_KEY:
          iputs("DEC KEY in\r\n");  //--zz debug   
		
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





