#ifndef ws2812_h__
#define ws2812_h__
#include "sys.h"

//用户修改处
#define WS_TIMES 0//和你的芯片主频有关，stm32f407用6，自己多试下，或者有条件的用示波器看下 

//RGB--定义了几个标准的rgb值 
// 注意：颜色种类只和R，G，B的比例有关系，R G B的大小只影响亮暗程度
// 线太长会有压降，在线后补电压或者降低亮度
#define WS_DARK 	0,0,0
#define WS_WHITE 	40,40,40
#define WS_RED 		255,0,0
#define WS_GREEN 	0,255,0
#define WS_BLUE 	0,50,50
#define WS_YELLO 	255,255,0
#define WS_PURPLE   255,0,255
#define WS_CYAN 	11,0,137
#define ORANGE    50,10,0

#define WORK_OVER   if(Order==0x16){return;}

extern u8 ws_data_led[];
extern u8 ws_data_led_buf[];

void ws2812_init(void);
void ws2812_rgb(u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b);
void ws2812_rgb_all(u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b);
void ws2812_refresh(u16 ws_i,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin); 
void ws2812_rgb_ws_i(u16 i,u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b);
void ws2812_rgb_move(u16 i,u16 ws_i);
void ws2812_rgb_move_all(u16 ws_i);
void ws2812_reset(u16 wi_i);
void ws_data_change(void);
void ws_data_change_return(void);


void delay_250ns(void);
void send_0(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void send_1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void send_res(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


void led_flowing_init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times);
void led_Bright_out(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i);
void led_Panel_flowing(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u8 mode,u16 ws_i,u16 time);
void led_flowing(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times);
void led_flowing_init_three_GPIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times);
void  led_flowing_three_gpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times);
void ceshi(u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times);


void work_1(void);
void work_2(void);
void work_3(void);
void work_4(void);
void work_5_1(void);
void work_5(void);
void work_5_2(void);
void work_6(void);
void work_7(void);
void work_8(void);
void work_41(void);
void work_71(void);
void over(void);
#endif //ws2812_h__
