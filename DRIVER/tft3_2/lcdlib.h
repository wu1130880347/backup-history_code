#ifndef __Lcdlib__
#define __Lcdlib__
/*--------------------------------------------------------------------------
// �ļ�����Lcdlib.h
// ������  LCD��ͷ�ļ�
// ����ߣ�EU����
// �������ڣ�2013��10��16��
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------
*  ��������					 
-------------------------------------------------------------------------------------------------------*/
//��ɫֵ
#include "sys.h"
//#include "Source.h"

#define BLACK          0x0000   //��

#define RED            0xF800   //��  
#define ORANGE 		   	 0xFB00   //��
#define YELLOW         0xFFE0   //��
#define GREEN          0x07E0   //��
#define BLUE           0x001F   //��
#define PURPLE		   	 0x881F   //��
#define GRAY  		     0X8430   //��
#define WHITE          0xFFFF   //��

#define GOLDEN         0XBC40   //��
#define LIGHTBLUE      0x051F   //ǳ��
#define MAGENTA        0xF81F   //����
#define CYAN           0x7FFF   //��
//----- RGB565ԭ�� ------
//xxxxxxxx xxxxxxxx
//rrrrrggg gggbbbbb
//11111000 00000000 �� 0xF800
//00000111 11100000 �� 0x07E0
//00000000 00011111 �� 0x001F

//lcd direction
#define  LCD_DIRECT        2           		   //1 ����  2 ����

//lcd bus chose
#define  LCD_BUSTYPE       1                 //1:16λ����  2:8λ����

/*-------------------------------------------------------------------------------------------------------
*  ����ӿ�								 
-------------------------------------------------------------------------------------------------------*/
//------------------------- APP USER LIB -----------------------------
//lcd��ʼ��
extern void LCD_Init(void);
extern void LCD_OpenWin(u16 x0, u16 y0, u16 x1, u16 y1);
//lcd����Ļ
extern void LCD_ClrScr(u16 BackColor);

//--------------------------------------------------------------------
//---------------- ��Ļ���Ͻ�Ϊԭ��,xָ��, yָ�� ---------------------
//--------------------------------------------------------------------
//------------------------- �� -----------------------------
//��x0,y0λ����ʾһ����ɫΪColor��
extern void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);

//------------------------- �� -----------------------------
//��x0,y0λ����ʾһ��ֱ��(dir:����1/����2),����Ϊlenth,��ɫΪcolor
extern void LCD_DisALine(u16 x0, u16 y0, u8 dir, u16 lenth, u16 color);
//��x0,y0 ~ �Խ�x1,y1,��һ����ɫΪColor�ľ��α߿�
extern void LCD_DisABorder(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);
//��x0,y0λ�û�һ���뾶Ϊr,��ɫΪColor��Բ��
extern void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);

//------------------------- �� -----------------------------
//��x0,y0λ�ÿ�ʼ,��ʾһ�����Ϊwide,��ɫΪColor��������
extern void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);   
//��x0,y0 ~ x1,y1λ�ô�,��ʾһ����ɫΪColor�ĳ�����
extern void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);

//--------------------- Ӣ���ַ� ---------------------------
//��x0,y0λ����ʾһ��ASC�ַ�,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisASCString16x8(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//--------------------- �����ַ� ---------------------------
//��x0,y0λ����ʾһ�����ַ���,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisGB2312String16x16(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);
//��x0,y0λ����ʾһ�����ַ���,��ɫΪfColor,����ɫΪbColor
extern void LCD_DisGB2312String32x32(u16 x0, u16 y0, u8 *s, u16 fColor, u16 bColor);

//------------------------ ͼƬ -----------------------------
//��x0,y0λ����ʾһ����Ϊhigh,��Ϊwide��ͼƬ, ͼƬ��Ϣ��RGB565��֯(��˸�ʽÿ�����ص�ռ��2���ֽ�)
//ͼƬ��Ϣͷ��ַָ��ΪpData
extern void LCD_DisAPhoto(u16 x0, u16 y0, u16 high, u16 wide, u8 *pData);

void LCD_DisASC_Char16x8(u16 x0, u16 y0, u8 s, u16 fColor, u16 bColor);
void LCD_Show_Num(u16 x,u16 y,u32 num,u16 fColor, u16 bColor);  					//��ʾһ������
void LCD_Show_0Num(u16 x,u16 y,u32 num,u8 len,u16 fColor, u16 bColor);				//��ʾ ����
void LCD_Show_Float(u16 x,u16 y,float num,u8 len,u16 fColor, u16 bColor); 
#endif



