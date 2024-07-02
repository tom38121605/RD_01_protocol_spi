/****************************************************************************
 * @version:	V1.00
 * @breif:	ir key header file	
 * @date: 	2013/2/28
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __irKey_h
#define __irKey_h

/****************************************************************/
/*					choose different ir control						*/
/****************************************************************/
#define __global_ir_remote__
//#define __TOSHIBA_ir_remote__
//#define __auvio__ir_remote__
//#define __vtrek__ir_remote__
//#define __lenco__ir_remote__
//#define __yamaha__ir_remote__
//#define __apple__ir_remote__
//#define __white__ir_remote__


/****************************************************************/
/*					IR decode pin is GPIO PB4						*/
/****************************************************************/
#define IR_KEY_PORT		PB
#define IR_KEY_PIN		14 
#define IR_KEY_PORT_PIN PB14
#define IR_IRQHandler		 EINT0_IRQHandler   
#define IR_IRQn			EINT0_IRQn
#define IR_INT_PRIORITY	2
#define ENABLE_GPIO_IR()	{	GPIO_EINT0();\
								GPIO_SET_MODE(PB, 14, GPIO_INPUT); \
								GPIO_EnableInt(PB, 14, GPIO_INT_FALLING); \
								GPIO_ENABLE_DEBOUNCE(PB,14);\
								GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_OSC10K, GPIO_DBCLKSEL_1); \
								NVIC_SetPriority (EINT0_IRQn, IR_INT_PRIORITY); \
								NVIC_EnableIRQ(EINT0_IRQn); \
							}
#define DISABLE_GPIO_IR() {GPIO_DisableInt(PB, 14, GPIO_INT_FALLING); }
#define IR_USE_TMR		TIMER3
#define IR_RST_IP()		SYS_RESET_IP2(SYS_RST_TMR3)
#define IR_TMR_CLKSRC()	SET_TMR3_CLKSRC(CLK_TMR3_HXT)
#define IRCLK_ENABLE()	APBCLK_ENABLE(CLK_TMR3)
#define IR_GET_TIMER()	TIM_GET_TDR(IR_USE_TMR);
#define CLEAR_IR_INTERRUPT_FLAG()   GPIO_CLR_PIN_IF(PB, 14)

/****************************************************************/
/*					IR key action time control						*/
/****************************************************************/

#define IR_KEY_RELAX_TIME	2	// ir key should relax at least  IR_KEY_RELAX_TIME*IR_KEY_SCANF_RATE
#define IR_KEY_SCANF_RATE	80 // define ir key scan rate  1000/IR_KEY_SCANF_RATE
#define KEY_POOL_SIZE	1	//shoule be 2^n like 1,2,4,8... and so on




#if(defined (__TOSHIBA_ir_remote__))
// open this will repaired the error code in some degree 
// but it also increase the chance to get a wrong key  and  use more cpu time

#define IR_LEADER_CODE_HEADER	9000
#define IR_LEADER_CODE_TAIL	4500 
#define IR_BIT_CODE_HEADER	560
#define IR_BIT_CODE_1_TAIL	565
#define IR_BIT_CODE_0_TAIL	1690


#define IR_LEADER_CODE			13 //(IR_LEADER_CODE_HEADER+IR_LEADER_CODE_TAIL) /1000
#define IR_BIT_0_CODE			11 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_0_TAIL) /100
#define IR_BIT_1_CODE			22 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_1_T	AI	L) /100
#define IR_PULSE_COMPENSATION		10 // compensation IR_BIT_0/1_CODE  
#define IR_REPEAT_1_HEAD_CODE		39
#define IR_REPEAT_MORE_HEAD_CODE	96	// 108 -IR_REPEAT_TAIL
#define IR_REPEAT_TAIL_CODE			11


/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/

#define IR_CUSTTOMER_CODE	 0XBD45 //0X42BA
#define IR_K1				 0X68 //0X12  // 0Xed
#define IR_K2				 0X6C //0Xbd  // 0X42
#define IR_K3				 0X6A //0X60  // 0X9f
#define IR_K4				 0X6B //0X61  // 0X9e
#define IR_K5				 0Xea  // 0X15
#define IR_K6				 0X60  // 0X97
#define IR_K7				 0X69  // 0X96
#define IR_K8				 0X12  // 0X95
#define IR_K9				 0XBD  // 0X94
#define IR_K10				 0X15  // 0X93




#elif (defined (__yamaha__ir_remote__) || \
	  defined (__lenco__ir_remote__) || \
	  defined (__auvio__ir_remote__) || \
	  defined (__white__ir_remote__) || \
	  defined (__vterk__ir_remote__) )
#define IR_LEADER_CODE_HEADER	9000
#define IR_LEADER_CODE_TAIL	4500
#define IR_BIT_CODE_HEADER	560
#define IR_BIT_CODE_1_TAIL	565
#define IR_BIT_CODE_0_TAIL	1690


#define IR_LEADER_CODE			13 //(IR_LEADER_CODE_HEADER+IR_LEADER_CODE_TAIL) /1000
#define IR_BIT_0_CODE			11 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_0_TAIL) /100
#define IR_BIT_1_CODE			22 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_1_TAIL) /100
#define IR_PULSE_COMPENSATION		40 // compensation IR_BIT_0/1_CODE  
#define IR_REPEAT_1_HEAD_CODE			40
#define IR_REPEAT_MORE_HEAD_CODE		96	// 108 -IR_REPEAT_TAIL
#define IR_REPEAT_TAIL_CODE				11


/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/
#if defined (__lenco__ir_remote__)
#define IR_CUSTTOMER_CODE	 0XFE01
#define IR_K1				 0X1A  
#define IR_K2				 0X1B  
#define IR_K3				 0X1c  
#define IR_K4				 0X1e  
#define IR_K5				 0X1f  
#define IR_K6				 0X68  
#define IR_K7				 0X69  
#define IR_K8				 0X6a  
#define IR_K9				 0X6b  
#define IR_K10			 0X6c  


/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/
#elif defined (__yamaha__ir_remote__)
#define IR_CUSTTOMER_CODE	 0X857A
#define IR_K1				 0X1A  
#define IR_K2				 0X1B  
#define IR_K3				 0X1c  
#define IR_K4				 0X30  
#define IR_K5				 0X1f  
#define IR_K6				 0X68  
#define IR_K7				 0X69  
#define IR_K8				 0X6a  
#define IR_K9				 0X6b  
#define IR_K10			 0X6c  

/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/
#elif defined (__auvio__ir_remote__)
#define IR_CUSTTOMER_CODE	 0XFE01 //0X42BA
#define IR_K1				 0X00 
#define IR_K2				 0X01  
#define IR_K3				 0X03  
#define IR_K4				 0X07  
#define IR_K5				 0X05  
#define IR_K6				 0X12  
#define IR_K7				 0X08 
#define IR_K8				 0X6a  
#define IR_K9				 0X6b  
#define IR_K10			 0X6c  

#elif defined (__vterk__ir_remote__)
#define IR_CUSTTOMER_CODE	 0XFE01 //0X42BA
#define IR_K1				 0X07 
#define IR_K2				 0X01  
#define IR_K3				 0X03 
#define IR_K4				 0X1e  
#define IR_K5				 0X40  
#define IR_K6				 0X12  
#define IR_K7				 0X69  
#define IR_K8				 0X6a  
#define IR_K9				 0X6b  
#define IR_K10			 0X6c  

/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/
#elif (defined (__white__ir_remote__))
#define IR_CUSTTOMER_CODE	 0XFF00 //0X42BA
#define IR_K1				 0X80  
#define IR_K2				 0X81  
#define IR_K3				 0X82  
#define IR_K4				 0X83  
#define IR_K5				 0X84 
#define IR_K6				 0X85  
#define IR_K7				 0X86  
#define IR_K8				 0X87  
#define IR_K9				 0X88  
#define IR_K10			 0X89  


#endif

#elif (defined (__apple__ir_remote__)   || \
	  defined (__global_ir_remote__) )
#define IR_LEADER_CODE_HEADER	9000
#define IR_LEADER_CODE_TAIL	4500
#define IR_BIT_CODE_HEADER	560
#define IR_BIT_CODE_1_TAIL	565
#define IR_BIT_CODE_0_TAIL	1690


#define IR_LEADER_CODE			13 //(IR_LEADER_CODE_HEADER+IR_LEADER_CODE_TAIL) /1000
#define IR_BIT_0_CODE			11 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_0_TAIL) /100
#define IR_BIT_1_CODE			22 //(IR_BIT_CODE_HEADER+IR_BIT_CODE_1_TAIL) /100
#define IR_PULSE_COMPENSATION	40 // compensation IR_BIT_0/1_CODE  
#define IR_REPEAT_1_HEAD_CODE	38
#if (defined (__global_ir_remote__))
#define IR_REPEAT_MORE_HEAD_CODE	97	// 108 -IR_REPEAT_TAIL
#else
#define IR_REPEAT_MORE_HEAD_CODE	96	// 108 -IR_REPEAT_TAIL
#endif
#define IR_REPEAT_TAIL_CODE		11

/****************************************************************/
/*							define ir_code 							*/
/****************************************************************/
#if(defined (__apple__ir_remote__))
#define IR_CUSTTOMER_CODE	 0X87EE //0X42BA
#define IR_K1				 0X0A 
#define IR_K2				 0X09  
#define IR_K3				 0X06  
#define IR_K4				 0X0C  
#define IR_K5				 0X03  
#define IR_K6				 0X5F  
#define IR_K7				 0X05  
#define IR_K8				 0X6a  
#define IR_K9				 0X6b  
#define IR_K10			 0X6c  


#elif(defined (__global_ir_remote__))
#define IR_CUSTTOMER_CODE	 0X87ee //0X42BA
#define IR_K1				 0X00 //0X12  // 0Xed
#define IR_K2				 0X04 //0Xbd  // 0X42
#define IR_K3				 0X0a //0X60  // 0X9f
#define IR_K4				 0X0c //0X61  // 0X9e
#define IR_K5				 0X07  // 0X15
#define IR_K6				 0X60  // 0X97
#define IR_K7				 0X03  // 0X96
#define IR_K8				 0X06  // 0X95
#define IR_K9				 0X09  // 0X94
#define IR_K10				 0X08  // 0X93

#endif

#endif

#define IR_BIT_01_000_CODE		(IR_BIT_0_CODE+IR_BIT_1_CODE)
#define IR_BIT_11_100_CODE		(IR_BIT_1_CODE+IR_BIT_1_CODE)

#if(defined (__global_ir_remote__) && IR_CODE_ERROR_REPAIRE_EN == TRUE)
#define IR_BIT_PULSE_MAX		IR_BIT_11_100_CODE+1
#else
#define IR_BIT_PULSE_MAX		(IR_BIT_1_CODE + 2)
#endif


/****************************************************************/
/*						IR SIGNAL LED  							*/
/****************************************************************/
#if(!Release)
#define IR_SIGNAL_LED_OFF()		{GPIO_SET_MODE(PB, 4, GPIO_OUTPUT); GPIO_SET_PIN(PB4);}
#define IR_SIGNAL_LED_ON()		{GPIO_SET_MODE(PB, 4, GPIO_OUTPUT); GPIO_CLR_PIN(PB4);}
#else
#define IR_SIGNAL_LED_OFF()		do{}while(0)
#define IR_SIGNAL_LED_ON()		do{}while(0)
#endif
    
enum IR_STATE{
	IR_ERROR_STATE,	//  can only accept leader code in future
	IR_IDLE_STATE,		//  can accept leader code and repeated code
	IR_LEADER_STATE,	// accepted a leader code
	IR_REPEAT_HEAD_STATE,	// accepted a repeate header code
	IR_REPEAT_TAIL_STATE	// accepted a repeate tail code
};

extern enum KEYWORD ir_keyword;
extern void irClkInit(void);
extern void irKeyTaskInit(void);
// extern void initIRKeyPool(void);

#endif



