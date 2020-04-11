#include "sys.h"
#include "stm32f4xx_conf.h"
#include "stm32_flash.h"

u8 Stm32FlashWriteByte(u32 address, u8* data, u8 len);
u8 Stm32FlashProgramWord(u32 address, uint32_t *pdata, u16 len);
u8 Stm32FlashEraseSector(u8 Page_Address);
u8 Stm32FlashAllErase(void);
void Stm32FlashReadByte(u32 addr, u8 *buff, u16 len);
void Stm32FlashReadWord(u32 addr, u32 *buff, u16 len);

/*******************************************************************************
函 数 名：	Stm32FlashWriteByte
功能描述：	向stm32的flash中写入1个字节
入口参数：	address:起始地址 data:数据 len:长度
出口参数：	u8 0 成功; 1 失败;
********************************************************************************/
u8 Stm32FlashWriteByte(u32 address, u8* data, u8 len)
{
	u32 temp_addr = address + len;
	if(address < 0x08000000) return 1;	//非法地址
	FLASH_Unlock();
	while (address < temp_addr)
	{
		if (FLASH_ProgramByte(address, *data) == FLASH_COMPLETE)
		{
			address += 1;
		}
		else
		{
			FLASH_Unlock();//写flash失败
			return 1;
		}
	}
	FLASH_Unlock();
	return 0;//写flash成功
}

/*******************************************************************************
函 数 名：	Stm32FlashProgram
功能描述：	向stm32的flash中写入数据
入口参数：	address:起始地址 pdata:数据指针 len:写入长度
出口参数：	u8 0 成功; 1 失败;
********************************************************************************/
u8 Stm32FlashProgramWord(u32 address, uint32_t *pdata, u16 len)
{
	u32 temp_addr = address + len * 4;
	if(address < 0x08000000 || address % 4) return 1;	//非法地址
	FLASH_Unlock();
	while (address < temp_addr)
	{
		if (FLASH_ProgramWord(address, *pdata) == FLASH_COMPLETE)
		{
			address += 4;
		}
		else
		{
			FLASH_Unlock();//写flash失败
			return 1;
		}
	}
	FLASH_Unlock();
	return 0;//写flash成功
}
/*******************************************************************************
函 数 名：	Stm32FlashAllErase
功能描述：	擦除stm32中的第0-11扇区;
				0扇区是boot1;
				1-3扇区私有数据
				4扇区是BOOT2;
				5-11扇区 APP;
入口参数：	无
出口参数：	无
********************************************************************************/
u8 Stm32FlashAllErase(void)
{
	u8 i = 0;
	FLASH_Unlock();
	for (i = 1; i < 12; i++)
	{
		if (i != 4)
			if(FLASH_EraseSector(FLASH_Sector_0 + 0x08 * i, VoltageRange_3) == FLASH_COMPLETE)
			{;}
			else
			{
				FLASH_Unlock();
				return 1;
			}
	}
	FLASH_Unlock();
	return 0;
}

/*******************************************************************************
函 数 名：	Stm32FlashReadByte
功能描述：	读取stm32 flash中的数据
入口参数：	addr:地址 buff:数据缓冲指针 len:读取的字节数目
出口参数：	无
********************************************************************************/
void Stm32FlashReadByte(u32 addr, u8 *buff, u16 len)
{
	u16 temp_len = len;
	u8 *temp_buff = buff;
	u8 *temp_addr = (u8 *)addr;
	u16 i = 0;

	while (i < temp_len)
	{
		*temp_buff++ = *temp_addr++;
		i++;
	}
}

/*******************************************************************************
函 数 名：	Stm32FlashReadWord
功能描述：	读取stm32 flash中的数据
入口参数：	addr:地址 buff:数据缓冲指针 len:读取的字节数目
出口参数：	无
********************************************************************************/
void Stm32FlashReadWord(u32 addr, u32 *buff, u16 len)
{
	u16 temp_len = len;
	u32 *temp_buff = buff;
	u32 *temp_addr = (u32 *)addr;
	u16 i = 0;

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
u8 Stm32FlashEraseSector(u8 Page_Address)
{
	if(Page_Address == 0 || Page_Address == 4)
		return 1;//不可删除BOOT1 BOOT2
	FLASH_Unlock();
	if(FLASH_EraseSector(FLASH_Sector_0 + 0x08 * Page_Address, VoltageRange_3) == FLASH_COMPLETE)
	{
		FLASH_Lock();
		return 0;
	}
	FLASH_Lock();
	return 1;//擦除失败
}

/* static void test_code(void)
{
	u32 abc = 0x12345678;
	u32 abd = 0;
	u8 a = 0x5a;
	u8 b = 0;
	Stm32FlashAllErase();
	Stm32FlashProgramWord(0x08004010, &abc, 1);
	Stm32FlashReadWord(0x08004010, &abd, 1);
	Stm32FlashEraseSector(1);
	Stm32FlashWriteByte(0x08004010, &a, 1);
	Stm32FlashReadByte(0x08004010, &b, 1);
	Stm32FlashAllErase();
} */