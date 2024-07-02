/****************************************************************************
 * @file:		datalog.c
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#include "datalog.h"

struct logSturct{
	Log_Type event;
	u32 time : 24;
}logbuf[128];

U32 temp_buf[64];
u32  log_buf[128];
U32 flash_start;
u32 flash_page;
u32 flash_w = 0;
static u32 log_index = 0;
static i2cPoolType eprom_block;
// static u32 eprom_address = 0;
static volatile u8 eprom_buf[3];
static u8* ptrlog;
Timer_ID log_id = TASK_FULL;
Log_Type event = un_used;


u8 *logInfo[] = {
	"un_used",
	"Time_stamp_wrapped",
	"Power_up_sequence_started",
	"Power_down_sequence_started",
	"Amplifier_mute",
	"Amplifier_unmute",
	"System_power_on",
	"System_standby",
	"system_power_off",
	"NFC_touch_detected",
	"NFC_removed",
	"key_power_pressed",
	"key_vol_add_pressed",
	"key_vol_dec_pressed",
	"key_source_pressed",
	"key_source_long_pressed",
	"key_u_pressed",
	"key_v_pressed",
	"key_w_pressed",
	"key_x_pressed",
	"key_y_pressed",
	"key_z_pressed",
	"RC_key1_pressed",
	"RC_key2_pressed",
	"RC_key3_pressed",
	"RC_key4_pressed",
	"RC_key5_pressed",
	"RC_key6_pressed",
	"RC_key7_pressed",
	"RC_key8_pressed",
	"RC_key9_pressed",
	"RC_key10_pressed",
	"RC_key11_pressed",
	"RC_key12_pressed",
	"RC_key13_pressed",
	"RC_key14_pressed",
	"RC_key15_pressed",
	"RC_key16_pressed",
	"RC_key17_pressed",
	"RC_key18_pressed",
	"RC_key19_pressed",
	"RC_key20_pressed",
	"RC_key21_pressed",
	"RC_key22_pressed",
	"RC_key23_pressed",
	"RC_key24_pressed",
	"RC_key25_pressed",
	"RC_key26_pressed",
	"RC_key27_pressed",
	"RC_key28_pressed",
	"RC_key29_pressed",
	"RC_key30_pressed",
	"RC_key31_pressed",
	"RC_key32_pressed",
	"call_in",
	"call_out",
	"call_pickup",
	"call_hangup",
	"bt_pairing",
	"bt_paired",
	"bt_power_down",
	"bt_disconnect",
	"demo_mode_pressed",
	"Gesture_event1",
	"Gesture_event2",
	"Gesture_event3",
	"Gesture_event4",
	"Gesture_event5",
	"Gesture_event6",
	"Gesture_event7",
	"amp_init_start",
	"amp_init_ok",
	"amp_i2c_werror",
	"amp_i2c_rerror",
	"Main_enclosure_over_temp_warning",
	"Amplifier1_over_temp_warning",
	"Amplifier2_over_temp_warning",
	"Amplifier3_over_temp_warning",
	"Amplifier1_thermal_shutdown",
	"Amplifier2_thermal_shutdown",
	"Amplifier3_thermal_shutdown",
	"Amplifier1_overcurrent_shutdown",
	"Amplifier2_overcurrent_shutdown",
	"Amplifier3_overcurrent_shutdown",
	"GSensor_event1",
	"GSensor_event2",
	"GSensor_event3",
	"GSensor_event4",
	"GSensor_event5",
	"GSensor_event6",
	"GSensor_event7",
	"GSensor_event8",
	"GSensor_event9",
	"GSensor_event10",
	"Overvoltage_on_DC_input",
	"Battery_full",
	"Battery_low",
	"Battery_chargin",
	"DC_inserted",
	"DC_removed",
	"Phone_call_detected",
	"Phone_call_ended",
	"SPDIF_source1_detected",
	"SPDIF_source2_detected",
	"SPDIF_source3_detected",
	"SPDIF_source4_detected",
	"BLUETOOTH_SOURE_IN",
	"AUX_SOURCE_IN",
	"LINEIN_SOUTCE_IN",
	"Flash_audio_track1_started",
	"Flash_audio_track2_started", 
	"Flash_audio_track3_started",
	"Flash_audio_track4_started",
	"Flash_audio_track5_started",
	"Flash_audio_track6_started",
	"Flash_audio_track7_started",
	"Flash_audio_track8_started",
	"Flash_audio_track1_finished",
	"Flash_audio_track2_finished",
	"Flash_audio_track3_finished",
	"Flash_audio_track4_finished",
	"Flash_audio_track5_finished",
	"Flash_audio_track6_finished",
	"Flash_audio_track7_finished",
	"Flash_audio_track8_finished",
	"BT_in_pairing_mode",
	"BT_forced_into_pairing_mode",
	"BT_pairing_complete",
	"No_audio_present_for10_mins",
	"No_audio_present_for30_mins",
	"Led_dark_color",
	"Led_red_color",
	"Led_green_color",
	"Led_blue_color",
	"Led_yellow_color",
	"Led_purple_color",
	"Led_white_color",
	"Led_soe_color",
	"Led_light_blue_color",
	"Led_bluetooth_color",
	"flash_error_page",
	"watch_dog_reset",
};

void WriteLogByte(u8 data, u32 address)
{
	initI2cDataBlock(	&eprom_block, 
					0xA0, 
					I2C_WR, 
					2,
					1,
					(u8*)&eprom_buf[0]
	);
	
	eprom_buf[1] = address&0xff;;
	eprom_buf[2] = data;
	eprom_buf[0] = I2C_BUSY;
	addI2cChain(&eprom_block, 0);
	while(eprom_buf[0] == I2C_BUSY)
			;
}


u8 ReadLogByte(u32 address)
{
	initI2cDataBlock(	&eprom_block, 
					0xA0, 
					I2C_RD, 
					2,
					1,
					(u8*)&eprom_buf[0]
	);
	
	eprom_buf[1] = address&0xff;;
	eprom_buf[2] = 0xff;
	eprom_buf[0] = I2C_BUSY;
	addI2cChain(&eprom_block, 0);
	while(eprom_buf[0] == I2C_BUSY)
			;
	return eprom_buf[2];
}

void writeOneLog(u32 data, u32 index)
{
	U32_BYTE log;
	u8 address;

	log = *(U32_BYTE*)&data;
	address = (index&0x3f)<<2;
	WriteLogByte(log.byte0, address++);
	WriteLogByte(log.byte1, address++);
	WriteLogByte(log.byte2, address++);
	WriteLogByte(log.byte3, address++);
}


u32 ReadOneLog(u32 address)
{
	U32_BYTE log;

	address <<= 2;
	log.byte0 =  ReadLogByte(address++);
	log.byte1 =  ReadLogByte(address++);
	log.byte2 =  ReadLogByte(address++);
	log.byte3 =  ReadLogByte(address++);

	return *(u32*)&log;
}


void writeFlashPage()
{
	if(flash_mode == FLASH_CONFIG){
		flash_w = 1;
		ptrlog[0] = EPROM_FULL;
		spiFlashPageWrite(flash_page, 256, ptrlog);
		flash_w = 0;
		flash_page += 256;
		WriteLogByte(EPROM_EMPTY, 0);
		if((flash_page&0xffff)==0){
			if(flash_page == 0xe0000)
				SF_SECTOR_ERASE(flash_page);
			if(flash_page == 0xf0000)
				SF_SECTOR_ERASE(flash_page);
			if(flash_page == 0x100000){
				flash_page = 0xe0000;
				SF_SECTOR_ERASE(flash_page);
			}
			sysDelayMs(200);
			SF_GET_STATUS();
			while(flash_status&1){
				sysDelayMs(200);
				SF_GET_STATUS();
			}
		}
	}else
		startATask(log_id, 30, 0, writeFlashPage);
}


void writeLogPage(u8* buf)
{
	ptrlog = buf;
	startATask(log_id, 3, 0, writeFlashPage);
}

/*-------------------------------------------------------------------------
 * @fucname:	 logdata
 * @brief: 
 * @param:  data: event_id ; flag: power event
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
void logEvent(Log_Type data)
{
// 	event = data;
// 	
// 	if((log_index&0x3f) == 0){
// 		log_buf[log_index] = EPROM_USING + (flash_page<<8);
// 		writeOneLog(log_buf[log_index],log_index);
// 		log_index++;
// 	}
// 	
// 	log_buf[log_index] = data + (systick_counter<<8);
// 	
// 	writeOneLog(log_buf[log_index],log_index);

// 	log_index++;
// 	
// 	if(log_index == 64){
// 		WriteLogByte(EPROM_FULL, 0);
// 		writeLogPage((u8*)&log_buf[0]);
// 	}

// 	if(log_index == 128){
// 		log_index = 0;
// 		WriteLogByte(EPROM_FULL, 0);
// 		writeLogPage((u8*)&log_buf[64]);
// 	}

}




void logInit()
{
	u32 i, log;
	
	log = ReadOneLog(0);

	if((log&0xff) == EPROM_EMPTY){
L1:
		flash_page = (log>>8);
		log_index = 0;
	}else if((log&0xff) == EPROM_USING){
		flash_page = log>>8;
		log_index = ReadOneLog(63);
		if(log_index>0x3f){
			log = ReadOneLog(0);
			goto L1;
		}
		for(i = 0; i < log_index; i++){
			log_buf[i] = ReadOneLog(i);
		}
		
	}else if((log&0xff) == EPROM_FULL){
		flash_w = 1;
		for(i = 0; i < 64; i++){
			log_buf[i] = ReadOneLog(i);
		}
		spiFlashPageWrite(flash_page, 256, (u8*)&log_buf[0]);
		flash_w = 0;
		log_index = 0;
		flash_page = (log>>8)+256;
		
		if((flash_page&0xffff)==0){
			if(flash_page == 0xe0000)
				SF_SECTOR_ERASE(0xe0000);
			if(flash_page == 0xf0000)
				SF_SECTOR_ERASE(0xf0000);
			if(flash_page == 0x100000){
				SF_SECTOR_ERASE(0xe0000);
				flash_page = 0xe0000;
			}
			sysDelayMs(200);
			SF_GET_STATUS();
			while(flash_status&1){
				sysDelayMs(200);
				SF_GET_STATUS();
			}
		}
		
	}else{
		flash_page = 0xe0000;
		log_index = 0;
		SF_SECTOR_ERASE(flash_page);
		sysDelayMs(200);
		SF_GET_STATUS();
		while(flash_status&1){
			sysDelayMs(200);
			SF_GET_STATUS();
		}
	}
}

void logClose()
{
// 	u32 log, i, *ptr32;

// 	log = ReadOneLog(0);
// 	
// 	if((log&0xff) == EPROM_USING ){
// 		writeOneLog(log_index,63);
// 		
// 	}else {
// 		if((log&0xff) == EPROM_FULL){
// 			flash_w = 1;
// 			ptrlog[0] = EPROM_FULL;
// 			spiFlashPageWrite(flash_page, 256, ptrlog);
// 			spiFlashRead(flash_page, 256,(u8*)&temp_buf[0]);
// 			
// 			ptr32 = (u32*)ptrlog;
// 			for(i = 0; i < 64; i++)
// 				if(temp_buf[i]!= ptr32[i]){
// 					logEvent(flash_error_page);
// 					break;
// 				}
// 				
// 			flash_page += 256;
// 			flash_w = 0;
// 			WriteLogByte(EPROM_EMPTY, 0);
// 			
// 			
// 			if((flash_page&0xffff)==0){
// 				if(flash_page == 0xe0000)
// 					SF_SECTOR_ERASE(0xf0000);
// 				if(flash_page == 0xf0000)
// 					SF_SECTOR_ERASE(0xf0000);
// 				if(flash_page == 0x100000){
// 					SF_SECTOR_ERASE(0xe0000);
// 					flash_page = 0xe0000;
// 				}
// 				sysDelayMs(200);
// 				SF_GET_STATUS();
// 				while(flash_status&1){
// 					sysDelayMs(200);
// 					SF_GET_STATUS();
// 				}
// 			}
// 		}
// 		
// 	}

}

// static printlogtime(u32 time)
// {
// 	u8 buf[16];
// 	u32 index;

// 	index = itoa(time, &buf[0]);
// 	for(; index<9; index++)
// 		buf[index] = ' ';
// 	buf[index++] = 0;
// 	iputs(buf);
// 	iputs(": ");
// }

void printLog(u32 addr, u32 flag)
{
// 	u32 index, i;
// 	u32 time, page;
// 	U32_BYTE tt;
// 	u8 buf[6];
// 	static u32 lastpage, c=0;

// 	if(flag == 0){
// L1:
// 		if(addr > 0xe0000 && addr <= 0xFFF00)
// 			lastpage = page = addr;
// 		else
// 			lastpage = page = flash_page;
// 		iputs("\t\t now log page: ");
// 		
// 	}else{
// 		if(++c > 3)
// 			goto L1;
// 		if(lastpage == 0xe0000){
// 			iputs("\t\t no more page!\r\n");
// 			return;
// 		}	
// 		lastpage -= 0x100;
// 		page = lastpage;
// 		iputs("\t\t last log page:");
// 	}

// 	c = 0;
// 	tt = *(U32_BYTE*)&page;
// 	iputs("0x");
// 	iputs(itoh(tt.byte3,buf));
// 	iputs(itoh(tt.byte2,buf));
// 	iputs(itoh(tt.byte1,buf));
// 	iputs(itoh(tt.byte0,buf));
// 	iputs("\r\n\n");
// 		
// 	if(page != flash_page){
// 		if(flash_mode == FLASH_PLAY_MUSIC)
// 			playSFMusicEnd();
// 		spiFlashRead(page, 256, (u8*)&logbuf[0]);
// 		index = 64;
// 		i = 1;
// 	}else{
// 		for(index = log_index&0X40; index < log_index; index++){
// 			logbuf[index] = *(struct logSturct*)&log_buf[index];
// 		}
// 		i = log_index&0X40;
// 		i++;
// 		index = log_index;
// 	}
// 	
// 	
// 	for(; i < index ; i++){
// 		while(UART_GET_TX_EMPTY(UART0) == 0)
// 			sysDelayAMs(1);

// 		time = logbuf[i].time;
// 		printlogtime(time);
// 		iputs(logInfo[logbuf[i].event]);	
// 		iputs("\r\n");
// 	}
		
}



void earseAllLog()
{
// 	writeOneLog(0xffffffff, 0);
// 	playSFMusicEnd();
// 	logInit();
// 	iputs("all log earse done!\r\n");
}





