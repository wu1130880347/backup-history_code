#include "sys.h"
#include "stm32_flash.h"
#include "stm32f4xx_conf.h"


/*******************************************************************************
* Function Name  : delay
* Description    : Inserts a time delay.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void delay(void)
{
	vu32 i = 0;

	for(i = 0xFF; i != 0; i--)
	{
		
	}
}

/*******************************************************************************
* Function Name  : FLASH_Unlock
* Description    : Unlocks the FLASH Program Erase Controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_Unlock(void)
{
	/* Authorize the FPEC Access */
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
* Function Name  : FLASH_Lock
* Description    : Locks the FLASH Program Erase Controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_Lock(void)
{
	/* Set the Lock Bit to lock the FPEC and the FCR */
	FLASH->CR |= 0x00000080;
}

/*******************************************************************************
* Function Name  : FLASH_GetStatus
* Description    : Returns the FLASH Status.
* Input          : None
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP or FLASH_COMPLETE
*******************************************************************************/
FLASH_Status FLASH_GetStatus(void)
{
	FLASH_Status flashstatus = FLASH_COMPLETE;

	if((FLASH->SR & 0x00000001) == 0x00000001)
	{
		flashstatus = FLASH_BUSY;
	}
	else
	{
		if(FLASH->SR & 0x00000004)
		{
			flashstatus = FLASH_ERROR_PG;
		}
		else
		{
			if(FLASH->SR & 0x00000010)
			{
				flashstatus = FLASH_ERROR_WRP;
			}
			else
			{
				flashstatus = FLASH_COMPLETE;
			}
		}
	}
	/* Return the Flash Status */
	return flashstatus;
}

/*******************************************************************************
* Function Name  : FLASH_WaitForLastOperation
* Description    : Waits for a Flash operation to complete or a TIMEOUT to occur.
* Input          : - Timeout: FLASH progamming Timeout
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_WaitForLastOperation(u32 Timeout)
{
	FLASH_Status status = FLASH_COMPLETE;

	/* Check for the Flash Status */
	status = FLASH_GetStatus();

	/* Wait for a Flash operation to complete or a TIMEOUT to occur */
	while((status == FLASH_BUSY) && (Timeout != 0x00))
	{
		delay();
		status = FLASH_GetStatus();
		Timeout--;
	}

	if(Timeout == 0x00 )
	{
		status = FLASH_TIMEOUT;
	}

	/* Return the operation status */
	return status;
}

/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status Flash_ErasePage(u32 Page_Address)
{
	FLASH_Status status = FLASH_COMPLETE;

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(EraseTimeout);

	if(status == FLASH_COMPLETE)
	{
		/* if the previous operation is completed, proceed to erase the page */
		FLASH->CR|= 0x00000002;
		FLASH->AR = Page_Address;
		FLASH->CR|= 0x00000040;
		
		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(EraseTimeout);
		
		if(status != FLASH_BUSY)
		{
			/* if the erase operation is completed, disable the PER Bit */
			FLASH->CR &= 0x00001FFD;
		}
	}
	/* Return the Erase Status */
	return status;
}

/*******************************************************************************
* Function Name  : FLASH_GetWriteProtectionOptionByte
* Description    : Returns the FLASH Write Protection Option Bytes Register value.
* Input          : None
* Output         : None
* Return         : The FLASH Write Protection  Option Bytes Register value
*******************************************************************************/
u32 FLASH_GetWriteProtectionOptionByte(void)
{
	/* Return the Falsh write protection Register value */
	return (u32)(FLASH->WRPR);
}

/*******************************************************************************
函 数 名：	Stm32FlashWrite
功能描述：	向stm32的flash中写入1个半字
入口参数：	address:起始地址 data:数据
出口参数：	无
********************************************************************************/
void Stm32FlashWrite(u32 address, u16 data)
{	
	FLASH_Unlock();

	FLASH->SR = 0x00000034;

	while (FLASH->SR & (1 << 0));
	FLASH->CR |= 0x00000001;
	*(vu16*)address = data;
	while (FLASH->SR & (1 << 0));
	FLASH->CR &= ~0x00000001;

	FLASH_Lock();
}

/*******************************************************************************
函 数 名：	Stm32FlashProgram
功能描述：	向stm32的flash中写入数据
入口参数：	address:起始地址 pdata:数据指针 len:写入长度
出口参数：	无
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len)
{
	u32 temp_addr = address;
	u16 temp_len  = len;
	u16 buff, temp;

	if ((address + len) & 1)
		temp_len = len - 1;

	if (temp_addr & 1)
	{
		temp_addr--;
		buff = *pdata++;
		buff <<= 8;
		buff |= 0x00ff;
		Stm32FlashWrite(temp_addr, buff);
		temp_addr += 2;
	}

	while (temp_addr < (address + temp_len))
	{
		buff = *pdata++;
		temp = *pdata++;
		temp <<= 8;
		buff = temp + buff;
		Stm32FlashWrite(temp_addr, buff);
		temp_addr += 2;
	}

	if ((address + len) & 1)
	{
		buff  = *pdata;
		buff |= 0xff00;
		Stm32FlashWrite(temp_addr, buff);
	}
}

/*******************************************************************************
函 数 名：	Stm32FlashAllErase
功能描述：	擦除stm32中的第8-125页;
				0-7页是boot;
				126页是GID;
				127页 语音表;
入口参数：	无
出口参数：	无
********************************************************************************/
void Stm32FlashAllErase(void)
{
	u8 n = 0;

	FLASH_Unlock();

	FLASH->SR = 0x00000034;

	Stm32FlashErasePage(125);//先擦除用户程序合法标志
	for (n = 8; n < 125; n++)
	{
		IWDG->KR  = 0xAAAA;//喂狗
		Stm32FlashErasePage(n);
	}

	FLASH_Lock();
}

/*******************************************************************************
函 数 名：	Stm32FlashRead
功能描述：	读取stm32 flash中的数据
入口参数：	addr:地址 buff:数据缓冲指针 len:读取的字节数目
出口参数：	无
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len)
{
	u16  temp_len  = len;
	u8 	*temp_buff = buff;
	u8  *temp_addr = (u8*)addr;
	u16  i = 0;

	while (i < temp_len)
	{
		*temp_buff++ = *temp_addr++;
		i++;
	}
}

/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY,
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
*                  FLASH_TIMEOUT.
*******************************************************************************/
void Stm32FlashErasePage(u8 Page_Address)
{	
	FLASH_Unlock();

	FLASH->SR = 0x00000034;

	Flash_ErasePage(0x8000000 + (Page_Address * 2048));//互联型128页，每页2K

	FLASH_Lock();
}

