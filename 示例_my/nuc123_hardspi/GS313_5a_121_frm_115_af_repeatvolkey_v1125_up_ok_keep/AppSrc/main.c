
#include <stdio.h>
#include <string.h>

#include "NUC123Series.h"
#include "sysinc.h" 
//#include "filter.h"
#include "gpio.h"
#include "74hc4096.h"


//float	 f_size1=1123.652f;
//u32    u_size1=0;		
//float	 f_size2=0;
//u32    u_size2=0;		


extern s32 breath_flag;
extern void fadeoff( u32 color);
extern DWHP83_RMSG dwhp83_receive_msg;

//--#define CMD_TOUCHKEY			0x30  

extern DWHP83_STATE dwhp83_status;
extern u32 Dwhp83_route_count;

//extern uint8 tx_poll_msg[];

	
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

extern u8 flg_measure;


void CLK_Init(void)
{
	SYS_UnlockReg();                                        //En:Unlock protected registers
	GPIO_XT1();
	CLK_HXT_ENABLE();                                        //En:Enable External XTAL (4~24 MHz)
	
	while(!CLK_IS_STABLE(CLK_HXT_STB)); 		  //En:Waiting for 12MHz clock ready
	
	//CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4A54);   // 0X4A54 PLL 73.714M.  0x4016 is 72M 
  CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4016);       // 0x4016 is 72M 
  //CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4221);       //70M 
  //CLK_SET_PLL_FRQ(CLK_PLLSRC_HXT,0X4012);       //60M 
	
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

/*
void bootLDROM()
{
	UNLOCKREG();   
	outpw(&SYS->RSTSRC, 3);   //clear bit
	outpw(&FMC->ISPCON, (inpw(&FMC->ISPCON) & 0xFFFFFFFC) |0x2);  
	outpw(&SCB->AIRCR, (0x05FA0000UL | 0x00000004UL));   //SYSRESETREQ
	
	while(1);   
}
*/

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
 	//u8  ibuf[4];
 	//u8  ibuf2[4];
  //u32 idev=0;

	
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
	
	
//	//DWAM83 INI
//--  GPIO_SET_MODE(PB, 14, GPIO_QUASI); 
//	GPIO_CLR_PIN(PB14); 
//	sysDelayMs(200); 
//	GPIO_SET_PIN(PB14); 
//	sysDelayMs(50); 


//	DWPH83Init();
//  //GPIO_SET_MODE(PB, 10, GPIO_QUASI);  //--zz debug
//--  GPIO_SET_MODE(PB, 10, GPIO_INPUT);  
	
	
	
	//iputs("main--01.\r\n");  //--zz debug         
	iputs("\r\n------------------ ");  //--zz debug         
	iputs(VERSION);       
	iputs(" -----------------\r\n");  //--zz debug         
		
		
//	//openMcuCtrlPin(); 
//--	PD->PMD &= ~(3UL<<(5*2));	
//	PD->PMD |= 1<<(5*2);	
//	MAIN_POWER_ON();  	
//--	//sysDelayMs(100);	


	WDTInit();	    //init watch dog timer	 
	
	   
	//--AMP
	//--standbyPowerCtrlSet();
	//--standbyPowerCtrlCLR();
	//--GPIO_SET_PIN(PB9);	
	

//-------------dw1000  ini------------------------------------	
  dw1000_init();
	
	ilocaladdr=0x80;

  //sysDelayMs(1000);	    

	
  while(1)     
	{	
		
//--		static u32 j1=0;
		//u8 acoco[3];
		
//-- iputs("main--031.\r\n");  //--zz debug    

	
//		uartdecodeH32(f_size1,"  size1: ");		 
//		
//		u_size1=*( u32* ) &f_size1;		
//		uartdecodeH32(u_size1," u_size1: ");		 
//		
//		
//		f_size2 = *(float *)&u_size1;
//		uartdecodeH32(u_size1," u_size1: ");		 
				
	
		
//-------------dw1000---code-----------------------		

			if(flg_measure==1)
			{
				
				//vol
				if(flg_volchg==1)
				{
	//uartdecode(ivol_adjust,"  ivol_adjust: ");   //--zz debug  
					
					//ivol_adjust =ivol_adjust;	 // *(u8 *)&ivol_adjust	
					flg_volchg = 0;  
				}
				else
					ivol_adjust = 0;			
				
				flg_measure=0;
        dw1000_twr_measure(0);				
        dw1000_twr_measure(1);				
        dw1000_twr_measure(2);		  		
        dw1000_twr_measure(3);	
				
//iputs("\r\n");  //--zz debug   
 				
			}

		//--u8 ieeprom_temp=0;
		   
		
//		//-------------24c02---------------------------------
//		
//		writeeeprom(69,0x06);           
//		sysDelayMs(1500);   

//		ieeprom_temp = readeeprom(0x06);   
//		sysDelayMs(1500);     

//		uartdecode(ieeprom_temp,"  eeprom_01: ");		 	
//		sysDelayMs(30);	 


//		writeeeprom(29,0x06);  
//		sysDelayMs(1500);  

//		ieeprom_temp = readeeprom(0x06);  
//		sysDelayMs(1500);  
 			
//		uartdecode(ieeprom_temp,"  eeprom_01B: ");	 		
//		sysDelayMs(30);	  

		
//-------------24c02-----end---------------------------------

	
		runTimedTasks();   //run timed tasks				
		
		SYS_UnlockReg();   
		WDT_TIMER_RESET();   //Clear WDT timer
		SYS_LockReg();     

	}		
	
}


//void resetHook(void)
//{;	
//}


/*
void GPCDF_IRQHandler()
{ 
	if(GPIO_GET_PIN_IF(PC,9))
	{
		NVIC->ISER[0]&= ~(1 << 5);   //Disable		
		
		iputs("PC9 INT in.\n");   //--zz debug
		sysDelayUs(50);
		
		flg_touchin=1;
			
		
		//gpio_int = SOC_TOUCH
		GPIO_CLR_PIN_IF(PC,9);   //Clear

		
		NVIC->ISER[0] |= (1 << 5);   //Enable
	}	
}
*/

