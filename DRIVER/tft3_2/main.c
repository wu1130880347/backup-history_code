//***************************************************************
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
// ��ע ��EU���ӡ� ΢�Ź��ںţ���ѻ�ȡ��������
//***************************************************************
#include "prohead.h"
#include "lcd_driver.h" 
#include "lcdlib.h"
//#include "qqImage.h"

void DIS_Delayms(u16 Num);


//***************************************************************
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
//***************************************************************

/*-------------------------------------------------------------------------------------------------------
*  ���������ִ��												 
-------------------------------------------------------------------------------------------------------*/
int main(void)
{
	Driver_MCU_Init();
	//TFT��ʼ��
	LCD_Init();
	//������ ����
	Driver_LcdBacklight(True);
	LCD_ClrScr(GREEN);
	//����LOGO
	LCD_DisARectangular(70+46, 20+29, 70+30+46, 10+60+29, YELLOW);
	LCD_DisARectangular(63+46, 28+29, 63+32+46, 28+50+29, GREEN);
	LCD_DisABorder(63+46, 28+29, 63+32+46, 28+50+29, WHITE);
	LCD_DisASCString16x8(72+46, 31+29, "EU", WHITE, GREEN);
	LCD_DisGB2312String16x16(72+46, 47+29, "����", WHITE, GREEN);
	LCD_DisGB2312String16x16(72+46, 90+29, "��ת����", RED, YELLOW);
	LCD_DisGB2312String16x16(100+46, 50+29, "�Ȱ�Ƕ��ʽ����", WHITE, GREEN);
	LCD_DisASCString16x8(120+46, 10+29, "http://euse.taobao.com", BLACK, GREEN);
	DIS_Delayms(1000);
	//ɫ����ʾ
	while(1)
	{
			LCD_ClrScr(BLACK);
			LCD_DisGB2312String16x16(15+46, 25+29, "ɫ�����", RED, BLACK);
			//LCD_DisARectangular(40+46, 0, 200+46, 239, WHITE);
			LCD_DisARectangular(40+46, 0+0+29, 200+46, 15+0+29, RED);
			LCD_DisARectangular(40+46, 16+0+29, 200+46, 31+0+29, ORANGE);
			LCD_DisARectangular(40+46, 32+0+29, 200+46, 47+0+29, YELLOW);
			LCD_DisARectangular(40+46, 48+0+29, 200+46, 63+0+29, GREEN);
			LCD_DisARectangular(40+46, 64+0+29, 200+46, 79+0+29, BLUE);
			LCD_DisARectangular(40+46, 80+0+29, 200+46, 95+0+29, LIGHTBLUE);
			LCD_DisARectangular(40+46, 96+0+29, 200+46, 111+0+29, MAGENTA);
			LCD_DisARectangular(40+46, 112+0+29, 200+46, 127+0+29, CYAN);
			LCD_DisARectangular(40+46, 128+0+29, 200+46, 143+0+29, PURPLE); 
			LCD_DisARectangular(40+46, 144+0+29, 200+46, 159+0+29, GRAY);  
			LCD_DisARectangular(40+46, 160+0+29, 200+46, 175+0+29, WHITE);	 
			DIS_Delayms(1000);
			//�������
			LCD_ClrScr(BLACK);
			LCD_DisGB2312String16x16(15+46, 25+29, "�������", RED, BLACK);
			LCD_DisALine(115+46, 8+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 28+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 48+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 68+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 88+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 108+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 128+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 148+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALine(115+46, 168+29, 2, 20, GREEN);
			DIS_Delayms(30);
			LCD_DisALoop(125+46, 88+29, 10, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 20, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 30, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 40, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 50, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 60, RED);
			DIS_Delayms(20);
			LCD_DisALoop(125+46, 88+29, 70, RED);
			DIS_Delayms(1000);
			//ͼ����ʾ
			LCD_ClrScr(BLACK);
			LCD_DisGB2312String16x16(15+46, 25+29, "��ͼ��ʾ", RED, BLACK);
			LCD_DisARectangular(40+66, 0, 200+36, 239, WHITE);
			LCD_DisAPhoto(100+46,27+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(100+46,59+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(100+46,91+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(100+46,123+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(130+46,27+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(130+46,59+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(130+46,91+29,23,23,(u8*)gImage_QQ);
			LCD_DisAPhoto(130+46,123+29,23,23,(u8*)gImage_QQ);
			DIS_Delayms(1000);
	}
}

/*-------------------------------------------------------------------------------------------------------
*  ��ʾ��ʱ												 
-------------------------------------------------------------------------------------------------------*/
void DIS_Delayms(u16 Num)
{
   	u16 Timer;
		while(Num--)
		{
		 	Timer = 60000;
			while(Timer--); 
		}
}																							


