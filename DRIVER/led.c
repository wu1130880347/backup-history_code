#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��
  //initial LED1(PC0)
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
			
  // initial LED2(PD3)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  	
  GPIO_SetBits(GPIOC,GPIO_Pin_0);//GPIOF9,F10���øߣ�����
  GPIO_SetBits(GPIOD,GPIO_Pin_3);//GPIOF9,F10���øߣ�����
}






