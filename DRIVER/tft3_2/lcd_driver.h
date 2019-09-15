#ifndef __lcd_Driver__
#define __lcd_Driver__
#include "sys.h"
#include "fsmc_driver.h"
/* type change */
//typedef unsigned char       bool;
//typedef unsigned char       u8;
//typedef unsigned short      u16;

/*-------------------------------------------------------------------------------------------------------
*  向外接口	为应用层提供的接口										 
-------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/
//mcu
void Driver_MCU_Init_(void);
void Driver_OpenEA(void);
void Driver_CloseEA(void);
void Driver_SystemTickON(void);
//lcd
void Driver_LcdBacklight(bool sta);
void Driver_LcdReset(bool sta);
void Driver_LcdCS(bool sta);
void Driver_LcdRS(bool sta);
void Driver_LcdRD(bool sta);
void Driver_LcdWR(bool sta);
void Driver_LcdSendData(u16 Temp);
//touch
void Driver_SPICS(bool sta);
void Driver_SPISCK(bool sta);
void Driver_SPIMOSI(bool sta);
bool Driver_SPIMISO(void);
bool Driver_TouchTINTSta(void);
//flash
void Driver_FlashRead(u32 Addr, u8 *pData, u16 Len);
void Driver_FlashWrite(u32 Addr, u8 *pData, u16 Len);
//key
bool Driver_Key1Sta(void);
bool Driver_Key2Sta(void);
#endif



