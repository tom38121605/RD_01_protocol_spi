/****************************************************************************
 * @file:		DWHP83.c
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define i2c_scl		PA11
#define i2c_sda		PA10
#define set_scl		GPIO_SET_PIN(PA11)
#define clr_scl		GPIO_CLR_PIN(PA11)
#define set_sda		GPIO_SET_PIN(PA10)
#define clr_sda		GPIO_CLR_PIN(PA10)
#define scl_output	GPIO_SET_MODE(PA,11,GPIO_OPEN_DRAIN)
#define sda_output	GPIO_SET_MODE(PA,10,GPIO_OPEN_DRAIN)
#define i2c_ack		GPIO_GET_PIN(i2c_sda)
#define I2CRATE		6


//MU
#define DP83_TXRX_SWITCH	0X01
#define DP83_VLM			0X02
#define DP83_POWER_ON		0X04


//CU
#define CMD_ROUTE			0x01
#define CMD_CUMU_SWITCH	0x02
#define CMD_PAIRING			0X03
#define CMD_VLM_CHANGE	0X04
#define CMD_POWER_ON		0X05


#define MU_COUNT			12

u8  WL_COCO_KEY1 = 1 ;
u8  WL_COCO_KEY2 = 2 ;
u8  WL_COCO_KEY3 = 3 ;
u8 	WL_COCO_KEY4 = 4 ;   //ma 20150427
u32 i2c_gpio_enable = 0;
// static i2cPoolType dwhp83_i2c_block;   //ma 20150629   
static volatile u8 dwhp83_buf[36];
// static u8 dwhp83_msg = 0;        //ma 20150629   
static u8 ack_error = 0;
static u8 coco[3]={0xff,0xff,0xff};
u8 WL_COCO_KEY = 0;          //ma 20150424
DWHP83_STATE dwhp83_status;
volatile u32 Dwhp83_CuMu = 0;
u32 Dwhp83_flag = 0;
u8 dwhp83_type = VOLUME_STREO;
u8 RX_mute_flag = 0;      //ma 20150606  add RX_mute_flag  for SOUNDBAR MODE VOLOME SYNC
static u32 Dwhp83_route_count = 0;

// u8 SB_link_flag = 0;      //ma 20150707   add sounbarlink flag 
struct Dwhp83_mu_container{
	u16 addr[MU_COUNT];
	u16 count[MU_COUNT];
	u8 event[MU_COUNT];
	U8 status[MU_COUNT];
	u8 type[MU_COUNT];
	u8 playload[MU_COUNT];
}mu_container;

void delayus(u32 c)     //ma 20150629  add u32
{
	sysDelayUs(c);
}


u32 i2cAck()
{
	volatile u32 ack;
	
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE);
	set_scl;
	delayus(I2CRATE>>1);
	ack = i2c_ack;
	delayus(I2CRATE>>1);
	clr_scl;
	delayus(I2CRATE);
	return ack;
}

void ack()
{
	clr_scl;
	delayus(I2CRATE>>1);
	clr_sda;
	delayus(I2CRATE>>1);
	set_scl;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE>>1);
}

void nack()
{
	clr_scl;
	delayus(I2CRATE>>1);
	set_sda;
	delayus(I2CRATE>>1);
	set_scl;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE>>1);
}

void i2cRestart()
{
	clr_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE*10);
	set_scl;
	delayus(I2CRATE);
	clr_sda;
	delayus(I2CRATE);
	clr_scl;
	delayus(I2CRATE);
}

void i2cStop()
{
	clr_sda;
	delayus(I2CRATE);
	set_scl;
	delayus(I2CRATE);
	set_sda;
	delayus(I2CRATE);
}


void i2cWriteByte(u8 data)
{
	u8 msk = 0x80;
	u32 i;
	
	for(i = 0; i<8; i++){
		clr_scl;
		delayus(I2CRATE>>1);
		if(data&msk)
			set_sda;
		else
			clr_sda;
		delayus(I2CRATE>>1);
		set_scl;
		delayus(I2CRATE);
		msk >>= 1;
	}
	
}

u8 i2cReadByte()
{
	u8 data = 0, msk = 0x80;
	u32 i;
	
	for(i = 0; i<8; i++){
		clr_scl;
		delayus(I2CRATE);
		set_scl;
		delayus(I2CRATE);
		if(i2c_ack)
			data |= msk;
		msk >>= 1;
	}
	return data;
}


u32 i2cStart(u8 address)
{
	set_sda;
	set_scl;
	sda_output;
	scl_output;
	i2c_gpio_enable = 1;
	sysDelayUs(10);
	clr_sda;
	delayus(20);
	i2cWriteByte(address);
	if(i2cAck())
		return 1;
	else
		return 0;
}

void exitGpioI2c()
{
//	i2cSysInit();
//	delayus(80);
//	set_sda;
//	i2c_gpio_enable = 0;
}

void sb_volume()                   //ma 20150805  add sb volume control
{
	u8 buf[2]={0x00,0x00};
  buf[0] = 0xc4;
  buf[1] = (u8)amp_volume;
	writeDwhp83(&buf[0],0x00,2);
}
void readDwhp83(u32 address, u32 count)
{
	u32 i;
//   u32 j;       //ma 20150629 
	ack_error = 0;
	if(i2cStart(0x80) == 0){
		delayus(10);
		i2cWriteByte(address);
		if(i2cAck() == 1){
			ack_error = 1;
			i2cStop();
			return;
		}
		delayus(10);
		i2cRestart();
		i2cWriteByte(0x81);
		if(i2cAck() == 0){
			for(i = 0; count-1 > i; i++){
			/*	j = 0;
				while(i2c_ack == 1){
					delayus(10);
					if(++j > 15)
						break;
				}
			*/	delayus(250);        
				dwhp83_buf[i] = i2cReadByte();
				ack();
			}		
			delayus(250);           
			dwhp83_buf[i] = i2cReadByte();
			nack();
			i2cStop();
			return;
		}else{
			ack_error = 1;
			i2cStop();
		}
			
	}else{
		ack_error = 1;
		i2cStop();
	}
		
	
}

void writeDwhp83(volatile u8 *buf, volatile u8 address, u32 count)    //ma 20150624   add volatile
{
	u32 i;

	ack_error = 0;
	if(i2cStart(0x80) == 0){
		i2cWriteByte(address);
		if(i2cAck() == 1){
			ack_error = 1;
			i2cStop();
			return;
		}
		delayus(50);         
		if(buf != 0)
			for(i = 0; i < count; i++){
				clr_scl;
				i2cWriteByte(buf[i]);
				if(i2cAck() == 1){
					ack_error = 1;
					i2cStop();
					return;
				}
				delayus(250);  
			}
		ack_error = 0;
	}else
		ack_error = 1;
	i2cStop();
}


void writeAmp38Cmd(u8 addr, u8 data)
{
	ack_error = 0;

	if(i2cStart(0x38) == 0){
		i2cWriteByte(addr);
		if(i2cAck() == 1){
			ack_error = 1;
			i2cStop();
			return;
		}

		clr_scl;
		delayus(I2CRATE);
		i2cWriteByte(data);
		if(i2cAck() == 1){
			ack_error = 1;
			return;
		}
	}
	i2cStop();
}


void Dwhp83EnterPairing()
{
	u8 buf = 0x01;
	writeDwhp83(&buf,0xc2,1);
	exitGpioI2c();
}

// void Dwhp83StopPairing()
// {
// 	u8 buf = 0x00;
// 	writeDwhp83(&buf,0xc2,1);
// 	exitGpioI2c();
// }


// void Dwhp83Standby()
// {
// 	writeDwhp83(0x00,0xcB,0);
// 	exitGpioI2c();
// }



// void Dwhp83OutStandby()
// {
// 	writeDwhp83(0x00,0xcd,0);
// 	exitGpioI2c();
// }

 
void Dwhp83GetStatus()
{
	readDwhp83(0x3f, 3);

	if(dwhp83_buf[2] == dwhp83_buf[0] + dwhp83_buf[1]){
		dwhp83_status.link = dwhp83_buf[0]&0x1;
		dwhp83_status.init = dwhp83_buf[1]&0x4;
		dwhp83_status.pairing = dwhp83_buf[1]&0x8;
		dwhp83_status.paired = dwhp83_buf[2]&0x1;
		//if(dwhp83_buf[3] != 0){
	}
	exitGpioI2c();
}

void Dwhp83Slave()
{	
	u8 buf = 0x00;
	writeDwhp83(&buf,0xca,1);
	exitGpioI2c();
}



//void DWP83CuPowerOn();
void Dwhp83Master()
{
	u8 buf = 0x01;
	writeDwhp83(&buf,0xca,1);
	exitGpioI2c();
	DWP83CuPowerOn();
}

void Dwhp83SendMsg(u8 msg)
{
	writeDwhp83(&msg,0x20,1);
	
	exitGpioI2c();
}

void getRandonCoCo()
{
	u8 seed = ((systick_counter>>2)^(systick_counter))&&0xff;
		
	coco[0] = seed;
	coco[1] = seed ^((systick_counter>>8)&0xff);
	coco[2] = seed |((systick_counter>>8)&0xff);
}

void Dwhp83SendCoCo()
{
	u8 buf[8] = {0x00,0x00,0x00,0x10,0xc6,0x01}; // CU send MU without linked

	buf[5] = coco[0];
	buf[6] = coco[1];
	buf[7] = coco[2];
	writeDwhp83(buf, 0x02, 8);
	exitGpioI2c();
}

void  DWHP83Mute()
{
	u8 buf = 0x00;
	writeDwhp83(&buf,0xc3,1);
	exitGpioI2c();
}

void Dwhp83WriteCoCo()
{
	writeDwhp83(coco, 0xc6, 3);
	sysDelayMs(20);
	exitGpioI2c();
}

void saveCoCo()
{
	readDwhp83(0x3d, 3);
	exitGpioI2c();
	coco[0] = dwhp83_buf[0];
	coco[1] = dwhp83_buf[1];
	coco[2] = dwhp83_buf[2];
	
}
u8 soundbar_coco_check()      //ma 20150703  add soundbar coco chenck  to decide 
{
	readDwhp83(0x3d, 3);
	exitGpioI2c();
	coco[0] = dwhp83_buf[0];
	coco[1] = dwhp83_buf[1];
	coco[2] = dwhp83_buf[2];
	if(coco[0]>0x04)
		return 1;
	else return 0;
	
}

u8 local_coco_check()                    //ma  20150806
{
	readDwhp83(0x3d, 3);
	exitGpioI2c();
	coco[0] = dwhp83_buf[0];
	coco[1] = dwhp83_buf[1];
	coco[2] = dwhp83_buf[2];
	if(coco[0]>4||coco[0]<1)
		return 0;
	else return 1;
}

/*****add read version cmd 0x22*****/

void Dwhp83ReadVersion()   //ma 20150625 
{
	readDwhp83(0x22, 3);
	exitGpioI2c();
	WL_VERSION_NUMBER[0] = dwhp83_buf[0];
	WL_VERSION_NUMBER[2] = dwhp83_buf[1];
	WL_VERSION_NUMBER[3] = dwhp83_buf[2];
	
}    //ma 20150625   

void  DWHP83UnMute()
{
	u8 buf = 0x01 ;
	writeDwhp83(&buf,0xc3,1);
	exitGpioI2c();
}

// void DWHP83TestF()
// {
// 	u8 buf = 0x01 ;
// 	writeDwhp83(&buf,0xd2,1);
// 	exitGpioI2c();
// }

u32 Dwhp83CuMuDetect()
{
	readDwhp83(0x21,1);
	exitGpioI2c();
	if(ack_error == 1)
		return 0;
	
	if(dwhp83_buf[0] == 0xcc)
		Dwhp83_CuMu = CU;
	else if(dwhp83_buf[0] == 0x55)
		Dwhp83_CuMu = MU;

	return 1;
}


DWHP83_SMSG dwhp83_send_msg;
DWHP83_RMSG dwhp83_receive_msg;
u16 mu_addr;
u16 cc_count = 0;
Timer_ID pt2314_id;

u8 TX_flag = 0;        //ma 20150603 add TX_flag to system
void dwph83Check(void)
{
	Dwhp83_CuMu = 0x55aa;
	Dwhp83CuMuDetect();
	if(Dwhp83_CuMu == 0x55aa){
				Dwhp83Slave();
				sysDelayMs(50);
				startATask(pt2314_id, 2500, 0, dwph83Check);
	}else{
		//if(!(coco[0] == coco[1] && coco[2] == coco[1] && coco[0] == 0xff))
		//	Dwhp83WriteCoCo();
		//Dwhp83EnterPairing();
	}
	//startATask(pt2314_id, 1000, 0, dwph83Check);
}	



void changetoWirelessSlave()
{

	Dwhp83_flag = 0;
	dev_input= USERDEF;
	changeInputChannel();
	startATask(pt2314_id, 3000, 0, dwph83Check);

}

void changetoWirelessMaster()
{

}



// void Dwhp83GetMsg()
// {
// 	readDwhp83(0x3e,5);
// 	if(ack_error == 0){
// 		if(dwhp83_buf[1] == 0xaa 
// 			&& dwhp83_buf[2]== 0xbb 
// 			&& dwhp83_buf[0] == 0x13){

// 			Dwhp83_CuMu = 0;
// 			Dwhp83CuMuDetect();
// 			if(Dwhp83_CuMu == 0x55aa)
// 				if(!SB_flag)       //ma 20150530
// 				changetoWirelessSlave();
// 			
// 		}
// 	}
// 	exitGpioI2c();
// }

void DWHP83RxtoTxMsg()
{
	dwhp83_buf[0] = 0x02;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0xcf;
	dwhp83_buf[6] = 0x13;
	dwhp83_buf[7] = 0xaa;
	dwhp83_buf[8] = 0xbb;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
extern u32 isMute(void);      //ma 20150605
// void SB_volume_trans()    //ma 20150604  add volume control  for soundbar the-one as RX     //ma 20150731 volume to sub
// {
// 	dwhp83_buf[0] = 0x00;        //ma 20150818 del useless function 
// 	dwhp83_buf[1] = 0x00;
// 	dwhp83_buf[2] = 0x00; 
// 	dwhp83_buf[3] = 0x00;
// 	dwhp83_buf[4] = 0x10;
// 	dwhp83_buf[5] = 0x00;
// 	dwhp83_buf[6] = 0xee;
// 	dwhp83_buf[7] = 0xee;
// 	if((isMute() || amp_volume==0) && dev_state != DEV_STANDBY)
// 		dwhp83_buf[8] = 0x00;
// 	else
// 		dwhp83_buf[8] = (amp_volume&(0xff));
// 	
// 	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// }



void RX_SB_alive()       //ma 20150707   SB tell SUB or SB tell TX
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0x5A;
	dwhp83_buf[7] = 0x5A;
	dwhp83_buf[8] = 0x5A;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}

void 	TX_SB_alive()       //ma 20150707   TX tell sub SB is here
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0xA5;
	dwhp83_buf[7] = 0xA5;
	dwhp83_buf[8] = 0xA5;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}

void SB_volume_mute()    //ma 20150604  add volume mute  for soundbar the-one as RX
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0xef;
	dwhp83_buf[7] = 0xef;
	dwhp83_buf[8] = 0xef;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
void SUB_volume_mute()    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0xfe;
	dwhp83_buf[7] = 0xfe;
	dwhp83_buf[8] = 0xfe;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
void SUB_volume_unmute()    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0xdc;
	dwhp83_buf[7] = 0xdc;
	dwhp83_buf[8] = 0xdc;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
void SB_volume_unmute()    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0xcd;
	dwhp83_buf[7] = 0xcd;
	dwhp83_buf[8] = 0xcd;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
void change_to_local_coco()    //ma 20150513   SB mode  tell SUB  chaneg to local  COCO
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0x22;
	dwhp83_buf[7] = 0x22;
	dwhp83_buf[8] = 0x22;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
// void DWHP83_enable_sb()       //ma 20150513   tell SUB  enable S/B mode 
// {
// 	dwhp83_buf[0] = 0x00;
// 	dwhp83_buf[1] = 0x00;
// 	dwhp83_buf[2] = 0x00;
// 	dwhp83_buf[3] = 0x00;
// 	dwhp83_buf[4] = 0x10;
// 	dwhp83_buf[5] = 0x00;
// 	dwhp83_buf[6] = 0x33;
// 	dwhp83_buf[7] = 0x33;
// 	dwhp83_buf[8] = 0x33;
// 	
// 	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// }
// void DWHP83_disable_sb()       //ma 20150513   tell SUB disable S/B mode 
// {
// 	dwhp83_buf[0] = 0x00;
// 	dwhp83_buf[1] = 0x00;
// 	dwhp83_buf[2] = 0x00;
// 	dwhp83_buf[3] = 0x00;
// 	dwhp83_buf[4] = 0x10;
// 	dwhp83_buf[5] = 0x00;
// 	dwhp83_buf[6] = 0x44;
// 	dwhp83_buf[7] = 0x44;
// 	dwhp83_buf[8] = 0x44;
// 	
// 	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// }
void DWHP83toEXT_MCU()       //ma 20150512   add mcu to ext_mcu send message  function
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0x55;
	dwhp83_buf[7] = 0x55;
	dwhp83_buf[8] = 0x55;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
void MU_tell_CU_change()       //ma 20150514  local the-one want to be CU ,he tell the current CU change to MU 
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0x77;
	dwhp83_buf[7] = 0x77;
	dwhp83_buf[8] = 0x77;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}

void set_local_coco()                             //ma 20150710 set auto local  coco change if no link sound bar 
{
	u8 local_coco_check[3] ={0x01,0x01,0x01}; 
	local_coco_check[0] = WL_COCO_KEY;
  local_coco_check[1] = WL_COCO_KEY;
	local_coco_check[2] = WL_COCO_KEY;
	writeDwhp83(local_coco_check, 0xc6, 3);
	sysDelayMs(20); 
}
void set_soundbar_coco()                       //ma 20150710  set auto soundbar coco change if no link sound bar 
{
	u8 soundbar_coco_check[3] ={0x05,0x05,0x05}; 
	soundbar_coco_check[0] = WL_COCO_KEY+4;
  soundbar_coco_check[1] = WL_COCO_KEY+4;
	soundbar_coco_check[2] = WL_COCO_KEY+4;
	writeDwhp83(soundbar_coco_check, 0xc6, 3);
	sysDelayMs(20); 
}
void DWHP83toSUB()       //ma 20150512   add local DWHP83 send message to SUB   function
{
	dwhp83_buf[0] = 0x00;
	dwhp83_buf[1] = 0x00;
	dwhp83_buf[2] = 0x00;
	dwhp83_buf[3] = 0x00;
	dwhp83_buf[4] = 0x10;
	dwhp83_buf[5] = 0x00;
	dwhp83_buf[6] = 0x66;
	dwhp83_buf[7] = 0x66;
	dwhp83_buf[8] = 0x66;
	
	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
}
// 	void is_there_any_TX()       //ma 20150603 is_there_any_TX
// 	{
// 	dwhp83_buf[0] = 0x00;
// 	dwhp83_buf[1] = 0x00;
// 	dwhp83_buf[2] = 0x00;
// 	dwhp83_buf[3] = 0x00;
// 	dwhp83_buf[4] = 0x10;
// 	dwhp83_buf[5] = 0x00;
// 	dwhp83_buf[6] = 0xbb;
// 	dwhp83_buf[7] = 0xbb;
// 	dwhp83_buf[8] = 0xbb;

// 	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// 	}
// 	// void this_is_TX()       //ma 20150603 this_is_TX
// {
// 	dwhp83_buf[0] = 0x00;
// 	dwhp83_buf[1] = 0x00;
// 	dwhp83_buf[2] = 0x00;
// 	dwhp83_buf[3] = 0x00;
// 	dwhp83_buf[4] = 0x10;
// 	dwhp83_buf[5] = 0x00;
// 	dwhp83_buf[6] = 0xcc;
// 	dwhp83_buf[7] = 0xcc;
// 	dwhp83_buf[8] = 0xcc;
// 	
// 	writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// }
// void DWHP83TxToRx()
// {
// 	//Dwhp83_flag |=                                
// }

void DWHP83SendCmd(u8 cmd, u8 playload, u16 addr)
{
	static u8 *p;
	
	dwhp83_send_msg.msg_head[0] = 0x02;
	dwhp83_send_msg.msg_head[1] = 0x00;
	dwhp83_send_msg.msg_head[2] = 0x00;
	dwhp83_send_msg.msg_head[3] = 0x00;

	dwhp83_send_msg.msg_cmd[0] = 0x10;
	dwhp83_send_msg.msg_cmd[1] = 0xcf;
	dwhp83_send_msg.msg_cmd[2] = 0x13;
	
	dwhp83_send_msg.count = ++cc_count;
	dwhp83_send_msg.mu_addr = addr;

	dwhp83_send_msg.sub_cmd = cmd;
	dwhp83_send_msg.playload[0] = playload;
	dwhp83_send_msg.playload[1] = dwhp83_receive_msg.playload[1];

	p = (u8*)&dwhp83_send_msg;
	p += 7;

	dwhp83_send_msg.playload[2] = 0;
	dwhp83_send_msg.playload[2] = p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7];
	
	writeDwhp83(&dwhp83_send_msg.msg_head[1] , dwhp83_send_msg.msg_head[0], 15);
	exitGpioI2c();
}


void  DWPH83VolumeChange(u8 type, u8 volume, u16 addr)
{
	u8 *p;
	
	dwhp83_send_msg.msg_head[0] = 0x02;
	dwhp83_send_msg.msg_head[1] = 0x00;
	dwhp83_send_msg.msg_head[2] = 0x00;
	dwhp83_send_msg.msg_head[3] = 0x00;

	dwhp83_send_msg.msg_cmd[0] = 0x10;
	dwhp83_send_msg.msg_cmd[1] = 0xcf;
	dwhp83_send_msg.msg_cmd[2] = 0x13;
	
	dwhp83_send_msg.count = ++cc_count;
	dwhp83_send_msg.mu_addr = addr;

	dwhp83_send_msg.sub_cmd = CMD_VLM_CHANGE;
	dwhp83_send_msg.playload[0] = type;
	dwhp83_send_msg.playload[1] = volume;
	
	p = (u8*)&dwhp83_send_msg;
	p += 7;

	dwhp83_send_msg.playload[2] = 0;
	dwhp83_send_msg.playload[2] = p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7];
	
	writeDwhp83(&dwhp83_send_msg.msg_head[1] , dwhp83_send_msg.msg_head[0], 15);
	exitGpioI2c();
}




u32 cvcv = 0;
void  DWP83CuCV()
{ 
	u32 i;

	if(dwhp83_type == VOLUME_STREO) 
		return;
	
	cvcv = 1;
	for(i = 0; i < MU_COUNT; i++)
		if(mu_container.count[i] != 0 && mu_container.type[i] == dwhp83_type){
			mu_container.event[i] = CMD_VLM_CHANGE;
			mu_container.playload[i] = amp_volume;
			DWPH83VolumeChange(mu_container.type[i],mu_container.playload[i],mu_container.addr[i]);
		}
}

void  DWP83CuCV0()
{ 
	u32 i;

	if(dwhp83_type == VOLUME_STREO) 
		return;
	
	for(i = 0; i < MU_COUNT; i++)
		if(mu_container.count[i] != 0 && mu_container.type[i] == dwhp83_type){
			mu_container.event[i] = CMD_VLM_CHANGE;
			mu_container.playload[i] = 1;
			DWPH83VolumeChange(mu_container.type[i],mu_container.playload[i],mu_container.addr[i]);
		}
}


void DWP83CuPowerOn()
{
	u32 i;
	
	for(i = 0; i < MU_COUNT; i++)
		mu_container.status[i] = CMD_POWER_ON;
}

void DWP83MuPowerOn()
{
	Dwhp83_flag |= DP83_POWER_ON;
}

u32 isDwhp83Linked()
{
	if(Dwhp83_route_count > 4)
		return 0;
	else
		return 1;
}

void SB_rx_volume_2_SUB()        //MA 20150818  add  sb at rx mode send volume to SUB ,especially when SUB power on 
{
		DWPH83VolumeChange(dwhp83_type, (u8)amp_volume, mu_addr);      //ma 20150818
}
void dwhp83CuAck(u8 cmd)
{
	DWHP83SendCmd(0x80|cmd, 0, dwhp83_receive_msg.mu_addr);
}


void dealMuContainer()
{
	static u32 i;
// 	u16 k = 0xff;  //ma 20150629 
	for(i = 0; i < MU_COUNT; i++){
		if(mu_container.count[i] != 0 && mu_container.addr[i] != 0){
			mu_container.count[i]--;
			if(mu_container.status[i] == CMD_POWER_ON)
				DWHP83SendCmd(CMD_POWER_ON, 0, mu_container.addr[i]);
			if(mu_container.event[i] == CMD_VLM_CHANGE)
				DWPH83VolumeChange(mu_container.type[i],mu_container.playload[i],mu_container.addr[i]);
		}else{
			mu_container.addr[i] = 0;
			mu_container.count[i] = 0;
		}
	}
}

void addMuContainer()
{
	u32 i;
	u16 k = 0xff;
	for(i = 0; i < MU_COUNT; i++){
		if(mu_container.addr[i] == 0)
			k = i;
		if(dwhp83_receive_msg.mu_addr == mu_container.addr[i] &&
			mu_container.addr[i] > 7){
			mu_container.count[i] = 5;
			mu_container.type[i] = dwhp83_receive_msg.playload[0];
			return;
		}
	}
	if(k != 0xff){
		mu_container.addr[k] = dwhp83_receive_msg.mu_addr;	
		mu_container.type[k] = dwhp83_receive_msg.playload[0];
		mu_container.count[k] = 5;
	}
}



void cuDealCmd(u8 cmd)
{
	switch(cmd){
		case CMD_VLM_CHANGE:
			
			break;
		default:
			break;
	}
}
void doCuMsg()
{
	u32 i;
	
	
	switch(dwhp83_receive_msg.sub_cmd){	
			case CMD_ROUTE:
					dwhp83CuAck(CMD_ROUTE);
					if(0 == tx_linked_flag)      //ma 20150906   set tx sound flag 
					{
						tx_linked_flag =1;
						TX_sound_flag = 0;  
					}
					addMuContainer();
					break;
			case CMD_CUMU_SWITCH:
					bt_amp_start_flag = 1;   //ma 20150727  avoid  changing cumu  noise
					dwhp83CuAck(CMD_CUMU_SWITCH);
					sysDelayMs(50);
// 					dwhp83CuAck(CMD_CUMU_SWITCH);
// 					sysDelayMs(50);
					changetoWirelessSlave();
					BT_OFF();     					 	//ma 20150731   DWHP83 RX MODE, turn off bt    
					btShutDown();    					//ma 20150731   DWHP83 RX MODE, turn off bt   
					sysDelayMs(2000);
					bt_amp_start_flag = 0;   //ma 20150727  avoid  changing cumu  noise
					break;
			case CMD_VLM_CHANGE:
					dwhp83CuAck(CMD_VLM_CHANGE);
					if(dwhp83_receive_msg.playload[0] == VOLUME_STREO)
						break;
					//if(dwhp83_receive_msg.playload[0] == dwhp83_type){
					//	amp_volume = dwhp83_receive_msg.playload[1];
					//	ampSetVolume(amp_volume);
					//}
 
					if(dwhp83_receive_msg.playload[1] == 0)
						dwhp83_receive_msg.playload[1] = 1;
						cvcv = 1;
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.count[i] != 0 && mu_container.type[i] == dwhp83_receive_msg.playload[0]){
							mu_container.event[i] = CMD_VLM_CHANGE;
							mu_container.playload[i] = dwhp83_receive_msg.playload[1];
							DWPH83VolumeChange(mu_container.type[i],mu_container.playload[i],mu_container.addr[i]);
						}
// 					for(i = 0; i < MU_COUNT; i++)
// 						if(mu_container.count[i] != 0 &&
// 						mu_container.addr[i] != dwhp83_receive_msg.mu_addr &&
// 						mu_container.type[i] == dwhp83_receive_msg.playload[0]){
// 							mu_container.event[i] = CMD_VLM_CHANGE;
// 							mu_container.playload[i] = dwhp83_receive_msg.playload[1];
// 							DWPH83VolumeChange(dwhp83_receive_msg.playload[0],dwhp83_receive_msg.playload[1],mu_container.addr[i]);
// 							sysDelayMs(50);    //ma 20150805
// 						}
// 					  sysDelayMs(100);
// 						dwhp83_buf[0] = 0x00;
// 						dwhp83_buf[1] = 0x00;
// 						dwhp83_buf[2] = 0x00;
// 						dwhp83_buf[3] = 0x00;
// 						dwhp83_buf[4] = 0x10;
// 						dwhp83_buf[5] = 0x00;
// 						dwhp83_buf[6] = 0xee;
// 						dwhp83_buf[7] = 0xee;
// 						dwhp83_buf[8] = dwhp83_receive_msg.playload[1];

// 						writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// 						led_flash = 4;       //ma 20150804                       //MA 20150819  no need always do indication
					break;
			case CMD_POWER_ON:
					dwhp83CuAck(CMD_POWER_ON);
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.count[i] != 0  
						   &&mu_container.addr[i] != dwhp83_receive_msg.mu_addr){
							mu_container.status[i] = CMD_POWER_ON;
							DWHP83SendCmd(CMD_POWER_ON, 0, mu_container.addr[i]);
						}
// 					if(dev_state == DEV_STANDBY)    //ma 20150727 no need to outstandby
// 						outStandby();
					break;
			//-------------------------------------
			case CMD_VLM_CHANGE+0x80:
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.addr[i] == dwhp83_receive_msg.mu_addr
							&& mu_container.playload[i] == dwhp83_receive_msg.playload[1]){
							mu_container.event[i] = 0;
							break;
						}
					break;
			case CMD_POWER_ON + 0x80:
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.addr[i] == dwhp83_receive_msg.mu_addr){
							mu_container.status[i] = 0;
							break;
						}
					break;
			default:
				break;
	}
	
}

void doMuMsg()
{
	if(mu_addr == dwhp83_receive_msg.mu_addr){
		switch(dwhp83_receive_msg.sub_cmd){	
			case CMD_ROUTE |0x80:
				Dwhp83_route_count = 0;		
				saveCoCo();
				break;
			case CMD_VLM_CHANGE:
				Dwhp83_route_count = 0;		
				DWHP83SendCmd(0x80|CMD_VLM_CHANGE, 0, mu_addr);
			//	if(dwhp83_receive_msg.playload[0] == dwhp83_type){
			//		amp_volume = dwhp83_receive_msg.playload[1];
			//		ampSetVolume(amp_volume);
			//	}
			
				break;
			case CMD_POWER_ON:
				DWHP83SendCmd(0x80|CMD_POWER_ON, 0, mu_addr);
// 				if(dev_state == DEV_STANDBY)           //ma 20150727 no need to outstandby
// 					outStandby();
				break;

		//----------------------------------------		
			case CMD_CUMU_SWITCH |0x80:
				Dwhp83_flag = 0;
// 				if(Dwhp83CuMuDetect())          //ma20150727  MU no need to change        // ma 20150807
// 				{
// 					if(Dwhp83_CuMu != MU)
// 					Dwhp83Slave();          //ma 20150714 
// 					sysDelayMs(1000);
// 				}
// 				Dwhp83Master();
// 			
// 			/****no  need to restart bt when  no soungbar mode*****/
// 			if(SB_flag)       //ma 20150702 
// 			{
// 				btShutDown();     //ma 20150605  WL dont turn off bt  shut down for WL volume SYNC
// // 				playMusic(DISCONECTED_SOUND);      // ma 20150605  disable this sound 
// 				sysDelayMs(1000);      //ma 20150605
// 				btStartUp();
// 				sysDelayMs(1500);
// 			}
// 			else 
// 				sysDelayMs(2500);
				break;
			case  CMD_VLM_CHANGE |0x80:
				if(amp_volume == dwhp83_receive_msg.playload[1])
					Dwhp83_flag &= ~DP83_VLM;
 				break;
			case CMD_POWER_ON | 0x80:
				Dwhp83_flag &= ~CMD_POWER_ON;
				break;
			default:
				break;
		}
	}
}


extern u8* strncpy(u8 *src, u8 *des, u32 n);
void DWHP83DealMsg()
{
	u8 sum;
	u32 i;
	if(Dwhp83CuMuDetect())
		readDwhp83(0x3e,33);                     //ma 20150805            change 10 to 25
		exitGpioI2c();
		sum = 0;
		for(i = 1; i < 8; i++)          //ma 20150512  add check the message send by ext-mcu
		{
			sum += dwhp83_buf[i]; 
			if((i == 4)&&(dwhp83_buf[0] == 0)) //ma 20150512  add check the message send by ext-mcu
			{
				if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[1]==0x55)  
				{
// 					ledColorSet(GREEN_COLOR);       //ma 20150606  no green color to indicate
// 					playMusic(PAIRED_SOUND);      //ma 20150605  diable this sound
					DWHP83toSUB();           //ma 20150512  CU send massage to SUB  0x66
// 					sysDelayAMs(1000);          //ma 20150606  no delay to trans 
				}
// 				if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==0xee)       //ma 20150604  add for SB_volume_control   //ma 20150818 del useless function 
// 				{
// // 					if(Dwhp83CuMuDetect())                       //ma 20150805
// // 					{
// // 						if(Dwhp83_CuMu == CU)       
// // 						{
// // 								dwhp83_buf[0] = 0x00;
// // 								dwhp83_buf[1] = 0x00;
// // 								dwhp83_buf[2] = 0x00;
// // 								dwhp83_buf[3] = 0x00;
// // 								dwhp83_buf[4] = 0x10;
// // 								dwhp83_buf[5] = 0x00;
// // 								dwhp83_buf[6] = 0xee;
// // 								dwhp83_buf[7] = 0xee;
// // 								dwhp83_buf[8] = dwhp83_buf[3];

// // 								writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0],8);
// // // 								led_flash = 4;       //ma 20150804  
// // // 								SB_volume_trans();
// // 						}
// // 					}
// 				}
			if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[2]==0xef)       //ma 20150608  add for SUB_volume_mute 
				{
					if(Dwhp83CuMuDetect())
					{
						if(Dwhp83_CuMu == CU)       
						{
								SUB_volume_mute();
						}
					}
				}
				if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[2]==0x5A)       //ma20150707   TX transmit the news 
				{
					if(Dwhp83CuMuDetect())
					{
						if(Dwhp83_CuMu == CU)       
						{
								TX_SB_alive();     // ma 20150704   transmit  sb is here 
						}
					}
				}
// 				if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[2]==0x5A)       //ma20150707   TX transmit the news  
// 				{
// 					if(Dwhp83CuMuDetect())             //ma 20150917  delete useless function
// 					{
// 						if(Dwhp83_CuMu == CU)       
// 						{
// 								TX_SB_alive();     // ma 20150704   transmit  sb is here 
// 						}
// 					}
// 				}
			if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[2]==0x77)       //ma 20150608  add for SUB_volume_mute 
				{
					if(Dwhp83CuMuDetect())
					{
						if(Dwhp83_CuMu == CU)       
						{
								changetoWirelessSlave();
								BT_OFF();     					 	//ma 20150731   DWHP83 RX MODE, turn off bt    
								btShutDown();    					//ma 20150731   DWHP83 RX MODE, turn off bt   
// 								ledColorSet(GREEN_COLOR); 
								sysDelayMs(2000);
								bt_amp_start_flag = 0;   //ma 20150727  avoid  changing cumu  noise
						}
					}
// 					led_flash = 4;
				}
        
			}
		}
		if(sum != dwhp83_buf[8])
			return;
		strncpy((u8*)&dwhp83_buf[1], (U8*)&dwhp83_receive_msg.mu_addr, 8);        //ma 20150630   type define    u8*
		if(enter_standby_flag == 0)        //MA 20150604
		{
			if(Dwhp83_CuMu == MU){
				//Dwhp83_route_count = 0;
				doMuMsg();
			}else{
				doCuMsg();
			}
		}
}


Timer_ID dwph83_id = TASK_FULL;

void DWP83SpiritTask()
{
// 	u32 i;    //ma 20150629 
	static u32 ck = 0;
	
	Dwhp83CuMuDetect();
	if(ack_error == 0){
		if(Dwhp83_CuMu == MU /*&& dev_input == WIRELESS*/){
			++Dwhp83_route_count;
			Dwhp83GetStatus();
			if(dwhp83_status.link == 1){
				if(++ck&0x1)
					DWHP83SendCmd(CMD_ROUTE,dwhp83_type,mu_addr);

				sysDelayMs(3);
				if(Dwhp83_flag&DP83_TXRX_SWITCH)
					DWHP83SendCmd(CMD_CUMU_SWITCH,0,mu_addr);

				sysDelayMs(3);
				if(Dwhp83_flag&DP83_POWER_ON)
					DWHP83SendCmd(CMD_POWER_ON,0,mu_addr);
				
				sysDelayMs(3);
				if(Dwhp83_flag&DP83_VLM)
					DWPH83VolumeChange(dwhp83_type, (u8)amp_volume, mu_addr);

			}
			if(++Dwhp83_route_count > 10){
				Dwhp83_route_count = 8;
				//if(dev_input == WIRELESS)
				//	Dwhp83OutStandby();
				//else
				//	Dwhp83Standby();
			}
			
		}else{
				
			dealMuContainer(); 
			
			if(cvcv == 1){
				ck = 0;
				cvcv = 0;
			}
			
			if(++ck > 3){
				ck = 0;
// 				DWP83CuCV
				if(ismute()){        //ma 20150606
							DWP83CuCV0();
						}else{
							DWP83CuCV();
						}
			}
		}
		
	}else{
			Dwhp83_route_count = 10;
		}
	
	startATask(dwph83_id, 1500, 0, DWP83SpiritTask);
}


void DWPH83Init()
{
	u32 i = 0;
	
	dwph83_id = getATask();
	pt2314_id = getATask();
	Dwhp83_route_count = 0;

	for(i = 0; i < 10; i++){
		mu_container.addr[i] = 0;
		mu_container.count[i] = 5;
	}

	coco[0] = coco[1] = coco[2] = 0xff;
	
	startATask(dwph83_id, 1000, 0, DWP83SpiritTask);
}


static u8 DEVICE_ADDRESS = 0xAE;
u32 readBuf(u8 *buf, u32 address, u32 count)
{
	u32 i;
//	u32 j;    //ma 20150629 

	ack_error = 0;
	if(i2cStart(DEVICE_ADDRESS) == 0){
		delayus(10);
		i2cWriteByte((address&0xff00)>>8);
		if(i2cAck() == 1){
			ack_error = 1;
			i2cStop();
			return ack_error;
		}
		delayus(10);
		i2cWriteByte(address&0xff);
		if(i2cAck() == 1){
			ack_error = 1;
			i2cStop();
			return ack_error;
		}
		delayus(10);
		i2cRestart();
		i2cWriteByte(DEVICE_ADDRESS | 0x1);
		if(i2cAck() == 0){
			for(i = 0; count-1 > i; i++){
			/*	j = 0;
				while(i2c_ack == 1){
					delayus(10);
					if(++j > 15)
						break;
				}
			*/	delayus(50);
				buf[i] = i2cReadByte();
				ack();
			}		
			delayus(50);
			buf[i] = i2cReadByte();
			nack();
			i2cStop();
			return ack_error;
		}else{
			ack_error = 1;
			i2cStop();
		}
			
	}else{
		ack_error = 1;
		i2cStop();
	}
		
	return ack_error;
}



u32 volatile debug_dwhp83 = 0;
void dwhp83TestProcess()
{
	while(1){
		switch(debug_dwhp83){
			case 1:
				Dwhp83Master();
				break;
			case 2:
				Dwhp83Slave();
				break;
			case 3:
				Dwhp83SendCoCo();
				break;
			case 4:
				Dwhp83EnterPairing();
				break;
			case 5:
				writeDwhp83(&dwhp83_buf[1], dwhp83_buf[0], 15);
				break;
			case 6:
				Dwhp83GetStatus();
			default:
				break;
		}
		sysDelayMs(1);
		debug_dwhp83 = 0;
	}
}


