#include "ws2812.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "usart.h"	
/**
 * @brief �����д��R��G��B��ֵ
 * @para   һ������ҪR,G,B�������ݣ�����1200��С������ռ�ɷ�400���Ƶ�����
 */

u8 ws_data_led[1200]={0};
u8 ws_data_led_buf[1200]={0};


                                             /**
                                               * @brief  �ײ�����
                                               * @para   ����0/1/��λ+GPIO��ʼ��
                                               */
/**
 * @brief GPIO��ʼ������
 * @para  
 * GPIO������Ϊ�������   
 *   
 */
void ws2812_init(void)
{
	
	/**
 * @brief GPIOA��ʼ������
 * @para  
 *   14������  
 *   Pin_0-----Pin_12 +  Pin_15
 */
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
	GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
//	GPIO_Pin_9|GPIO_Pin_10Ϊ����
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
	GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	/**
 * @brief GPIOB��ʼ������
 * @para  
 *   13������  
 *   Pin_0-----Pin_1  +  Pin_3-----Pin_9  +  Pin_12 ----- Pin_15
 */
 
	GPIO_InitTypeDef  GPIO_InitStructure_B;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure_B.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
	GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	
	GPIO_InitStructure_B.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure_B.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure_B);	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
	GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	/**
 * @brief GPIOC��ʼ������
 * @para  
 *   3������  
 *   Pin_13-----Pin_15
 */
  GPIO_InitTypeDef  GPIO_InitStructure_C;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure_C.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	
	GPIO_InitStructure_C.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure_C.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure_C);	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
}

/**
 * @brief �޸ĵ����Ƶ�RGB��ֵ
 * @para
 * ws_i����1����----��400���ƣ�
 * ws_r��R
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
 * @brief ����i��ֵ������ws_i����
 * @para
 * ws_r��R
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
 * @brief ��ǰws_i���Ƶ���������һλ
 * @para
 * ws_r��R
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
 * @brief �޸Ķ���Ƶ�RGB��ֵ(ǰws_i��)
 * @para
 * ws_i����0����----��ws_i����
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
 * @brief ��һ���ƴ��ĵƵ�RGBֵȫ������
 * @para
 * ws_i����0����----��ws_i����
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
 * @brief �޸ĵƴ��ϴ�ws_i��i���Ƶ���ֵ
 * @para
 * ws_i����i����----��ws_i����
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
 * @brief����ǰws_i��LED����
 * @para
 * ws_i����0����----��ws_i����
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
 * @brief��ʱ250ns
 * @para
 * ��ƵԽ�ͣ�WS_TIMESԽ��
 */
void delay_250ns(void)
{
	u8 del_t=WS_TIMES;
	while(del_t--)
		__NOP();
}
/**
 * @brief��������0
 * @para
 * 
 */
void send_0(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //����
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BSRR = GPIO_Pin;
	
  delay_250ns();
	  
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //����
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
  delay_250ns();
  delay_250ns();
	delay_250ns();
//  delay_250ns();
}
/**
 * @brief��������1
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
	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //����
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
  delay_250ns();
}
/**
 * @brief��λ����
 * @para
 * 
 */
void send_res(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));   //����
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  GPIOx->BRR = GPIO_Pin;
	
 delay_us(300); 
}



/**
 * @brief ���黺�溯��
 * @para  
 * ��ws_data_led[]�����е�����   ����  ws_data_led_buf[]��  
 * return:��ws_data_led_buf[]�����е�����   ����   ws_data_led[]��  
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
                                               * @brief  ���ܺ���
                                               * @para   �˸�����
                                               * ����ѭ������ʹ��break���
                                               * ������ǰ��������ʹ��return���
                                               */


/**
 * @brief �ƴ��������ܺ�����
 * @para ������GPIO��+��ɫ+�ܵ���+һ����ˮ�Ƶ���+���+����ʱ����(ms)
 * ע�⣺times=1��ʵ��һ�γ�ʼ������
 *  times>1��ʵ��ѭ������
 */
void led_flowing_init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
	   
	    			 				     
		    WORK_OVER; //�������
		
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
	    ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// ����
	
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
			 
			 			 				     
		    WORK_OVER; //�������
		
			 
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	     ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// ����
			 
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //�������
		
}
/**
 * @brief �ƴ��������ܺ�����                   ����FPIO��
 * @para ������GPIO��+��ɫ+�ܵ���+һ����ˮ�Ƶ���+���+����ʱ����(ms)
 * ע�⣺times=1��ʵ��һ�γ�ʼ������
 *  times>1��ʵ��ѭ������
 */
void led_flowing_init_three_GPIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
				 				     
		    WORK_OVER; //�������
		
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
	    ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// ����
	    ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);	// ����
	
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
			 
			 			 				     
		    WORK_OVER; //�������
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	     ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);	// ����
	     ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);	// ����
			 
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //�������
		
}
/**
 * @brief �ƴ��������ܺ�����
 * @para ������GPIO��+��ɫ+�ܵ���+һ����ˮ�Ƶ���+���+����ʱ����(ms)+ִ�д���
 * 
 * 
 */
void  led_flowing(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
      
			 				     
		    WORK_OVER; //�������
		
			
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
	    			 				     
		    WORK_OVER; //�������
		
	
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
			ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);// ����
			
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
			 
			 			 				     
		    WORK_OVER; //�������
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
			 ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second); // ����
			 
			 
       delay_ms(time);		 
                      }
}
/**
 * @brief �ƴ��������ܺ���������GPIO
 * @para ������GPIO��+��ɫ+�ܵ���+һ����ˮ�Ƶ���+���+����ʱ����(ms)+ִ�д���
 * 
 * 
 */
void  led_flowing_three_gpio(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,GPIO_TypeDef* GPIOx_three,uint16_t GPIO_Pin_three,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
				 				     
		    WORK_OVER; //�������
		
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
	    			 				     
		    WORK_OVER; //�������
		
	
      ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
			ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);// ����
			ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);// ����
						 				     
		    WORK_OVER; //�������
										
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
			 			 				     
		    WORK_OVER; //�������
		
       ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
			 ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second); // ����
		  	ws2812_refresh(ws_i,GPIOx_three,GPIO_Pin_three);// ����			 
			 			 				     
		    WORK_OVER; //�������
		
       delay_ms(time);		 
                      }
						 				     
		    WORK_OVER; //�������
		
}
/**
 * @brief �ƴ�����/���ܺ�����
 * @para ������GPIO��(����)+��ɫ+�ܵ���  
 *  �β���������GPIO������ư����ʹ�á�
 *  ʹ�õƴ�ʱ����������β���ͬ
 */
void led_Bright_out(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin,GPIO_TypeDef* GPIOx_second,uint16_t GPIO_Pin_second,u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i)
{
      ws2812_rgb_all(ws_i,ws_r,ws_g,ws_b);
				 				     
		    WORK_OVER; //�������
		
	    ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
		  ws2812_refresh(ws_i,GPIOx_second,GPIO_Pin_second);
				 				     
		    WORK_OVER; //�������
		
}


/**
 * @brief �ư��������ܺ�����
 * @para ������GPIO��(����)+��ɫ+����+����+����ʱ��
 * mode=1;����ȼ��
 * mode=2;����ȼ��
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
		
//		if(Order==Stop){ break; }	        //ָֹͣ��
			
		}
	 break;}
  

  case(2):{
	  
      	ws2812_rgb_all(ws_i,ws_r,ws_g,ws_b);
				ws2812_refresh(ws_i,GPIOx,GPIO_Pin);
	
				ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);//ɾ��	
	
			  for(;i<ws_i;i++)
		{
			
		ws2812_rgb(ws_i-i,WS_WHITE);  	  
		ws2812_refresh(ws_i,GPIOx,GPIO_Pin);	
		ws2812_refresh(ws_i,GPIOA,GPIO_Pin_1);//ɾ��	
			
		delay_ms(time);	
		
//		if(Order==Stop){ break; }        //ָֹͣ��	    
			
		}
       break;
   }
  
 }
 

 
 
 
/**
                                               * @brief  ��������
                                               * @para   
*/
 
 
}

/**
 * @brief �Գ�̨���
 * @para 
 *     ��ʼ״̬�� 4���--ȫ��
 *                1��� -- ȫ��
 *         ָ�      0x01
 *   ȫ�� 4��޶�ӦGPIOA,GPIO_Pin_0---7; 
 *   ȫ�� 1��޶�ӦGPIOC,GPIO_Pin_14��15
 */
  void work_1()
{
   	    ws2812_reset(1200);
			 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4��
				 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1��
}

/**
 * @brief ��������̨
 * @para 
 *     �������ݣ�   �����
 *                  ���ֳ�ʼ״̬
 *         ָ�   0x02
 *   ȫ�� 4��޶�ӦGPIOA,GPIO_Pin_0---7; 
 *   ȫ�� 1��޶�ӦGPIOC,GPIO_Pin_14��15
 */
  void work_2()
{
				 				     
		    WORK_OVER; //�������
		
   	    ws2812_reset(1200);
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4��
				 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1��
	
	
}
/**
 * @brief ��������װ
 * @para 
 *     �������ݣ�   �޶���
 *         ָ�   0x03
 *   ȫ�� 4��޶�ӦGPIOA,GPIO_Pin_0---7; 
 *   ȫ�� 1��޶�ӦGPIOC,GPIO_Pin_14��15
 */
  void work_3()
{
				 				     
		    WORK_OVER; //�������
		
   	    ws2812_reset(1200);
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4��
				 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1��
}

/**
 * @brief ʵ��ϵͳ׼��
 * @para 
 *     �������ݣ�  4���--�½�
 *                 �ƴ�---����
 *                1��� -- ����
 *        ָ�  0x04      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 */
void work_4()
{
/**
 * @brief ��ʼҺ��״̬
 * @para 
 *     �������ݣ�  4���--ȫ��
 *                1��� -- ȫ��
 */	
			 				     
		    WORK_OVER; //�������
		
   	    ws2812_reset(1200);
	
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4��
				 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,11); //1��

}
/**
 * @brief ʵ��ϵͳ׼��
 * @para 
 *     �������ݣ�  4���--�½�
 *                 �ƴ�---����
 *                1��� -- ����
 *        ָ�  0x41      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 */
void work_41()
{
	u8 paiqi_flag=0x43;
	
/**
 * @brief ��ʼҺ��״̬
 * @para 
 *     �������ݣ�  4���--ȫ��
 *                1��� -- ȫ��
 */	
			 				     
		    WORK_OVER; //�������
		
   	    ws2812_reset(1200);
	
	led_Bright_out(GPIOA,GPIO_Pin_0,GPIOA,GPIO_Pin_1,WS_CYAN,22);
	led_Bright_out(GPIOA,GPIO_Pin_2,GPIOA,GPIO_Pin_3,WS_CYAN,22); 	
	led_Bright_out(GPIOA,GPIO_Pin_4,GPIOA,GPIO_Pin_5,WS_CYAN,22);   
	led_Bright_out(GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_7,WS_CYAN,22); //4��
				 				     
		    WORK_OVER; //�������
		
	led_Bright_out(GPIOC,GPIO_Pin_14,GPIOC,GPIO_Pin_15,WS_WHITE,12); //1��

/**
 * @brief �ƴ�����
 * @para 
 *     �������ݣ�  ���̵ƴ�ͬʱ����
 *	              
 *                
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 *          �޸ĵƴ�led_flowing�������������������޸�����ʱ��
 */	
   				 				     
		WORK_OVER; //�������
		
		ws2812_reset(1200);	

					 				     
		    WORK_OVER; //�������
		
		led_flowing_init(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//���ƴ���ʼ����

		 usart1_1_byte(zheng_tou);
     usart1_1_byte(paiqi_flag);
     usart1_1_byte(zheng_wei_1);
     usart1_1_byte(zheng_wei_2); 	//������������***************
		 
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,1,WS_CYAN);	
	  ws2812_rgb_ws_i(2,12,WS_WHITE);
		
					 				     
		    WORK_OVER; //�������
		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������1��
		
/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4��� --�½�һ��
 *
 *                1��� -- ����������
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */			
	   	    ws2812_reset(1200);
	


	  ws2812_rgb_ws_i(1,21,WS_CYAN);
	  ws2812_rgb(22,WS_WHITE);
					 				     
		WORK_OVER; //�������
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½�һ��
					 				     
		    WORK_OVER; //�������
		

						 				     
		    WORK_OVER; //�������
			
		ws2812_rgb_ws_i(1,2,WS_CYAN);
	  ws2812_rgb_ws_i(3,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1����������
				 				     
		    WORK_OVER; //�������
		
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������
		
/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4���--�½�����
 *
 *                1��� -- �������ĸ�
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */			

				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,3,WS_CYAN);
	  ws2812_rgb_ws_i(4,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������3��
			 				     
		    WORK_OVER; //�������
		
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������
					 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,4,WS_CYAN);
	  ws2812_rgb_ws_i(5,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1�������ĸ�
						 				     
		    WORK_OVER; //�������
		
/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4���--�½�����
 *
 *                1��� -- ����������
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */		

	   	    ws2812_reset(1200);
			 				     
		    WORK_OVER; //�������
		
	  ws2812_rgb_ws_i(1,20,WS_CYAN);
	  ws2812_rgb_ws_i(21,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½�����
		
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������5��
		

  
		    WORK_OVER; //�������
		
	  led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������
		
			 				     
		    WORK_OVER; //�������
				
		ws2812_rgb_ws_i(1,6,WS_CYAN);
	  ws2812_rgb_ws_i(7,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1����������
				 				     
		    WORK_OVER; //�������
		
	  led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������

		
/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4���--�½��ĸ�
 *
 *                1��� -- �������˸�
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */			

//	  ws2812_reset(1200);
//			 				     
//		    WORK_OVER; //�������
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
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½�����
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,7,WS_CYAN);
	  ws2812_rgb_ws_i(8,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������7��
			 				     
		    WORK_OVER; //�������
				
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //�������
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½�����		
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������



		    WORK_OVER; //�������
				
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1�������˸�
			 				     
		    WORK_OVER; //�������

		
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������

/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4���--�½����
 *
 *                1��� -- ������ʮ��
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */			

//	  ws2812_reset(1200);
//	
//			 				     
//		    WORK_OVER; //�������
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
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½����
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������9��
			 				     
		    WORK_OVER; //�������
		

			led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������
			 				     

				 				     
		
			 				     
		    WORK_OVER; //�������
		
/**
 * @brief Һ��仯
 * @para 
 *     �������ݣ�  4���--�½�����
 *
 *                1��� -- ������ʮһ��
 *
 *    ע�⣺���̵ƴ�һ�������ĵ�����ͬ���Լ������ͬ��
 */			

//	  ws2812_reset(1200);
//	

//			 				     
//		    WORK_OVER; //�������
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
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½�����
			 				     
		    WORK_OVER; //�������
				
				led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,100,5,5,33,1);	//�ƴ���������
			 				     
		    WORK_OVER; //�������
				
//		ws2812_rgb_ws_i(1,12,WS_CYAN);
//    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
//    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������ʮһ��
//			 				     
//		    WORK_OVER; //�������
			

//	ws2812_rgb_ws_i(1,15,WS_CYAN);
//	ws2812_rgb_ws_i(16,22,WS_WHITE);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//	ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//	ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//	ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //4���½��߸�
			 				     
		    WORK_OVER; //�������
		}


/**
 * @brief ��Ʒ���估�����
 * @para 
 *     �������ݣ�  �̵ƴ�---����
 *                1��� -- �½�һ��
 *        ָ�  0x05      
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ���̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12
 */
void work_5()
{
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //�������
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //��һ��״̬
				 				     
		    WORK_OVER; //�������
		
		    WORK_OVER; //�������
					
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
		
		    WORK_OVER; //�������		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //1������ʮ��
}


/**
 * @brief ��Ʒ���估�����
 * @para 
 *     �������ݣ�  �̵ƴ�---����
 *                1��� -- �½�һ��
 *        ָ�  0x51      
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ���̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12
 */
void work_5_1()
{
		  ws2812_reset(1200);
			 				     
		    WORK_OVER; //�������
		
	  ws2812_rgb_ws_i(1,19,WS_CYAN);
	  ws2812_rgb_ws_i(20,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //��һ��״̬
	
		    WORK_OVER; //�������
					
		ws2812_rgb_ws_i(1,9,WS_CYAN);
	  ws2812_rgb_ws_i(10,12,WS_WHITE);
		
		    WORK_OVER; //�������		
    ws2812_refresh(12,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(12,GPIOC,GPIO_Pin_15); //��һ��״̬
		
				 				     
		    WORK_OVER; //�������
		 
	  led_flowing_init(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,50,5,5,33,2);	//�̵ƴ���ʼ����
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //�������
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //1���½�һ��	 


   led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_DARK,61);//�ر�һ��̵ܶƴ�	
}
/**
 * @brief ʵ��ϵͳ׼��
 * @para 
 *     �������ݣ�  4���--�½�����
 *                 �����̵ƴ�---����
 *        ָ�  0x05      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 */
void work_5_2()
{
				 				     
		    WORK_OVER; //�������
		
    led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_DARK,61);//�ر�һ��̵ܶƴ�
	
				 				     
		    WORK_OVER; //�������
		
			led_flowing_init(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//��,�̵ƴ���ʼ����
				 				     
		    WORK_OVER; //�������
		
		  ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
				 				     
		    WORK_OVER; //�������
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //�����½�һ��
		
					 				     
		    WORK_OVER; //�������
		
	
		led_flowing(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//�ƴ���������
					 				     
		    WORK_OVER; //�������
	
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//���̵ƴ�����
  			 				     
		    WORK_OVER; //�������
				
		led_flowing(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,60,5,5,33,1);	//�ƴ���������		

//			  ws2812_rgb_ws_i(1,17,WS_CYAN);
//	  ws2812_rgb_ws_i(18,22,WS_WHITE);
//					 				     
//		    WORK_OVER; //�������
//		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
//    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
//    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
//    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //�����½�����
//	 			 				     
//		    WORK_OVER; //�������
		
}
/**
 * @brief ���������ж�
 * @para 
 *     �������ݣ�  4���--������һ��״̬
 *                 �����̵ƴ�---����
 *        ָ�  0x06      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 */
void work_6()
{
	
	
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //�������
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ�����һ��״̬	 
				 				     
		    WORK_OVER; //�������
		

	
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//���̵ƴ�����
  
				 				     
		    WORK_OVER; //�������
		
		
		  ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
				 				     
		    WORK_OVER; //�������
		
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);     //�������һ��״̬	
	 			 				     
		    WORK_OVER; //�������
		
}


/**
 * @brief �Գ�
 * @para 
 *     �������ݣ�  4���--�����½�
 *                1���--΢΢�½�
 *                 �����̵ƴ�---��ˮ
 *        ָ�  0x07      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 *
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ���̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12
 */
void work_7()
{
	
				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //�������
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ�����һ��״̬	 
	
			 				     
		    WORK_OVER; //�������
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//���̵ƴ�����
  
				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
    			 				     
		    WORK_OVER; //�������
		
		ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //�������һ��״̬
			 				     
		    WORK_OVER; //�������
			
 }
/**
 * @brief �Գ�
 * @para 
 *     �������ݣ�  4���--�����½�
 *                1���--΢΢�½�
 *                 �����̵ƴ�---��ˮ
 *        ָ�  0x71      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 *
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ���̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12
 */
void work_71()
{
	
	
		ws2812_rgb_ws_i(1,8,WS_CYAN);
	  ws2812_rgb_ws_i(9,11,WS_WHITE);
  			 				     
		    WORK_OVER; //�������
		 
  	 ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ�����һ��״̬	 
	
			 				     
		    WORK_OVER; //�������
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//���̵ƴ�����
  
				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,18,WS_CYAN);
	  ws2812_rgb_ws_i(19,22,WS_WHITE);
    			 				     
		    WORK_OVER; //�������
		
		ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //�������һ��״̬
			 				     
		    WORK_OVER; //�������
		
			 				     
		    WORK_OVER; //�������
		
		led_flowing_init_three_GPIO(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//�ƴ���������
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,7,WS_CYAN);
	  ws2812_rgb_ws_i(8,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ����½�һ�� 
				 				     
		    WORK_OVER; //�������
		
	
			  ws2812_rgb_ws_i(1,17,WS_CYAN);
	  ws2812_rgb_ws_i(18,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½�һ�� 	 
					 				     
		    WORK_OVER; //�������
		
		
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//�ƴ���������	
					 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,6,WS_CYAN);
	  ws2812_rgb_ws_i(7,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ����½����� 	 
	
				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,16,WS_CYAN);
	  ws2812_rgb_ws_i(17,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½�����
					 				     
		    WORK_OVER; //�������
			
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//�ƴ���������	
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,11,WS_WHITE);
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ����½����� 	 
	
				 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,15,WS_CYAN);
	  ws2812_rgb_ws_i(16,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½�����
					 				     
		    WORK_OVER; //�������
			
	  led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//�ƴ���������		
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,14,WS_CYAN);
	  ws2812_rgb_ws_i(15,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½��ĸ�
			 				  
		
			 				     
		    WORK_OVER; //�������
 
 led_flowing_three_gpio(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12,WS_CYAN,61,5,5,30,1);	//�ƴ���������		
			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,13,WS_CYAN);
	  ws2812_rgb_ws_i(14,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½��ĸ�
			 				  
		
			 				     
		    WORK_OVER; //�������		
}
/**
 * @brief �Ժ��������ݽ���
 * @para 
 *     �������ݣ�  4���--������һ��״̬
 *                1���--������һ��״̬
 *                 �����̵ƴ�---����
 *        ָ�  0x08      
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    һ�����ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �����̵ƴ� ��Ӧ GPIOB,GPIO_Pin_13 
 *
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    һ���̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12
 */
void work_8()
 {
	 		    WORK_OVER; //�������
		ws2812_rgb_ws_i(1,5,WS_CYAN);
	  ws2812_rgb_ws_i(6,11,WS_WHITE);
	 		    WORK_OVER; //�������
    ws2812_refresh(11,GPIOC,GPIO_Pin_14); 
    ws2812_refresh(11,GPIOC,GPIO_Pin_15); //һ����½�����	 	
	 
		    WORK_OVER; //�������
		
 		ws2812_rgb_ws_i(1,13,WS_CYAN);
	  ws2812_rgb_ws_i(14,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½��ĸ�
			 				     
		    WORK_OVER; //�������
		
    led_Bright_out(GPIOC,GPIO_Pin_13,GPIOB,GPIO_Pin_13,WS_CYAN,61);//���鳤�̵ƴ�����	  
    led_Bright_out(GPIOB,GPIO_Pin_12,GPIOB,GPIO_Pin_12,WS_CYAN,61);//һ��̵ƴ�����
	 			 				     
		    WORK_OVER; //�������
		
		ws2812_rgb_ws_i(1,12,WS_CYAN);
	  ws2812_rgb_ws_i(13,22,WS_WHITE);
    ws2812_refresh(22,GPIOA,GPIO_Pin_0);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_1);
    ws2812_refresh(22,GPIOA,GPIO_Pin_2);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_3);	 	
    ws2812_refresh(22,GPIOA,GPIO_Pin_4);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_5);
    ws2812_refresh(22,GPIOA,GPIO_Pin_6);		
    ws2812_refresh(22,GPIOA,GPIO_Pin_7);   //������½����
 			 				     
		    WORK_OVER; //�������
		
  
 }
 
 /**
 * @brief ȫ��
 * @para 
 *     �������ݣ� 
 *    4��� ��Ӧ GPIOA,GPIO_Pin_0---7; 
 *    1��� ��Ӧ GPIOC,GPIO_Pin_14��15
 *    ���ƴ� ��Ӧ GPIOC,GPIO_Pin_13
 *    �̵ƴ� ��Ӧ GPIOB,GPIO_Pin_12                  
 *        ָ�  0x16     
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
 * @brief ����
 * @para 
 *     �������ݣ����еƴ�������ˮ
 */
void ceshi(u8 ws_r,u8 ws_g,u8 ws_b,u16 ws_i,u16 i_zu,u16 gap,u16 time,u16 times)
{  
      u16 i,flag=0;
      u8 R=0,G=0,B=0;
	
	    ws2812_reset(1200);
    	ws2812_rgb_ws_i(1,i_zu, ws_r, ws_g, ws_b);
	   
	    			 				     
		    WORK_OVER; //�������
		

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
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_11);	// ����	
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
			 
			 			 				     
		    WORK_OVER; //�������
		
			 

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
	    ws2812_refresh(ws_i,GPIOB,GPIO_Pin_11);	// ����	
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
						 				     
		    WORK_OVER; //�������
		
}

