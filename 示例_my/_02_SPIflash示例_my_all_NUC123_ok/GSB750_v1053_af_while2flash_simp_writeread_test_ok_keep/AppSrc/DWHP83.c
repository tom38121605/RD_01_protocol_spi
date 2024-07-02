/****************************************************************************
 * @file:		DWHP83.c
 * @version:		
 * @breif:		
 * @date: 		
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Globla silicon . All rights reserved.
 * 
 ****************************************************************************/
#include "sysinc.h"
#define i2c_scl		WL_I2C_SCL_PORT_PIN
#define i2c_sda		WL_I2C_SDA_PORT_PIN
#define set_scl		GPIO_SET_PIN(WL_I2C_SCL_PORT_PIN)
#define clr_scl		GPIO_CLR_PIN(WL_I2C_SCL_PORT_PIN)
#define set_sda		GPIO_SET_PIN(WL_I2C_SDA_PORT_PIN)
#define clr_sda		GPIO_CLR_PIN(WL_I2C_SDA_PORT_PIN)
#define scl_output	GPIO_SET_MODE(WL_I2C_SCL_PORT,WL_I2C_SCL_PIN,GPIO_OPEN_DRAIN)
#define sda_output	GPIO_SET_MODE(WL_I2C_SDA_PORT,WL_I2C_SDA_PIN,GPIO_OPEN_DRAIN)
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


u8 cumu_flag = 0;      //ma   20150525   add cumu flag
u32 i2c_gpio_enable = 0;
// static i2cPoolType dwhp83_i2c_block;
static  u8 dwhp83_buf[36];
// static u8 dwhp83_msg = 0;
static u8 ack_error = 0;
static u8 coco[3]={0xff,0xff,0xff};
volatile u32 Dwhp83_CuMu = 0;
u32 Dwhp83_flag = 0;
u8 dwhp83_type = VOLUME_STREO;       //ma 20150526  change the default type
static u32 Dwhp83_route_count = 0;
struct Dwhp83_mu_container{
	u16 addr[MU_COUNT];
	u16 count[MU_COUNT];
	u8 event[MU_COUNT];
	U8 status[MU_COUNT];
	u8 type[MU_COUNT];
	u8 playload[MU_COUNT];
}mu_container;


DWHP83_STATE dwhp83_status;

void delayus(u32 c)
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
//	I2C_DISABLE_I2C(I2C0);
//	I2C_DISABLE_TOC(I2C0);
//	outpw(&SYS->GPF_MFP, inpw(&SYS->GPF_MFP) & ~(0x3<<2));
//	outpw(&SYS->ALT_MFP1, (inpw(&SYS->ALT_MFP1) & ~(0xF<<24)));
	set_sda;
	set_scl;
	sda_output;
	scl_output;
//	i2c_gpio_enable = 1;
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
	delayus(80);
	set_sda;
	i2c_gpio_enable = 0;
}

void readDwhp83(u32 address, u32 count)
{
	u32 i;

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

void writeDwhp83(u8 *buf, u8 address, u32 count)
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


void Dwhp83EnterPairing()
{
	u8 buf = 0x01;
	writeDwhp83(&buf,0xc2,1);
	exitGpioI2c();
}

void Dwhp83StopPairing()
{
	u8 buf = 0x00;
	writeDwhp83(&buf,0xc2,1);
	exitGpioI2c();
}


void Dwhp83Standby()
{
	writeDwhp83(0x00,0xcB,0);
	exitGpioI2c();
}



void Dwhp83OutStandby()
{
	writeDwhp83(0x00,0xcd,0);
	exitGpioI2c();
}

 
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



void DWP83CuPowerOn(void);
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
	#if(FIXED_COCO_MODE)       //20150525
	coco[0] = 0x50;
	coco[1] = 0x72;
	coco[2] = 0x31;       //ma 20150522   set fixed coco
	#endif
	buf[5] = coco[0];
	buf[6] = coco[1];
	buf[7] = coco[2];
	writeDwhp83(buf, 0x02, 8);
	exitGpioI2c();
}



void Dwhp83FixedCoCo()     //20150525
{
	u8 fixed_coco[3]= {0x50,0x72,0x31};
		#if(MINI_LINK_CORE)        //ma 20151130 
		fixed_coco[0] = 0x04;
		fixed_coco[1] = 0x04;
		fixed_coco[2] = 0x04;       //ma 20150522   set fixed coco
		#else
		fixed_coco[0] = 0x37;
		fixed_coco[1] = 0x35;
		fixed_coco[2] = 0x30;       //ma 20150522   set fixed coco
		#endif

	writeDwhp83(fixed_coco, 0xc6, 3);
	sysDelayMs(20);
	exitGpioI2c();
}
u8 check_coco()      //ma 20150522   check  coco whether fixed coco
{
	readDwhp83(0x3d, 1);
	exitGpioI2c();
	coco[0] = dwhp83_buf[0];
#if(MINI_LINK_CORE) 
	if(coco[0]!=0x04 || coco[0]!= 0x04 )    //ma 20151130  for CORE channel 1
#else
	if(coco[0]!=0x37 || coco[0]!= 0x35 )
#endif
	return 1;
	else  return 0;
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
// 	#if(FIXED_COCO_MODE)
		#if(MINI_LINK_CORE)        //ma 20151130 
		coco[0] = 0x04;
		coco[1] = 0x04;
		coco[2] = 0x04;       //ma 20150522   set fixed coco
		#else
		coco[0] = 0x37;
		coco[1] = 0x35;
		coco[2] = 0x30;       //ma 20150522   set fixed coco
		#endif

// 	#endif
}

void  DWHP83UnMute()
{
	u8 buf = 0x01 ;
	writeDwhp83(&buf,0xc3,1);
	exitGpioI2c();
}



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
extern Timer_ID pt2314_id;

void dwph83Check(void)
{
	Dwhp83_CuMu = 0x55aa;
	Dwhp83CuMuDetect();
	if(Dwhp83_CuMu == 0x55aa){
				Dwhp83Slave();
				sysDelayMs(50);
// #if(FIXED_COCO_MODE)
				cumu_flag = 0;   //ma 20150525
				setConfig();   //ma 20150525
// #endif
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
	wireless_mode_open = 1;
	led_saving_couter = 0;
	led_saving_power_flag = 0;
	change_source_mute = 1;
	change_source_counter = 0;
	ampMute();
	dev_input= WIRELESS;
	state_color = input_channel_color[dev_input];
	ledColorSet(state_color);
	GPIO_CLR_PIN(WIRELESS_BT_SEL_PORT_PIN);
	sysDelayMs(500);
	unMute();          //ma   20160406
	sysDelayMs(20);
// 	playMusic(DISCONECTED_SOUND);
	Dwhp83Slave();
// #if(FIXED_COCO_MODE)
	cumu_flag = 0;   //ma 20150525
	setConfig();   //ma 20150525
// #endif
// 	sysDelayMs(100);
// 	BT_POWER_OFF();
// 	sysDelayMs(200); 
// 	BT_POWER_ON();
// 	sysDelayMs(200);
// 	BT_POWER_OFF();


	startATask(pt2314_id, 2800, 0, dwph83Check);
	WB_paired_flag = 0;
}

void changetoWirelessMaster()
{

}



void Dwhp83GetMsg()
{
	readDwhp83(0x3e,5);
	if(ack_error == 0){
		if(dwhp83_buf[1] == 0xaa 
			&& dwhp83_buf[2]== 0xbb 
			&& dwhp83_buf[0] == 0x13){

			Dwhp83_CuMu = 0;
			Dwhp83CuMuDetect();
			if(Dwhp83_CuMu == 0x55aa)
				changetoWirelessSlave();
			
		}
	}
	exitGpioI2c();
}

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

void DWHP83TxToRx()
{
	//Dwhp83_flag |=                                
}

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
extern s32 sum_H;
void getMuAddr()
{ 
	enum PT2314_INPUT_CHANNEL i;
	
	i = dev_input;
	dev_input = LINEIN;
	GPIO_SET_PIN(WIRELESS_BT_SEL_PORT_PIN);
	GPIO_CLR_PIN(BT_AUX_CTRL_PORT_PIN);
	sysDelayMs(5); 
	mu_addr = (sum_H>>8);
	if(mu_addr < 8){
		mu_addr = ((mu_addr+3)<<8);
	}
	dev_input = i;
} 




void dwhp83CuAck(u8 cmd)
{
	DWHP83SendCmd(0x80|cmd, 0, dwhp83_receive_msg.mu_addr);
}


void dealMuContainer()
{
	static u32 i;
// 	u16 k = 0xff;
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
					addMuContainer();
					break;
			case CMD_CUMU_SWITCH:
					dwhp83CuAck(CMD_CUMU_SWITCH);
					sysDelayMs(50);
					dwhp83CuAck(CMD_CUMU_SWITCH);
					sysDelayMs(50);
					changetoWirelessSlave();
					break;
			case CMD_VLM_CHANGE:
					dwhp83CuAck(CMD_VLM_CHANGE);
					if(dwhp83_receive_msg.playload[0] == VOLUME_STREO)
						break;
					if(dwhp83_receive_msg.playload[0] == dwhp83_type){
						amp_volume = dwhp83_receive_msg.playload[1];
						ampSetVolume(amp_volume);
					}
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.count[i] != 0 &&
						mu_container.addr[i] != dwhp83_receive_msg.mu_addr &&
						mu_container.type[i] == dwhp83_receive_msg.playload[0]){
							mu_container.event[i] = CMD_VLM_CHANGE;
							mu_container.playload[i] = dwhp83_receive_msg.playload[1];
							DWPH83VolumeChange(dwhp83_receive_msg.playload[0],dwhp83_receive_msg.playload[1],mu_container.addr[i]);
						}
					break;
			case CMD_POWER_ON:
					dwhp83CuAck(CMD_POWER_ON);
					for(i = 0; i < MU_COUNT; i++)
						if(mu_container.count[i] != 0  
						   &&mu_container.addr[i] != dwhp83_receive_msg.mu_addr){
							mu_container.status[i] = CMD_POWER_ON;
							DWHP83SendCmd(CMD_POWER_ON, 0, mu_container.addr[i]);
						}
					if(dev_state == DEV_STANDBY)
						outStandby();
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

				DWHP83SendCmd(0x80|CMD_VLM_CHANGE, 0, mu_addr);
				if(dev_input == WIRELESS)
				{
					if(dwhp83_receive_msg.playload[0] == dwhp83_type){
						amp_volume = dwhp83_receive_msg.playload[1];
						ampSetVolume(amp_volume);
					}
				}
				break;
			case CMD_POWER_ON:
				DWHP83SendCmd(0x80|CMD_POWER_ON, 0, mu_addr);
// 				if(dev_state == DEV_STANDBY)
// 					outStandby();
				break;

		//----------------------------------------		
			case CMD_CUMU_SWITCH |0x80:
				Dwhp83_flag = 0;
				Dwhp83Master();
// 				playMusic(1);
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
		readDwhp83(0x3e,10);
		exitGpioI2c();
		sum = 0;
		for(i = 1; i < 8; i++)
	{
			sum += dwhp83_buf[i]; 
		#if(FIXED_COCO_MODE)
		if((i == 4)&&(dwhp83_buf[0] == 0)) //ma 20150525    portium told SUB start  play music
					{
						if(dwhp83_buf[1]==dwhp83_buf[2]&&dwhp83_buf[2]==dwhp83_buf[3]&&dwhp83_buf[1]==0x99)  
						{
								DWP83CuCV();
						}
					}
		#endif	
	}
		if(sum != dwhp83_buf[8])
			return;
		strncpy(&dwhp83_buf[1], (U8*)&dwhp83_receive_msg.mu_addr, 8);
		if(Dwhp83_CuMu == MU){
			//Dwhp83_route_count = 0;
			doMuMsg();
		}else{
			doCuMsg();
		}
}


Timer_ID dwph83_id = TASK_FULL;
void DWP83SpiritTask()
{
// 	u32 i;
	static u32 ck = 0;
	
	Dwhp83CuMuDetect();
	if(ack_error == 0){
		breath_flag = 0xff;
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
			if(dev_state != DEV_STANDBY){
				if(dev_input == BLUETOOTH && state_color != LIGHT_BLUE_COLOR)
					breath_flag = 1;
				else if(dev_input == LINEIN)
					breath_flag = 5;
				else
					breath_flag = 0xff;
			}
		}
		
	}else{
			Dwhp83_route_count = 10;
		}
	
	startATask(dwph83_id, 1500, 0, DWP83SpiritTask);
}


void  DWP83CuCV()
{ 
	u32 i;

	if(dwhp83_type == VOLUME_STREO) 
		return;
	
	for(i = 0; i < MU_COUNT; i++)
		if(mu_container.count[i] != 0 && mu_container.type[i] == dwhp83_type){
			mu_container.event[i] = CMD_VLM_CHANGE;
			mu_container.playload[i] = amp_volume;
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



void DWPH83Init()
{
	u32 i = 0;
	
	dwph83_id = getATask();
	Dwhp83_route_count = 0;

	for(i = 0; i < 10; i++){
		mu_container.addr[i] = 0;
		mu_container.count[i] = 5;
	}

	coco[0] = coco[1] = coco[2] = 0xff;
	#if(FIXED_COCO_MODE)
	coco[0] = 0x50;
	coco[1] = 0x72;
	coco[2] = 0x31;     // ma 20150521   set fixed  coco 
	#endif
//	GPIO_SET_PIN(PC5);
//	GPIO_SET_PIN(PC4);
//	GPIO_SET_MODE(PC,5,GPIO_OUTPUT);
//	GPIO_SET_MODE(PC,4,GPIO_OUTPUT);
	startATask(dwph83_id, 1000, 0, DWP83SpiritTask);
}




u32 volatile debug_dwhp83 = 0;
void dwhp83TestProcess()
{
	while(1){
		switch(debug_dwhp83){
			case 1:
				Dwhp83Master();
// #if(FIXED_COCO_MODE)
				cumu_flag = 1;   //ma 20150525
				setConfig();   //ma 20150525
// #endif			
				break;
			case 2:
				Dwhp83Slave();
// #if(FIXED_COCO_MODE)
				cumu_flag = 0;   //ma 20150525
				setConfig();   //ma 20150525
// #endif
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






