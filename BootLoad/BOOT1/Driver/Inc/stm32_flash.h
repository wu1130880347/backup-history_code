#ifndef __STM32_FLASH_H__
#define __STM32_FLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif

//typedef enum
//{
//  FLASH_BUSY = 1,
//  FLASH_ERROR_PG,
//  FLASH_ERROR_WRP,
//  FLASH_COMPLETE,
//  FLASH_TIMEOUT
//}FLASH_Status;

#define IAPSTART		0x8004000

#define FLASH_KEY1 		((u32)0x45670123)
#define FLASH_KEY2		((u32)0xCDEF89AB)

/* Delay definition */
//#define EraseTimeout    ((u32)0x00000FFF)
//#define ProgramTimeout  ((u32)0x0000000F)
#define EraseTimeout    ((u32)0x000B0000)
#define ProgramTimeout  ((u32)0x00002000)


/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
*                  FLASH_TIMEOUT.
*******************************************************************************/
void Stm32FlashErasePage(u8 Page_Address);

/*******************************************************************************
 函 数 名：	Stm32FlashAllErase
 功能描述：	擦除stm32中的第8-125页，127页 用于存储参数  126页预留
 入口参数：	无
 出口参数：	无
********************************************************************************/
void Stm32FlashAllErase(void);

/*******************************************************************************
 函 数 名：	Stm32FlashProgram
 功能描述：	向stm32的flash中写入数据
 入口参数：	address:起始地址 pdata:数据指针 len:写入长度
 出口参数：	无
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len);

/*******************************************************************************
 函 数 名：	Stm32FlashRead
 功能描述：	读取stm32 flash中的数据
 入口参数：	addr:地址 buff:数据缓冲指针 len:读取的字节数目
 出口参数：	无
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len);

#ifdef __cplusplus
}
#endif


#endif

