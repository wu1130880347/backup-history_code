#include "led.h" 
	    
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟
  //initial LED1(PC0)
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
			
  GPIO_ResetBits(GPIOA,GPIO_Pin_8);//亮灯
}






