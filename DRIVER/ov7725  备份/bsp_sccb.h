#ifndef	__SCCB_H_
#define __SCCB_H_

#include "lcd_12864.h"

#define ENABLE 1
#define DISABLE 0
#define SCL_H         PCout(6) = 1
#define SCL_L         PCout(6) = 0
   
#define SDA_H         PCout(7) = 1
#define SDA_L         PCout(7) = 0

#define SCL_read      PCin(6)
#define SDA_read      PCin(7)

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
int SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);

#endif
