/****************************************************************************
 * @file     main.c
 * @version  V1.00	
 * @breif:  main.c 		
 * @date: 2013/2/21			
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include <stdio.h>
#include "NUC123Series.h"
#include "sysinc.h" 
#include "filter.h"

#define FIXED_COCO_MODE  1 


extern u8 wrbuf[256];
u32 iwraddr =0x100000;  //1M

u8 rdbuf[256]={0};

void Erase_Page_Data(u32 ipageaddr);

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
	ledClkInit();				// led pin pwm init
	adcKeyClkInit();	// adc clk init
	SYS_LockReg();                               //En:Lock protected registers
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
	
// 	openMcuCtrlPin(); // peripheral chip power is off       //MA 20150104

	
}

/*-------------------------------------------------------------------------
 * @fucname:	 showVersion
 * @brief: 
 * @param: 
 * @return: 
 * @details: 
 *-----------------------------------------------------------------------*/
u32 f_test = 0;
void showVersion()
{
	u8 version[] = VERSION;
	u32 f_n, sec_n, mid_n,  end_n, i;

	f_n = version[1] - '0';
	sec_n = version[2] - '0';
	mid_n = version[4] - '0';
	end_n = version[5] - '0';

	for(i = 0; i < f_n; i++){
		sysDelayMs(500);
		ledColorSet(YELLOW_COLOR);
		sysDelayMs(200);
		ledColorSet(DARK_COLOR);
	}
	for(i = 0; i < sec_n; i++){
			sysDelayMs(500);
			ledColorSet(SOE_COLOR);
			sysDelayMs(200);
			ledColorSet(DARK_COLOR);
	}
	for(i = 0; i < mid_n; i++){
		sysDelayMs(500);
		ledColorSet(PURPLE_COLOR);
		sysDelayMs(200);
		ledColorSet(DARK_COLOR);
	}

	for(i = 0; i < end_n; i++){
		sysDelayMs(500);
		ledColorSet(GREEN_COLOR);
		sysDelayMs(200);
		ledColorSet(DARK_COLOR);
	}

	f_test = 1;
	
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
			/*---------------------------------------------------------------------------------------------------------*/
extern u32 dc_time_stamp;
extern void DEVtest(void);
extern u32 Dwhp83_CuMu;
int main(void)
{
// 	u32 cc, k;
	
	Hal_Init();
// #if(FIXED_COCO_MODE)
// 	getConfig();     //ma  20150526   
// #endif	
	ledTaskInit(); // flash the led in the borards
	//irKeyTaskInit();	// ir key task init 
	pt2314Init(); 
	ampInit();
	adckeyTaskInit();	// adc key task init
	DWPH83Init();
// 	i2cSysInit(); //i2c init
// 	DEVtest();
// 	if(f_test)
// 		goto L1;
// 		
// 	cc = 3;
// 	k = 6;

// 	if(DECT_NFC())
// 		goto L1;
// 	
// 	if(DECT_DC()){
// 		dc_time_stamp = 0;
// 		while(k){
// 			k--;
// 			if(!GPIO_GET_PIN(POWER_KEY_PORT_PIN)){
// 				--cc;
// 				if(cc == 0)
// 					dc_time_stamp = 1;
// 			}else{
// 				cc = 3;
// 			}	
// 			sysDelayUs(20000);
// 		}
// 	}

// 	if(dc_time_stamp){
// 		I2SClkInit(48000);	// init i2s
// 		i2sStart();
// 		uartInit();
// 		sysDelayMs(200);
// 		spiFlashInit();
// 		ledColorSet(RED_COLOR);
// 		powerOn();//sleep();
// 		powerOff();
// // #if(FIXED_COCO_MODE)       //ma 20150525 
// // 		if(cumu_flag == 0)
// // 		{
// // 			Dwhp83Slave();
// // 		}
// // 		else 
// // 		{
// // 			Dwhp83Master();
// // 		}
// // #elif(!FIXED_COCO_MODE)
// // 		Dwhp83Slave();
// // 		cumu_fgflag = 0;   //ma 20150522
// // #endif
// 	}else{
// L1:
// 		sysDelayMs(200);
// 		ledColorSet(state_color);
		outStandby();
// // 		#if(FIXED_COCO_MODE)                               //MA 20151201
// // 		if(check_coco())          //ma 20150521  function error    //ma 20151130
// // 		{
// // 			sysDelayMs(100);        //ma 20150525 add 2sec to dwhp83
// // 			Dwhp83FixedCoCo();         //ma  20150521  set fixed  coco
// // 		}
// // 		#endif
// // #if(FIXED_COCO_MODE)       //ma 20150525 
// // 		if(cumu_flag == 0)
// // 		{
// // 			Dwhp83Slave();
// // 		}
// // 		else 
// // 		{
// // 			Dwhp83Master();
// // 		}
// // #elif(!FIXED_COCO_MODE)
// // 		Dwhp83Slave();
// // 		cumu_fgflag = 0;   //ma 20150522
// // #endif
// 	}


// iputs("main--01.\r\n");  //--zz debug		


//   //--test write flash 
// 	iwraddr =0xf0000;
// 	wrbuf[0]='1';
// 	wrbuf[1]='2';
// 	wrbuf[2]='3';

// iputs("begin write.\r\n");  //--zz debug		

// 	//spiFlashPageWrite(iwraddr, 256, wrbuf);		
// 	spiFlashPageWrite(iwraddr, 3, wrbuf);	
// 			
// 	sysDelayMs(200);
// 	
//   //--test read flash
// 	
// iputs("begin read.\r\n");  //--zz debug		
// 	
//   spiFlashRead(iwraddr, 3, rdbuf);	 
// 	
//   uartdecodeH(rdbuf[0]," buf0: "); 
//   uartdecodeH(rdbuf[1]," buf1: "); 
//   uartdecodeH(rdbuf[2]," buf2: "); 
// 	
// 	breath_flag = 0xff;	
// 	
// 	sysDelayMs(20000);


//-----------------------------------------------

  iputs("main--01.\r\n");  //--zz debug		

// iputs("begin write.\r\n");  //--zz debug		

// // 	  u8 wrbuf[256]={0};

// 	Erase_Page_Data(0xf0000); 
// 	iwraddr =0xf0000;
// 	wrbuf[0]='A';
// 	wrbuf[1]='B';
// 	wrbuf[2]='C';
// 	
// 	//spiFlashPageWrite(iwraddr, 256, wrbuf);		
// 	spiFlashPageWrite(iwraddr, 3, wrbuf);	


//   //--test read flash
// 	
// iputs("begin read.\r\n");  //--zz debug		
// 	
//   spiFlashRead(iwraddr, 3, rdbuf);  	 
// 	
//   uartdecodeH(rdbuf[0]," buf0: "); 
//   uartdecodeH(rdbuf[1]," buf1: "); 
//   uartdecodeH(rdbuf[2]," buf2: "); 

iputs("begin write.\r\n");  //--zz debug		

// 	  u8 wrbuf[256]={0};

      iwraddr =0xe8000;
			Erase_Page_Data(iwraddr); 
			
			wrbuf[0]='3';
			wrbuf[1]='6';
			wrbuf[2]='9';
			
			//spiFlashPageWrite(iwraddr, 256, wrbuf);		
			spiFlashPageWrite(iwraddr, 3, wrbuf);	
			

  //--test read flash
	
iputs("begin read.\r\n");  //--zz debug		
	
  spiFlashRead(iwraddr, 3, rdbuf);  	 
	
  uartdecodeH(rdbuf[0]," buf0: "); 
  uartdecodeH(rdbuf[1]," buf1: "); 
  uartdecodeH(rdbuf[2]," buf2: "); 
	
sysDelayMs(12000);

	return 0;
	
  //--Erase_Page_Data(WRITEFLASHSTART);  //0X20000

  playMusic2(8);

	while(1){
		
		if(flg_testflash == 2) return 0;	
		
		
		#if(DMAI2S == TRUE)		 
		filter();
		#endif

		if(!GPIO_GET_PIN(WL_IRQ_PORT_PIN)){
			DWHP83DealMsg();
		}

		#if(UART_EN)
		sendBack();
		#endif

		#if(GESTURE_EN)
		gs_event();
		#endif
	
		#if(SPIFLASH_ENABLE == TRUE && ADPCM_FORMAT == IMA_ADPCM)
		if(flash_mode == FLASH_RECODE_MUSIC)
			writeFlashRecodeData();
		#endif		
		runTimedTasks(); // run timed tasks

		if(reset_en == 1){
			if(dev_input != WIRELESS)
			{
				AMP_MUTE();
				sysDelayMs(100);
				standbyPowerCtrlCLR(); 
				sysDelayMs(200);
	// 			reset_en = 0;
				outStandby();
			}
			else 
			{
				if(!isDwhp83Linked())
				{
					reset_en = 0;
				}
				else
				{
// 					AMP_MUTE();
// 				sysDelayMs(100);
// 				standbyPowerCtrlCLR(); 
// 				sysDelayMs(200);
	// 			reset_en = 0;
// 				outStandby();
					reset_en = 0;
				}
			}
		}
		
		if(dev_state == DEV_STANDBY && !DECT_DC()){
			// if didn't dect DC and it is in DEV_STANDBY, then turn off the power
			doPowerOff();
		}
		SYS_UnlockReg();
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();
	} 

}

void Erase_Page_Data(u32 ipageaddr)
{
	SPI_CLR_SS0(SPI2);          //--zz
	SPI_DISABLE_REORDER(SPI2);  //--zz

	SPI_SET_SS0(SPI2);   
	spiSendWord(M25P80_WRENABLE, 0x08); 
	SPI_CLR_SS0(SPI2);                               

	SPI_SET_SS0(SPI2);                                
	spiSendWord(M25p80_SE, 0x08);  
  spiSendWord(ipageaddr, 24);    
	SPI_CLR_SS0(SPI2);                                   
																																													 
	SPI_SET_SS0(SPI2);                                  
	spiSendWord(M25P80_WRDISABLE, 0x08);               
	SPI_CLR_SS0(SPI2);     

	SF_GET_STATUS();        //--zz
	while(flash_status&1)   //--zz
	SF_GET_STATUS();        //--zz	
}






