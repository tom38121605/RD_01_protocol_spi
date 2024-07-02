#ifndef __deca_h
#define __deca_h
#include "typedef.h"

 
extern u8 ilocaladdr; 
extern u8 idestaddr; 
extern u8 idestaddr_poll; 
 
void dw1000_init(void); 
void dw1000_twr_measure(u8 idestaddr);
 

#endif
