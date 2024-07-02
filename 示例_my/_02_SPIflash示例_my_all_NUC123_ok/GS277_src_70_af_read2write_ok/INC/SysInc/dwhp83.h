#ifndef __dwhp83_h
#define __dwhp83_h

#define CU		0X55AA
#define MU		0XAA55

#define VOLUME_STREO		0XA0
#define VOLUME_L			0XA1
#define VOLUME_R			0XA2
#define VOLUME_C			0XA3
#define VOLUME_SBAR		0XA4	
#define VOLUME_SUB			0XA5

extern u8  WL_COCO_KEY1;
extern u8  WL_COCO_KEY2;
extern u8  WL_COCO_KEY3;
extern u8	 WL_COCO_KEY4;   //ma 20150427

extern  u8  WL_COCO_KEY;     //ma 20150424
typedef struct {
	u8 link;
	u8 pairing;
	u8 init;
	u8 paired; 
}DWHP83_STATE;

typedef __packed struct {
	u8 msg_head[4];
	u8 msg_cmd[3];
	u16 mu_addr;
	u16 count;
	u8 sub_cmd;
	u8 playload[3];
}DWHP83_SMSG;


typedef __packed struct {
	u16 mu_addr;
	u16 count;
	u8 sub_cmd;
	u8 playload[3];
}DWHP83_RMSG;

extern volatile u32 Dwhp83_CuMu;
extern u8 dwhp83_type;
extern void writeDwhp83(volatile u8 *buf, volatile u8 address, u32 count);    //ma 20150624   add volatile
extern void DWHP83toEXT_MCU(void);         //ma 20150512   add mcu to ext_mcu send message  function
extern void DWHP83toSUB(void);         //ma 20150512   add local DWHP83 send message to SUB   function


extern void DWHP83_enable_sb(void);       //ma 20150513   tell SUB  enable S/B mode
extern void DWHP83_disable_sb(void);       //ma 20150513   tell SUB disable S/B mode
extern void change_to_locol_coco(void);    //ma 20150513   SB mode  tell SUB  chaneg to local  COCO
extern void MU_tell_CU_change(void);       //ma 20150514  local the-one want to be CU ,he tell the current CU change to MU 
extern void Dwhp83Slave(void);             // ma 20150514  declear MU
extern void Dwhp83Master(void);           // ma 20150514  declear CU
extern void DWP83CuPowerOn(void);             // ma 20150514 declear CU power on 
extern void DWP83MuPowerOn(void);            // ma 20150514 declear MU power on 
extern void  DWP83CuCV(void);         //ma 20150525 declare DWP83CuCV
extern void is_there_any_TX(void);       //ma 20150603 is_there_any_TX
extern void this_is_TX(void);       //ma 20150603 this_is_TX
extern u8 TX_flag;        //ma 20150603 add TX_flag to system
extern void SB_volume_trans(void);    //ma 20150604  add volume control  for soundbar the-one as RX
extern void  DWHP83Mute(void);         //ma 20150605  declare DWHP83Mute
extern void  DWHP83UnMute(void);         //ma 20150605  declare DWHP83UnMute
extern void SB_volume_mute(void);    //ma 20150604  add volume mute  for soundbar the-one as RX
// extern u8 RX_mute_flag;      //ma 20150606  add RX_mute_flag  for SOUNDBAR MODE VOLOME SYNC
extern void SB_volume_mute(void);    //ma 20150604  add volume mute  for soundbar the-one as RX
extern void SUB_volume_mute(void);    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
extern void SB_volume_unmute(void);    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
extern void SUB_volume_unmute(void);    //ma 20150608  add SUBvolume mute  for soundbar the-one as RX
extern u32 Dwhp83_flag ;      //ma 20150612   for CUMU commend

/*****add read version cmd 0x22*****/

extern void Dwhp83ReadVersion(void);   //ma 20150625 
extern void Dwhp83GetStatus(void);     //ma 20150629  declare  Dwhp83GetStatus
extern u32 isDwhp83Linked(void);       //ma 20150629   declare isDwhp83Linked
extern void DWP83CuCV0(void);     //ma 20150629  declare  DWP83CuCV0
extern void DWHP83DealMsg(void);     //ma 20150629  declare  DWHP83DealMsg
extern void DWPH83Init(void);     //ma 20150629  declare  DWPH83Init

extern u8 soundbar_coco_check(void);      //ma 20150703  add soundbar coco chenck  to decide 

extern void RX_SB_alive(void);             //ma 20150707   SB tell SUB or SB tell TX
extern void TX_SB_alive(void);             //ma 20150707  TX tell sub SB is here

extern u8 SB_link_flag;                 //ma 20150707   add sounbarlink flag 

extern void DWHP83toEXT_MCU(void);       //ma 20150512   add mcu to ext_mcu send message  function       //ma 20150707
extern void SB_volume_trans(void);    //ma 20150604  add volume control  for soundbar the-one as RX     //ma 20150707

extern void change_to_local_coco(void);    //ma 20150513   SB mode  tell SUB  chaneg to local  COCO

extern void set_local_coco(void);                             //ma 20150710  set auto local  coco change if no link sound bar
extern void set_soundbar_coco(void);                       //ma 20150710  set auto soundbar coco change if no link sound bar

extern void DWHP83SendCmd(u8 cmd, u8 playload, u16 addr);     //ma 20150716

extern void DWP83SpiritTask(void);              //ma 20150717
extern void changetoWirelessSlave(void);         //ma 20150803
extern void sb_volume(void);                   //ma 20150805  add sb volume control


extern u8 local_coco_check(void);                    //ma  20150806
extern void SB_rx_volume_2_SUB(void);        //MA 20150818  add  sb at rx mode send volume to SUB ,especially when SUB power on 
#endif

