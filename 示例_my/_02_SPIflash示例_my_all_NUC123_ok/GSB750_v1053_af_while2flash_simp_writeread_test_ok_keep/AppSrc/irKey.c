// /****************************************************************************
//  * @file:		irKey.c
//  * @version:	V1.00
//  * @breif:	ir key source file
//  * @date: 	2013/2/28
//  * 
//  * @note 		
//  *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
//  * 
//  ****************************************************************************/
// #include "sysinc.h"

// Timer_ID ir_key_task = TASK_FULL;
// enum KEYWORD ir_keyword;
// typedef struct {
// 	U32 inPos;
// 	u32 outPos;
// 	enum KEYWORD pool[KEY_POOL_SIZE];
// }KEY_POOL;
// KEY_POOL ir_keyword_pool = {0, 0, {NONE_KEY}};
// static enum IR_STATE ir_state = IR_IDLE_STATE;
// static U32 tcap_old = 0;		//last capture timer value 
// static u32 ir_key_bits = 0;	// store bits when ir decode  
// static u32 ir_key_bit_mask = 0; 	 // mask which indicate the  bit that ir pulse corresponded
// static enum KEYWORD last_key = NONE_KEY;

// #if(defined (__global_ir_remote__) && IR_CODE_ERROR_REPAIRE_EN == TRUE)	
// static u32 ir_key_bits_counter = 0;     // indicate which bits the ir pulse corresponded
// static u32 ir_key_bits_fixed_0 = 0;	// fixed zero pulse will set the bit corresponded
// static u32 ir_key_bits_fixed_1 = 0;	// fixed one pulse will set the bit corresponded
// static U32 ir_key_bits_fixed = 0;  // fixed bits will set in this var, always use the high 16 bits
// static u32 ir_temp_bits_mask = 0;
// static u32 ir_key_bits_bak = 0;
// static u32 ir_key_temp_counter = 0;
// #endif 
// enum KEYWORD IrKeyRemap(U32 key);
// void putKey2Pool(enum KEYWORD key);

// #if(IR_DEBUG_EN)
// /******************* resume ir signal************************/
// static u32 test_open = 0;
// static U32 ir_pulse_pool[80],ir_test_i=0, ir_time_stamp[80];
// static u32 ir_key_bits_pool[8], ir_key_bits_index = 0;
// #endif


// /*-------------------------------------------------------------------------
//  * @fucname:	 ir_error
//  * @brief: 	 deal ir error case
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 deal ir error case
//  *-----------------------------------------------------------------------*/
// void ir_error()
// {
// 	ir_state = IR_ERROR_STATE;
// //	sys_error = SYS_IR_ERROR; 	// complete it in the future
// }
// #if(IR_DEBUG_EN)
// //================================================
// /*   look  the  ir  key  code  in ir_pulse_pool   */
// //================================================
// void IR_IRQHandler_Test()
// {

//     	ir_time_stamp[ir_test_i] = IR_GET_TIMER();
// 	if(ir_test_i != 0)
// 		ir_pulse_pool[ir_test_i] = ir_time_stamp[ir_test_i] - ir_time_stamp[ir_test_i-1];
// 	if(++ir_test_i == 80) 
// 		ir_test_i = 0;
// 	
// }
// #endif

// #if(defined (__global_ir_remote__) && (IR_CODE_ERROR_REPAIRE_EN == TRUE))

// static __inline void fill_000()
// {
// 	ir_key_bits &=  ~ir_key_bit_mask 
// 				& ~(ir_key_bit_mask << 1) 
// 				& ~(ir_key_bit_mask << 2);
// 			
// 	ir_key_bit_mask <<= 3;
// 	ir_key_bits_counter += 3;
// }

// static __inline void fill_0000()
// {
// 	ir_key_bits &=  ~ir_key_bit_mask 
// 				& ~(ir_key_bit_mask << 1) 
// 				& ~(ir_key_bit_mask << 2)
// 				& ~(ir_key_bit_mask << 3) ;
// 				
// 	ir_key_bit_mask <<= 4;
// 	ir_key_bits_counter += 4;
// }

// static __inline void fill_01()
// {
// 	ir_key_bits &= ~ir_key_bit_mask;
// 	ir_key_bit_mask <<=2;
// 	ir_key_bits_counter += 2;
// }

// static __inline void fill_10()
// {
// 	ir_key_bits &= ~(ir_key_bit_mask << 1);
// 	ir_key_bit_mask <<=2;
// 	ir_key_bits_counter += 2;
// }

// static __inline void fill_11()
// {
// 	ir_key_bit_mask <<= 2;
// 	ir_key_bits_counter+=2;
// }

// static __inline void fill_100()
// {
// 	ir_key_bits &=  ~(ir_key_bit_mask<<1) 
// 				& ~(ir_key_bit_mask<<2);
// 		
// 	ir_key_bit_mask <<= 3;
// 	ir_key_bits_counter += 3;
// }

// static __inline void fill_010()
// {
// 	ir_key_bits &=  ~ir_key_bit_mask 
// 				& ~(ir_key_bit_mask<<2);
// 				
// 	ir_key_bit_mask <<= 3;
// 	ir_key_bits_counter += 3;
// }

// static __inline void fill_001()
// {
// 	ir_key_bits &=  ~ir_key_bit_mask 
// 				& ~( ir_key_bit_mask<<1);
// 				
// 	ir_key_bit_mask <<= 3;
// 	ir_key_bits_counter += 3;
// }

// static __inline void fix_01()
// {
// 	ir_key_bits_fixed_0 |= ir_key_bit_mask;
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask << 1;
// }

// static __inline void fix_10()
// {
// 	ir_key_bits_fixed_0 |= ir_key_bit_mask << 1;
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask;
// }

// static __inline void fix_11()
// {
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask + (ir_key_bit_mask << 1);
// }

// static __inline void fix_100()
// {
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask;
// 	ir_key_bits_fixed_0 |= (ir_key_bit_mask << 1)
// 						+ (ir_key_bit_mask << 2) ;
// }

// static __inline void fix_010()
// {
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask << 1;
// 	ir_key_bits_fixed_0 |= ir_key_bit_mask + (ir_key_bit_mask << 2) ;
// }

// static __inline void fix_001()
// {
// 	ir_key_bits_fixed_0  |=    ir_key_bit_mask
// 						 + (ir_key_bit_mask << 1);
// 	ir_key_bits_fixed_1 |= ir_key_bit_mask << 2;
// }

// static __inline void fix_000()
// {
// 	ir_key_bits_fixed_0 |=  ir_key_bit_mask
// 						+(ir_key_bit_mask << 1) 
// 						+(ir_key_bit_mask << 2);
// }

// static __inline void fix_0000()
// {
// 	ir_key_bits_fixed_0 |=  ir_key_bit_mask
// 						+ (ir_key_bit_mask << 1)
// 						+ (ir_key_bit_mask << 2)
// 						+ (ir_key_bit_mask << 3);
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 irFill_01_000_in_last9bit
//  * @brief: 	 fill 01 10 000 to last 9 bits  ir_key_bits_counter >= 23
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 fill 01 10 000 to last 9 bits 
//  *-----------------------------------------------------------------------*/
// static void irFill_01_000_in_last9bit()
// {

// 	ir_temp_bits_mask = (ir_key_bit_mask >>8) ; 
// 	ir_key_temp_counter = 0xffff0000 << ir_key_bits_counter;
// 	ir_key_bits_fixed = ir_key_bits_fixed_1 |ir_key_bits_fixed_0;
// 	
// 	if(ir_key_bits_counter != 23){
// 		if(ir_key_bits_fixed_1 & ir_temp_bits_mask){ // ahead8bit.0 is fixed 1
// 			if(ir_key_bits_fixed_1 & (ir_temp_bits_mask <<1)){ // ahead8bit.1 fixed 1
// 				if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2)){// ahead8bit.2 fixed 1. =111

// 					fix_000();
// 					fill_000();
// 					
// 				}else if(ir_key_bits_fixed_0 & ir_temp_bits_mask << 2){// ahead8bit.2 fixed 0

// 					last_key = NONE_KEY;	
// 					ir_state = IR_ERROR_STATE;
// 					
// 				}else{ // ahead8bit.2 is guessed  11x x=1

// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 2;
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 3;
// 					ir_key_bits |= ir_temp_bits_mask << 2;
// 					ir_key_bits &= ~(ir_temp_bits_mask << 3);
// 					fix_000();
// 					fill_000();
// 				}
// 			}
// 			else if(ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1)){//ahead8bit.1 fixed 0. =10

// 				ir_key_bits_fixed_0 |=  ir_key_bit_mask;
// 				ir_key_bits_fixed_1 |= ir_key_bit_mask << 1;
// 				fill_01();
// 				
// 			}else{	// ahead8bit.1 is guessed  =1x, x=0
// 				ir_key_bits_fixed_0 |= ir_key_bit_mask;
// 				ir_key_bits_fixed_1 |= ir_key_bit_mask << 1;
// 				ir_key_bits_fixed_1 |= ir_temp_bits_mask << 2;
// 				ir_key_bits_fixed_0 |= ir_temp_bits_mask << 1;
// 				ir_key_bits |= ir_temp_bits_mask << 2;
// 				ir_key_bits &= ~(ir_temp_bits_mask << 1);
// 				fill_01();
// 			}
// 			
// 		}// fill 01/000 end
// 		else if (ir_key_bits_fixed_0 & ir_temp_bits_mask){	// ahead8bit.0 fixed 0
// 			if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1)){ // ahead8bit.1 fixed 1. =01

// 				fix_10();
// 				fill_10();
// 			}
// 			else if(ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1)){ // ahead8bit.1 fixed 0. =00

// 				last_key = NONE_KEY;	
// 				ir_state = IR_ERROR_STATE;
// 				
// 			}else{ // // ahead8bit.1 is guessed means 0x,and x=1
// 	
// 				ir_key_bits_fixed_1 |= ir_temp_bits_mask  << 1;
// 				ir_key_bits_fixed_0 |=  ir_temp_bits_mask << 2;
// 				ir_key_bits |= ir_temp_bits_mask << 1;
// 				ir_key_bits &= ~(ir_temp_bits_mask << 2);
// 				fix_10();
// 				fill_10();
// 			}
// 		}	
// 		else if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1)){ //ahead8bit.0 is guessed and aheadbit.1 fixed 1
// 			if( !(ir_key_bits_fixed & ((ir_temp_bits_mask >> 1)+ir_temp_bits_mask)) ){
// 					// ahead8bit.0 and ahead8bit.-1 is guessed
// 				ir_key_bits_fixed_1 |= ir_temp_bits_mask  >> 1;
// 				ir_key_bits_fixed_0 |=  ir_temp_bits_mask;
// 				ir_key_bits |= ir_temp_bits_mask >> 1;
// 				ir_key_bits &= ~ir_temp_bits_mask;
// 				fix_10();
// 				fill_10();
// 				
// 			}else{

// 				last_key = NONE_KEY;	
// 				ir_state = IR_ERROR_STATE;
// 			}
// 		}else if(ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1)){// ahead8bit.0 is guessed and aheadbit.1 fixed 0
// 			if( !(ir_key_bits_fixed & ((ir_temp_bits_mask >> 1)+ir_temp_bits_mask)) ){
// 					// ahead8bit.0 and ahead8bit.-1 is guessed
// 				ir_key_bits_fixed_1 |= ir_temp_bits_mask;
// 				ir_key_bits_fixed_0 |=  ir_temp_bits_mask >> 1;
// 				ir_key_bits |= ir_temp_bits_mask;
// 				ir_key_bits &= ~(ir_temp_bits_mask >> 1);
// 				fix_01();
// 				fill_01();
// 				
// 			}else{

// 				last_key = NONE_KEY;	
// 				ir_state = IR_ERROR_STATE;
// 			}
// 		}else if( (ir_key_bits & (ir_temp_bits_mask))  //ahead8bit.xx is guessd as 10
// 			&& !(ir_key_bits & (ir_temp_bits_mask << 1))){
// 			
// 			fill_01();
// 		
// 		}else if( !(ir_key_bits & (ir_temp_bits_mask)) //ahead8bit.xx is guessd as 01
// 			&& (ir_key_bits & (ir_temp_bits_mask << 1)) ){

// 			fill_10();
// 		
// 		}else if( (ir_key_bits & (ir_temp_bits_mask)) //ahead8bit.xxx is guessd as 111
// 			&& (ir_key_bits & (ir_temp_bits_mask << 1))
// 			&& (ir_key_bits & (ir_temp_bits_mask << 2)) )
// 			
// 			fill_000();
// 		else{

// 			last_key = NONE_KEY;	
// 			ir_state = IR_ERROR_STATE;
// 		}
// 	}else if(ir_key_bits_counter == 23){ //ir_key_bit_mask is point bit.23
// 		if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1)){ // bit.16 is fixed 1

// 			ir_key_bits_fixed_1 |= ir_key_bit_mask;
// 			ir_key_bits_fixed_0 |= (ir_key_bit_mask << 1);
// 			fill_10();
// 			
// 		}else if(ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1)){ // bit.16 is fixed 0

// 			ir_key_bits_fixed_1 |= (ir_key_bit_mask << 1);
// 			ir_key_bits_fixed_0 |= ir_key_bit_mask;
// 			fill_01();
// 			
// 		}else if( ir_key_bits & (ir_temp_bits_mask >> 1) )  // bit.16 is guessed as 1
// 			fill_10();
// 		else  // bit.16 is guessed as 1
// 			fill_01(); 

// 	}
// 	
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 irFill_11_100_in_last9bit
//  * @brief: 	 fill 11 100 001 010 0000 to last 10 bits ir_key_bits_counter>=22
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 fill 11 100 001 010 0000 to last 10 bits 
//  *-----------------------------------------------------------------------*/
// static void irFill_11_100_in_last9bit()
// {
// 	ir_temp_bits_mask = (ir_key_bit_mask >>8) ;
// 	ir_key_temp_counter = 0xffff0000 << ir_key_bits_counter;
// 	ir_key_bits_fixed = ir_key_bits_fixed_1 |ir_key_bits_fixed_0;
// 	
// 	if(ir_key_bits_counter == 23)
// 		if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1) ) {// bit.16 fixed 1
// 		
// 			if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2) ) {// bit.17 fixed 1 ,fill 100
// 				fix_100();
// 				fill_100(); 
// 			}else if(  ir_key_bits_fixed_0 & (ir_temp_bits_mask << 2) ){// bit.17 fixed 0 ,fill 001
// 				fix_001();
// 				fill_001(); 
// 			}else {
// 				if(ir_key_bits & (ir_temp_bits_mask << 2)) // // bit.17 guessed 1, fill 100
// 					fill_100(); 
// 				else 
// 					fill_001();
// 			}
// 		}
// 		else if( ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1) ){ // bit.16 fixed 0
// 		
// 			if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2) ) {// bit.17 fixed 1 ,fill 010
// 			
// 				fix_010();
// 				fill_010();
// 			}else if(  ir_key_bits_fixed_0 & (ir_temp_bits_mask << 2) ){ // bit.17 fixed 0, fill 11
// 			
// 				fix_11();
// 				fill_11();
// 			}else{
// 				if(ir_key_bits & (ir_temp_bits_mask << 2)) // // bit.17 guessed 1, fill 010
// 					fill_010(); 
// 				else 
// 					fill_11();
// 			}
// 		}
// 		else{	// bit.16 is gussed
// 			if( ir_key_bits & (ir_temp_bits_mask << 1) ) // bit.16 guessed 1, bit.17 may guessed 0
// 				fill_001();
// 			else
// 				fill_010();
// 		}
// 		
// 	else{	// ir_key_bits_counter>23
// 	
// 		if( ir_key_bits_fixed_1 & ir_temp_bits_mask ) {// ahead8bit.1
// 		
// 			if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1) ) {// ahead8bit.11 
// 				if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2) ) //ahead8bit.111
// 					if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 3)){ //ahead8bit.1111, fill 0000
// 						fix_0000();
// 						fill_0000();
// 					}else{
// 						last_key = NONE_KEY;	
// 						ir_state = IR_ERROR_STATE;
// 					}
// 				else if( ir_key_bits_fixed_0 & (ir_temp_bits_mask << 2)){ //ahead8bit.110, fill 001	
// 					fix_001();
// 					fill_001();
// 				}else{										 // ahead8bit.11x 
// 					fix_001();
// 					fill_001();
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 2;
// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 3;
// 					ir_key_bits |=  ir_temp_bits_mask << 3;
// 					ir_key_bits &=  ~(ir_temp_bits_mask << 2);
// 				}
// 						
// 			}
// 			else if(  ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1) ){// ahead8bit.10 

// 				if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2) ){ //ahead8bit.101 , fill 010
// 					fix_010();
// 					fill_010();
// 		
// 				}else if( ir_key_bits_fixed_0 & (ir_temp_bits_mask << 2)){ //ahead8bit.100, error
// 					last_key = NONE_KEY;	
// 					ir_state = IR_ERROR_STATE;
// 				}else{										 // ahead8bit.10x the x should be 1
// 					fix_010();
// 					fill_010();
// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 2;
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 3;
// 					ir_key_bits &= ~(ir_temp_bits_mask << 3);
// 					ir_key_bits |= (ir_temp_bits_mask << 2);
// 				}
// 			}else {
// 				if(ir_key_bits & (ir_temp_bits_mask << 2)){ // ahead8bit.1x1
// 					fix_010();
// 					fill_010();
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 1;
// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 2;
// 					ir_key_bits |=  ir_temp_bits_mask << 2;
// 					ir_key_bits &=  ~(ir_temp_bits_mask << 1);
// 				}else { // ahead8bit.1x0
// 					fix_001();
// 					fill_001();
// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 1;
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 2;
// 					ir_key_bits &= ~(ir_temp_bits_mask << 2);
// 					ir_key_bits |= (ir_temp_bits_mask << 1);
// 				}
// 					
// 			}
// 		}
// 		else if( ir_key_bits_fixed_0 & ir_temp_bits_mask ){ // ahead8bit.0 fixed 0
// 		
// 			if( ir_key_bits_fixed_1 & (ir_temp_bits_mask << 1) ) {// ahead8bit.01
// 				if(ir_key_bits_fixed_1 & (ir_temp_bits_mask << 2)){ // ahead8bit.011, fill 100
// 					fix_100();
// 					fill_100();
// 				}else if(ir_key_bits_fixed_0 & (ir_temp_bits_mask << 2)){  // ahead8bit.010,error
// 					last_key = NONE_KEY;	
// 					ir_state = IR_ERROR_STATE;
// 				}else{ 							// ahead8bit.01x. change the old gussed result
// 					fix_100();
// 					fill_100();
// 					ir_key_bits_fixed_1 |= ir_temp_bits_mask << 2;
// 					ir_key_bits_fixed_0 |= ir_temp_bits_mask << 3;
// 					ir_key_bits &= ~(ir_temp_bits_mask << 3);
// 					ir_key_bits |= (ir_temp_bits_mask << 2);
// 				}
// 				
// 			}else if(  ir_key_bits_fixed_0 & (ir_temp_bits_mask << 1) ){ // ahead8bit.00
// 					fix_11();
// 					fill_11();
// 			}else{ // ahead8bit.0xx
// 				if(ir_key_bits & ((ir_temp_bits_mask << 1) +(ir_temp_bits_mask << 2)))  
// 					// ahead8bit.0xx and xx is guessed as 11
// 					fill_100();
// 				else {
// 					last_key = NONE_KEY;	
// 					ir_state = IR_ERROR_STATE;
// 				}
// 					
// 			}
// 		}
// 		else{	// ahead8bit.xx 
// 			if(ir_key_bits & ((ir_temp_bits_mask << 1) + ir_temp_bits_mask ) ) // ahead8bit.xx guessed as ahead8bit.11
// 				fill_11();
// 			else if(ir_key_bits_fixed_1 & (ir_temp_bits_mask <<2) ){ // ahead8bit.xx1
// 				if(ir_key_bits & ir_temp_bits_mask){	
// 					fill_010();
// 				}else {
// 					fill_100();
// 				}
// 			}else {
// 				last_key = NONE_KEY;	 
// 				ir_state = IR_ERROR_STATE;
// 			}	
// 		}
// 	}
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 irRepair_11
//  * @brief: 	 expend the old ir_key_bits's first guessed bits 11 to 100/010/001/0000
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 expend the old ir_key_bits's first guessed bits 11 to 100/010/001/0000
//  *-----------------------------------------------------------------------
// static void irRepair_11()
// {

// }
// */



// /*-------------------------------------------------------------------------
//  * @fucname:	 changeIrKeyBits
//  * @brief: 	 expend the old ir_key_bits's first guessed bits 01/10 to 000 
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 expend the old ir_key_bits's first guessed bits 01/10 to 000 
//  *-----------------------------------------------------------------------*/
// static void irRepair_000()
// {
// 	ir_temp_bits_mask = 0x10000;
// 	ir_key_temp_counter = 0xffff0000;
// 	while(ir_temp_bits_mask <= 0x100000){

// 		if( !(ir_key_bits_fixed & ir_temp_bits_mask) ){
// 			// change the guessed two bits to three bits in ir_key_bits
// 			ir_key_bits_bak = ir_key_bits;
// 			ir_key_bits <<= 1;
// 			ir_key_bits &= ir_key_temp_counter;
// 			ir_key_bits_bak &= ~(ir_key_temp_counter);
// 			ir_key_bits |= ir_key_bits_bak;

// 			// change the guessed two bits to three bits in ir_key_bits
// 			ir_key_bits_bak = ir_key_bits_fixed_0;
// 			ir_key_bits_fixed_0 <<= 1;
// 			ir_key_bits_fixed_0 &= ir_key_temp_counter;
// 			ir_key_bits_bak &= ~(ir_key_temp_counter);
// 			ir_key_bits_fixed_0 |= ir_key_bits_bak;

// 			// change the guessed two bits to three bits in ir_key_bits
// 			ir_key_bits_bak = ir_key_bits_fixed_1;
// 			ir_key_bits_fixed_1 <<= 1;
// 			ir_key_bits_fixed_1 &= ir_key_temp_counter;
// 			ir_key_bits_bak &= ~(ir_key_temp_counter);
// 			ir_key_bits_fixed_1 |= ir_key_bits_bak;
// 			
// 			ir_key_bits &= (~ir_temp_bits_mask 
// 						& ~(ir_temp_bits_mask << 1)
// 						& ~(ir_temp_bits_mask << 2) );

// 			ir_key_bits_fixed_1 &= ( ~ir_temp_bits_mask 
// 								& ~(ir_temp_bits_mask << 1)
// 								& ~(ir_temp_bits_mask << 2) );
// 									
// 			ir_key_bits_fixed_0 |= ( ir_temp_bits_mask 
// 								+ (ir_temp_bits_mask << 1)
// 								+ (ir_temp_bits_mask << 2) );
// 										
// 			ir_key_bit_mask <<= 1;
// 			break;
// 		}
// 		ir_temp_bits_mask <<= 1;
// 		ir_key_temp_counter <<= 1;
// 	}
// 									
// }
// #endif

// /*-------------------------------------------------------------------------
//  * @fucname:	 IR_IRQHandler
//  * @brief: 	 decode the IR PWM signal
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 decode the IR PWM signal, and put the key value in 
//  *			 ir_keyword_pool,some key can continue scan and some key can't
//  *-----------------------------------------------------------------------*/
// void IR_IRQHandler()
// {
// 	 u32 tcap;
// 	 u32 ir_pulse, ir_pulse_bak;
// 	 static u32 ir_pulse_compensation = IR_PULSE_COMPENSATION;

// 	CLEAR_IR_INTERRUPT_FLAG();          //Clear interrupt flag
// #if(IR_DEBUG_EN)
// 	IR_IRQHandler_Test();	// get the details of ir code
// #endif
//  
// 	tcap = IR_GET_TIMER();// get the ir fall edag time 

// 	// timer3 is 24bits reg, but ir_pluse is 32 bits var.
// 	// So clear the highest 8 bits can aviod the error when timer overflow   
// 	ir_pulse =  (tcap - tcap_old) & 0XFFFFFF;  

// 	tcap_old = tcap;
// 	
// #if(IR_DEBUG_EN)
// do{ // ir debug
// #endif

// 	ir_pulse_bak = ir_pulse;	

// 	if(ir_pulse_compensation != IR_PULSE_COMPENSATION)
// 		ir_pulse = ir_pulse_compensation + ir_pulse; 	// filter the short pulse
// 	if(ir_pulse<1100 && ir_pulse >1050 ||
// 		ir_pulse<2200 && ir_pulse >2150 ||
// 		ir_pulse < 3300 && ir_pulse > 3250)
// 			ir_pulse += 50;	// the Ir code may be have small error, and fix it
// 	 if(ir_pulse<2360 && ir_pulse >2299 ||
// 	 	ir_pulse<1260 && ir_pulse >1199 ||
// 	 	ir_pulse < 3460 && ir_pulse > 3399)
// 			ir_pulse -= 60;	// the Ir code may be have small error, and fix it

// 	if(ir_pulse_bak/1000 == IR_LEADER_CODE) 
// 		// quick switch ir key will add IR_LEADER_CODE pulse to ir_pulse_compensation,
// 		// so add this judgement
// 		ir_pulse = ir_pulse_bak;
// 	
// #if( defined (__global_ir_remote__) )
// 	
// 	// the first repeat code is dangerous,  it is always 39080 and may drop to below 39000
// 	if(ir_pulse < IR_REPEAT_1_HEAD_CODE*1000 
// 		&& ir_pulse > IR_REPEAT_1_HEAD_CODE*1000 -IR_PULSE_COMPENSATION*4
// 		)
// 		ir_pulse += IR_PULSE_COMPENSATION*4; // the Ir code may be have small error, and fix it

// #elif( defined (__apple__ir_remote__))

//      // the first repeat code is dangerous,  it always 38870 and may come upto above 39000
// 	if(ir_pulse > 1000*(IR_REPEAT_1_HEAD_CODE+1) 
// 		&& ir_pulse < 1000*(IR_REPEAT_1_HEAD_CODE+1) + IR_PULSE_COMPENSATION*4
// 		)
// 		ir_pulse -= IR_PULSE_COMPENSATION*4; // the Ir code may be have small error, and fix it


//     // can added more when use other ir controller
// #endif

// 	ir_pulse /= 100;  	
// 	//IR_SIGNAL_LED_ON();
// 	
// 	if(ir_pulse>IR_BIT_PULSE_MAX){	// is not bit signal may be leader code or 
// 		if(ir_state != IR_LEADER_STATE)
// 			switch(ir_pulse/10){
// 				case IR_LEADER_CODE:
// 					ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 					ir_state = IR_LEADER_STATE;
// 					ir_key_bits = 0xffffffff;
// 					ir_key_bit_mask = 0x1;
// #if(defined (__global_ir_remote__) && (IR_CODE_ERROR_REPAIRE_EN == TRUE))	
// 					ir_key_bits_counter = 0;
// 					ir_key_bits_fixed_0 = 0;
// 					ir_key_bits_fixed_1 = 0;
// #endif								
// 					break;
// 					
// 				case IR_REPEAT_1_HEAD_CODE:	 // the first reqeat code
// 					if(last_key != NONE_KEY 
// 						&& ir_state == IR_IDLE_STATE) {
// 						ir_state = IR_REPEAT_HEAD_STATE;
// 						ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 						break;
// 					}else
// 						goto end1;
// 					
// 				case IR_REPEAT_MORE_HEAD_CODE:	// more than two repeat code 
// 					if(last_key != NONE_KEY
// 						&& ir_state == IR_IDLE_STATE){
// 						ir_state = IR_REPEAT_HEAD_STATE;
// 						ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 						break;
// 					}else
// 						goto end1;
// 					
// 				case IR_REPEAT_TAIL_CODE: // repeat end
// 					if(ir_state == IR_REPEAT_HEAD_STATE){
// 						ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 						ir_state = IR_IDLE_STATE;
// 						putKey2Pool(last_key);
// 					//	IR_SIGNAL_LED_ON();
// 						break;
// 					}else
// 						goto end1;
// 					
// 				default:
// 					ir_pulse_compensation += ir_pulse_bak;
// end1:				if(ir_pulse > 960){	// bigger than the biggest repeated header code pulse time
// 						ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 						//last_key = NONE_KEY;	// bigger than the biggest repeated time
// 						//ir_state = IR_ERROR_STATE; //  can only accept leader code in future
// 					}
// 					break;
// 			}
// 			else{
// 				ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 				ir_state = IR_ERROR_STATE;
// 			}
// 	}
// 	else if(ir_state == IR_LEADER_STATE){ // decode the ir code to ir key
// 		
// 		switch(ir_pulse){
// 			
// 			case IR_BIT_0_CODE:

// 					ir_pulse_compensation = IR_PULSE_COMPENSATION;
// #if(defined (__global_ir_remote__) &&  (IR_CODE_ERROR_REPAIRE_EN == TRUE))			
// 					ir_key_bits_fixed_0 |= ir_key_bit_mask;
// 					if(ir_key_bits_counter > 15){
// 						if( ir_key_bits_counter < 24 )
// 							// fixed the complementonone for the complementonone may get 
// 							// big width pulse which contain several bit pulse width
// 							ir_key_bits_fixed_1 |= ir_key_bit_mask<<8;

// 						else{// the front pulse may be guess wrong, here can fixed it

// 							ir_key_bits_fixed = ir_key_bits_fixed_1 |ir_key_bits_fixed_0;
// 							
// 							
// 							if(!( (ir_key_bit_mask >> 8) &  ir_key_bits_fixed ) ){
// 							// the  corresponded bit in low is guessed wrong.
// 								ir_key_bits_fixed_1 |=  ir_key_bit_mask>>8;
// 								ir_key_bits |= ir_key_bit_mask>>8;
// 								if(!( (ir_key_bit_mask >> 9) &  ir_key_bits_fixed )){
// 									
// 									ir_key_bits_fixed_0 |= ir_key_bit_mask>>9;
// 									ir_key_bits &= ~(ir_key_bit_mask>>9);
// 									
// 									if( !( (ir_key_bit_mask >> 1) &  ir_key_bits_fixed) )
// 										ir_key_bits |= (ir_key_bit_mask >> 1);
// 									
// 								}else if(!( (ir_key_bit_mask >> 7) &  ir_key_bits_fixed )){
// 									ir_key_bits_fixed_0 |= ir_key_bit_mask>>7;
// 									ir_key_bits &= ~(ir_key_bit_mask>>7);
// 								}
// 							}
// 							else{ // the  bits ahead may be expanded. 01 or 10 maybe 000
// 							
// 								irRepair_000(); //expend the old ir_key_bits's first guessed bits 01/10 to 000 
// 							}
// 								

// 						}
// 					}
// 					ir_key_bits_counter++;
// #endif
// 					ir_key_bits &= ~ir_key_bit_mask;
// 					ir_key_bit_mask <<= 1;
// 					if(ir_key_bit_mask == 0){
// 						ir_state = IR_IDLE_STATE;
// #if(IR_DEBUG_EN)
// 						ir_key_bits_pool[ir_key_bits_index++] = ir_key_bits;
// 						ir_key_bits_index	&= 7;
// #endif
// 						last_key = IrKeyRemap(ir_key_bits);
// 						putKey2Pool(last_key);
// 					//	IR_SIGNAL_LED_ON();
// 					}
// 					break;
// 					
// 			case IR_BIT_1_CODE:
// 				
// 					ir_pulse_compensation = IR_PULSE_COMPENSATION;
// #if(defined (__global_ir_remote__) && (IR_CODE_ERROR_REPAIRE_EN == TRUE))						
// 					ir_key_bits_fixed_1 |= ir_key_bit_mask;
// 					if(ir_key_bits_counter > 15){
// 						if(ir_key_bits_counter < 24 )
// 							// fixed the complementonone for the complementonone may  
// 							// get a big width pulse which contain several bits pulse width
// 							ir_key_bits_fixed_0 |= (ir_key_bit_mask<<8);
// 						else{// the front pulse may be guess wrong, here can fixed it

// 							ir_key_bits_fixed = ir_key_bits_fixed_1 |ir_key_bits_fixed_0;
// 							
// 							
// 							if(!( (ir_key_bit_mask >> 8) &  ir_key_bits_fixed ) ){
// 							// the  corresponded bit in low is guessed wrong.
// 								ir_key_bits_fixed_0 |=  ir_key_bit_mask>>8;
// 								ir_key_bits &= ~(ir_key_bit_mask>>8);
// 								if(!( (ir_key_bit_mask >> 9) &  ir_key_bits_fixed )){
// 									
// 									ir_key_bits_fixed_1 |= ir_key_bit_mask>>9;
// 									ir_key_bits |= (ir_key_bit_mask>>9);
// 									
// 									if( !( (ir_key_bit_mask >> 1) &  ir_key_bits_fixed) )
// 										ir_key_bits &= ~(ir_key_bit_mask >> 1);
// 									
// 								}else if(!( (ir_key_bit_mask >> 7) &  ir_key_bits_fixed )){
// 									ir_key_bits_fixed_1 |= ir_key_bit_mask>>7;
// 									ir_key_bits |= (ir_key_bit_mask>>7);
// 								}
// 							}
// 							else{ // // the  bits ahead may be expanded. 01 or 10 maybe 000

// 								irRepair_000(); //expend the old ir_key_bits's first guessed bits 01/10 to 000 
// 							}
// 						}
// 					}
// 					ir_key_bits_counter++;
// #endif
// 					ir_key_bit_mask <<= 1;
// 					if(ir_key_bit_mask == 0){
// 						ir_state = IR_IDLE_STATE;			
// #if(IR_DEBUG_EN)
// 						ir_key_bits_pool[ir_key_bits_index++] = ir_key_bits;
// 						ir_key_bits_index	&= 7;
// #endif
// 						last_key = IrKeyRemap(ir_key_bits);
// 						putKey2Pool(last_key);	
// 					//	IR_SIGNAL_LED_ON(); 
// 					}
// 					break;
// 					
// #if(defined (__global_ir_remote__) && (IR_CODE_ERROR_REPAIRE_EN == TRUE))	
// 			// the follow code is optimized for global ir controller		

// 			case IR_BIT_01_000_CODE:

// 				ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 				if(ir_key_bits_counter < 16) // the custom code
// 						switch(ir_key_bits_counter){
// 							case 15:
// 								ir_key_bits_fixed_0 |= (ir_key_bit_mask << 1);
// 							case 0: case 2: case 6: case 8:case 13: // 10
// 								fill_10();
// 								break;
// 							case 1: case 5: case 7: case 9: case 14: // 01
// 								fill_01();
// 								break;
// 							case 3: // 000
// 								fill_000();
// 								break;
// 							default:
// 								last_key = NONE_KEY;	// CUSTOMER CODE IS WRONG 
// 								ir_state = IR_ERROR_STATE; //  can only accept leader code in future
// 								break;			
// 						}
// 				else{ // the key code
// 						if(ir_key_bits_counter < 23) // if the last bit is 0 then fill 10 else fill 01
// 							if(ir_key_bits_fixed_0 == 1)
// 								fill_01();
// 							else if ( ir_key_bits_fixed_1 == 1)
// 								fill_10();
// 							else if(ir_key_bits & (ir_key_bit_mask >> 1)){ //fill 01
// 								fill_01();
// 							}else{ // fill 10
// 								fill_10();
// 							}
// 						else { // ir_key_bits_counter >= 23, can use the info of 8 bits ahead
// 							
// 							irFill_01_000_in_last9bit();
// 						}
// 							
// 				}
// 						
// 				if(ir_key_bit_mask == 0){
// 						ir_state = IR_IDLE_STATE;
// 						last_key = IrKeyRemap(ir_key_bits);
// 						putKey2Pool(last_key);	
// 					//	IR_SIGNAL_LED_ON();
// 				}
// 				break;

// 			case IR_BIT_11_100_CODE:
// 			case IR_BIT_11_100_CODE+1:
// 				ir_pulse_compensation = IR_PULSE_COMPENSATION;
// 				if(ir_key_bits_counter < 16){
// 						switch(ir_key_bits_counter){
// 							case 15://11	
// 							case 10: case 11:case 12: //11
// 								fill_11();
// 								break;
// 							case 4:   //001
// 								fill_001();
// 								break;
// 							case 14: //010
// 								ir_key_bits_fixed_0 |= (ir_key_bit_mask << 2);
// 							case 5:case 7://010
// 								fill_010();
// 								break;
// 							case 3: //100
// 								fill_100();
// 								break;
// 							default:
// 								last_key = NONE_KEY;	// CUSTOMER CODE IS WRONG 
// 								ir_state = IR_ERROR_STATE; //  can only accept leader code in future
// 								break;
// 						}
// 				}
// 				else if (ir_key_bits_counter < 23){ // filled 11
// 					fix_11(); // because i  haven't written repair_11();
// 					fill_11();
// 				}
// 				else{ // ir_key_bits_counter >= 22
// 					
// 					irFill_11_100_in_last9bit();
// 					
// 				}
// 				if(ir_key_bit_mask == 0){
// 					ir_state = IR_IDLE_STATE;
// 					last_key = IrKeyRemap(ir_key_bits);
// 					putKey2Pool(last_key);	
// 				//	IR_SIGNAL_LED_ON();
// 				}
// 				break;		
// 					
// #endif

// 			default:
// 					ir_pulse_compensation += ir_pulse_bak;
// 					break;
// 		}	
// 	}
// 	else{
// 		ir_pulse_compensation += ir_pulse_bak;
// 	}

// #if(IR_DEBUG_EN)
// if(test_open)
// 	ir_pulse = ir_pulse_pool[test_open++]; // for ir debug
// }while(test_open != 0);// for ir debug
// #endif
// //for ir debuf
//   
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 irClkInit
//  * @brief: 	 ir clk init
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 irq clk init
//  *-----------------------------------------------------------------------*/
// void irClkInit()
// {
// 	IR_RST_IP();		
// 	IR_TMR_CLKSRC();	
// 	IRCLK_ENABLE();
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 irKeyInit
//  * @brief: ¡¤  
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void irKeyInit()
// {
// 	ENABLE_GPIO_IR();
// 	DrvTIMER_Init(IR_USE_TMR, 11, 1000000, TIM_MODE_CONTINUOUS);
// 	//TIM_CAPTURE_DEBOUNCE_ENABLE(IR_USE_TMR) ;            
// 	//TIM_SET_CAP_EDGE_MODE(IR_USE_TMR,TIM_CAPTURE_FALL);          //capture rise and fall edge.
// 	//TIM_CAPTURE_ENABLE(IR_USE_TMR) ;
// 	//DrvTIMER_EnableInt(IR_USE_TMR, TIM_TEX_INT_EN);
// 	TIM_ENABLE(IR_USE_TMR);
// }



// /*-------------------------------------------------------------------------
//  * @fucname:	 IrKeyRemap
//  * @brief: 	 remap the key
//  * @param: 	ir key
//  * @return: 	user key
//  * @details: 	remap the key
//  *-----------------------------------------------------------------------*/
// enum KEYWORD IrKeyRemap(U32 key)
// {
// 	U32_WB ir_decode = *(U32_WB*)&key;

// 	if(ir_decode.word0 != IR_CUSTTOMER_CODE)
// 		return NONE_KEY;
// #if(defined(__apple__ir_remote__))
// 	else if(ir_decode.byte3 == 0x6e ){
// #else
// 	else if((u8)ir_decode.byte2 == (u8)~ir_decode.byte3 ){
// #endif
// 		switch(ir_decode.byte2){
// 			case IR_K1:
// 				return POWER_KEY;
// 			case IR_K2:
// 				return SOUND_CLOSE_KEY;
// 			case IR_K3:
// 				return INC_KEY;
// 			case IR_K4:
// 				return DEC_KEY;
// 			case IR_K5:
// 				return CHANNEL_SWITCH_KEY;
// 			case IR_K6:
// 				return BLUETOOTH_KEY;
// 			case IR_K7:
// 				return W3D_KEY;
// 			case IR_K8:
// 				return MOVIE_KEY;
// 			case IR_K9:
// 				return MUSIC_KEY;
// 			case IR_K10:
// 				return GAME_KEY;
// 			
// 			default:
// 				return NONE_KEY;
// 		}
// 	}
// 	return NONE_KEY;
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 putKey2Pool
//  * @brief: 	 put the key to pool
//  * @param: 	 ir key 
//  * @return: 	 none
//  * @details: 	 put the key to pool
//  *-----------------------------------------------------------------------*/
// void putKey2Pool(enum KEYWORD key)
// {
// 	ir_keyword_pool.pool[ir_keyword_pool.inPos++] = key;
// 	ir_keyword_pool.inPos &= KEY_POOL_SIZE -1;
// }



// /*-------------------------------------------------------------------------
//  * @fucname:	 initIRKeyPool
//  * @brief: 	 init ir_keyword_pool
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void initIRKeyPool()
// {
// 	u32 i=KEY_POOL_SIZE;
// 	
// 	while(i--)
// 		ir_keyword_pool.pool[i] = NONE_KEY;
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 getIrKey
//  * @brief: 	 get ir_keyword from ir_keyword pool
//  * @param: 	 none
//  * @return: 	 ir_keyword
//  * @details: 	 get ir_keyword from ir_keyword pool which is a queue.
//  *-----------------------------------------------------------------------*/
// enum KEYWORD getIrKey()
// {
// 	static enum KEYWORD ir_key_bak = NONE_KEY;	// the last output key, which had gave  the caller
// 	static u32 key_c_r = 0, key_c_c = 0;
// 	static u32 ir_key_relax_counter = IR_KEY_RELAX_TIME;
// 	enum KEYWORD key;

// 	key = ir_keyword_pool.pool[ir_keyword_pool.outPos]; // get irkey form the pool
// 	if(key != NONE_KEY){ // clear the position and goto next position
// 		ir_keyword_pool.pool[ir_keyword_pool.outPos++] = NONE_KEY;
// 		ir_keyword_pool.outPos &= KEY_POOL_SIZE -1;
// 	}else{ // get a NONE_KEY
// 		if(--ir_key_relax_counter == 0){	// when ir_key_relax_counter is 0 means the button really relaxed
// 			ir_key_relax_counter = IR_KEY_RELAX_TIME;
// 			return  ir_key_bak = NONE_KEY;
// 		}
// 		else if(ir_key_bak != INC_KEY && ir_key_bak != DEC_KEY)
// 		// because the ir key scan time 80ms is shorter than ir key repeat time 108ms,
// 		// so even if you  pressed the key without relax, the key scan may got a none_key.
// 		// and these keys which pressed without relaxed would only recorded once,which means
// 		// now it is none key no matter whether it is really relaxed
// 			return NONE_KEY;		
// 		else
// 			key = ir_key_bak;
// 			goto L1;
// 	}
// 	
// 	ir_key_relax_counter = IR_KEY_RELAX_TIME;	// reload the button relax delay counter
// L1:
// 	if(key == ir_key_bak)	// is the same key
// 		if(key != INC_KEY && key != DEC_KEY){
// 			return NONE_KEY;		// not the special key return none key
// 		}else if(++key_c_c >= 3 ){
// 			key_c_c = 0;
// 			if(--key_c_r)
// 				return NONE_KEY;
// 			else{
// 				key_c_r = 1;
// 				return key;
// 			}
// 		}else
// 			return NONE_KEY;
// 		
// 	if(key == INC_KEY || key == DEC_KEY)
// 		key_c_r = 2;
// 	
// 	ir_key_bak = key;
// 	
// 	return key;
// }



// /*-------------------------------------------------------------------------
//  * @fucname:	 devTestDealIrKey
//  * @brief: 	 do test key
//  * @param: 	 ir_keyword
//  * @return: 	 none
//  * @details: 	 do test key
//  *-----------------------------------------------------------------------*/
// void devTestDealIrKey(enum KEYWORD ir_keyword)
// {
// 	dealTestKey(ir_keyword);
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 devStartDealIrKey
//  * @brief: 	 deal with key event in dev start status
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void devStartDealIrKey(enum KEYWORD ir_keyword)
// {
// 	delStartkey(ir_keyword);
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 devStartbyDealIrKey
//  * @brief: 
//  * @param: 
//  * @return: 
//  * @details: 
//  *-----------------------------------------------------------------------*/
// void devStartbyDealIrKey(enum KEYWORD ir_keyword)
// {
// 	delStandbytkey(ir_keyword);
// }

// /*-------------------------------------------------------------------------
//  * @fucname:	 irKeyScan
//  * @brief: 	 ir key scan and do ir key event
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 ir key scan and do ir key event
//  *-----------------------------------------------------------------------*/
// void irKeyScan()
// {
// 	enum KEYWORD irkey_wrod;
// 	
// 	//IR_SIGNAL_LED_OFF();
// 	irkey_wrod = getIrKey();


// 	switch(dev_state){		
// 	// the dev_state is init to DEV_START and if the CHANNEL3_KEY pressed at  the dev is power on
// 	// qithin DEV_START_TIME then the dev state will change to DEV_TEST
// 		case DEV_START:
// 			devStartDealIrKey(irkey_wrod);
// 			break;
// 		case DEV_TEST:
// 			devTestDealIrKey(irkey_wrod);	// if dev is in DEV_TEST STATE do it
// 			break;
// 		case DEV_STANDBY:
// 			devStartbyDealIrKey(irkey_wrod);
// 			break;
// 		default:
// 			break;
// 	}
// 	startATask(ir_key_task, IR_KEY_SCANF_RATE, 0, irKeyScan);
// }


// /*-------------------------------------------------------------------------
//  * @fucname:	 irKeyTaskInit
//  * @brief: 	 start the ir key task to do ir key scan and key event
//  * @param: 	 none
//  * @return: 	 none
//  * @details: 	 start the ir key task to do ir key scan and key event
//  *-----------------------------------------------------------------------*/
// void irKeyTaskInit()
// {  
// 	irKeyInit();

// 	ir_key_task = getATask();
// 	if(ir_key_task != TASK_FULL)
// 		startATask(ir_key_task, IR_KEY_SCANF_RATE, 0, irKeyScan);
// }

