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

#define MINI_LINK_CORE   0       //ma 20151130   1 for link CORE   0 for link  

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


extern u8 check_coco(void); //20150521
extern u8 cumu_flag;      //ma   20150525   add cumu flag
extern void Dwhp83FixedCoCo(void);     //ma 20150526
extern u32 Dwhp83CuMuDetect(void);     //ma 20150526
extern void Dwhp83EnterPairing(void);
extern void Dwhp83GetStatus(void);
extern u32 isDwhp83Linked(void);
extern void Dwhp83Master(void);
extern void DWHP83Mute(void);
extern void DWHP83UnMute(void);
extern void DWP83CuCV(void);
extern void DWPH83Init(void);
extern void Dwhp83Slave(void);
extern void DWHP83DealMsg(void);
extern void getMuAddr(void);
extern void DWP83MuPowerOn(void);
#endif

