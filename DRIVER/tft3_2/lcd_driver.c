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
//typedef unsigned char       u8;
//typedef unsigned short      u16;
#define  True  1
#define  False 0

//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

/* INCLUDES */
//MCU
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "string.h"
//Driver
#include "lcd_driver.h"
//时钟频率
#define  DRIVER_SYSCLK  168000000  

/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------
*  执行代码													 
-------------------------------------------------------------------------------------------------------*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++  MCU ++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU初始化               
*  输入： 无
*  输出： 无								                         	                                     
*  备注： MCU启动的一些配置                                      
********************************************************************************************************/
void Driver_MCU_Init_(void)
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
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);		

		//---------------------------- INT CONFIG ---------------------------
		//2-level interrupt 
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
		//---------------------------- JTAG CONFIG ---------------------------
		//JTAG/SWD disable
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

			//开中断
		//__enable_irq(); 
}	


/********************************************************************************************************
*  Function: Driver_OpenEA						                                                           
*  Object: 开启MCU总中断  
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 无                               
********************************************************************************************************/
void Driver_OpenEA(void)
{
		__enable_irq();
}

/********************************************************************************************************
*  Function: Driver_CloseEA						                                                           
*  Object: 关闭MCU总中断  
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 无                               
********************************************************************************************************/
void Driver_CloseEA(void)
{
		__disable_irq();
}

/********************************************************************************************************
*  Function: Driver_SystemTickON						                                                           
*  Object: 开启滴答时钟 用于运行程序           
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 开启10MS 定时器                       
********************************************************************************************************/
void Driver_SystemTickON(void)
{
		SysTick_Config(DRIVER_SYSCLK / 100);
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------- LCD DRIVER -----------------------------------------------------
//-------------------------------------------------------------------------------------------------------
/********************************************************************************************************
*  Function: Driver_LcdBacklight				                                                           
*  Object: lcd backlight control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdBacklight(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PB0
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(sta)
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/********************************************************************************************************
*  Function: Driver_LcdReset				                                                           
*  Object: lcd reset control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdReset(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PC5
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		else
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
}

/********************************************************************************************************
*  Function: Driver_LcdCS		                                                           
*  Object: lcd enable/disable control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdCS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PG12
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
			GPIO_Init(GPIOG, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOG, GPIO_Pin_12);
		else
			GPIO_SetBits(GPIOG, GPIO_Pin_12);
}

/********************************************************************************************************
*  Function: Driver_LcdRS		                                                           
*  Object: lcd RS(cmd/data chose) control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdRS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PG0
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
			GPIO_Init(GPIOG, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOG, GPIO_Pin_0);
		else
			GPIO_SetBits(GPIOG, GPIO_Pin_0);
}

/********************************************************************************************************
*  Function: Driver_LcdRD		                                                           
*  Object: lcd read control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdRD(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PD4
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);
		else
			GPIO_SetBits(GPIOD, GPIO_Pin_4);
}

/********************************************************************************************************
*  Function: Driver_LcdWR		                                                           
*  Object: lcd write control
*  Input: sta
*  Output: none                                  
*  brief: none
********************************************************************************************************/
void Driver_LcdWR(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			//PD5
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOD, GPIO_Pin_5);
		else
			GPIO_SetBits(GPIOD, GPIO_Pin_5);
}

/********************************************************************************************************
*  Function: Driver_LcdSendData		                                                           
*  Object: lcd data send
*  Input: Temp
*  Output: none                                  
*  brief: len is 16bits
********************************************************************************************************/
void Driver_LcdSendData(u16 Temp)
{
		//PD10 PD9 PD8    PE15 PE14 PE13 PE12 PE11 PE10 PE9 PE8 PE7     PD1 PD0 PD15 PD14
		static bool StartFlag = True;
		if(StartFlag)
		{
				GPIO_InitTypeDef GPIO_InitStructure;
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_15 | GPIO_Pin_14;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
				GPIO_Init(GPIOD, &GPIO_InitStructure);
			
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  	
				GPIO_Init(GPIOE, &GPIO_InitStructure);
				StartFlag = False;
		}		
		/*
		if(Temp&CHSBIT15)
			GPIO_SetBits(GPIOD, GPIO_Pin_10);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_10);
		if(Temp&CHSBIT14)
			GPIO_SetBits(GPIOD, GPIO_Pin_9);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_9);
		if(Temp&CHSBIT13)
			GPIO_SetBits(GPIOD, GPIO_Pin_8);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_8);
		if(Temp&CHSBIT12)
			GPIO_SetBits(GPIOE, GPIO_Pin_15);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_15);
		if(Temp&CHSBIT11)
			GPIO_SetBits(GPIOE, GPIO_Pin_14);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
		if(Temp&CHSBIT10)
			GPIO_SetBits(GPIOE, GPIO_Pin_13);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_13);
		if(Temp&CHSBIT9)
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
		if(Temp&CHSBIT8)
			GPIO_SetBits(GPIOE, GPIO_Pin_11);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_11);
		if(Temp&CHSBIT7)
			GPIO_SetBits(GPIOE, GPIO_Pin_10);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_10);
		if(Temp&CHSBIT6)
			GPIO_SetBits(GPIOE, GPIO_Pin_9);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
		if(Temp&CHSBIT5)
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
		if(Temp&CHSBIT4)
			GPIO_SetBits(GPIOE, GPIO_Pin_7);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_7);
		if(Temp&CHSBIT3)
			GPIO_SetBits(GPIOD, GPIO_Pin_1);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		if(Temp&CHSBIT2)
			GPIO_SetBits(GPIOD, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_0);
		if(Temp&CHSBIT1)
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		if(Temp&CHSBIT0)
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
		else
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		*/
		GPIO_SetBits(GPIOD, ((Temp)/0x2000)<<8 & 0x0700);
		GPIO_ResetBits(GPIOD, (~((Temp)/0x2000)<<8 & 0x0700));
		GPIO_SetBits(GPIOE, ((Temp<<3)/0x80)<<7 & 0xff80);
		GPIO_ResetBits(GPIOE, (~((Temp<<3)/0x80)<<7 & 0xff80));
		GPIO_SetBits(GPIOD, ((Temp<<12)/0x4000) & 0x0003);
		GPIO_ResetBits(GPIOD, (~((Temp<<12)/0x4000) & 0x0003));
		GPIO_SetBits(GPIOD, ((Temp<<14)/0x4000)<<14 & 0xC000);
		GPIO_ResetBits(GPIOD, (~((Temp<<14)/0x4000)<<14 & 0xC000));
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------- TOUCH SPI DRIVER -----------------------------------------------
//-------------------------------------------------------------------------------------------------------
/********************************************************************************************************
*  Function: Driver_TouchCS					                                                           
*  Object: touch ic spi enable/unable
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none                                 
********************************************************************************************************/
void Driver_SPICS(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   	
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
			//FCS/SDCS UNABLE
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
			GPIO_SetBits(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
		}	
		if(!sta)
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
}

/********************************************************************************************************
*  Function: Driver_SPIMISO					                                                           
*  Object: SPI_MOSI
*  Input: none
*  Output: 1/0 							                         	                                     
*  brief:	none                                   
********************************************************************************************************/
bool Driver_SPIMISO(void)
{
		static bool StartFlag = True;
		if(StartFlag)
		{	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}
		return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
}

/********************************************************************************************************
*  Function: Driver_SPIMOSI					                                                           
*  Object: SPI_MOSI
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none                                   
********************************************************************************************************/
void Driver_SPIMOSI(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		else
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
}

/********************************************************************************************************
*  Function: Driver_SPISCK					                                                           
*  Object: SPI_SCK
*  Input: 1/0
*  Output: none 							                         	                                     
*  brief:	none  	                                  
********************************************************************************************************/
void Driver_SPISCK(bool sta)
{
		static bool StartFlag = True;
		if(StartFlag)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			StartFlag = False;
		}	
		if(!sta)
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

bool Driver_TouchTINTSta(void)
{
		static bool StartFlag = True;
		if(StartFlag)
		{	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;	//上拉	
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}
		return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10) ;
}

//内部ROM Flash起始地址
#define  DERIVE_ROM_START_ADDRESS         0x08000000  
//系统参数存储地址 存储在1022K的地方
#define  DRIVER_FLASHPAGE_ADDRESS        (DERIVE_ROM_START_ADDRESS+(1022*1024))
/********************************************************************************************************
*  Function: Driver_FlashRead						                                                           
*  Object: 读FLASH参数
*  输入： 头地址(0~xxx) 数据存放指针 数据长
*  输出： 无                               
*  备注： Addr: 1K 0-1023   从DRIVER_FLASHPAGE_ADDRESS处, 读FLASH参数                            
********************************************************************************************************/
void Driver_FlashRead(u32 Addr, u8 *pData, u16 Len)
{
		u8 *p;
		//p指针指向Flash空间
		p = (u8*)DRIVER_FLASHPAGE_ADDRESS + Addr; 
		//读数据
		memcpy(pData, p, Len);
}

/********************************************************************************************************
*  Function: Driver_FlashWrite						                                                           
*  Object: 写FLASH参数
*  输入： 头地址(0~xxx) 数据存放指针 数据长
*  输出： 无                               
*  备注： Addr: 1K 0-1023    从DRIVER_FLASHPAGE_ADDRESS处, 写FLASH参数                           
********************************************************************************************************/
void Driver_FlashWrite(u32 Addr, u8 *pData, u16 Len)
{
		u16 i;
		RCC_HSICmd(ENABLE);
		Driver_CloseEA();
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY |FLASH_FLAG_OPERR| FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
		FLASH_EraseSector(FLASH_Sector_11,VoltageRange_3); //擦除最后一个扇区
		for(i = 0; i < Len; i = i+4)
		{
				union{
					u8 Data[4];
					u32 Temp;
				}WriteNum;
				//将u8转为u32
				memset(&WriteNum, 0, sizeof(WriteNum));
				WriteNum.Data[0] = *(pData+i);
				if(i+1 < Len)
					WriteNum.Data[1] = *(pData+i+1);
				if(i+2 < Len)
					WriteNum.Data[2] = *(pData+i+2);
				if(i+3 < Len)
					WriteNum.Data[3] = *(pData+i+3);
				//保存参数
				FLASH_ProgramWord(DRIVER_FLASHPAGE_ADDRESS + Addr + i, WriteNum.Temp);	
		}		
		FLASH_Lock();
		Driver_OpenEA();
		RCC_HSICmd(DISABLE);
}

/********************************************************************************************************
*  Function: Driver_Key1Sta						                                                           
*  Object: 按键1状态
*  输入: 无
*  输出: 1/0                                 
*  备注: 无                         
********************************************************************************************************/
bool Driver_Key1Sta(void)
{
		static bool StartFlag = True;
		if(StartFlag)
		{	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			StartFlag = False;
		}
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) ;
}

/********************************************************************************************************
*  Function: Driver_Key2Sta						                                                           
*  Object: 按键2状态
*  输入: 无
*  输出: 1/0                                 
*  备注: 无                              
********************************************************************************************************/
bool Driver_Key2Sta(void)
{
		static bool StartFlag = True;
		if(StartFlag)
		{	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
			GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
			GPIO_Init(GPIOF, &GPIO_InitStructure);
			StartFlag = False;
		}
		return GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11);
}


