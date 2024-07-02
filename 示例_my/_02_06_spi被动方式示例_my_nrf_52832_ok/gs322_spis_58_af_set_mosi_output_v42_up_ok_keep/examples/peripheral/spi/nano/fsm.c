
//#include "NUC123Series.h"
//#include "typedef.h"

#include "icrc.h"
#include "fsm.h"



u8 FSM_data_in_1[16] = 	  {0, 1 ,2 ,3 ,4 ,5, 6, 7, 8 ,9 ,10 ,11 ,12 ,13 ,14 ,15};
u8 FSM_data_in_2[16] = 	  {15, 14 ,13 ,12 ,11 ,10, 9, 8, 7 ,6 ,5 ,4 ,3 ,2 ,1 ,0};
u8 FSM_data_out[16]= 	  {0, 0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 FSM_data_buffer[8]=	{0, 0 ,0 ,0 ,0 ,0, 0, 0};
u8 transfer_mode = FSM_IDLE_MODE;
u16 crc_data_1 = 0;
u16 crc_data_2 = 0;

u8 crc_fsm_data1[4] = {0 ,0 ,0 ,0 };
u8 crc_fsm_data2[4] = {0 ,0 ,0 ,0 }; 


u8 fsm_channel = FSM_CHANEL_2;

u8 FSM_encoder(u8 four_bits_data) 
{
	switch(four_bits_data)
	{
		case 0x0:
		{
			return 0xe;
		}
		case 0x1:
		{
			return 0xd;
		}
		case 0x2:
		{
			return 0xb;
		}
		case 0x3:
		{
			return 0x1c;
		}
		case 0x4:
		{
			return 0x19;
		}
		case 0x5:
		{
			return 0x1a;
		}
		case 0x6:
		{
			return 0x13;
		}
		case 0x7:
		{
			return 0x16;
		}
		case 0x8:
		{
			return 0x31;
		}
		case 0x9:
		{
			return 0x32;
		}
		case 0xa:
		{
			return 0x34;
		}
		case 0xb:
		{
			return 0x23;
		}
		case 0xc:
		{
			return 0x26;
		}
		case 0xd:
		{
			return 0x25;
		}
		case 0xe:
		{
			return 0x2c;
		}
		case 0xf:
		{
			return 0x29;
		}
		default:
		{
			return 0xff;
		}
	}
   
}


u8 FSM_decoder(u8 four_bits_data)
{
	switch(four_bits_data)
	{
		case 0xe:
		{
			return 0x0;
		}
		case 0xd:
		{
			return 0x1;
		}
		case 0xb:
		{
			return 0x2;
		}
		case 0x1c:
		{
			return 0x3;
		}
		case 0x19:
		{
			return 0x4;
		}
		case 0x1a:
		{
			return 0x5;
		}
		case 0x13:
		{
			return 0x6;
		}
		case 0x16:
		{
			return 0x7;
		}
		case 0x31:
		{
			return 0x8;
		}
		case 0x32:
		{
			return 0x9;
		}
		case 0x34:
		{
			return 0xa;
		}
		case 0x23:
		{
			return 0xb;
		}
		case 0x26:
		{
			return 0xc;
		}
		case 0x25:
		{
			return 0xd;
		}
		case 0x2c:
		{
			return 0xe;
		}
		case 0x29:
		{
			return 0xf;
		}
		default:
		{
			return 0xff;
		}
	}
}


void FSM_data_trans(u8 *data)
{
	u8  i = 0;
	for(i = 0 ;  i <  16 ;  i++)
	{
		FSM_data_out[i] = FSM_encoder(data[i]);
	}
}

//void FSM_fill_data(u8 *data)   //6 bits * 8  to  8bits *6
//{
//	u64  data_stack = 0;
//	u8   i = 0;
//	
//	for(i = 0 ;  i <  8 ;  i++)
//	{
//		data_stack = (data[i] & 0x3F) << 6;
//	}
//		
//	for(i = 0;  i  <  6 ;  i++)
//	{
//		FSM_data_buffer[i] = (data_stack >> (i*8)) &0xff ;
//	}
//	
//}

////void FSM_fill_crc()
////{
////	u16 crc_num = 0;
////	crc_num = get_crc_data(FSM_data_buffer,6);
////	FSM_data_buffer[6] = crc_num & 0xff;
////	FSM_data_buffer[7] =(crc_num>>8) & 0xff;
////}


void crc_fsm_trans1(u16 crc_data)
{
	u8 transdata = 0;
	transdata = crc_data & 0x0f;
	crc_fsm_data1[0] = transdata;
 
	transdata = (crc_data & 0xf0) >>   4;
	crc_fsm_data1[1] = transdata;
 
	transdata = (crc_data & 0xf00)>>   8;
	crc_fsm_data1[2] = transdata;
 
	transdata = (crc_data & 0xf000)>> 12;
	crc_fsm_data1[3] = transdata;
 
}

void crc_fsm_trans2(u16 crc_data)
{
	u8 transdata = 0;
	transdata = crc_data & 0x0f;
	crc_fsm_data2[0] = transdata;
 
	transdata = (crc_data & 0xf0) >>   4;
	crc_fsm_data2[1] = transdata;
 
	transdata = (crc_data & 0xf00)>>   8;
	crc_fsm_data2[2] = transdata;
 
	transdata = (crc_data & 0xf000)>> 12;
	crc_fsm_data2[3] = transdata;
 
}
