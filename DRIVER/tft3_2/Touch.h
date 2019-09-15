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
// �ļ�����Touch.h
// ������  ���败�������
// ����ߣ�EU����
// �������ڣ�2015��10��16��
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/* ͷ�ļ����� INCLUDES */
/* type change */
//typedef unsigned char       bool;
//typedef unsigned char       u8;
//typedef unsigned short      u16;
//�������ֵ ��������
typedef struct{
		u16 x;
		u16 y;	
}TOUCH_VAL_TYPE;

/*-------------------------------------------------------------------------------------------------------
*  ������У���㴦��		 
-------------------------------------------------------------------------------------------------------*/


//��������
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

