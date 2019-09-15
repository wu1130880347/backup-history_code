/*--------------------------------------------------------------------------
// 文件名：Driver.c
// 描述：  MCU底层驱动  
// 设计者：EU电子
// 深圳EU电子出品-版权所有-翻版必究
// EU-热爱嵌入式开发
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/* 头文件包含 INCLUDES */
/* type change */
typedef unsigned char       bool;
#define  True  1
#define  False 0


/* INCLUDES */
//MCU
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_fsmc.h"
//Driver
#include "fsmc_driver.h"
void Driver_LcdFSMCWriteReg(u16 Index);
//LCD地址结构体
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;		    
#define	 LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define  LCD             ((LCD_TypeDef *) LCD_BASE)

/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/

//-------------------------------------------------------------------------------------------------------
//-------------------------------------- LCD DRIVER -----------------------------------------------------
//-------------------------------------------------------------------------------------------------------
/********************************************************************************************************
*  Function: Driver_LcdFSMCON				                                                           
*  Object: FSMC init
*  Input: none
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCON(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
		FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
		FSMC_NORSRAMTimingInitTypeDef  writeTiming;
		//使能FSMC时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,   ENABLE);	
		RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
		//PORTD复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;			
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
		//PORTE复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOE, &GPIO_InitStructure); 
   	//PORTG复用推挽输出  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 		 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOG, &GPIO_InitStructure); 
		
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12 
   
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12 
   
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_FSMC);//PG0,AF0 
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC); //PG12,AF12 	
		
		//FSMC时序设置
		readWriteTiming.FSMC_AddressSetupTime = 0x0f;	
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 
    readWriteTiming.FSMC_DataSetupTime = 0x3c;	
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	
	  writeTiming.FSMC_AddressSetupTime = 0x09;
    writeTiming.FSMC_AddressHoldTime = 0x00;		
    writeTiming.FSMC_DataSetupTime = 0x08;	
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 
		//LCD配置
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; 
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming; 
		//初始化FSMC 使能BANK
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  
   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteReg				                                                           
*  Object: 通过FSMC接口写REG
*  Input: Index
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteReg(u16 Index)
{
		LCD->LCD_REG = Index;
}

/********************************************************************************************************
*  Function: Driver_LcdFSMCWriteData				                                                           
*  Object: 通过FSMC接口写Data
*  Input: Data
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdFSMCWriteData(u16 Data)
{
		LCD->LCD_RAM = Data;
}
