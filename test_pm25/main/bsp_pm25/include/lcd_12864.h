#ifndef __LCD_12864_H_
#define __LCD_12864_H_
#include "bsp_includes.h"

#ifndef OLED_RAM
#define OLED_RAM FALSE
#endif

#ifndef OLED_PICTURE
#define OLED_PICTURE TRUE
#endif

#ifndef OLED_CHINESE
#define OLED_CHINESE FALSE
#endif

#ifndef OLED_NORMAL
#define OLED_NORMAL TRUE
#endif




#if (defined OLED_NORMAL) && (OLED_NORMAL == TRUE)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define Max_Column	128
#define Max_Row		64
//OLED控制用函数
extern void OLED_Init(void);
extern void OLED_Clear(void);

extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
extern void OLED_Show0Num(u8 x,u8 y,u32 num,u8 len,u8 size);
extern void OLED_ShowFloat(u8 x,u8 y,float num,u8 len,u8 size);
extern void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
//void IIC_Wait_Ack();

#endif

#if (defined OLED_PICTURE) && (OLED_PICTURE == TRUE)

extern void fill_picture(unsigned char fill_Data);
extern void Picture();
extern void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char *arg);
#endif

#if (defined OLED_RAM) && (OLED_RAM == TRUE)
extern u8 OLED_GRAM[128][8];
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 t);
void OLED_Refresh_Gram(void);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
#endif

#if (defined OLED_CHINESE) && (OLED_CHINESE == TRUE)
extern void OLED_ShowCHinese(u8 x,u8 y,u8 no);
#endif

#endif