#ifndef __TOUCH__
#define __TOUCH__
#include "sys.h"
#include "delay.h"
#include "ProHead.h"
#include "lcd_driver.h"

#include "lcdlib.h"
#include "string.h"
#include "stdlib.h"
/*--------------------------------------------------------------------------
// 文件名：Touch.h
// 描述：  电阻触摸屏检测
// 设计者：EU电子
// 创建日期：2015年10月16日
// 深圳EU电子出品-版权所有-翻版必究
// EU-热爱嵌入式开发
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/* 头文件包含 INCLUDES */
/* type change */
//typedef unsigned char       bool;
//typedef unsigned char       u8;
//typedef unsigned short      u16;
//触摸检测值 数据类型
typedef struct{
		u16 x;
		u16 y;	
}TOUCH_VAL_TYPE;

/*-------------------------------------------------------------------------------------------------------
*  触摸调校计算处理		 
-------------------------------------------------------------------------------------------------------*/


//函数声明
void Touch_ChoseColor(u16 Val);
void TouchHandle(void);
void Touch_DrawCross(TOUCH_VAL_TYPE CalibraPoint,u16 Color);
int  Touch_GetPressPointVal(u16 *pPointY, u16 *pPointX);
void Touch_Calibration(void);
void Touch_GetVal(TOUCH_VAL_TYPE *Point);
void Touch_GetVal_(u16 *X, u16 *Y);
void LoadTouchFaceVal(void);
void Touch_FaceInit(void);
void TouchDrawHandle(void);
void GetSiteHandle(void);
bool GetScrSite_(u16 X, u16 Y, u16 *Row, u16 *Column);
#endif //__TOUCH__

