
#include <stdio.h>
#include "NUC123Series.h"
#include "sysinc.h" 
//#include "filter.h"
#include "gpio.h"
#include "74hc4096.h"

extern s32 breath_flag;
extern void fadeoff( u32 color);
extern DWHP83_RMSG dwhp83_receive_msg;

//--#define CMD_TOUCHKEY			0x30  

extern DWHP83_STATE dwhp83_status;
extern u32 Dwhp83_route_count;

//----------------- GPIO INT -------------------------------------------------
#define GPIO_PMD_QUASI          0x3UL        //!< Quasi-bidirectional Mode 
#define GPIO_IMD_EDGE           0UL            //!< IMD Setting for Edge Trigger Mode 
#define GPIO_IEN_IR_EN_Pos          16         //!< GPIO IEN: IR_EN Position 
#define GPIO_DBCLKSRC_LIRC      0x00000010UL   //!< DBNCECON setting for de-bounce counter clock source is the internal 10 kHz 
#define GPIO_IEN_IF_EN_Pos          0          //!< GPIO IEN: IF_EN Position 

extern u32 writeCAP8298(u32 data, u32 address);


u8 coco[3]={0};

u8 icmd=0;
enum KEYWORD ikey=NONE_KEY;
u8 ikey_vol=0;
u8 flg_irmute=0;

void CLK_Init(void)
{
	SYS_UnlockReg();                                        //En:Unlock protected registers
	GPIO_XT1();
	CLK_HXT_ENABLE();                                        //En:Enable External XTAL (4~24 MHz)
	
	while(!CLK_IS_STABLE(CLK_HXT_STB)); 		  //En:Waiting for 12MHz clock ready
	CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4A54);   // 0X4A54 PLL 73.714M.  0x4016 is 72M 
	
	while(!CLK_IS_STABLE(CLK_PLL_STB));
	SET_HCLK_CLKSRC(CLK_HCLK_PLL);
	
	//CLK_SET_APBDIV(CLK_APBDIV_DIV1);
	CLK_SET_APBDIV(CLK_APBDIV_DIV2);
	
	SystemCoreClockUpdate();  //En:Update the variable SystemCoreClock	
	
	SysTickInit(6000); 		// systick per 1ms 
	adcKeyClkInit();	// adc clk init
	
	//irClkInit();	  // IR KEY CLK INIT

	SYS_LockReg();     // En: Lock protected registers	
}

void Hal_Init(void)
{
	u32 status;	  
	
	ENTER_SECTION(status);	// disable irq and systick   
	
	CLK_Init();  //Init system clock to 72Mhz and other ip(bus) clk  //-- 73.714MHz  

	OUT_SECTION(status);  // enable irq and systick  
	
	//--I2SClkInit(48000); 

		
  sysTaskInit(); // systask init   
	
	ledInit();	// init the pin to pwm function  
	
	//penMcuCtrlPin(); // peripheral chip power is off 	  
}

 

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
extern u32 dc_time_stamp;   
//--extern void devTest();  
extern u32 volatile motor_cc;  
extern u32 motor_direction;  
u32 bvb = 100;  
extern u32 bt_cycle;  
u8 upgrade_buf[4];  
extern u32 ack_error;  
extern void readBuf(u8 *buf, u32 address, u32 count);  
u8 DC_FLAG = 0;  //--u32 DC_FLAG = 0;  

extern u8 DEVICE_ADDRESS;  

u8 Key_Scan(u8 mode);

//volatile u32 delay_us = 0;

void resetHook(void);   


int main(void)   
{  
 	u8  ibuf[4];
 	u8  ibuf2[4];
   u32 idev=0;

	
	Hal_Init();  

	flg_softmute=1;
	ledTaskInit(); // flash the led in the borards

	
//	//openMcuCtrlPin(); 
	PD->PMD &= ~(3UL<<(5*2));	
	PD->PMD |= 1<<(5*2);	
	MAIN_POWER_ON();  

	
//--	//sysDelayMs(100);		
	
	uartInit();       //--zz debug 
	ledInit();
	breath_flag=0xff;
	ledColorSet(BLUE_COLOR);	 	//RED_COLOR
	
	i2cSysInit();   //PF2,PF3	
	
	adckeyTaskInit();	//adc key task init
	
	
	//iputs("main--01.\r\n");  //--zz debug         
	iputs("\r\n------------------ ");  //--zz debug         
	iputs(VERSION);       
	iputs(" -----------------\r\n");  //--zz debug         
		

	spiFlashInit();  //--zz
	
	

	WDTInit();	    //init watch dog timer	 
	


//-------------dw1000 read test------------------------------------	

    reset_DW1000(); 
    spi_set_rate_low();			
	 sysDelayMs(2);	   
		

	 spi_set_rate_high();  
    sysDelayMs(2);	 		
		
		

//-------------dw1000 read test------end---------------------------	


   sysDelayMs(1000);	    
	
	flg_softmute=0;
	
  while(1)     
	{	 
 
//-- iputs("main--031.\r\n");  //--zz debug      

//   sysDelayMs(150); 
	//ledColorSet((enum LED_COLOR)2);  		
		
		
//-------------test dw1000--------------------------		
		
		
		spiFlashRead2(0x0,4,ibuf);
      uartdecodeH(ibuf[0],"  0ibuf0: ");
      uartdecodeH(ibuf[1],"  0ibuf1: ");
      uartdecodeH(ibuf[2],"  0ibuf2: ");
      uartdecodeH(ibuf[3],"  0ibuf3: ");
		iputs("\r\n"); 

		sysDelayMs(3000);	   
	
//		spiFlashRead2(0x01,4,ibuf);
//    uartdecodeH(ibuf[0],"  1ibuf0: ");
//    uartdecodeH(ibuf[1],"  1ibuf1: ");
//    uartdecodeH(ibuf[2],"  1ibuf2: ");
//    uartdecodeH(ibuf[3],"  1ibuf3: ");
//		iputs("\r\n"); 

//		sysDelayMs(3000);	  
		
	  ibuf2[0]=0x18;
	  ibuf2[1]=0x18;
	  ibuf2[2]=0x18;
	  ibuf2[3]=0x18;
		
		//spiFlashWrite2(0x03,4,ibuf2);
		dwt_writetodevice(0x03,0,4,ibuf2);
		sysDelayMs(1000);	  		
		
		//spiFlashRead2(0x03,4,ibuf);
      dwt_readfromdevice(0x03,0,4,ibuf);
      
      uartdecodeH(ibuf[0],"  ibuf0: ");
      uartdecodeH(ibuf[1],"  ibuf1: ");
      uartdecodeH(ibuf[2],"  ibuf2: ");
      uartdecodeH(ibuf[3],"  ibuf3: ");
		iputs("\r\n"); 

		sysDelayMs(2000);	  

		
	   ibuf2[0]=0x28;
	   ibuf2[1]=0x28;
  	   ibuf2[2]=0x28;
	   ibuf2[3]=0x28;
		
		dwt_writetodevice(0x03,0,4,ibuf2);
		sysDelayMs(1000);	  
		
		
		//spiFlashRead2(0x03,4,ibuf);
      dwt_readfromdevice(0x03,0,4,ibuf);
      
      uartdecodeH(ibuf[0],"  ibuf0: ");
      uartdecodeH(ibuf[1],"  ibuf1: ");
      uartdecodeH(ibuf[2],"  ibuf2: ");
      uartdecodeH(ibuf[3],"  ibuf3: ");
		iputs("\r\n"); 

		sysDelayMs(2000);	  			
		
		
//-------test dw1000 ----end-------------------- 

	
		runTimedTasks();   //run timed tasks				
		
		SYS_UnlockReg();   
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();     

	}		
	
}








