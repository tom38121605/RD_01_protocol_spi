/****************************************************************************
 * @file:		adcKey.h
 * @version:	V1.00
 * @breif:	adcKey header file	
 * @date: 	2013/2/25	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef	__adcKey_h
#define	__adcKey_h

#define ADC_KEY_PORT	PD
#define ADC_KEY_PIN		0
#define BATTERY_V_DECT_PIN	4
#define ADC_KEY_MSK	(ADC_CH0 + ADC_CH4)
#define ADC_KEY_ENABLE	{ GPIO_ADC0(); GPIO_ADC4();}
#define ADC_INT_PRIORITY	2
#define MAX_LRF_KEYS		3	// MAX number of the LRF = 1 << MAX_LRF_KEYS
#define ADC_KEY_SCANF_RATE	30	// key scan rate is 1000/KEY_SCANF_RATE
#define KEY_CONTINUE_COUNTER	1 //key_w_c will be producted  1000/KEY_SCANF_RATE/KEY_CONTIUNE_COUNTER times in a sec
#define KEY_CONTINUE_START		-6 // K_W_C START AFTER K_W (KEY_CONTINUE_START+KEY_CONTINUE_COUNTER) * KEY_SCANF_RATE
#define INC_KEY_COUNTER		8	// key delay count in continue key event
#define DEC_KEY_COUNTER	2
#define CHANNEL0_KEY_COUNTER	3
#define CHANNEL1_KEY_COUNTER	4
#define CHANNEL2_KEY_COUNTER	5
#define ADC_CLK_PRESCALE		3 	// ADC CLK IS CLK_HXT / ADC_CLK_PRESCALE
#define BAT_DEC_COUNT			7  // 2^7 * 30 ms = 4s, time of geting the average the  battey volage
#define BATTERY_SHOW_INFO_TIME	180 // 130 * 30MS= 4s

// button that press down without relax, key_word will 
// record once untill relax the button
extern enum KEYWORD adc_key_word;	
extern u32 auto_standy_delay_minunte;
extern u32 battery_volage ;
// button that press down without relax,key_word_continue
// will record per configuration interval, e.g record per 100ms
extern enum KEYWORD adc_key_word_continue;
extern u32 BAT_STATE_DECT_DELAY_COUNT;
	
extern void adcKeyClkInit(void);
extern void adckeyTaskInit(void);
extern void devTest(void);

extern void ampSetR(void);
extern void ampSetL(void);
extern void ampSetS(void);
#endif


