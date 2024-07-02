// /****************************************************************************
//  * @file:		sta339.c
//  * @version:		
//  * @breif:		STA339 AMP source file
//  * @date: 		
//  * 
//  * @note 		
//  *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
//  * 
//  ****************************************************************************/
//  #include "sysinc.h"
//  
// #define cap_config_num 46  //46
//  
//  u8 tp_key_flag = 0;       //ma 20151230 
//  u8 tp_key_value = 0;     //ma 20151230 
// volatile  uint8_t   CAP_cfg[cap_config_num][2] =   
// {   //  Address     Value
// 			
// 				{   0x00,       0x00    },      // Main Control    0x00-->1, 0xc0-->8
// 				{   0x1F,       0x2F    },      // Sensitivity Control  //0x6f //3f  //1f
// 				{   0x20,       0x20    },      // General Configuration
// 				{   0x21,       0xFF    },      // Sensor Input Enable
// 				{   0x22,       0xA4    },      // Sensor Input Configuration
// 				{   0x23,       0x07    },      // Sensor Input Configuration 2
// 				{   0x24,       0x39    },      // Averaging and Sampling Config
// 				{   0x26,       0x00    },      // Calibration Activate
// 				{   0x27,       0xFF    },      // Interrupt Enable
// 				{   0x28,       0xFF    },      // Repeat Rate Enable
// 				{   0x2A,       0x00    },      // Multiple Touch Configuration
// 				{   0x2B,       0x00    },      // Multiple Touch Pattern Config
// 				{   0x2D,       0xFF    },      // Multiple Touch Pattern
// 				{   0x2F,       0x8A    },      // Recalibration Configuration
// 				{   0x30,       0x40    },      // Sensor Input 1 Threshold
// 				{   0x31,       0x40    },      // Sensor Input 2 Threshold
// 				{   0x32,       0x40    },      // Sensor Input 3 Threshold
// 				{   0x33,       0x40    },      // Sensor Input 4 Threshold
// 				{   0x34,       0x40    },      // Sensor Input 5 Threshold
// 				{   0x35,       0x40    },      // Sensor Input 6 Threshold
// 				{   0x36,       0x40    },      // Sensor Input 7 Threshold
// 				{   0x37,       0x40    },      // Sensor Input 8 Threshold
// 				{   0x38,       0x01    },      // Sensor Input Noise Threshold
// 				{   0x40,       0x00    },      // Standby Channel
// 				{   0x41,       0x39    },      // Standby Configuration
// 				{   0x42,       0x02    },      // Standby Sensitivity
// 				{   0x43,       0x40    },      // Standby Threshold
// 				{   0x44,       0x40    },      // Configuration 2
// 				{   0x71,       0x00    },      // LED Output Type
// 				{   0x72,       0x00    },      // LED Sensor Input Linking
// 				{   0x73,       0x00    },      // LED Polarity
// 				{   0x74,       0x00    },      // LED Output Control
// 				{   0x77,       0x00    },      // LED Linked Transition Control
// 				{   0x79,       0x00    },      // LED Mirror Control
// 				{   0x81,       0x00    },      // LED Behavior 1
// 				{   0x82,       0x00    },      // LED Behavior 2
// 				{   0x84,       0x20    },      // LED Pulse 1 Period
// 				{   0x85,       0x14    },      // LED Pulse 2 Period
// 				{   0x86,       0x5D    },      // LED Breathe Period
// 				{   0x88,       0x04    },      // LED Config
// 				{   0x90,       0xF0    },      // LED Pulse 1 Duty Cycle
// 				{   0x91,       0xF0    },      // LED Pulse 2 Duty Cycle
// 				{   0x92,       0xF0    },      // LED Breathe Duty Cycle
// 				{   0x93,       0xF0    },      // LED Direct Duty Cycle
// 				{   0x94,       0x00    },      // LED Direct Ramp Rates
// 				{   0x95,       0x00    }       // LED Off Delay	
// 	
// };
//  
//  
//  
// static i2cPoolType touch_i2c_block;
// static volatile u8 touch_buf[16];
// u8 readCAP8298(u32 address) 
// {  
// 	initI2cDataBlock(	&touch_i2c_block,  
// 					0x50,  
// 					I2C_RD,  
// 					2, 
// 					1,   
// 					(u8*)&touch_buf[0] 
// 				);  
// 	
// 	touch_buf[1] = address&0xff; 
// 	touch_buf[2] = 0x0; 
// 	touch_buf[0] = I2C_BUSY;	 

// 	addI2cChain(&touch_i2c_block, 0); 
// 	while(touch_buf[0] == I2C_BUSY) 
// 			; 
// 	return touch_buf[2]; 
// 	
// }

// u8 capack(void)
// {
// 	return touch_buf[0];
// }

// u32 readCAPBlock(u32 address, u32 count)
// {
// 	initI2cDataBlock(	&touch_i2c_block, 
// 					0x50, 
// 					I2C_RD, 
// 					1+count,
// 					1,
// 					(u8*)&touch_buf[0]
// 				);
// 	
// 	touch_buf[1] = address&0xff;;
// 	touch_buf[2] = 0x0;
// 	touch_buf[0] = I2C_BUSY;
// 	addI2cChain(&touch_i2c_block, 0);
// 	while(touch_buf[0] == I2C_BUSY)
// 			;
// 	return touch_buf[0] == I2C_IDLE;
// }

// u32 writeCAP8298(u32 data, u32 address)
// {
// 	initI2cDataBlock(	&touch_i2c_block, 
// 					0x50, 
// 					I2C_WR, 
// 					2, 
// 					1, 
// 					(u8*)&touch_buf[0]
// 			);
// 	
// 	touch_buf[1] = address&0xff;
// 	touch_buf[2] = (u8)data;
// 	touch_buf[0] = I2C_BUSY;
// 	

// 	addI2cChain(&touch_i2c_block, 0);
// 	while(touch_buf[0] == I2C_BUSY)
// 		;
// 	return (touch_buf[0] == I2C_IDLE)
// 		;
// }


// u32 writeCAP8298Block(volatile u8 *buf, u32 count)
// {
// 	initI2cDataBlock(	&touch_i2c_block, 
// 					0x50, 
// 					I2C_WR, 
// 					count+1,
// 					1,
// 					(u8*)&buf[0]
// 			);
// 	
// 	buf[0] = I2C_BUSY;
// 	addI2cChain(&touch_i2c_block, 0);
// 	while(buf[0] == I2C_BUSY)
// 			;
// 	
// 	return (buf[0] == I2C_IDLE);
// }

// void touchKeyconfig()
// {
// 	u32 i;
// 	
// 	for(i = 0; i < cap_config_num; i++)
// 		writeCAP8298(CAP_cfg[i][1],CAP_cfg[i][0]);
// }

// // void capIntEnable(void)
// // {
// // 	
// // }

// // void capIntDisable(void)
// // {
// // 	writeCAP8298(0x00,0xc9);
// // 	sysDelayMs(50);
// // }

// // void capIntDisable1(void)
// // {
// // 	writeCAP8298(0x02,0xc9);
// // 	sysDelayMs(50);
// // }

// // void capPinEnable(u8 msk)
// // {
// // 	writeCAP8298(msk,0x21);
// // }

// void resetCap(void)
// {
// 	writeCAP8298(0xAA,0xCE);	
// }
//  

// void cap1298_scan_key()
// {
// // 	static u32 tp_scan_counter = 0;
// // 	if(tp_scan_counter > 0)
// // 		tp_scan_counter++;
// // 	if(tp_scan_counter>500)
// // 	{
// // 		tp_scan_counter = 0;
// // 		writeCAP8298(0x27,0xff);
// // 	}
// // 	if(0 == tp_scan_counter)
// // 	{
// 		if(TP_IRQ())
// 		{
// // 			tp_scan_counter = 1;
// 			tp_key_value = 	readCAP8298(0x03);
// 			uartdecode(tp_key_value, "TP:");
// 			iputs("\r\n\n");
// // 			sysDelayMs(10);
// 			writeCAP8298(0x00,0x0);		 //Clear the INT bit (bit 0)
// 			sysDelayMs(10);
// 				writeCAP8298(0x00,0x0);		 //Clear the INT bit (bit 0)
// // 			writeCAP8298(0x27,0x00);	
// // 			sysDelayMs(20);
// // 			tp_key_flag = 1;
// 		}
// 		else
// 		{
// 			tp_key_value = 0;
// // 			tp_scan_counter = 0;
// // 		}
// 	}
// }
