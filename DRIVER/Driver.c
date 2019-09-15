#include "led.h"
#include "Driver.h"
#include "ProHead.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"

void Driver_MCU_Init(void)
{

		#define PLL_M      8
		#define PLL_N      336	
		#define PLL_P      2	
		#define PLL_Q      7	
  	//--------------------------- CLK INIT, HSE PLL ----------------------------
		ErrorStatus HSEStartUpStatus;
		//RCC reset
		RCC_DeInit();
		//Enable HSE
		RCC_HSEConfig(RCC_HSE_ON); 
		//Wait HSE is ready
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		//If HSE start fail, wail and wail.
		while(HSEStartUpStatus == ERROR);
		//Set bus clock
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//(HCLK=SYSCLK)=168MHz
		RCC_PCLK1Config(RCC_HCLK_Div4);		//(PCLK1=SYSCLK/4)=42MHz
		RCC_PCLK2Config(RCC_HCLK_Div2);		//(PCLK2=SYSCLK/2)=84MHz
		//HSE 8M PLL-> 168M	
		//PLL=8MHz * N/ (M*P)=8MHz* 336 /(8*2) = 168MHz
		RCC_PLLConfig(RCC_PLLSource_HSE,PLL_M ,PLL_N ,PLL_P,PLL_Q);
		RCC_PLLCmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		//Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);
		
		

		//----------------------------- CLOSE HSI ---------------------------
		RCC_HSICmd(DISABLE);

		//--------------------------- OPEN GPIO CLK -------------------------
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, DISABLE);
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE);
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE);
                RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, DISABLE);

		//---------------------------- INT CONFIG ---------------------------
		//2-level interrupt 
	        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
                __enable_irq();
			
		//---------------------------- JTAG CONFIG ---------------------------
		//JTAG/SWD disable
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
}		   

void TIM3_us_on(void)
{
                TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		//开启定时器外设时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		//配置定时器参数
		TIM_DeInit(TIM3); 
		TIM_TimeBaseStructure.TIM_Period = 15000; 								 	//10ms定时			 
		TIM_TimeBaseStructure.TIM_Prescaler = (84000000/100000 - 1);              
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
		//中断配置
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级2 低优先级别中断 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  	 //响应优先级0 高级别的响应中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
		NVIC_Init(&NVIC_InitStructure);	  
		//开中断
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);					  
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
		//开启定时器			 
		TIM_Cmd(TIM3, ENABLE);
                //中断配置 2-level interrupt 
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		//开总中断
		__enable_irq(); 
}

void TIM3_IRQHandler(void)
{
     TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}

