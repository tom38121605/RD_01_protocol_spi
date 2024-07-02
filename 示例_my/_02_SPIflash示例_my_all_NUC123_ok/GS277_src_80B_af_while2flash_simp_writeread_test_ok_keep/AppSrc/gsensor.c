// /****************************************************************************
//  * @file:		gsensor.c
//  * @version:	v1.00
//  * @breif:		gesture sensor source file	
//  * @date: 	
//  * 
//  * @note 		
//  *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
//  * 
//  ****************************************************************************/
// #include "sysinc.h"
// #include "gsensor.h"
// #define GS_trigger_level		0x10
// #define ACT_INTERVAL		0X16
// #define GS_SCAN_RATE		7	// 4MS
// #define HPF_OFFSET 			1000
// #define HPF_CONSTANT 		31
// #define HPF_SHIFT			5
// #define LPF_SIZE				8
// #define LPF_SHIFT			3
// #define GS_MAX				(1<<13)
// #define WIN_SHIFT			(6)
// #define WIN_SIZE				(1<<WIN_SHIFT)
// #define THR_PEAK			35

// typedef struct{
// 		s16 min;
// 		s16 max;
// 		u16 min_frame;
// 		u16 max_frame; 
// }ChannelT;

// extern u32 BT_MATCH_FLAG;
// extern u32 volume_key_delay;
// extern Timer_ID pt2314_id;

// static u16 frame_counter = 0;
// u32 gs_trigger = FALSE;
// static volatile u32 delay = 0;
// u32 daL=0, daR=0;
// static u32 act_interval = 0;
// GsData gsdata;
// s16 rawData[4];
// s16 hpfData[4];
// s16 lpfData[4];
// S16 lpfFirst[4][LPF_SIZE];
// s32 sumDataFirst[4];
// s32 sumDataSecond[4];
// s32 lpfSecond[4][LPF_SIZE];
// S16 sample_windows[4][WIN_SIZE];
// s32 avi[4];
// u32 ch_locked[4];
// u16 last_counter = 0;

// static s16 y_1[4];
// static s16 x_1[4];


// i2cPoolType gs_i2c_block;
// GStatusbuf gs_i2c_data;
// ChannelT gs_channels[4];

// Timer_ID gs_id = 0xff;

// void gs_key(enum KEYWORD key);
// void do_gs_event();

// /*==================================================== */
// /*                                                       CODE                                                    */
// /*========================== ==========================*/
// static u32 abs(s32 d)
// {
// 	return d<0? -d : d;
// }

// void hpf()
// {
// 	s32 i;
// 	s16 sum;

// 	for(i=0; i<4; i++){
// 		sum = rawData[i];
// 		sum += y_1[i] - (y_1[i]>>32);
// 		sum -= x_1[i];
// 		x_1[i] = rawData[i];
// 		y_1[i] = sum;
// 		hpfData[i] = sum + HPF_OFFSET;
// 	}
// 	

// }

// void lpf()
// {
// 	s16 x;
// 	s16 tempData[4];
// 	static u32 position = 0;
// 	
// 	for (x =0; x<4; x++)
// 	{
// 		sumDataFirst[x] -= lpfFirst[x][position];
// 		sumDataFirst[x] += hpfData[x];
// 		lpfFirst[x][position] = hpfData[x];
// 		
// 		tempData[x] = (s16)(sumDataFirst[x]>>LPF_SHIFT);
// 		
// 		sumDataSecond[x] -= lpfSecond[x][position];
// 		sumDataSecond[x] += tempData[x];
// 		lpfSecond[x][position] = tempData[x];
// 		
// 		lpfData[x] = (s16)(sumDataSecond[x]>>LPF_SHIFT);		
// 	}

// 	position++;
// 	if(position >= LPF_SIZE)
// 		position = 0;	
// }

//  void uartdecode(u32 x, u8* str)
//  {
// 	u8 buf[16];
// 	u32 index;
//  
// 	index = itoa(x, &buf[0]);
// 	for(; index<6; index++)
// 		buf[index] = ' ';
// 	buf[index++] = 0;
// 	iputs(str);
// 	iputs(buf);
//  }

// static u16 frame_counter_bak;
// u32 gs_verify()
// {
// 	u16 * ptr_gsdata;
// 	static u8 bcode[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
// 	u16 temp;
// 	u32 bits_count, i;
// 	
// 	
// 	ptr_gsdata = (u16*)&gs_i2c_data.buf;

// 	for(i =0; i<4; i++){
// 		//if(abs(rawData[i]-(ptr_gsdata[i]<<2))<0x1000)
// 		rawData[i] = ((ptr_gsdata[i]&0x3fff) >>2); 
// 		
// 		bits_count = 0;
// 		bits_count = bcode[ptr_gsdata[i]&0xf]
// 				+   bcode[(ptr_gsdata[i]>> 4)&0xf]
// 				+   bcode[(ptr_gsdata[i]>> 8)&0xf]
// 				+   bcode[(ptr_gsdata[i]>> 12)&0x3];
// 		bits_count &= 0x3;
// 		if(bits_count != (ptr_gsdata[i]>> 14)&0x3)
// 			return 1;
// 	}
// 	
// 	frame_counter = (gs_i2c_data.buf[12]<<8) + gs_i2c_data.buf[11];
// 	//uartdecode(frame_counter,"f:");
// 	
// 	if(abs(frame_counter -frame_counter_bak) > 0x800)
// 		return 0;
// 	else
// 		frame_counter_bak = frame_counter;
// 	
// 	return 0;
// }

// static void gsDataInit()
// {
// 	u32 i,j;
// 	
// 	gs_trigger = FALSE;
// 	frame_counter = 0;
// 	last_counter = 0;
// 	frame_counter_bak = 0;
// 	for(i=0;i<4;i++){
// 		avi[i] = 0;
// 		ch_locked[i] = FALSE;
// 		rawData[i]=0;
// 		sumDataFirst[i]=0;
// 		sumDataSecond[i]=0;
// 		//sumDataHigh[i] = 0;
// 		gs_channels[i].max = 0;
// 		gs_channels[i].max_frame = 0;
// 		gs_channels[i].min = GS_MAX;
// 		gs_channels[i].min_frame = 0;
// 		x_1[i] = 0;
// 		y_1[i] = 0;
// 		
// 		for(j=0; j<LPF_SIZE; j++){
// 			lpfFirst[i][j] = 0;
// 			lpfSecond[i][j] = 0;
// 			avi[i] = 0;
// 		}

// 		for(j=0; j<WIN_SIZE; j++)
// 			sample_windows[i][j] = 0;
// 		
// 	}
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 judgePeaks
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details:	if all channel locked judge which action to take. 
//  *-----------------------------------------------------------------------*/
// static void judgePeaks()
// {
// 	u32 i;
// 	u32 flag = FALSE;


// 	for(i = 0; i < 4; i++){
// 		flag = FALSE;
// 		
// 		if(	gs_channels[i].max - gs_channels[i].min > (THR_PEAK<<1)
// 			&& frame_counter - gs_channels[i].min_frame > 8)
// 				flag = TRUE;
// 		
// 		if(flag == FALSE)
// 			break;
// 	}

// 	if(flag){
// 		if(abs(last_counter - gs_channels[0].max_frame) > 30){
// 			last_counter = gs_channels[0].max_frame;
// 			do_gs_event();
// 		}
// 	}
// }


// static void findMax(u32 channel)
// {	
// 	if(abs(frame_counter - gs_channels[channel].max_frame) > 30 
// 		|| lpfData[channel] > gs_channels[channel].max){
// 		
// 		gs_channels[channel].max_frame = frame_counter;
// 		gs_channels[channel].max = lpfData[channel];
// 	}
// }

// static void findMin(u32 channel)
// {
// 	if(abs(frame_counter - gs_channels[channel].min_frame) > 30  
// 		|| lpfData[channel] < gs_channels[channel].min){
// 		
// 		gs_channels[channel].min_frame = frame_counter;
// 		gs_channels[channel].min = lpfData[channel];
// 	}
// 		
// }

// static void recodePeaks()
// {
// 	u32 i,temp;
// 	static u32 recode_counter = 0;
// 	static u32 position = 0;

// 	for(i = 0; i < 4; i++){
// 		avi[i] -= sample_windows[i][position];
// 		avi[i] += lpfData[i];
// 		sample_windows[i][position] = lpfData[i];
// 		if(frame_counter < 50)
// 			return;
// 		if(ch_locked[i] != 0x55AA){ 
// 			temp = lpfData[i];
// 			temp = avi[i]>>WIN_SHIFT;
// 			//uartdecode(temp,"v:");
// 			if(temp < lpfData[i] -THR_PEAK )
// 				findMax(i);
// 			else if(temp >  lpfData[i] +THR_PEAK)
// 				findMin(i);	
// 		}
// 	}

// 	judgePeaks();
// 	
// 	if(++position > WIN_SIZE-1)
// 		position = 0;

// 	gsdata.ch1 = lpfData[0];
// 	gsdata.ch2 = lpfData[1];
// 	gsdata.ch3 = lpfData[2];
// 	gsdata.ch4 = lpfData[3];
// 	
// }


// void gsEnable(void)
// {	
// 	GPIO_SET_PIN(GS_CE_PORT_PIN);
// }
// /*-------------------------------------------------------------------------
//  * @fucname:	 gsResetDone
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// static void gsResetDone(void)
// {
// 	if(dev_state == DEV_STANDBY)
// 		return;
// 	initI2cDataBlock(&gs_i2c_block, 
// 						GS_I2C_ADDR, 
// 						I2C_WR, 
// 						10, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 		
// 	gs_i2c_data.state = I2C_BUSY;
// 	gs_i2c_data.cmd = SET_STATUS; // read fifo
// 	gs_i2c_data.buf[0] = 0x3f;                         
// 	gs_i2c_data.buf[1] = 0x01;                         
// 	gs_i2c_data.buf[2] = 0x20;                         
// 	gs_i2c_data.buf[3] = 0x09;                         
// 	gs_i2c_data.buf[4] = 0x00;                         
// 	gs_i2c_data.buf[5] = 0x00;                         
// 	gs_i2c_data.buf[6] = 0x00;                         
// 	gs_i2c_data.buf[7] = (GS_I2C_ADDR>>1);             
// 	gs_i2c_data.buf[8] = 0x0;
// 	gs_i2c_data.buf[9] = 0x0;               
// 	
// 	addI2cChain(&gs_i2c_block, 0);
// 	startATask(gs_id, GS_SCAN_RATE, 0, gsReadData);
// 	while(gs_i2c_data.state == I2C_BUSY){
// 		SYS_UnlockReg();
// 		WDT_TIMER_RESET();   //Clear WDT timer
// 		SYS_LockReg();
// 	};
// 	gs_trigger = TRUE;
// 	startATask(gs_id, 200, 0, gsReadData); 	
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 gsReset
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// static void gsReset()
// {
// 	if(dev_state == DEV_STANDBY)
// 		return;
// 	gsDataInit();
// 	GPIO_SET_PIN(GS_CE_PORT_PIN);
// 	sysDelayMs(100);
// 	gs_trigger = FALSE;
// 	if(GS_I2C_ADDR != 0){
// 		initI2cDataBlock(&gs_i2c_block, 
// 						0XA8, 
// 						I2C_WR, 
// 						2, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 			
// 		gs_i2c_data.state = I2C_BUSY;
// 		gs_i2c_data.cmd = BROAD_ADDR;
// 		gs_i2c_data.buf[0] = GS_I2C_ADDR>>1;
// 		addI2cChain(&gs_i2c_block, 0);
// 		while(gs_i2c_data.state == I2C_BUSY);
// 	}
// 	sysDelayMs(2);
// 	initI2cDataBlock(&gs_i2c_block, 
// 						GS_I2C_ADDR, 
// 						I2C_WR, 
// 						10, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 		
// 	gs_i2c_data.state = I2C_BUSY;
// 	gs_i2c_data.cmd = SET_STATUS; // read fifo
// 	gs_i2c_data.buf[0] = 0x1f;                         
// 	gs_i2c_data.buf[1] = 0x01;                         
// 	gs_i2c_data.buf[2] = 0x20;                         
// 	gs_i2c_data.buf[3] = 0x00;                         
// 	gs_i2c_data.buf[4] = 0x00;                         
// 	gs_i2c_data.buf[5] = 0x00;                         
// 	gs_i2c_data.buf[6] = 0x00;                         
// 	gs_i2c_data.buf[7] = (GS_I2C_ADDR>>1);             
// 	gs_i2c_data.buf[8] = 0x0;
// 	gs_i2c_data.buf[9] = 0x0;               
// 	                                   
// 	
// 	addI2cChain(&gs_i2c_block, 0);
// 	while(gs_i2c_data.state == I2C_BUSY){
// 		SYS_UnlockReg();
// 		WDT_TIMER_RESET();   //Clear WDT timer
// 		SYS_LockReg();
// 	};
// 	sysDelayMs(50);
// 	gs_trigger = FALSE;
// 	startATask(gs_id, 500, 0, gsResetDone);
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 gsrReadStatus
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void do_gs_event();
// void gsReadData(void)
// {
// 	static u32 temp_counter = 500;
// 	u32 i;
// 	u16 temp1, temp2;

// 	if(dev_state == DEV_STANDBY)
// 		return;
// 	sysDelayUs(30);
// 	if(++temp_counter > 500){ // read temp every 2 second
// 	
// 			temp_counter = 0;

// 			initI2cDataBlock(&gs_i2c_block, 
// 						GS_I2C_ADDR, 
// 						I2C_RD, 
// 						4, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 			
// 			gs_i2c_data.state = I2C_BUSY;
// 			gs_i2c_data.cmd = 0x02;
// 			addI2cChain(&gs_i2c_block, 0);	
// 			startATask(gs_id, GS_SCAN_RATE, 0, gsReadData);
// 			while(gs_i2c_data.state == I2C_BUSY);

// 			 if(gs_i2c_data.state == I2C_IDLE){
// 				gs_trigger = TRUE;
// 				((U32_BYTE*)&gsdata.temp)->byte0 = gs_i2c_data.buf[1];
// 				((U32_BYTE*)&gsdata.temp)->byte1 = gs_i2c_data.buf[2];
// 			}

// 	}
// 	
// 			initI2cDataBlock(&gs_i2c_block, 
// 						GS_I2C_ADDR, 
// 						I2C_RD, 
// 						14, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 		
// 			gs_i2c_data.state = I2C_BUSY;
// 			gs_i2c_data.cmd = READ_DATA; // read fifo
// 			if(!addI2cChain(&gs_i2c_block, 0))
// 				while(1);
// 			while(gs_i2c_data.state == I2C_BUSY){
// 				SYS_UnlockReg();
// 				WDT_TIMER_RESET();   //Clear WDT timer
// 				SYS_LockReg();
// 			};

// 		
// 			initI2cDataBlock(&gs_i2c_block,  
// 						GS_I2C_ADDR, 
// 						I2C_WR, 
// 						1, 
// 						1, 
// 						(u8*)&gs_i2c_data
// 					);
// 			
// 			
// 			gs_i2c_data.state = I2C_BUSY; 
// 			gs_i2c_data.cmd = FIFO_SHFIT;
// 			addI2cChain(&gs_i2c_block, 0);
// 			while(gs_i2c_data.state == I2C_BUSY){
// 				SYS_UnlockReg();
// 				WDT_TIMER_RESET();   //Clear WDT timer
// 				SYS_LockReg();
// 			};
// 		
// 			if(gs_i2c_data.state == I2C_ERROR){
// error:			sendInfo("i2c read gs error!\r\n");
// 				GPIO_CLR_PIN(GS_CE_PORT_PIN);
// 				gs_trigger = FALSE;
// 				startATask(gs_id, 100, 0, gsReset);
// 				return;
// 			}else if(gs_i2c_data.state == I2C_IDLE){
// 				
// 				for(i=0; i<10; i++){
// 					gs_i2c_data.buf[i] = gs_i2c_data.buf[3+i];
// 				}
// 				//if(gs_verify())
// 				//	goto gs_end;
// 				gs_verify();

// 				/*
// 				uartdecode(rawData[0], " ");
// 				uartdecode(rawData[1], " ");
// 				uartdecode(rawData[2], " ");
// 				uartdecode(rawData[3], " ");
// 				iputs("\r\n");
// 				*/
// 				
// 				hpf();
// 				 
// 				lpf();
// 				
// 				recodePeaks();
// 				
// 				
// 				
// 				
// gs_end:				
// 			}

// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 gsensor_init
//  * @brief: 	 init the gpio pc13 to be the signal line and Timer2 to be the clock 
//  * @param: 
//  * @return: 
//  * @details:  
//  *-----------------------------------------------------------------------*/
// void gsensor_init(void)
// {
// 	GPIO_SET_MODE(GS_IRQ_PORT, GS_IRQ_PIN, GPIO_INPUT);  

// 	GPIO_CLR_PIN(GS_CE_PORT_PIN);
// 	GPIO_SET_MODE(GS_CE_PORT, GS_CE_PIN, GPIO_OUTPUT); 
// 	if(gs_id == 0xff)
// 		gs_id = getATask();
// 	gs_trigger = FALSE;
// 	startATask(gs_id, 50, 0, gsReset);
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 gs_event
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void gs_event()
// {
// 	if(dev_state == DEV_START && gs_trigger == TRUE && !GPIO_GET_PIN(GS_IRQ_PORT_PIN))
// 		 gsReadData();
// }
// /*---------------------------------------------
//    ---------------     GS  HOOK    -----------------
//    ---------------------------------------------*/

// void gshook1(void)
// {
// 	
// 	stopATask(gs_id);
// }

// void gshook2(void)
// {
// 		
// 	stopATask(gs_id);
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 gs_key
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void gs_key(enum KEYWORD key)
// {
// 	if(dev_state == DEV_START){
// 		switch(key){
// 			case INC_KEY: 
// 				if(amp_volume >= MAX_VOLUME -1)
// 					return;
// 				if(amp_volume < MAX_VOLUME -4)
// 					amp_volume += 2;
// 				else
// 					amp_volume = MAX_VOLUME-2;
// 				delStartkey(key);
// 				break;
// 			case DEC_KEY:
// 				if(amp_volume == 0)
// 					return;
// 				if(amp_volume > 2)
// 					amp_volume -= 2;
// 				else
// 					amp_volume = 1;
// 				delStartkey(key);
// 				break;
// 			case MOVIE_KEY: 
// 				if(state_color == BLUETOOTH_COLOR
// 				&& BT_MATCH_FLAG == 1){
// 					led_flash = 4;
// 					GPIO_SET_PIN(PC9);
// 					startATask(gs_id, 300, 0, gshook2);
// 					act_interval = 22;
// 				}
// 				break;
// 			case GAME_KEY:
// 				if(state_color == BLUETOOTH_COLOR 
// 				&& BT_MATCH_FLAG == 1){
// 					led_flash = 4;
// 					GPIO_SET_PIN(PC8);
// 					startATask(gs_id, 300, 0, gshook1);
// 					act_interval = 22;
// 				}
// 				break;
// 			default: 
// 				break;
// 		}
// 	}else if(dev_state == DEV_TEST){
// 		switch(key){
// 			case INC_KEY: case DEC_KEY:
// 				led_flash = 4;
// 				playMusic(1);
// 				break;
// 			case MOVIE_KEY: case GAME_KEY:
// 				led_flash = 4;
// 				playMusic(0);
// 				break;
// 			default:
// 				break;
// 		}
// 	}else{
// 		delStandbytkey(key);
// 	}
// }

// void analyseMaxFrame(u32 *queue)
// {
// 	u32 i, max_c, min_c, temp = 0;

// 	for(i = 0; i < 4; i++){
// 		if(temp < queue[i]){
// 			temp = queue[i];
// 			max_c = i+1;
// 		}
// 	}


// 	temp = 0xfffff;
// 	for(i = 0; i < 4; i++){
// 		if(temp > queue[i]){
// 			temp = queue[i];
// 			min_c = i+1;
// 		}
// 	}

// 	
// 	i = max_c*min_c; 
// 	
// 	if(i ==  6){
// 		
// 		if(max_c == 2){
// 			//sendInfo("Gsensor X DOWN\r\n");
// 			btNextMusic();
// 			ledColorSet(WHITE_COLOR);
// 			sysDelayMs(100);
// 		}else{
// 			//sendInfo("Gsensor X UP\r\n");
// 			btLastMusic();
// 			ledColorSet(YELLOW_COLOR);
// 			sysDelayMs(100); 
// 		}

// 	}else if(i == 4){ 
// 	
// 		if(max_c == 4){
// 			//sendInfo("Gsensor Y RIGHT\r\n");
// 			//ledColorSet(LIGHT_BLUE_COLOR);
// 			//sysDelayMs(100);
// 		}else{
// 			//sendInfo("Gsensor Y LEFT\r\n");
// 			//ledColorSet(SOE_COLOR);
// 			//sysDelayMs(100);
// 		}

// 	}else if(i == 2){

// 	}
// 	
// }


// void analyseMinFrame(u32 *queue)
// {

// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 gs_event
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void do_gs_event()
// {
// 	static s32  ch_max[4], ch_min[4];
// 	u32 i, avi_max, avi_min;

// 	avi_max = 0;
// 	avi_min = 0;
// 	for(i = 0; i < 4; i++){
// 		avi_max += gs_channels[i].max_frame;
// 		avi_min += gs_channels[i].min_frame;
// 		gs_channels[i].min = GS_MAX;
// 		gs_channels[i].max = 0;
// 	}

// 	avi_max >>= 2;
// 	avi_min >>= 2;

// 	for(i = 0; i < 4; i++){
// 		
// 		ch_max[i] = gs_channels[i].max_frame;
// 		ch_min[i] = gs_channels[i].min_frame;
// 		
// 		if(gs_channels[i].max_frame - avi_max > 200){
// 			if(gs_channels[i].max_frame < 100)
// 				ch_max[i] += 0x10000;
// 		}
// 		
// 		if(gs_channels[i].min_frame - avi_min > 200){ 
// 			if(gs_channels[i].min_frame < 100)
// 				ch_min[i] += 0x10000;
// 		}	

// 		if(ch_min[i] < ch_max[i])
// 			return;
// 	}


// 	analyseMaxFrame(ch_max);
// 	analyseMinFrame(ch_min);

// }





