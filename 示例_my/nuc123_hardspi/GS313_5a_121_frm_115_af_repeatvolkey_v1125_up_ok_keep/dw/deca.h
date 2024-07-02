#ifndef __deca_h
#define __deca_h
#include "typedef.h"

 
extern u8 ilocaladdr; 
extern u8 idestaddr; 
extern u8 idestaddr_poll; 

extern u8 flg_volchg;
extern u8 ivol_adjust;

extern u8 flg_powerchg;
extern u8 ipowerctrl;

 
void dw1000_init(void); 
void dw1000_twr_measure(u8 idestaddr);
 

#endif
