/*--------------------------------------------------------------------------
// �ļ�����Touch.c
// ������  ���败�������
// ����ߣ�EU���� 
// ����EU���ӳ�Ʒ-��Ȩ����-����ؾ�
// EU-�Ȱ�Ƕ��ʽ����
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/* ͷ�ļ����� INCLUDES */
#include "Touch.h"
/*-------------------------------------------------------------------------------------------------------
*  ��Դ����											 
-------------------------------------------------------------------------------------------------------*/


//��ǰ������ʾ��ɫֵ
u16 TouchColor;
static u8 TouchPreSta  = 0; // 0, ������û�а��£�,1 �����������ˡ�
static u8 OneTouchFinishFlag = 0; //0, ����������û���ͷţ�1�����²����ͷ���
//��Ļ�ֱ���
#define LCD_ROWNUM 320
#define LCD_COLUMNNUM 240

//X Yֵ��������
enum{
	X_TYPE = 1,
	Y_TYPE = 2
};
//��ֵ��������
enum{
	NUM_ADD = 1,
	NUM_SUBB = 2
};

//----------- ����У�� ����ⲿ�ֵĲ��� �ɽ���д������������ ---------
//����ROW��ֵ���� ����ֵ�仯
//��X����Y�仯
u8 RowChgXYType;
//��ֵ�Ǳ���Ǳ�С
u8 RowChgNumType;
//��Χֵ�Ƕ���
u16 RowChgNumMAX;
u16 RowChgNumMIN;

//����COLUMN��ֵ���� ����ֵ�仯
//��X����Y�仯
u8 ColumnChgXYType;
//��ֵ�Ǳ���Ǳ�С
u8 ColumnChgNumType;
//��Χֵ�Ƕ���
u16 ColumnChgNumMAX;
u16 ColumnChgNumMIN;
//------------------------------------------------------------------
//��ǰ������ʾ��ɫֵ
u16 TouchColor;
//��ǰ����λ��ֵ
bool TouchPointNewFlag = False;
u16 TouchRow, TouchColumn;

//ȫ�ֱ��������ڱ��津����У׼����
typedef struct
{
	u8   CalFlag; // ������У׼��ǣ�0xAA�� �Ѿ�У׼������ֵ��û��У׼���ϵ��ʱ����Ҫ�ȶ�ȡ�˱������ж�
	float xFactor ;
	float yFactor ;
	short xOffset ;
	short yOffset ;
}TouchCalParameter ;
TOUCH_VAL_TYPE TouchVal;
TouchCalParameter TouchParameter;

//����У׼�õ�LCD����ֵ����ͬ����Ļ����ͨ���޸������ֵ��������
TOUCH_VAL_TYPE CalibraSamplePoint[4]=
{
	{30,30},  // point1
	{30,210}, // point2
	{290,30}, // point3
	{290,210} // point4

};


/*-------------------------------------------------------------------------------------------------------
*  ����IC������					 
-------------------------------------------------------------------------------------------------------*/
/*
�ڴ������ļ����У�ˮƽ�����ʾX�ᣬ��ֱ�����ʾY�ᣬ
������EU��3.2TFT��ͼ�����У�ˮƽ�����ʾY�ᣬ��ֱ�����ʾX�� (X���ʾ�ڼ��У�Y���ʾ�ڼ���)
(LCDx , LCDy) ΪTFT��Ļ�ϵ����ص㡣 (Pointx,Pointy)Ϊ����������ʱ��������������ֵ
LCDx=yFactor*Pointy+yOffset; 
LCDy=xFactor*Pointx+xOffset;
*/

//�޸������ֵ����������У׼�ľ��ȣ�������ߣ�У׼���Ѷ�Ҳ������
#define ERROROFFSETRANGE 50
/********************************************************************************************************
*  Function: Touch_Calibration						                                                           
*  Object: ������У׼ִ�к���
*  ���룺 ��
*  ����� ��                               
*  ��ע��                           
********************************************************************************************************/
void Touch_Calibration(void)
{
		TOUCH_VAL_TYPE Point[4];//���水�µ��ĸ���Ĵ���ֵ
		u8 TPCnt = 0;
		u8 *pToucPara;
		u16 Y1Y2OffsetRange, Y3Y4OffsetRange, X1X3OffsetRange, X2X4OffsetRange; //���水�µĵ��ƫ����
		
		LCD_ClrScr(WHITE);
		Touch_DrawCross(CalibraSamplePoint[0],RED);//�����Ͻǣ�����һ�� + 
		//����ʾ��
		LCD_DisASCString16x8(140, 15, (u8*)"Touch Screen Need Calibrate,", BLUE, WHITE);	
		LCD_DisASCString16x8(140+16, 15 , (u8*)"Please Press the Cross to Start", BLUE, WHITE);
		memset(&TouchParameter,0,sizeof(TouchParameter));//У׼֮ǰ���������
		delay_ms(300);
		while(1)
		{	
				delay_ms(300);
				Touch_GetPressPointVal(&(Point[TPCnt].y),&(Point[TPCnt].x));//��ȡ���������µ�ֵ
				if(TPCnt>=4)
				{
						delay_ms(1500);//У׼�Ѿ��ɹ���������ʱ���˳�У׼����
						break;		
				}
				if(OneTouchFinishFlag == 1)//������������һ�Σ������Ѿ��ɿ��ˡ�
				{
						OneTouchFinishFlag = 0;		
						TPCnt ++;
						switch(TPCnt)
						{
							case 1:
								Touch_DrawCross(CalibraSamplePoint[0],WHITE);//�����1
								Touch_DrawCross(CalibraSamplePoint[1],RED);	 //����2
								break;
							case 2:
								Touch_DrawCross(CalibraSamplePoint[1],WHITE);//�����2
								Touch_DrawCross(CalibraSamplePoint[2],RED);		//����3			
								break;
							case 3:
								Touch_DrawCross(CalibraSamplePoint[2],WHITE);//�����3
								Touch_DrawCross(CalibraSamplePoint[3],RED);		//����4			
								break;
							case 4: //�ĸ����Ѿ���ȡ�ɹ��������ж���Щ���Ƿ����Ҫ��
											//�ж�
								Y1Y2OffsetRange = abs(Point[0].y - Point[1].y);
								Y3Y4OffsetRange = abs(Point[2].y - Point[3].y);
								X1X3OffsetRange = abs(Point[0].x - Point[2].x);
								X2X4OffsetRange = abs(Point[1].x - Point[3].x);
								//���ݺϸ�	
								//X1-X3<С�ڹ̶�ֵ
								//X2-X4<С�ڹ̶�ֵ
								//Y1-Y2<С�ڹ̶�ֵ
								//Y3-Y4< С�ڹ̶�ֵ
							
								if(Y1Y2OffsetRange>ERROROFFSETRANGE || Y3Y4OffsetRange>ERROROFFSETRANGE || 
									 X1X3OffsetRange>ERROROFFSETRANGE || X2X4OffsetRange>ERROROFFSETRANGE )
								{
										//����㲻����Ҫ�����»���һ���㣬����У׼
										LCD_ClrScr(WHITE);
										Touch_DrawCross(CalibraSamplePoint[3],WHITE);	//�����4 
										Touch_DrawCross(CalibraSamplePoint[0],RED);//����һ�� + 
										TPCnt = 0;
										LCD_DisASCString16x8(140, 30, (u8*)"Calibrate Failed, Again", BLUE, WHITE);	
								}
								else //���µĴ��������������Χ֮�ڡ����� Offset �� Factor 
								{	
									//����X�᷽��ı������� xFactor = (Y0 - Y1)/(x0 - x1);  
									TouchParameter.xFactor = (float)(CalibraSamplePoint[0].y - CalibraSamplePoint[3].y)/(Point[0].x - Point[3].x);
									//����X�᷽���ƫ���� xOffset = [(Y0 + Y1)-xFactor(x0 + x1)]/2;
									TouchParameter.xOffset = (short)((CalibraSamplePoint[0].y + CalibraSamplePoint[3].y) - TouchParameter.xFactor*(Point[0].x + Point[3].x))/2;
									//����Y�᷽��ı�������  yFactor = (X0 - X1)/(y0 - y1);
									TouchParameter.yFactor = (float)(CalibraSamplePoint[0].x - CalibraSamplePoint[3].x)/(Point[0].y - Point[3].y);
									//����Y�᷽���ƫ���� YOffset = [(X0 + X1)-yFactor(y0 + y1)]/2;
									TouchParameter.yOffset = (short)((CalibraSamplePoint[0].x + CalibraSamplePoint[3].x) - TouchParameter.yFactor*(Point[0].y + Point[3].y))/2;
									LCD_ClrScr(WHITE);
									LCD_DisASCString16x8(140, 15, (u8*)"Touch Screen Calibrate OK", BLUE, WHITE);
									TouchParameter.CalFlag = 0xAA;//д�� 0xAA����Ǵ����Ѿ�У׼�ɹ�
									pToucPara = (u8*)&TouchParameter;
									Driver_FlashWrite((u32)0,pToucPara, sizeof(TouchParameter)); //��У׼�õ�ֵ��������ROM �ڲ���Flash�У������ַ��	DRIVER_FLASHPAGE_ADDRESS						
								}
								break;									
						}
			}
		}	
}

/********************************************************************************************************
*  Function: Touch_GetPressPointVal						                                                           
*  Object: ���������������ȡ����
*  ���룺 u16 *pPointY �� ���水�µĴ������Y���꣬ u16 *pPointX �����水�µĴ������X���꣬ 
*  ����� OneTouchFinishFlag ��ֵ                               
*  ��ע��                           
********************************************************************************************************/
int Touch_GetPressPointVal(u16 *pPointY, u16 *pPointX)
{
		u8 i,j;
		u16 Temp;
		TOUCH_VAL_TYPE Point[10];
		if(Driver_TouchTINTSta()== 0)//�������а�������
		{
		
				if(TouchPreSta == 0)     //֮ǰû�б�����
				{	
						TouchPreSta = 1;		//��¼�����������ˡ�
						OneTouchFinishFlag = 0 ;
					//��10�δ�������ֵ
					for(i = 0; i < 10; i++)
						Touch_GetVal(&Point[i]);	
						//���� ȥ��ǰ2����ֵ
						for(j = 0; j < 2; j++)
						{
								for(i = 0; i < 10-1; i++)
								{
										if(Point[i].x > Point[i+1].x)
										{
												Temp = Point[i+1].x;
												Point[i+1].x = Point[i].x;
												Point[i].x = Temp;
										}
										if(Point[i].y > Point[i+1].y)
										{
												Temp = Point[i+1].y;
												Point[i+1].y = Point[i].y;
												Point[i].y = Temp;
										}
								}
						}
						//���� ȥ��ǰ2��Сֵ
						for(j = 0; j < 2; j++)
						{
								for(i = 0; i < 10-1; i++)
								{
										if(Point[i].x < Point[i+1].x)
										{
												Temp = Point[i+1].x;
												Point[i+1].x = Point[i].x;
												Point[i].x = Temp;
										}
										if(Point[i].y < Point[i+1].y)
										{
												Temp = Point[i+1].y;
												Point[i+1].y = Point[i].y;
												Point[i].y = Temp;
										}
								}
						}
						//ʣ����ƽ��ֵ
						for(i = 1; i < 6; i++)
							Point[0].x += Point[i].x;
						Point[0].x /= 6;
						for(i = 1; i < 6; i++)
							Point[0].y += Point[i].y;
						Point[0].y /= 6;	
						//���津���������ֵ
						*pPointY = Point[0].y;
						*pPointX = Point[0].x;		
							
				}
				
		}
		else// �����������ɿ�
		{
			if(TouchPreSta == 1) //֮ǰ���������¡� 
			{
				TouchPreSta = 0;
				OneTouchFinishFlag = 1 ; //�������ɿ���
			}
		}
			return OneTouchFinishFlag;
}

/********************************************************************************************************
*  Function: Touch_GetSite						                                                           
*  Object: �������ݲ������� ��ת��ΪLCD��Ļ�ϵ�����
*  ���룺 u16 *pRow �� LCD�У� u16 *pColumn ��LCD��
*  ����� 1/0  1: ��������ת���ɹ�  0 �� ����û�а���                           
*  ��ע��                           
********************************************************************************************************/
int Touch_GetSite(u16 *pRow, u16 *pColumn)
{
		u8 i,j;
		u16 Temp;
		TOUCH_VAL_TYPE Point[10];
		if(Driver_TouchTINTSta()== 0)//�������а������� ��TINT����Ϊ�͵�ƽ
		{
				//��10�δ�������ֵ
				for(i = 0; i < 10; i++)
					Touch_GetVal(&Point[i]);
				//���� ȥ��ǰ2����ֵ
				for(j = 0; j < 2; j++)
				{
						for(i = 0; i < 10-1; i++)
						{
								if(Point[i].x > Point[i+1].x)
								{
										Temp = Point[i+1].x;
										Point[i+1].x = Point[i].x;
										Point[i].x = Temp;
								}
								if(Point[i].y > Point[i+1].y)
								{
										Temp = Point[i+1].y;
										Point[i+1].y = Point[i].y;
										Point[i].y = Temp;
								}
						}
				}
				//���� ȥ��ǰ2��Сֵ
				for(j = 0; j < 2; j++)
				{
						for(i = 0; i < 10-1; i++)
						{
								if(Point[i].x < Point[i+1].x)
								{
										Temp = Point[i+1].x;
										Point[i+1].x = Point[i].x;
										Point[i].x = Temp;
								}
								if(Point[i].y < Point[i+1].y)
								{
										Temp = Point[i+1].y;
										Point[i+1].y = Point[i].y;
										Point[i].y = Temp;
								}
						}
				}
				//ʣ����ƽ��ֵ
				for(i = 1; i < 6; i++)
					Point[0].x += Point[i].x;
				Point[0].x /= 6;
				for(i = 1; i < 6; i++)
					Point[0].y += Point[i].y;
				Point[0].y /= 6;		
				//������ADCֵ ת��Ϊ����Ӧ��������
				//LCDx=yFactor*Pointy+yOffset; 
				//LCDy=xFactor*Pointx+xOffset;				
				*pRow = (short)(TouchParameter.yFactor * (float)Point[0].y + TouchParameter.yOffset );
				*pColumn = (short)(TouchParameter.xFactor * (float)Point[0].x + TouchParameter.xOffset );	
				return 1;
	}
	else
			 return 0;
}
/********************************************************************************************************
*  Function: TouchHandle						                                                           
*  Object: ���庯�����Ѵ������µ�ֵ��LCD������ʾ����������ѡ���ٴν��봥��У׼ģʽ
*  ���룺 ��
*  ����� ��                        
*  ��ע�� �Ȱ��´����� Ȼ����KEY2 ��ͬʱ���֣����Խ��봥��У׼ģʽ��                          
********************************************************************************************************/
void TouchHandle(void)
{
		u16 Row, Column;
		int Back;
		while(1)
		{
				static u16 RowOld, ColumnOld;
			  static bool CheckEN = False;
				//���������ֵ
				Back = Touch_GetSite(&Row, &Column);
				//�״ε����Ļ��ʱ����
				if(!Back)
				{
						CheckEN = False;
						continue;
				}
				else
				{
						if(!CheckEN)
						{
								delay_ms(20);
								CheckEN = True;
								Touch_GetSite(&RowOld, &ColumnOld);
								continue;
						}
				}
				//ƽ���㷨���� ����
				if(Back)
				{
						if(CheckEN)
						{
								//����Ծ�ؼ��
								if((RowOld > Row) && (RowOld - Row > 5))
									continue;
								if((RowOld < Row) && (Row - RowOld > 5))
									continue;
								//����Ծ�ؼ��
								if((ColumnOld > Column) && (ColumnOld - Column > 5))
									continue;
								if((ColumnOld < Column) && (Column - ColumnOld > 5))
									continue;
						}
						RowOld = Row;
						ColumnOld = Column;
				}
				//��ȡ����λ�ò���TFT�Ͻ��л���
				if(Back)   
				{
						//ֻ�ڴ�����ʾ������Ч
						if(Row < 275)
							LCD_DisASquare(Row, Column, 2, TouchColor);
						//�����CLR��ִ������
						if(Row > 276 && Row < 320 && Column > 180 && Column < 240)
						{
								LCD_DisARectangular(276,180,319,239,BLACK);
								delay_ms(100);
								LCD_DisARectangular(276,180,319,239,GRAY);
								LCD_DisARectangular(0,0,275,239,WHITE);
								LCD_DisASCString16x8(292, 198, (u8*)"CLR", BLACK, GRAY);
						}	
						//ѡ��RED
						if(Row > 276 && Row < 320 && Column > 0 && Column < 60)
							Touch_ChoseColor(RED);
						//ѡ��GREEN
						if(Row > 276 && Row < 320 && Column > 60 && Column < 120)
							Touch_ChoseColor(GREEN);
						//ѡ��BLUE
						if(Row > 276 && Row < 320 && Column > 120 && Column < 180)
							Touch_ChoseColor(BLUE);
				}
		}
}
//��ָ��������λ�û�һ��ʮ�֣����ڱ�ʾУ׼�Ĵ������
void Touch_DrawCross(TOUCH_VAL_TYPE CalibraSamplePoint,u16 Color)
{
	LCD_DisALine(CalibraSamplePoint.x ,CalibraSamplePoint.y-10,1,20,Color);
	LCD_DisALine(CalibraSamplePoint.x-10,CalibraSamplePoint.y,2,20,Color);	

}

//ѡ������ɫ��������ʾ
void Touch_ChoseColor(u16 Val)
{
		u16 a = RED, b = GREEN, c = BLUE;
		switch(Val)
		{
				case RED:  	TouchColor = RED;   a = BLACK;  break;
			  case GREEN: TouchColor = GREEN; b = BLACK;  break;
				case BLUE:  TouchColor = BLUE;  c = BLACK;  break;
		}
		//��ʾѡ���
		LCD_DisABorder(276, 0, 319, 59, a);
		LCD_DisABorder(277, 1, 318, 58, a);
		LCD_DisABorder(276, 60, 319, 119, b);
		LCD_DisABorder(277, 61, 318, 118, b);
		LCD_DisABorder(276, 120, 319, 179, c);
		LCD_DisABorder(277, 121, 318, 178, c);
}

//��SPI�ӿڷ�������
void Touch_SPIWrite(u8 Val)
{
    u8 i;
    Driver_SPISCK(0);
    for(i=0; i<8; i++)
		{
				if(Val&CHSBIT7)
					Driver_SPIMOSI(1);
				else
					Driver_SPIMOSI(0);
				Val <<= 1;
        Driver_SPISCK(0);
        __NOP();
				__NOP();
				__NOP();
        Driver_SPISCK(1);
        __NOP();
				__NOP();
				__NOP();
    }
}

//��SPI�ӿڽ�������
u16 Touch_SPIRead(void)
{
    u8 i;
    u16 Val = 0;
    for(i=0; i<12; i++)
		{
        Val <<= 1;
        Driver_SPISCK(1);
        __NOP();
				__NOP();
				__NOP();
        Driver_SPISCK(0);
        __NOP();
				__NOP();
				__NOP();
        if(Driver_SPIMISO())
					Val++;
    }
    return Val;
}

//��һ�δ������ֵ
void Touch_GetVal(TOUCH_VAL_TYPE *Point)
{
    Driver_SPICS(0);
    Touch_SPIWrite(0x90);   //��ȡY�� ���ֵ
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    (*Point).y = Touch_SPIRead();
    Touch_SPIWrite(0xd0);  //��ȡX�� ���ֵ
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    (*Point).x = Touch_SPIRead();
    Driver_SPICS(1);
}
//���뾭У׼���Ĵ�����Ļ ���ֵ
//��������ò���׼ȷ ��ִ����Ļ������У Ȼ�󽫵�У��ʾ��� ��ֵ���·�����
void LoadTouchFaceVal(void)
{
				//ROW
				RowChgXYType = 2;
				RowChgNumType = 1;
				RowChgNumMAX = 3931;  
				RowChgNumMIN = 360; 
				//COLUMN
				ColumnChgXYType = 1;
				ColumnChgNumType = 1;
				ColumnChgNumMAX = 3918; 
				ColumnChgNumMIN = 158; 
}
//��Row�� Column�д�Ϊ���ĵ㣬��ʾһ��Color��ɫ��ʮ���� ʮ���Ǻ�������Ϊ16�����ص�
void Dis_AXPoint(u16 Row, u16 Column, u16 Color)
{
		LCD_DisALine(Row-1, Column-7, 1, 14, Color);
		LCD_DisALine(Row,   Column-8, 1, 16, Color);
		LCD_DisALine(Row+1, Column-7, 1, 14, Color);
		LCD_DisALine(Row-7, Column-1, 2, 14, Color);
		LCD_DisALine(Row-8, Column,   2, 16, Color);
		LCD_DisALine(Row-7, Column+1, 2, 14, Color);
		LCD_DisAPoint(Row, Column, WHITE);
}
//��������ʼ��
void Touch_FaceInit(void)
{	
		//��ʾ��������
		LCD_ClrScr(WHITE);
		LCD_DisARectangular(276,0,319,59,RED);
		LCD_DisARectangular(276,60,319,119,GREEN);
		LCD_DisARectangular(276,120,319,179,BLUE);
		LCD_DisARectangular(276,180,319,239,GRAY);
		LCD_DisASCString16x8(292, 198, "CLR", BLACK, GRAY);
		//��ʼ��ѡ���
		Touch_ChoseColor(RED);
}
//Ӧ�ò㻭�ߴ���
void TouchDrawHandle(void)
{
		u16 Row, Column;
		#define KEY_LONGTIME 2000  /*�����ⲿ����1 ���봥����У���� ʱ��ֵ�趨 */
		//static u16 KeyTimer = KEY_LONGTIME;   
		while(1)
		{
				//ִ�д�����ֵ����
				GetSiteHandle();
				//ִ�л��ߴ���
				if(TouchPointNewFlag == True)   
				{
						//��ô�������ֵ
						Row = TouchRow;
						Column = TouchColumn;
						//ֻ�ڴ�����ʾ������Ч
						if(Row < 275)
							LCD_DisASquare(Row, Column, 2, TouchColor);
						//�����CLR��ִ������
						if(Row > 276 && Row < 320 && Column > 180 && Column < 240)
						{
								LCD_DisARectangular(276,180,319,239,BLACK);
								delay_ms(100);
								LCD_DisARectangular(276,180,319,239,GRAY);
								LCD_DisARectangular(0,0,275,239,WHITE);
								LCD_DisASCString16x8(292, 198, "CLR", BLACK, GRAY);
						}	
						//ѡ��RED
						if(Row > 276 && Row < 320 && Column > 0 && Column < 60)
							Touch_ChoseColor(RED);
						//ѡ��GREEN
						if(Row > 276 && Row < 320 && Column > 60 && Column < 120)
							Touch_ChoseColor(GREEN);
						//ѡ��BLUE
						if(Row > 276 && Row < 320 && Column > 120 && Column < 180)
							Touch_ChoseColor(BLUE);
						TouchPointNewFlag = False;
				}
				delay_ms(1);
		}
}

//��ȡ��ǰ�Ĵ���λ��ֵ ÿ��1ms��������һ��
void GetSiteHandle(void)
{
 		enum{
				REDA_FIRST = 0,
				READ_SECOND,
				READ_CHECK,
				READ_WAIT
		};
		static u8 TickHStep = REDA_FIRST;
		static u16 TouchPoint1X, TouchPoint1Y, TouchPoint2X, TouchPoint2Y;
		static u16 TouchSiteRow1, TouchSiteColumn1, TouchSiteRow2, TouchSiteColumn2;
		switch(TickHStep)
		{
				//�ȴ����
				case READ_WAIT:
					{
							if(TouchPointNewFlag == True)
								break;
							else
								TickHStep = 0;
					}
				//�ȶ�һ����
				case REDA_FIRST:
					{
							Touch_GetVal_(&TouchPoint1X, &TouchPoint1Y);   /* ��ȡ����X Yֵ */
							if(GetScrSite_(TouchPoint1X, TouchPoint1Y, &TouchSiteRow1, &TouchSiteColumn1))  /* ������X Yֵת��Ϊ��Ļ����ֵ */
								TickHStep++;
							else
								break;
					} 
				//�ٶ�һ����
				case READ_SECOND:
					{
							Touch_GetVal_(&TouchPoint2X, &TouchPoint2Y);   /* ��ȡ����X Yֵ */
							if(GetScrSite_(TouchPoint2X, TouchPoint2Y, &TouchSiteRow2, &TouchSiteColumn2))  /* ������X Yֵת��Ϊ��Ļ����ֵ */
								TickHStep++;
							else
								break;
					}
        //�жϵ����Ч�� ��Ծ��Ϊ��� ȥ��				
				case READ_CHECK: 
					{	
							//����Ծ�ؼ��
							if((TouchSiteRow2 > TouchSiteRow1) && (TouchSiteRow2 - TouchSiteRow1 > 3))
							{
									TickHStep = 0;
									TouchPointNewFlag = False;
									break;
							}
							if((TouchSiteRow2 < TouchSiteRow1) && (TouchSiteRow1 - TouchSiteRow2 > 3))
							{
									TickHStep = 0;
									TouchPointNewFlag = False;
									break;
							}
							//����Ծ�ؼ��
							if((TouchSiteColumn2 > TouchSiteColumn1) && (TouchSiteColumn2 - TouchSiteColumn1 > 3))
							{
									TickHStep = 0;
									TouchPointNewFlag = False;
									break;
							}
							if((TouchSiteColumn2 < TouchSiteColumn1) && (TouchSiteColumn1 - TouchSiteColumn2 > 3))
							{
									TickHStep = 0;
									TouchPointNewFlag = False;
									break;
							}
							//�õ���Ч�Ĵ������ֵ ����ƽ��ֵ����
							TouchRow = (TouchSiteRow1+TouchSiteRow2)/2;
							TouchColumn = (TouchSiteColumn1+TouchSiteColumn2)/2;
							TouchPointNewFlag = True;
							TickHStep++;							
					}	
					break;
		}	
}
//��һ�δ������ֵ
void Touch_GetVal_(u16 *X, u16 *Y)
{
    Driver_SPICS(0);
    Touch_SPIWrite(0x90);   //��ȡY�� ���ֵ
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    *Y = Touch_SPIRead();
    Touch_SPIWrite(0xd0);  //��ȡX�� ���ֵ
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    *X = Touch_SPIRead();
    Driver_SPICS(1);
}
//ͨ������AD��õ�X Yֵ, ������Ļ��ָ�������λ��ֵ
//��������1 ��ʾ�����Ч��λ�� ����0 ��ʾ����λ����Ч ���߿հ�
bool GetScrSite_(u16 X, u16 Y, u16 *Row, u16 *Column)
{
		if(RowChgXYType == X_TYPE)
		{
				if((X > RowChgNumMIN && X < RowChgNumMAX) && (Y > ColumnChgNumMIN && Y < ColumnChgNumMAX))
				{
						//����ROW
						if(RowChgNumType == NUM_ADD)
						{
								*Row = (X-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
						}
						else
						{
								*Row = (X-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
								*Row = LCD_ROWNUM - *Row;
						}
						//����COLUMN
						if(ColumnChgNumType == NUM_ADD)
						{
								*Column = (Y-ColumnChgNumMIN)/((ColumnChgNumMAX - ColumnChgNumMIN)/LCD_COLUMNNUM);
						}
						else
						{
								*Column = (Y-ColumnChgNumMIN)/((ColumnChgNumMAX - ColumnChgNumMIN)/LCD_COLUMNNUM);
								*Column = LCD_COLUMNNUM - *Column;
						}
						return 1;
				}
				return 0;
		}
		else
		{
				if((Y > RowChgNumMIN && Y < RowChgNumMAX) && (X > ColumnChgNumMIN && X < ColumnChgNumMAX))
				{
						//����ROW
						if(RowChgNumType == NUM_ADD)
						{
								*Row = (Y-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
						}
						else
						{
								*Row = (Y-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
								*Row = LCD_ROWNUM - *Row;
						}
						//����COLUMN
						if(ColumnChgNumType == NUM_ADD)
						{
								*Column = (X-ColumnChgNumMIN)/((ColumnChgNumMAX - ColumnChgNumMIN)/LCD_COLUMNNUM);
						}
						else
						{
								*Column = (X-ColumnChgNumMIN)/((ColumnChgNumMAX - ColumnChgNumMIN)/LCD_COLUMNNUM);
								*Column = LCD_COLUMNNUM - *Column;
						}
						return 1;
				}
				return 0;
		}
}