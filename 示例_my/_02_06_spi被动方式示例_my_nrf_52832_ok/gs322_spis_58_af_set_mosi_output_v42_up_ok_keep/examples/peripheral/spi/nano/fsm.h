#ifndef FSM_H
#define FSM_H


////typedef unsigned          	char u8;
////typedef unsigned short	int 	u16;
////typedef unsigned           	int 	u32;
////typedef unsigned       	long long u64;

////typedef	signed          	char S8;
////typedef   	signed short     int 	S16;
////typedef   	signed           	int   	S32;
////typedef   	signed       long long	S64;


#define FSM_SYNC               0x38
#define FSM_PREAMBLE					 0x15
#define FSM_END      					 0x07
#define FSM_PREAMBLE_SHAPE     0x2a



#define FSM_20K_MODE  	 0
#define FSM_SYNC_MODE 	 1
#define FSM_DATA_MODE 	 2
#define FSM_CRC_MODE  	 3
#define FSM_END_MODE     4
#define FSM_IDLE_MODE    5

#define FSM_CHANEL_1   1
#define FSM_CHANEL_2   2

#define  FSM_CHANEL_DUMMY 0xFF 


//// #define FSM_CHANEL   FSM_CHANEL_1


extern u8 fsm_channel;
extern  u16 crc_data_1;
extern  u16 crc_data_2;
extern u8 transfer_mode;
extern u8 FSM_data_in_1[16];
extern u8 FSM_data_in_2[16];
extern u8 FSM_data_buffer[8];

extern u8 crc_fsm_data1[4];
extern u8 crc_fsm_data2[4];


extern u8 FSM_data_out[16];
extern u8 FSM_encoder(u8 four_bits_data);
extern u8 FSM_decoder(u8 four_bits_data);

//extern void FSM_out( u8  send_flag );
extern void FSM_data_trans(u8 *data);
extern void crc_fsm_trans1(u16 crc_data);
extern void crc_fsm_trans2(u16 crc_data);


//extern u8 index_turn(u8 index);

//  


#endif








