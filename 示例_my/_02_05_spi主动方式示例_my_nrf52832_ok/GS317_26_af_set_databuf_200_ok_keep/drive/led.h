#ifndef __LED_H
#define	__LED_H

#include "nrf.h"

#define LED_START      16
#define LED_0          16
#define LED_1          17
#define LED_2          18
#define LED_3          19
#define LED_4          20
#define LED_5          21
#define LED_6          22
#define LED_7          23
#define LED_STOP       23

void LED_Init(void);
void LED_Open(int LED_X);
void LED_Close(int LED_X);
void LED_Toggle(int LED_X);
//void LED2_Open(void);
//void LED2_Close(void);
//void LED2_Toggle(void);



#endif /* __LED_H */

