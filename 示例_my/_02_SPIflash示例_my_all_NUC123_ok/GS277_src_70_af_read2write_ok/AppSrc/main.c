/****************************************************************************
 * @file     main.c
 * @version  V1.00	
 * @breif:  main.c 		
 * @date: 2013/2/21			
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/

#include <stdio.h>
#include "NUC123Series.h"
#include "sysinc.h" 
#include "filter.h"

extern u8 flash_status;

u8 wrbuf[256]={0};
u32 iwraddr =0x10000;

//void Program_Page_Data(void);
void Program_Page_Data(u32 ipageaddr);
void Erase_All_Chip(void);
void Erase_Page_Data(u32 ipageaddr);

#define VER_SOUND 1     //MA 2O150615   add ver_sound play function   1 for ennable  0 for disable 
#define FLASH_MUSIC_TEST 1    //MA 20150615  add flash music test
/*-------------------------------------------------------------------------
 * @fucname:	 CLK_Init
 * @brief: 	 sysclk init and other IP clk init
 * @param:	 none
 * @return: 	 none
 * @details: 	 set PLL to 72M, HCLK = 72M, PCLK = 72M 
 *-----------------------------------------------------------------------*/
void CLK_Init(void)
{
	SYS_UnlockReg();                                        //En:Unlock protected registers
	GPIO_XT1();
	CLK_HXT_ENABLE();                                        //En:Enable External XTAL (4~24 MHz)
	while(!CLK_IS_STABLE(CLK_HXT_STB)); 		//En:Waiting for 12MHz clock ready
	CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4A54); // 0X4A54 PLL 73.714M.  0x4016 is 72M 
	while(!CLK_IS_STABLE(CLK_PLL_STB));
	SET_HCLK_CLKSRC(CLK_HCLK_PLL);
	CLK_SET_APBDIV(CLK_APBDIV_DIV2);
	SystemCoreClockUpdate();                                //En:Update the variable SystemCoreClock
	SysTickInit(6000); 		// systick per 1ms 
	adcKeyClkInit();	// adc clk init
	irClkInit();	// IR KEY CLK INIT
	SYS_LockReg();                                          //En:Lock protected registers
}



/*-------------------------------------------------------------------------
 * @fucname:	 Hal_Init
  * @brief:	chip and board init
  * @param:	None
  * @return:	None
  * @details:	Init mcu sys clock and mcu interface used in the board 
  			and other chips on the board 
 *-----------------------------------------------------------------------*/
void Hal_Init(void)
{
	u32 status;
	
	
	
	ENTER_SECTION(status);	// disable irq and systick
	
	CLK_Init();  //Init system clock to 72Mhz and other ip(bus) clk

	OUT_SECTION(status);  // enable irq and systick
	
	sysTaskInit(); // systask init 

	
	dealPowerKey();
	
	ledInit();	// init the pin to pwm function
	

	//openMcuCtrlPin(); // peripheral chip power is off 

	
}

/*-------------------------------------------------------------------------
 * @fucname:	 showVersion
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
// u32 f_test = 0;                  //ma 20150706   save  aprom     
// u8 v[]  = "v2.62";//VERSION;
// void showVersion()
// {
// 	
// 	u32 f_n, mid_n, end_n, i;
// 	
// 	f_n = v[1] - '0';
// 	mid_n = v[3] - '0';
// 	end_n = v[4] - '0';

// 	for(i = 0; i < f_n; i++){
// 		sysDelayMs(500);
// 		ledColorSet(GREEN_COLOR);
// 		sysDelayMs(200);
// 		ledColorSet(DARK_COLOR);
// 	}

// 	for(i = 0; i < mid_n; i++){
// 		sysDelayMs(500);
// 		ledColorSet(WHITE_COLOR);
// 		sysDelayMs(200);
// 		ledColorSet(DARK_COLOR);
// 	}

// 	for(i = 0; i < end_n; i++){
// 		sysDelayMs(500);
// 		ledColorSet(BLUE_COLOR);
// 		sysDelayMs(200);
// 		ledColorSet(DARK_COLOR);
// 	}

// 	sysDelayMs(500);
// 	systick_counter = 100;
// 	f_test = 1;
// 	
// }
// extern u8 BT_VERSION_NUMBER[4];
// extern u8 MCU_VERSION_NUMBER[4];
// extern u8 WL_VERSION_NUMBER[4];
// extern u8 FLASH_VERSION_NUMBER[4];

void bootLDROM()
{
	UNLOCKREG();
	outpw(&SYS->RSTSRC, 3);//clear bit
	outpw(&FMC->ISPCON, (inpw(&FMC->ISPCON) & 0xFFFFFFFC) |0x2);
	outpw(&SCB->AIRCR, (0x05FA0000UL | 0x00000004UL));//SYSRESETREQ
	while(1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
extern u32 dc_time_stamp;
u8 upgrade_buf[4];
extern u32 readBuf(u8 *buf, u32 address, u32 count);
u8 COCO_KEY_BEFORE = 0; 	  // ma 20150507
u8 coco_local[3] = {0x00,0x00,0x00};
extern u32 ir_key_bits;     //ma 20150616  add  irkey bits
extern u32 ccp;       //ma 20150521  extern  ccp for  bt 
u8 eeprom_test_data = 0x55;   //ma 20150612

int main(void)
{
	u32 cc;
	//u32 k;   // ma 20150629 
	u32 ir_count =0;     // ma 20150616   add ir_count  for factory mode 
	Hal_Init(); 		// init system clocl and interface used in the board
// 	getConfig();
	
BTInit();  //--zz debug		
BTSendCmd("main--01.\r\n");  //--zz debug		
uartdecode(0xf8,"  code1: ");				
	
	ledTaskInit();  // flash the led in the borards
	adckeyTaskInit();	 // adc key task init
	irKeyTaskInit();	 // ir key task init 
	cs8422Init();  
	seW58Init(); 
	DWPH83Init();  	

	openMcuCtrlPin();
	MAIN_POWER_ON();
	standbyPowerCtrlSet();
	sysDelayMs(100);
	
	/****check if enter factory mode***/ //20150617
	while(GPIO_GET_PIN(PB6)&&(DECT_DC()))           //ma  20150910  IR always low     
	{
		ir_count++;
		sysDelayMs(20);
		if(ir_count>50)
		{
			factory_mode_flag = 1;
			ir_count=0;
			break;
		}
	}
	getConfig();
	
	/****volume store****/
	volume_store = amp_volume;
	if(volume_store>25)
	{
		volume_store = 25;
	}
	else if(volume_store <9)
	{
		volume_store = 9;
	}
	cc = readBuf(&upgrade_buf[0], 0, 4);
	
	if(cc == 0)
		 bootLDROM();
	/***PRE-READ COCO*****/
	if(dev_input == BLUETOOTH)       //ma 20150608
		bt_power_flag = 0;

	
// 	devTest();                           //ma 20150707   del useless function   
// 	if(dev_state == DEV_TEST)
// 		goto L1;

	if(DECT_DC())
		dc_time_stamp = 1;

	GPIO_SET_MODE(PB,7,GPIO_INPUT);
	
	//dc_time_stamp = 0;
	if(dc_time_stamp){
		I2SClkInit(48000);	// init i2s
		i2sStart();
	#if(UART_EN)
		uartInit();
	#endif
		
		/*********add enable /disable adc key power on function******/
		if(dc_auto_down_flag == 0)        //ma 20150629        
			enterStandby();
		else if(dc_auto_down_flag == 5)      //ma 20150706  add sleep
			sleep();
			
	}else{
// 		if(factory_mode_flag)
// 			dev_input = AUX; 
// 		else 
		
		if(DECT_NFC()) 
		{
		NFC_flag = 1;    //ma 20150609
		bt_power_flag = 0;
		dev_input = BLUETOOTH;  //MA 20150609	
		}	
		state_color = input_channel_color[dev_input];
		ledColorSet(state_color);
		
// L1:                                              //ma 20150707   del useless function   
		outStandby();
		/*****set volume*****/  // 20150617
		amp_volume = volume_store;
		
// #if(FLASH_MUSIC_TEST)	   //ma 20150615  play all flash sound 
// 		play_all_sound();
// #endif
// 		if(!SB_flag)       // ma 201606 
// 			Dwhp83Slave();
// 		else 
// 			Dwhp83Master();
// 		sysDelayMs(500);   // ma 20150515  wait change to mu
		
	}

	//dwhp83TestProcess();
/***eeprom test****/
// 	writeeeprom(eeprom_test_data,0x08);
//   sysDelayMs(30);
// 	eeprom_test_data = readeeprom(0x08);
// 	sysDelayMs(5);
// 	if(eeprom_test_data == 0x55)
// 		ledColorSet(GREEN_COLOR);
// 	sysDelayMs(1000);
// 	
// 	eeprom_test_data = 0xAA;
// 	writeeeprom(eeprom_test_data,0x08);
//   sysDelayMs(30);
// 	eeprom_test_data = readeeprom(0x08);
// 	sysDelayMs(5);
// 	if(eeprom_test_data == 0xAA)
// 		ledColorSet(PURPLE_COLOR);
// 	sysDelayMs(1000);
// 	
// 	eeprom_test_data = 0xF0;
// 	writeeeprom(eeprom_test_data,0x08);
//   sysDelayMs(30);
// 	eeprom_test_data = readeeprom(0x08);
// 	sysDelayMs(5);
// 	if(eeprom_test_data == 0xF0)
// 		ledColorSet(GREEN_COLOR);
// 		while(1);
// 	sysDelayMs(1000);
// 		ccp = 8;     //ma  20150521 set ccp  to start btreconnect 


//-----------------------------------------------

// 	  u8 wrbuf[256]={0};

// Erase_Page_Data(0x00000); 

// 			iwraddr =0x000000;
// 			wrbuf[0]='1';
// 			wrbuf[1]='2';
// 			wrbuf[2]='3';
// 			
// 			//spiFlashPageWrite(iwraddr, 256, wrbuf);		
// 			spiFlashPageWrite(iwraddr, 3, wrbuf);		
			
// 			iwraddr =0x001000;
// 			wrbuf[0]=7;
// 			wrbuf[1]=8;
// 			wrbuf[2]=9;
// 			
// 			spiFlashPageWrite(iwraddr, 256, wrbuf);		
			

  //Program_Page_Data(0x000);	 //sysDelayMs(1000); 
  //Program_Page_Data(0x200); 	
	//Erase_All_Chip(); 
	
	//Erase_Page_Data(0x00000); 
	//Erase_Page_Data(0x20000); 
	
	
	spiFlashRead(0, 256, wrbuf );
	spiFlashPageWrite(0x10000, 256, wrbuf);		
	
	while(1) 
	{			
// 		static u32 isoundcount=0;
// 		static u8 isound =0;			
// 		
// 		if (isoundcount++>580000)
// 		{
// 			isoundcount=0;
// 			if(isound>9)  isound=0;    //DD_SOUND
// 		
// 			//playMusic(isound++);	 
// 			playMusic(7);		
// 		}			
 		
// 		/****auto COCO*****/            //MA 20150805 del useless function
// 		if((WL_COCO_KEY != COCO_KEY_BEFORE)&&(WL_COCO_KEY != 0)&&(SB_play_flag == 0)&&(!SB_flag))  //ma 20150515  add sbplay flag condition
// 		{
// 			if(WL_COCO_KEY == 1)                                  
// 				{
// 					coco_local[0] = 0x01;
// 					coco_local[1] = 0x01;
// 					coco_local[2] = 0x01;
// 					writeDwhp83(coco_local, 0xc6, 3);
// 					sysDelayMs(20);
// 				}
// 				else if(WL_COCO_KEY == 2)
// 				{
// 					coco_local[0] = 0x02;
// 					coco_local[1] = 0x02;
// 					coco_local[2] = 0x02;
// 					writeDwhp83(coco_local, 0xc6, 3);
// 					sysDelayMs(20);
// 				}
// 				else if(WL_COCO_KEY == 3)
// 				{
// 					coco_local[0] = 0x03;
// 					coco_local[1] = 0x03;
// 					coco_local[2] = 0x03;
// 					writeDwhp83(coco_local, 0xc6, 3);
// 					sysDelayMs(20);
// 				}
// 				else if(WL_COCO_KEY == 4)
// 				{
// 					coco_local[0] = 0x04;
// 					coco_local[1] = 0x04;
// 					coco_local[2] = 0x04;
// 					writeDwhp83(coco_local, 0xc6, 3);
// 					sysDelayMs(20);
// 				}
// 				else 
// 				{
// 					coco_local[0] = 0x00;
// 					coco_local[1] = 0x00;
// 					coco_local[2] = 0x00;
// 					writeDwhp83(coco_local, 0xc6, 3);
// 					sysDelayMs(20);                               //ma 20150507
// 				}
// 				COCO_KEY_BEFORE = WL_COCO_KEY;
// 			}
		
#if(DMAI2S == TRUE)		 
		filter();
#endif    

#if(UART_EN)
// 	sendBack();
#endif

	btGetMsg();
	
//#if(GESTURE_EN)
//	gs_event();
//#endif

#if((SPIFLASH_ENABLE == TRUE) && (ADPCM_FORMAT == IMA_ADPCM))
		if(flash_mode == FLASH_RECODE_MUSIC)
			writeFlashRecodeData();
#endif

		runTimedTasks(); //run timed tasks

		//DISABLE_SYSTICK();	
		//__nop();	// int may happen at the next instrument that disable systick count 
	  /*	
	   *  disable systick to counter will delay the systick int
	   *	and disble systick int will lose systick int during,
	   *	so I should look the COUNTFLAG in SYS_CSR.16
	   *	to see whether it had a systick int. These was too trouble.
	   *	so i did't protect it because i can do some remedies in outputdebugsignal() 
	   */
		
	  //MipsCounter++;	
		//ENABLE_SYSTICK();
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();		

	} 
		
}



//================================================================

// void Program_Page_Data(void)
// {
// 	u32 uiCounter1;
// 	
//     SPI_SET_SS0(SPI1);   
// 	
// //     SPI_Send_1W(WRITER_ENABLE, 0x08);      
// //     while(SPI_IS_BUSY(SPI1) != 0); 
// 	  spiSendWord(M25P80_WRENABLE, 0x08); 
//     SPI_CLR_SS0(SPI1);                               

//     SPI_SET_SS0(SPI1);                                
//     spiSendWord(M25P80_PAGE_PROG, 0x08);    
//     //while(SPI_IS_BUSY(SPI1) != 0);                  

//     spiSendWord(0x000000, 24);                  
//     //spiSendWord(0x10000, 24);                  
//     //while(SPI_IS_BUSY(SPI1) != 0);                

//     for(uiCounter1=0; uiCounter1<256; uiCounter1++)   
//     {	
//         //spiSendWord(uiCounter1, 8);                 
//         spiSendWord(7, 8);                 
//         //while(SPI_IS_BUSY(SPI1) != 0);                
//     }
//     SPI_CLR_SS0(SPI1);                                   
//                                                                                              
//     SPI_SET_SS0(SPI1);                                  
//     spiSendWord(M25P80_WRDISABLE, 0x08);     
//     //while(SPI_IS_BUSY(SPI1) != 0);                 
//     SPI_CLR_SS0(SPI1);                                 
// }

//void Program_Page_Data(void)
void Program_Page_Data(u32 ipageaddr)
{
	  u32 uiCounter1;
	
	  SPI_CLR_SS0(SPI1);          //--zz
    SPI_DISABLE_REORDER(SPI1);  //--zz
	
	
    SPI_SET_SS0(SPI1);   	
	  spiSendWord(M25P80_WRENABLE, 0x08);   //send write
    SPI_CLR_SS0(SPI1);                               

// // 	   __nop();   //--zz

    SPI_SET_SS0(SPI1);                                
    spiSendWord(M25P80_PAGE_PROG, 0x08);     
    spiSendWord(ipageaddr, 24);                 
	
	
	  for(uiCounter1=0; uiCounter1<256; uiCounter1++)                  
      spiSendWord(2, 8);  //spiSendWord(uiCounter1, 8);  // 	

    SPI_CLR_SS0(SPI1);                                   
    SPI_DISABLE_REORDER(SPI1);  //--zz
		
		
    SPI_SET_SS0(SPI1);                                  
    spiSendWord(M25P80_WRDISABLE, 0x08);                  
    SPI_CLR_SS0(SPI1);  
		

		SF_GET_STATUS();        //--zz
		while(flash_status&1)   //--zz
		SF_GET_STATUS();        //--zz
}


void Erase_All_Chip(void)
{
	SPI_CLR_SS0(SPI1);          //--zz
	SPI_DISABLE_REORDER(SPI1);  //--zz

	SPI_SET_SS0(SPI1);   
	spiSendWord(M25P80_WRENABLE, 0x08); 
	SPI_CLR_SS0(SPI1);                               

	SPI_SET_SS0(SPI1);                                
	spiSendWord(M25p80_BE, 0x08);    
	SPI_CLR_SS0(SPI1);                                   
																																													 
	SPI_SET_SS0(SPI1);                                  
	spiSendWord(M25P80_WRDISABLE, 0x08);               
	SPI_CLR_SS0(SPI1);  

// 	SF_GET_STATUS();        //--zz     //must to remove these code
// 	while(flash_status&1)   //--zz
// 	SF_GET_STATUS();        //--zz		
}


void Erase_Page_Data(u32 ipageaddr)
{
	SPI_CLR_SS0(SPI1);          //--zz
	SPI_DISABLE_REORDER(SPI1);  //--zz

	SPI_SET_SS0(SPI1);   
	spiSendWord(M25P80_WRENABLE, 0x08); 
	SPI_CLR_SS0(SPI1);                               

	SPI_SET_SS0(SPI1);                                
	spiSendWord(M25p80_SE, 0x08);  
  spiSendWord(ipageaddr, 24);    
	SPI_CLR_SS0(SPI1);                                   
																																													 
	SPI_SET_SS0(SPI1);                                  
	spiSendWord(M25P80_WRDISABLE, 0x08);               
	SPI_CLR_SS0(SPI1);     

	SF_GET_STATUS();        //--zz
	while(flash_status&1)   //--zz
	SF_GET_STATUS();        //--zz	
}

