#include "timer.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "lcd_12864.h"




void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
        TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //PA0复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
        TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
        TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
        TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
        TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
        TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
        TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5

 
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
static u8       TIM5CH1_flag = 0;   //输入捕获状态		    				
static u32	TIM5CH1_VAL[3];	    //输入捕获值(TIM2/TIM5是32位)
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 		    

		if(TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)   //捕获1发生捕获事件
		{	
			
			TIM5CH1_VAL[TIM5CH1_flag] = TIM_GetCapture2(TIM5);//获取当前的捕获值.
	 		TIM5CH1_flag++;
                        if(TIM5CH1_flag==1)TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
                        else TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
                        if(TIM5CH1_flag==2){Work_T();TIM5CH1_flag=0;}
		}			     	    					   
 	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); //清除中断标志位
}
void Work_T(void)
{
  u32 Ta,Tb;
  float t_D;
  if((TIM5CH1_VAL[0]<TIM5CH1_VAL[1])&&(TIM5CH1_VAL[1]<TIM5CH1_VAL[2]))
  {
    Ta = TIM5CH1_VAL[2] - TIM5CH1_VAL[0];
    Tb = TIM5CH1_VAL[1] - TIM5CH1_VAL[0];
    t_D = Tb*1.0/(float)Ta*100.0;
    OLED_ShowNum(20,2,(u16)Ta,5,8);
    OLED_ShowNum(36,3,(u8)t_D,2,8);
    OLED_ShowChar(52,3,'%',8);
    OLED_ShowNum(20,2,(u16)Tb,5,8);
    delay_ms(500);
  }
  else
  {
    TIM5CH1_VAL[0] = 0;
    TIM5CH1_VAL[1] = 0;
    TIM5CH1_VAL[2] = 0;
  }
}
