#ifndef __STM32_FLASH_H__
#define __STM32_FLASH_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern u8 Stm32FlashWriteByte(u32 address, u8 *data, u8 len);
extern u8 Stm32FlashProgramWord(u32 address, uint32_t *pdata, u16 len);
extern u8 Stm32FlashEraseSector(u8 Page_Address);
extern u8 Stm32FlashAllErase(void);
extern void Stm32FlashReadByte(u32 addr, u8 *buff, u16 len);
extern void Stm32FlashReadWord(u32 addr, u32 *buff, u16 len);

#ifdef __cplusplus
}
#endif

#endif
