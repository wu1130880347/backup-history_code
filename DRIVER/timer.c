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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //PA0����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
        TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
        TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
        TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
        TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
        TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
        TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
static u8       TIM5CH1_flag = 0;   //���벶��״̬		    				
static u32	TIM5CH1_VAL[3];	    //���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		    

		if(TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)   //����1���������¼�
		{	
			
			TIM5CH1_VAL[TIM5CH1_flag] = TIM_GetCapture2(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
	 		TIM5CH1_flag++;
                        if(TIM5CH1_flag==1)TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
                        else TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
                        if(TIM5CH1_flag==2){Work_T();TIM5CH1_flag=0;}
		}			     	    					   
 	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); //����жϱ�־λ
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
