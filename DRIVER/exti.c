#include "exti.h"
#include "delay.h" 
#include "led.h"

static u8 _st;
 static u16 pwm_D;
void scan_exti(u8 pin);

void scan_exti(u8 pin)
{
                                _st = 0;
                                PBout(12) = 0;
                                PBout(14) = 1;
                                PEout(12) = 0;
                                PEout(14) = 0; 
                                _st |= PDin(pin);
                                PBout(12) = 1;
                                PBout(14) = 0;
                                PEout(12) = 0;
                                PEout(14) = 0;
                                _st |= PDin(pin)<<1;
                                PBout(12) = 0;
                                PBout(14) = 0;
                                PEout(12) = 0;
                                PEout(14) = 1;
                                _st |= PDin(pin)<<2;
                                PBout(12) = 0;
                                PBout(14) = 0;
                                PEout(12) = 1;
                                PEout(14) = 0;
                                _st |= PDin(pin)<<3;                              
                                PBout(12) = 1;
                                PBout(14) = 1;
                                PEout(12) = 1;
                                PEout(14) = 1;
}
void EXTI0_IRQHandler(void)
{
       
        delay_ms(10);
        if(PAin(0)==1)
        {
        pwm_D += 10;
	LED0 ^= 1;
        LCD_DisASCString16x8(100,0,"Key0:",WHITE,BLACK);
        LCD_Show_Num(100,40,pwm_D,WHITE,BLACK);
        TIM_SetCompare1(TIM14,pwm_D);
        if(pwm_D >= 250)pwm_D = 50;
        }
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
        
}

void EXTI15_10_IRQHandler(void)
{
  
               if(EXTI_GetITStatus(EXTI_Line11)) 
		{
                  delay_ms(10);
                  if(PFin(11)==1)
                  {
                               pwm_D -= 10;
	LED0 ^= 1;
        LCD_DisASCString16x8(200,0,"Key1:",GREEN,BLACK);
        LCD_Show_Num(200,40,pwm_D,WHITE,BLACK);
        TIM_SetCompare1(TIM14,pwm_D);
        if(pwm_D <= 50)pwm_D = 250; 
                  }
				EXTI_ClearITPendingBit(EXTI_Line11);
		}
               else if(EXTI_GetITStatus(EXTI_Line10)) 
		{
                                LED1 ^= 1;
                                scan_exti(10);
                                switch(_st)
                                 {
                                    case 0x01:OLED_ShowChar(120,7,'7',8);break;
                                    case 0x02:OLED_ShowChar(120,7,'8',8);break;
                                    case 0x04:OLED_ShowChar(120,7,'9',8);break;
                                    case 0x08:OLED_ShowChar(120,7,'C',8);break;
                                    default :break;
                                  }
                                  _st = 0; 
				EXTI_ClearITPendingBit(EXTI_Line10);
		}
               else if(EXTI_GetITStatus(EXTI_Line14)) 
		{
                                LED1 ^= 1;
                                scan_exti(14);
                                switch(_st)
                                 {
                                    case 0x01:OLED_ShowChar(120,7,'1',8);break;
                                    case 0x02:OLED_ShowChar(120,7,'2',8);break;
                                    case 0x04:OLED_ShowChar(120,7,'3',8);break;
                                    case 0x08:OLED_ShowChar(120,7,'A',8);break;
                                    default :break;
                                  }
                                  _st = 0; 
				EXTI_ClearITPendingBit(EXTI_Line14);
		}
               else if(EXTI_GetITStatus(EXTI_Line12)) 
		{
                                LED1 ^= 1;
                                scan_exti(12);
                                switch(_st)
                                 {
                                    case 0x01:OLED_ShowChar(120,7,'4',8);break;
                                    case 0x02:OLED_ShowChar(120,7,'5',8);break;
                                    case 0x04:OLED_ShowChar(120,7,'6',8);break;
                                    case 0x08:OLED_ShowChar(120,7,'B',8);break;
                                    default :break;
                                  }
                                  _st = 0;
				EXTI_ClearITPendingBit(EXTI_Line12);
		}
               
               else return;
}

//void EXTI9_5_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line8)) 
//  {
//    LED0 ^= 1;
//    scan_exti(8);
//    switch(_st)
//    {
//      case 0x01:OLED_ShowChar(120,7,'*',8);break;
//      case 0x02:OLED_ShowChar(120,7,'0',8);break;
//      case 0x04:OLED_ShowChar(120,7,'#',8);break;
//      case 0x08:OLED_ShowChar(120,7,'D',8);break;
//      default :break;
//    }
//    _st = 0;
//    EXTI_ClearFlag(EXTI_Line8); 
//    EXTI_ClearITPendingBit(EXTI_Line8); //清除LINE0上的中断标志位
//  }
//}

void Key_4x4_Init(void)
{
                        NVIC_InitTypeDef   NVIC_InitStructure;
	                EXTI_InitTypeDef   EXTI_InitStructure;
	                GPIO_InitTypeDef GPIO_InitStructure;
                        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
                        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
                        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
                        
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
			GPIO_Init(GPIOE, &GPIO_InitStructure);
                        GPIO_Init(GPIOB, &GPIO_InitStructure);
                        GPIO_SetBits(GPIOE,GPIO_Pin_12|GPIO_Pin_14); //GPIOF9,F10设置高，灯灭
                        GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_14); //GPIOF9,F10设置高，灯灭
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource12);
        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource14);
        
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line8|EXTI_Line10|EXTI_Line12|EXTI_Line14;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
   	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
  
	__enable_irq();	  
}
//Key1 and Key2  exti_init
void EXTIX_Init(void)
{
	                NVIC_InitTypeDef   NVIC_InitStructure;
	                EXTI_InitTypeDef   EXTI_InitStructure;
	                GPIO_InitTypeDef GPIO_InitStructure;
                        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
                        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
                        
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
			GPIO_Init(GPIOF, &GPIO_InitStructure);
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
  
	__enable_irq();	   
}












