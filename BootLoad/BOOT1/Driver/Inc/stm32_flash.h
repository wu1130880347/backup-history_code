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
 �� �� ����	Stm32FlashAllErase
 ����������	����stm32�еĵ�8-125ҳ��127ҳ ���ڴ洢����  126ҳԤ��
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashAllErase(void);

/*******************************************************************************
 �� �� ����	Stm32FlashProgram
 ����������	��stm32��flash��д������
 ��ڲ�����	address:��ʼ��ַ pdata:����ָ�� len:д�볤��
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len);

/*******************************************************************************
 �� �� ����	Stm32FlashRead
 ����������	��ȡstm32 flash�е�����
 ��ڲ�����	addr:��ַ buff:���ݻ���ָ�� len:��ȡ���ֽ���Ŀ
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len);

#ifdef __cplusplus
}
#endif


#endif

