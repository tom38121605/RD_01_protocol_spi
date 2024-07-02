#include "ws2812.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"	
/**
 * @brief 数组中存放R、G、B数值
 * @para   一个灯需要R,G,B三种数据，所以1200大小的数组空间可放400个灯的数据
 */

u8 ws_data_led[1200]={0};
u8 ws_data_led_buf[1200]={0};


                                             /**
                                               * @brief  底层驱动
                                               * @para   发送0/1/复位+GPIO初始化
                                               */
/**
 * @brief GPIO初始化函数
 * @para  
 * GPIO口配置为推挽输出   
 *   
 */
void ws2812_init(void)
{
	
	/**
 * @brief GPIOA初始化函数
 * @para  
 *   14个引脚  
 *   Pin_0-----Pin_12 +  Pin_15
 */
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
	GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
//	GPIO_Pin_9|GPIO_Pin_10为串口
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
	GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	/**
 * @brief GPIOB初始化函数
 * @para  
 *   13个引脚  
 *   Pin_0-----Pin_1  +  Pin_3-----Pin_9  +  Pin_12 ----- Pin_15
 */
 
	GPIO_InitTypeDef  GPIO_InitStructure_B;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure_B.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
	GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	
	GPIO_InitStructure_B.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure_B.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure_B);	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
	GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	/**
 * @brief GPIOC初始化函数
 * @para  
 *   3个引脚  
 *   Pin_13-----Pin_15
 */
  GPIO_InitTypeDef  GPIO_InitStructure_C;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure_C.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	
	GPIO_InitStructure_C.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure_C.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure_C);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
}

/**
 * @brief 修改单个灯的RGB数值
 * @para
 * ws_i：第1个灯----第400个灯；
 * ws_r：R
 * ws_g:G
 * ws_b:B
 */

void ws2812_rgb(u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b)	    
{
  ws_data_led[(ws_i-1)*3]=ws_g;
	ws_data_led[(ws_i-1)*3+1]=ws_r;
  ws_data_led[(ws_i-1)*3+2]=ws_b; 
}
/**
 * @brief 将第i的值赋给第ws_i个灯
 * @para
 * ws_r：R
 * ws_g:G
 * ws_b:B
 */

void ws2812_rgb_move(u16 ws_i,u16 i)	    
{
	
	ws_data_led[(ws_i-1)*3]  =	ws_data_led[(i-1)*3]  ;
	ws_data_led[(ws_i-1)*3+1]=	ws_data_led[(i-1)*3+1];
  ws_data_led[(ws_i-1)*3+2]=  ws_data_led[(i-1)*3+2]; 

}
/**
 * @brief 将前ws_i个灯的数据右移一位
 * @para
 * ws_r：R
 * ws_g:G
 * ws_b:B
 */

void ws2812_rgb_move_all(u16 ws_i)	    
{
	u16 i;
	
	for(i=0;i<ws_i*3;i++){
	ws_data_led[(ws_i)*3-i]  =	ws_data_led[(ws_i)*3-i-1]  ;
	}
	for(i=0;i<ws_i*3;i++){
	ws_data_led[(ws_i)*3-i+1]  =	ws_data_led[(ws_i)*3-i-1+1]  ;
	}	
	for(i=0;i<ws_i*3;i++){
	ws_data_led[(ws_i)*3-i+2]  =	ws_data_led[(ws_i)*3-i-1+2]  ;
	}	
}

/**
 * @brief 修改多个灯的RGB数值(前ws_i个)
 * @para
 * ws_i：第0个灯----第ws_i个灯
 */

void ws2812_rgb_all(u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b)
{
	static u16 rgb_wsi;
	
	for(rgb_wsi=1;rgb_wsi<=ws_i;rgb_wsi++)
              {
              	ws_data_led[(rgb_wsi-1)*3]=ws_g;
              	ws_data_led[(rgb_wsi-1)*3+1]=ws_r;
              	ws_data_led[(rgb_wsi-1)*3+2]=ws_b;
              }
}

/**
 * @brief 将一个灯带的灯的RGB值全部归零
 * @para
 * ws_i：第0个灯----第ws_i个灯
 */

void ws2812_reset(u16 ws_i)       
{
	u16 i=0;	
           for (i=0;i<ws_i;i++)
           {
           	ws_data_led[i]=0;
           }
}
/**
 * @brief 修改灯带上从ws_i到i个灯的数值
 * @para
 * ws_i：第i个灯----第ws_i个灯
 */

void ws2812_rgb_ws_i(u16 i,u16 ws_i,u8 ws_r,u8 ws_g,u8 ws_b)       
{

	for(;i<=ws_i;i++)
              {
              	ws_data_led[(i-1)*3]=ws_g;
              	ws_data_led[(i-1)*3+1]=ws_r;
              	ws_data_led[(i-1)*3+2]=ws_b;
              }					 			 
}
/**
 * @brief发送前ws_i个LED数据
 * @para
 * ws_i：第0个灯----第ws_i个灯
 */

void ws2812_refresh(u16 ws_i,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)       
{
	u16 ws_ri=0;
	
			for(;ws_ri<ws_i*3;ws_ri++)
	{
		if((ws_data_led[ws_ri]&0x80)==0)	send_0(GPIOx,GPIO_Pin); else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x40)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x20)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x10)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x08)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x04)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x02)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
		if((ws_data_led[ws_ri]&0x01)==0)	send_0(GPIOx,GPIO_Pin);	else send_1(GPIOx,GPIO_Pin);
	}
	send_res(GPIOx,GPIO_Pin);
}
/**
 * @brief延时250ns
 * @para
 * 主频越低，WS_TIMES越低
 */
void delay_250ns(void)
{
	u8 del_t=WS_TIMES;
	while(del_t--)
		__NOP();
}
/**
 * @brief发送数据0
 * @para
 * 
 */
void send_0(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //拉高
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BSRR = GPIO_Pin;
	
  delay_250ns();
	  
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //拉低
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
  delay_250ns();
  delay_250ns();
	delay_250ns();
//  delay_250ns();
}
/**
 * @brief发送数据1
 * @para
 * 
 */
void send_1(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BSRR = GPIO_Pin;
	
    delay_250ns();
	  delay_250ns();
		delay_250ns();
//		delay_250ns();
	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //拉低
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
  delay_250ns();
}
/**
 * @brief复位发送
 * @para
 * 
 */
void send_res(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //拉低
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
 delay_us(300); 
}



/**
 * @brief 数组缓存函数
 * @para  
 * 把ws_data_led[]数组中的数据   放入  ws_data_led_buf[]中  
 * return:把ws_data_led_buf[]数组中的数据   放入   ws_data_led[]中  
 */
void ws_data_change()
{
	u16 i=0;
   for(;i<=1200;i++)
  {
    ws_data_led_buf[i]=ws_data_led[i];
	}
}
 
void ws_data_change_return()
{
	u16 i=0;
   for(;i<=1200;i++)
  {
   ws_data_led[i]=ws_data_led_buf[i];
	}
}
/**
                                               * @brief  功能函数
                                               * @para   八个动作
                                               * 跳出循环可以使用break语句
                                               * 跳出当前函数可以使用return语句
                                               */


/**
 * @brief 灯带流动功能函数：
 * @para 参数：GPIO口+颜色+总灯数+一组流水灯灯数+间隔+流动时间间隔(ms)
 * 注意：times=1，实现一次初始化流动
 *  times>1，实现循环流动
 */
void led_flowing_init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
	   
	    			 				     
		    WORK_OVER; //检测跳出
		
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
	    ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// 发送
	
      delay_ms(time);	

      for(i=1;i<=ws_i*times;i++){ 
				
       ws2812_rgb_move_all(i+i_zu-1);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			 }
			 
			 			 				     
		    WORK_OVER; //检测跳出
		
			 
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	     ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// 发送
			 
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //检测跳出
		
}
/**
 * @brief 灯带流动功能函数：                   三个FPIO口
 * @para 参数：GPIO口+颜色+总灯数+一组流水灯灯数+间隔+流动时间间隔(ms)
 * 注意：times=1，实现一次初始化流动
 *  times>1，实现循环流动
 */
void led_flowing_init_three_GPIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
				 				     
		    WORK_OVER; //检测跳出
		
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
	    ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// 发送
	    ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);	// 发送
	
      delay_ms(time);	

      for(i=1;i<=ws_i*times;i++){ 
				
       ws2812_rgb_move_all(i+i_zu-1);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			 }
			 
			 			 				     
		    WORK_OVER; //检测跳出
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	     ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// 发送
	     ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);	// 发送
			 
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //检测跳出
		
}
/**
 * @brief 灯带流动功能函数：
 * @para 参数：GPIO口+颜色+总灯数+一组流水灯灯数+间隔+流动时间间隔(ms)+执行次数
 * 
 * 
 */
void  led_flowing(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
      
			 				     
		    WORK_OVER; //检测跳出
		
			
	         for(i=1;i<=ws_i;i++){ 
				
       ws2812_rgb_move_all(i+i_zu-1);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			}		 
                      }
	    			 				     
		    WORK_OVER; //检测跳出
		
	
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
			ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);// 发送
			
      for(i=1;i<=ws_i*times;i++){ 
				
       ws2812_rgb_move_all(ws_i);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			 }
			 
			 			 				     
		    WORK_OVER; //检测跳出
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
			 ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second); // 发送
			 
			 
       delay_ms(time);		 
                      }
}
/**
 * @brief 灯带流动功能函数：三个GPIO
 * @para 参数：GPIO口+颜色+总灯数+一组流水灯灯数+间隔+流动时间间隔(ms)+执行次数
 * 
 * 
 */
void  led_flowing_three_gpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
				 				     
		    WORK_OVER; //检测跳出
		
	         for(i=1;i<=ws_i;i++){ 
				
       ws2812_rgb_move_all(i+i_zu-1);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			}		 
                      }
	    			 				     
		    WORK_OVER; //检测跳出
		
	
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
			ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);// 发送
			ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);// 发送
						 				     
		    WORK_OVER; //检测跳出
										
      for(i=1;i<=ws_i*times;i++){ 
				
       ws2812_rgb_move_all(ws_i);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			 }
			 			 				     
		    WORK_OVER; //检测跳出
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
			 ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second); // 发送
		  	ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);// 发送			 
			 			 				     
		    WORK_OVER; //检测跳出
		
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //检测跳出
		
}
/**
 * @brief 灯带常亮/灭功能函数：
 * @para 参数：GPIO口(两个)+颜色+总灯数  
 *  形参中有两个GPIO，方便灯板控制使用。
 *  使用灯带时候可以两个形参相同
 */
void led_Bright_out(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i)
{
      ws2812_rgb_all(ws_i,ws_r,ws_g,ws_b);
				 				     
		    WORK_OVER; //检测跳出
		
	    ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
		  ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);
				 				     
		    WORK_OVER; //检测跳出
		
}


/**
 * @brief 灯板流动功能函数：
 * @para 参数：GPIO口(两个)+颜色+方向+灯数+流动时长
 * mode=1;增加燃料
 * mode=2;减少燃料
 */
void led_Panel_flowing(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u8 mode,u16 ws_i,u16 time)
{
 u16 i;

 switch(mode){
  case(1):{
		
		
        ws2812_rgb_all(ws_i,WS_WHITE);
		
				ws2812_refresh(ws_i,GPIOx,GPIO_Pin);		
				ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);
		
		
		    for(;i<=ws_i;i++)
		{
			
		ws2812_rgb(i,ws_r,ws_g,ws_b);
		ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
		ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);
			
		delay_ms(time);	
		
//		if(Order==Stop){ break; }	        //停止指令
			
		}
	 break;}
  

  case(2):{
	  
      	ws2812_rgb_all(ws_i,ws_r,ws_g,ws_b);
				ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	
				ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);//删除	
	
			  for(;i<ws_i;i++)
		{
			
		ws2812_rgb(ws_i-i,WS_WHITE);  	  
		ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
		ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);//删除	
			
		delay_ms(time);	
		
//		if(Order==Stop){ break; }        //停止指令	    
			
		}
       break;
   }
  
 }
 

 
 
 
/**
                                               * @brief  工作程序
                                               * @para   
*/
 
 
}

/**
 * @brief 试车台简介
 * @para 
 *     初始状态： 4组罐--全蓝
 *                1组罐 -- 全白
 *         指令：      0x01
 *   全蓝 4组罐对应GPIOA,GPIO_Pin_0---7; 
 *   全白 1组罐对应GPIOC,GPIO_Pin_14，15
 */
  void work_1()
{
   	    ws2812_reset(1200);
			 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4组
				 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1组
}

/**
 * @brief 发动机上台
 * @para 
 *     工作内容：   打开碗灯
 *                  保持初始状态
 *         指令：   0x02
 *   全蓝 4组罐对应GPIOA,GPIO_Pin_0---7; 
 *   全白 1组罐对应GPIOC,GPIO_Pin_14，15
 */
  void work_2()
{
				 				     
		    WORK_OVER; //检测跳出
		
   	    ws2812_reset(1200);
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4组
				 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1组
	
	
}
/**
 * @brief 发动机安装
 * @para 
 *     工作内容：   无动作
 *         指令：   0x03
 *   全蓝 4组罐对应GPIOA,GPIO_Pin_0---7; 
 *   全白 1组罐对应GPIOC,GPIO_Pin_14，15
 */
  void work_3()
{
				 				     
		    WORK_OVER; //检测跳出
		
   	    ws2812_reset(1200);
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4组
				 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1组
}

/**
 * @brief 实验系统准备
 * @para 
 *     工作内容：  4组罐--下降
 *                 灯带---流动
 *                1组罐 -- 上升
 *        指令：  0x04      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 */
void work_4()
{
/**
 * @brief 初始液面状态
 * @para 
 *     工作内容：  4组罐--全蓝
 *                1组罐 -- 全白
 */	
			 				     
		    WORK_OVER; //检测跳出
		
   	    ws2812_reset(1200);
	
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4组
				 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1组

}
/**
 * @brief 实验系统准备
 * @para 
 *     工作内容：  4组罐--下降
 *                 灯带---流动
 *                1组罐 -- 上升
 *        指令：  0x41      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 */
void work_41()
{
	u8 paiqi_flag=0x43;
	
/**
 * @brief 初始液面状态
 * @para 
 *     工作内容：  4组罐--全蓝
 *                1组罐 -- 全白
 */	
			 				     
		    WORK_OVER; //检测跳出
		
   	    ws2812_reset(1200);
	
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4组
				 				     
		    WORK_OVER; //检测跳出
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,12); //1组

/**
 * @brief 灯带流动
 * @para 
 *     工作内容：  长短灯带同时流动
 *	              
 *                
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 *          修改灯带led_flowing（）；流动次数，可修改流动时长
 */	
   				 				     
		WORK_OVER; //检测跳出
		
		ws2812_reset(1200);	

					 				     
		    WORK_OVER; //检测跳出
		
		led_flowing_init(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//长灯带初始流动

		 usart1_1_byte(zheng_tou);
     usart1_1_byte(paiqi_flag);
     usart1_1_byte(zheng_wei_1);
     usart1_1_byte(zheng_wei_2); 	//发送排气数据***************
		 
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,1,WS_CYAN);	
	  ws2812_rgb_ws_i(2,12,WS_WHITE);
		
					 				     
		    WORK_OVER; //检测跳出
		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升1格
		
/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐 --下降一格
 *
 *                1组罐 -- 上升至两格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */			
	   	    ws2812_reset(1200);
	


	  ws2812_rgb_ws_i(1,21,WS_CYAN);
	  ws2812_rgb(22,WS_WHITE);
					 				     
		WORK_OVER; //检测跳出
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降一格
					 				     
		    WORK_OVER; //检测跳出
		

						 				     
		    WORK_OVER; //检测跳出
			
		ws2812_rgb_ws_i(1,2,WS_CYAN);
	  ws2812_rgb_ws_i(3,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升两格
				 				     
		    WORK_OVER; //检测跳出
		
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动
		
/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐--下降两格
 *
 *                1组罐 -- 上升至四格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */			

				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,3,WS_CYAN);
	  ws2812_rgb_ws_i(4,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升3格
			 				     
		    WORK_OVER; //检测跳出
		
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动
					 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,4,WS_CYAN);
	  ws2812_rgb_ws_i(5,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升四格
						 				     
		    WORK_OVER; //检测跳出
		
/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐--下降三格
 *
 *                1组罐 -- 上升至六格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */		

	   	    ws2812_reset(1200);
			 				     
		    WORK_OVER; //检测跳出
		
	  ws2812_rgb_ws_i(1,20,WS_CYAN);
	  ws2812_rgb_ws_i(21,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降两格
		
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升5格
		

  
		    WORK_OVER; //检测跳出
		
	  led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动
		
			 				     
		    WORK_OVER; //检测跳出
				
		ws2812_rgb_ws_i(1,6,WS_CYAN);
	  ws2812_rgb_ws_i(7,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升六格
				 				     
		    WORK_OVER; //检测跳出
		
	  led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动

		
/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐--下降四格
 *
 *                1组罐 -- 上升至八格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */			

//	  ws2812_reset(1200);
//			 				     
//		    WORK_OVER; //检测跳出
//		
//	  ws2812_rgb_ws_i(1,19,WS_CYAN);
//	  ws2812_rgb_ws_i(20,22,WS_WHITE);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降三格
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,7,WS_CYAN);
	  ws2812_rgb_ws_i(8,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升7格
			 				     
		    WORK_OVER; //检测跳出
				
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //检测跳出
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降三格		
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动



		    WORK_OVER; //检测跳出
				
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升八格
			 				     
		    WORK_OVER; //检测跳出

		
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动

/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐--下降五格
 *
 *                1组罐 -- 上升至十格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */			

//	  ws2812_reset(1200);
//	
//			 				     
//		    WORK_OVER; //检测跳出
//		

//	  ws2812_rgb_ws_i(1,17,WS_CYAN);
//	  ws2812_rgb_ws_i(18,22,WS_WHITE);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降五格
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升9格
			 				     
		    WORK_OVER; //检测跳出
		

			led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动
			 				     

				 				     
		
			 				     
		    WORK_OVER; //检测跳出
		
/**
 * @brief 液面变化
 * @para 
 *     工作内容：  4组罐--下降六格
 *
 *                1组罐 -- 上升至十一格
 *
 *    注意：长短灯带一组流动的灯数相同，以及间隔相同；
 */			

//	  ws2812_reset(1200);
//	

//			 				     
//		    WORK_OVER; //检测跳出
//		
//	  ws2812_rgb_ws_i(1,16,WS_CYAN);
//	  ws2812_rgb_ws_i(17,22,WS_WHITE);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降六格
			 				     
		    WORK_OVER; //检测跳出
				
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//灯带持续流动
			 				     
		    WORK_OVER; //检测跳出
				
//		ws2812_rgb_ws_i(1,12,WS_CYAN);
//    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
//    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升十一格
//			 				     
//		    WORK_OVER; //检测跳出
			

//	ws2812_rgb_ws_i(1,15,WS_CYAN);
//	ws2812_rgb_ws_i(16,22,WS_WHITE);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//	ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4组下降七格
			 				     
		    WORK_OVER; //检测跳出
		}


/**
 * @brief 产品遇冷及抽真空
 * @para 
 *     工作内容：  短灯带---流动
 *                1组罐 -- 下降一格
 *        指令：  0x05      
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条短灯带 对应 GPIOB,GPIO_Pin_12
 */
void work_5()
{
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //检测跳出
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //上一个状态
				 				     
		    WORK_OVER; //检测跳出
		
		    WORK_OVER; //检测跳出
					
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
		
		    WORK_OVER; //检测跳出		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1组上升十格
}


/**
 * @brief 产品遇冷及抽真空
 * @para 
 *     工作内容：  短灯带---流动
 *                1组罐 -- 下降一格
 *        指令：  0x51      
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条短灯带 对应 GPIOB,GPIO_Pin_12
 */
void work_5_1()
{
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //检测跳出
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //上一个状态
	
		    WORK_OVER; //检测跳出
					
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
		
		    WORK_OVER; //检测跳出		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //上一个状态
		
				 				     
		    WORK_OVER; //检测跳出
		 
	  led_flowing_init(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,50,5,5,33,2);	//短灯带初始流动
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //检测跳出
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //1组下降一格	 


   led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_DARK,61);//关闭一组管短灯带	
}
/**
 * @brief 实验系统准备
 * @para 
 *     工作内容：  4组罐--下降两格
 *                 长、短灯带---常亮
 *        指令：  0x05      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 */
void work_5_2()
{
				 				     
		    WORK_OVER; //检测跳出
		
    led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_DARK,61);//关闭一组管短灯带
	
				 				     
		    WORK_OVER; //检测跳出
		
			led_flowing_init(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//长,短灯带初始流动
				 				     
		    WORK_OVER; //检测跳出
		
		  ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
				 				     
		    WORK_OVER; //检测跳出
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组下降一格
		
					 				     
		    WORK_OVER; //检测跳出
		
	
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//灯带持续流动
					 				     
		    WORK_OVER; //检测跳出
	
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//长短灯带常量
  			 				     
		    WORK_OVER; //检测跳出
				
		led_flowing(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//灯带持续流动		

//			  ws2812_rgb_ws_i(1,17,WS_CYAN);
//	  ws2812_rgb_ws_i(18,22,WS_WHITE);
//					 				     
//		    WORK_OVER; //检测跳出
//		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组下降两格
//	 			 				     
//		    WORK_OVER; //检测跳出
		
}
/**
 * @brief 开车条件判读
 * @para 
 *     工作内容：  4组罐--保持上一个状态
 *                 长、短灯带---常亮
 *        指令：  0x06      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 */
void work_6()
{
	
	
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //检测跳出
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐上一个状态	 
				 				     
		    WORK_OVER; //检测跳出
		

	
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//长短灯带常量
  
				 				     
		    WORK_OVER; //检测跳出
		
		
		  ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
				 				     
		    WORK_OVER; //检测跳出
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //四组罐上一个状态	
	 			 				     
		    WORK_OVER; //检测跳出
		
}


/**
 * @brief 试车
 * @para 
 *     工作内容：  4组罐--持续下降
 *                1组罐--微微下降
 *                 长、短灯带---流水
 *        指令：  0x07      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 *
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条短灯带 对应 GPIOB,GPIO_Pin_12
 */
void work_7()
{
	
				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //检测跳出
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐上一个状态	 
	
			 				     
		    WORK_OVER; //检测跳出
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//长短灯带常量
  
				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
    			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐上一个状态
			 				     
		    WORK_OVER; //检测跳出
			
 }
/**
 * @brief 试车
 * @para 
 *     工作内容：  4组罐--持续下降
 *                1组罐--微微下降
 *                 长、短灯带---流水
 *        指令：  0x71      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 *
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条短灯带 对应 GPIOB,GPIO_Pin_12
 */
void work_71()
{
	
	
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //检测跳出
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐上一个状态	 
	
			 				     
		    WORK_OVER; //检测跳出
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//长短灯带常量
  
				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
    			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐上一个状态
			 				     
		    WORK_OVER; //检测跳出
		
			 				     
		    WORK_OVER; //检测跳出
		
		led_flowing_init_three_GPIO(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//灯带持续流动
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,7,WS_CYAN);
	  ws2812_rgb_ws_i(8,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐下降一格 
				 				     
		    WORK_OVER; //检测跳出
		
	
			  ws2812_rgb_ws_i(1,17,WS_CYAN);
	  ws2812_rgb_ws_i(18,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降一格 	 
					 				     
		    WORK_OVER; //检测跳出
		
		
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//灯带持续流动	
					 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,6,WS_CYAN);
	  ws2812_rgb_ws_i(7,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐下降两格 	 
	
				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,16,WS_CYAN);
	  ws2812_rgb_ws_i(17,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降两格
					 				     
		    WORK_OVER; //检测跳出
			
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//灯带持续流动	
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐下降三格 	 
	
				 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,15,WS_CYAN);
	  ws2812_rgb_ws_i(16,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降三格
					 				     
		    WORK_OVER; //检测跳出
			
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//灯带持续流动		
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,14,WS_CYAN);
	  ws2812_rgb_ws_i(15,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降四格
			 				  
		
			 				     
		    WORK_OVER; //检测跳出
 
 led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//灯带持续流动		
			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,13,WS_CYAN);
	  ws2812_rgb_ws_i(14,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降四格
			 				  
		
			 				     
		    WORK_OVER; //检测跳出		
}
/**
 * @brief 试后处理与数据交付
 * @para 
 *     工作内容：  4组罐--保持上一个状态
 *                1组罐--保持上一个状态
 *                 长、短灯带---常亮
 *        指令：  0x08      
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    一条长灯带 对应 GPIOC,GPIO_Pin_13
 *    三条短灯带 对应 GPIOB,GPIO_Pin_13 
 *
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    一条短灯带 对应 GPIOB,GPIO_Pin_12
 */
void work_8()
 {
	 		    WORK_OVER; //检测跳出
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,11,WS_WHITE);
	 		    WORK_OVER; //检测跳出
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //一组罐下降三格	 	
	 
		    WORK_OVER; //检测跳出
		
 		ws2812_rgb_ws_i(1,13,WS_CYAN);
	  ws2812_rgb_ws_i(14,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降四格
			 				     
		    WORK_OVER; //检测跳出
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//四组长短灯带常量	  
    led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,61);//一组短灯带常量
	 			 				     
		    WORK_OVER; //检测跳出
		
		ws2812_rgb_ws_i(1,12,WS_CYAN);
	  ws2812_rgb_ws_i(13,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //四组罐下降五格
 			 				     
		    WORK_OVER; //检测跳出
		
  
 }
 
 /**
 * @brief 全关
 * @para 
 *     工作内容： 
 *    4组罐 对应 GPIOA,GPIO_Pin_0---7; 
 *    1组罐 对应 GPIOC,GPIO_Pin_14，15
 *    长灯带 对应 GPIOC,GPIO_Pin_13
 *    短灯带 对应 GPIOB,GPIO_Pin_12                  
 *        指令：  0x16     
 */
void over()
{
			 ws2812_reset(1300);

	     ws2812_refresh(1300,GPIOA, GPIO_Pin_0);     
	     ws2812_refresh(1300,GPIOA, GPIO_Pin_1); 
	     ws2812_refresh(1300,GPIOA, GPIO_Pin_2); 
	     ws2812_refresh(1300,GPIOA, GPIO_Pin_3); 
		   ws2812_refresh(1300,GPIOA, GPIO_Pin_4); 
	     ws2812_refresh(1300,GPIOA, GPIO_Pin_5);
 		   ws2812_refresh(1300,GPIOA, GPIO_Pin_6); 
	     ws2812_refresh(1300,GPIOA, GPIO_Pin_7);
	
	     ws2812_refresh(1300,GPIOC, GPIO_Pin_14); 
	     ws2812_refresh(1300,GPIOC, GPIO_Pin_15);

	     ws2812_refresh(1300,GPIOC,GPIO_Pin_13);     
	     ws2812_refresh(1300,GPIOB, GPIO_Pin_13); 	
	     ws2812_refresh(1300,GPIOB, GPIO_Pin_12); 
}

 /**
 * @brief 测试
 * @para 
 *     工作内容：所有灯带都跑流水
 */
void ceshi(u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
	   
	    			 				     
		    WORK_OVER; //检测跳出
		

      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_0);	
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);	
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_2);	
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_3);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_4);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_5);
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_6);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_7);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_0);	
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_1);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_10);  
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_11);	// 发送	
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_12);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_13);
	    ws2812_refresh(ws_i,GPIOC,GPIO_Pin_15);
      ws2812_refresh(ws_i,GPIOC,GPIO_Pin_14);
	    ws2812_refresh(ws_i,GPIOC,GPIO_Pin_13);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_9);
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_8);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_7);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_6);
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_5);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_4);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_3);
			
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_14);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_15);
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_8);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_11);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_12);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_15);
      delay_ms(time);	

      for(i=1;i<=ws_i*times;i++){ 
				
       ws2812_rgb_move_all(i+i_zu-1);
       flag++; 
				if(flag<=gap)
			 {
				ws2812_rgb(1,R,G,B);
			 }else
			 {
				if(flag==2*gap){flag=0;}
				ws2812_rgb(1,ws_r,ws_g,ws_b);			 
			 }
			 
			 			 				     
		    WORK_OVER; //检测跳出
		
			 

      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_0);	
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);	
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_2);	
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_3);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_4);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_5);
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_6);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_7);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_0);	
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_1);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_10);  
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_11);	// 发送	
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_12);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_13);
	    ws2812_refresh(ws_i,GPIOC,GPIO_Pin_15);
      ws2812_refresh(ws_i,GPIOC,GPIO_Pin_14);
	    ws2812_refresh(ws_i,GPIOC,GPIO_Pin_13);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_9);
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_8);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_7);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_6);
      ws2812_refresh(ws_i,GPIOB,GPIO_Pin_5);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_4);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_3);
			
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_14);
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_15);
      ws2812_refresh(ws_i,GPIOA,GPIO_Pin_8);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_11);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_12);
	    ws2812_refresh(ws_i,GPIOA,GPIO_Pin_15);			 
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //检测跳出
		
}

