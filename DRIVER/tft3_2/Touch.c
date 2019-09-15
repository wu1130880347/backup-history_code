/*--------------------------------------------------------------------------
// 文件名：Touch.c
// 描述：  电阻触摸屏检测
// 设计者：EU电子 
// 深圳EU电子出品-版权所有-翻版必究
// EU-热爱嵌入式开发
// http://euse.taobao.com
//-------------------------------------------------------------------------*/
/* 头文件包含 INCLUDES */
#include "Touch.h"
/*-------------------------------------------------------------------------------------------------------
*  资源定义											 
-------------------------------------------------------------------------------------------------------*/


//当前触摸显示颜色值
u16 TouchColor;
static u8 TouchPreSta  = 0; // 0, 触摸屏没有按下，,1 触摸屏按下了。
static u8 OneTouchFinishFlag = 0; //0, 触摸屏按下没有释放，1，按下并且释放了
//屏幕分辨率
#define LCD_ROWNUM 320
#define LCD_COLUMNNUM 240

//X Y值类型声明
enum{
	X_TYPE = 1,
	Y_TYPE = 2
};
//数值增减类型
enum{
	NUM_ADD = 1,
	NUM_SUBB = 2
};

//----------- 经调校后 获得这部分的参数 可进行写参数保存起来 ---------
//随着ROW行值增加 触摸值变化
//是X还是Y变化
u8 RowChgXYType;
//数值是变大还是变小
u8 RowChgNumType;
//范围值是多少
u16 RowChgNumMAX;
u16 RowChgNumMIN;

//随着COLUMN列值增加 触摸值变化
//是X还是Y变化
u8 ColumnChgXYType;
//数值是变大还是变小
u8 ColumnChgNumType;
//范围值是多少
u16 ColumnChgNumMAX;
u16 ColumnChgNumMIN;
//------------------------------------------------------------------
//当前触摸显示颜色值
u16 TouchColor;
//当前触摸位置值
bool TouchPointNewFlag = False;
u16 TouchRow, TouchColumn;

//全局变量，用于保存触摸屏校准数据
typedef struct
{
	u8   CalFlag; // 触摸屏校准标记，0xAA： 已经校准，其他值，没有校准。上电的时候，需要先读取此变量做判断
	float xFactor ;
	float yFactor ;
	short xOffset ;
	short yOffset ;
}TouchCalParameter ;
TOUCH_VAL_TYPE TouchVal;
TouchCalParameter TouchParameter;

//定义校准用的LCD坐标值，不同的屏幕可以通过修改下面的值，来适配
TOUCH_VAL_TYPE CalibraSamplePoint[4]=
{
	{30,30},  // point1
	{30,210}, // point2
	{290,30}, // point3
	{290,210} // point4

};


/*-------------------------------------------------------------------------------------------------------
*  触摸IC检测代码					 
-------------------------------------------------------------------------------------------------------*/
/*
在触摸屏的计算中，水平方向表示X轴，垂直方向表示Y轴，
但是在EU的3.2TFT换图函数中，水平方向表示Y轴，垂直方向表示X轴 (X轴表示第几行，Y轴表示第几列)
(LCDx , LCDy) 为TFT屏幕上的像素点。 (Pointx,Pointy)为触摸屏按下时，读出来的坐标值
LCDx=yFactor*Pointy+yOffset; 
LCDy=xFactor*Pointx+xOffset;
*/

//修改下面的值，可以增加校准的精度，精度提高，校准的难度也会提升
#define ERROROFFSETRANGE 50
/********************************************************************************************************
*  Function: Touch_Calibration						                                                           
*  Object: 触摸屏校准执行函数
*  输入： 无
*  输出： 无                               
*  备注：                           
********************************************************************************************************/
void Touch_Calibration(void)
{
		TOUCH_VAL_TYPE Point[4];//保存按下的四个点的触摸值
		u8 TPCnt = 0;
		u8 *pToucPara;
		u16 Y1Y2OffsetRange, Y3Y4OffsetRange, X1X3OffsetRange, X2X4OffsetRange; //保存按下的点的偏移量
		
		LCD_ClrScr(WHITE);
		Touch_DrawCross(CalibraSamplePoint[0],RED);//（左上角）画第一个 + 
		//画提示语
		LCD_DisASCString16x8(140, 15, (u8*)"Touch Screen Need Calibrate,", BLUE, WHITE);	
		LCD_DisASCString16x8(140+16, 15 , (u8*)"Please Press the Cross to Start", BLUE, WHITE);
		memset(&TouchParameter,0,sizeof(TouchParameter));//校准之前，清楚数据
		delay_ms(300);
		while(1)
		{	
				delay_ms(300);
				Touch_GetPressPointVal(&(Point[TPCnt].y),&(Point[TPCnt].x));//读取触摸屏按下的值
				if(TPCnt>=4)
				{
						delay_ms(1500);//校准已经成功，稍作延时，退出校准函数
						break;		
				}
				if(OneTouchFinishFlag == 1)//触摸屏按下了一次，并且已经松开了。
				{
						OneTouchFinishFlag = 0;		
						TPCnt ++;
						switch(TPCnt)
						{
							case 1:
								Touch_DrawCross(CalibraSamplePoint[0],WHITE);//清除点1
								Touch_DrawCross(CalibraSamplePoint[1],RED);	 //画点2
								break;
							case 2:
								Touch_DrawCross(CalibraSamplePoint[1],WHITE);//清除点2
								Touch_DrawCross(CalibraSamplePoint[2],RED);		//画点3			
								break;
							case 3:
								Touch_DrawCross(CalibraSamplePoint[2],WHITE);//清除点3
								Touch_DrawCross(CalibraSamplePoint[3],RED);		//画点4			
								break;
							case 4: //四个点已经读取成功，下面判断这些点是否符合要求。
											//判断
								Y1Y2OffsetRange = abs(Point[0].y - Point[1].y);
								Y3Y4OffsetRange = abs(Point[2].y - Point[3].y);
								X1X3OffsetRange = abs(Point[0].x - Point[2].x);
								X2X4OffsetRange = abs(Point[1].x - Point[3].x);
								//数据合格：	
								//X1-X3<小于固定值
								//X2-X4<小于固定值
								//Y1-Y2<小于固定值
								//Y3-Y4< 小于固定值
							
								if(Y1Y2OffsetRange>ERROROFFSETRANGE || Y3Y4OffsetRange>ERROROFFSETRANGE || 
									 X1X3OffsetRange>ERROROFFSETRANGE || X2X4OffsetRange>ERROROFFSETRANGE )
								{
										//如果点不符合要求，重新画第一个点，重新校准
										LCD_ClrScr(WHITE);
										Touch_DrawCross(CalibraSamplePoint[3],WHITE);	//清除点4 
										Touch_DrawCross(CalibraSamplePoint[0],RED);//画第一个 + 
										TPCnt = 0;
										LCD_DisASCString16x8(140, 30, (u8*)"Calibrate Failed, Again", BLUE, WHITE);	
								}
								else //按下的触摸点在误差允许范围之内。计算 Offset 与 Factor 
								{	
									//计算X轴方向的比例因子 xFactor = (Y0 - Y1)/(x0 - x1);  
									TouchParameter.xFactor = (float)(CalibraSamplePoint[0].y - CalibraSamplePoint[3].y)/(Point[0].x - Point[3].x);
									//计算X轴方向的偏移量 xOffset = [(Y0 + Y1)-xFactor(x0 + x1)]/2;
									TouchParameter.xOffset = (short)((CalibraSamplePoint[0].y + CalibraSamplePoint[3].y) - TouchParameter.xFactor*(Point[0].x + Point[3].x))/2;
									//计算Y轴方向的比例因子  yFactor = (X0 - X1)/(y0 - y1);
									TouchParameter.yFactor = (float)(CalibraSamplePoint[0].x - CalibraSamplePoint[3].x)/(Point[0].y - Point[3].y);
									//计算Y轴方向的偏移量 YOffset = [(X0 + X1)-yFactor(y0 + y1)]/2;
									TouchParameter.yOffset = (short)((CalibraSamplePoint[0].x + CalibraSamplePoint[3].x) - TouchParameter.yFactor*(Point[0].y + Point[3].y))/2;
									LCD_ClrScr(WHITE);
									LCD_DisASCString16x8(140, 15, (u8*)"Touch Screen Calibrate OK", BLUE, WHITE);
									TouchParameter.CalFlag = 0xAA;//写入 0xAA，标记触摸已经校准成功
									pToucPara = (u8*)&TouchParameter;
									Driver_FlashWrite((u32)0,pToucPara, sizeof(TouchParameter)); //将校准好的值，保存在ROM 内部的Flash中，保存地址：	DRIVER_FLASHPAGE_ADDRESS						
								}
								break;									
						}
			}
		}	
}

/********************************************************************************************************
*  Function: Touch_GetPressPointVal						                                                           
*  Object: 触摸屏按下坐标读取函数
*  输入： u16 *pPointY ： 保存按下的触摸点的Y坐标， u16 *pPointX ：保存按下的触摸点的X坐标， 
*  输出： OneTouchFinishFlag 的值                               
*  备注：                           
********************************************************************************************************/
int Touch_GetPressPointVal(u16 *pPointY, u16 *pPointX)
{
		u8 i,j;
		u16 Temp;
		TOUCH_VAL_TYPE Point[10];
		if(Driver_TouchTINTSta()== 0)//触摸屏有按键按下
		{
		
				if(TouchPreSta == 0)     //之前没有被按下
				{	
						TouchPreSta = 1;		//记录触摸屏按下了。
						OneTouchFinishFlag = 0 ;
					//读10次触摸采样值
					for(i = 0; i < 10; i++)
						Touch_GetVal(&Point[i]);	
						//消抖 去掉前2个大值
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
						//消抖 去掉前2个小值
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
						//剩余求平均值
						for(i = 1; i < 6; i++)
							Point[0].x += Point[i].x;
						Point[0].x /= 6;
						for(i = 1; i < 6; i++)
							Point[0].y += Point[i].y;
						Point[0].y /= 6;	
						//保存触摸点的坐标值
						*pPointY = Point[0].y;
						*pPointX = Point[0].x;		
							
				}
				
		}
		else// 触摸屏按键松开
		{
			if(TouchPreSta == 1) //之前触摸屏按下。 
			{
				TouchPreSta = 0;
				OneTouchFinishFlag = 1 ; //触摸屏松开了
			}
		}
			return OneTouchFinishFlag;
}

/********************************************************************************************************
*  Function: Touch_GetSite						                                                           
*  Object: 触摸数据采样处理 并转换为LCD屏幕上的坐标
*  输入： u16 *pRow ： LCD行， u16 *pColumn ：LCD列
*  输出： 1/0  1: 触摸坐标转换成功  0 ： 触摸没有按下                           
*  备注：                           
********************************************************************************************************/
int Touch_GetSite(u16 *pRow, u16 *pColumn)
{
		u8 i,j;
		u16 Temp;
		TOUCH_VAL_TYPE Point[10];
		if(Driver_TouchTINTSta()== 0)//触摸屏有按键按下 ，TINT引脚为低电平
		{
				//读10次触摸采样值
				for(i = 0; i < 10; i++)
					Touch_GetVal(&Point[i]);
				//消抖 去掉前2个大值
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
				//消抖 去掉前2个小值
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
				//剩余求平均值
				for(i = 1; i < 6; i++)
					Point[0].x += Point[i].x;
				Point[0].x /= 6;
				for(i = 1; i < 6; i++)
					Point[0].y += Point[i].y;
				Point[0].y /= 6;		
				//将触摸ADC值 转换为屏对应像素坐标
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
*  Object: 画板函数，把触摸按下的值在LCD上面显示出来。或者选择再次进入触摸校准模式
*  输入： 无
*  输出： 无                        
*  备注： 先按下触摸屏 然后按下KEY2 再同时松手，可以进入触摸校准模式。                          
********************************************************************************************************/
void TouchHandle(void)
{
		u16 Row, Column;
		int Back;
		while(1)
		{
				static u16 RowOld, ColumnOld;
			  static bool CheckEN = False;
				//读触摸检测值
				Back = Touch_GetSite(&Row, &Column);
				//首次点击屏幕延时消抖
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
				//平滑算法处理 消抖
				if(Back)
				{
						if(CheckEN)
						{
								//行跳跃重检测
								if((RowOld > Row) && (RowOld - Row > 5))
									continue;
								if((RowOld < Row) && (Row - RowOld > 5))
									continue;
								//列跳跃重检测
								if((ColumnOld > Column) && (ColumnOld - Column > 5))
									continue;
								if((ColumnOld < Column) && (Column - ColumnOld > 5))
									continue;
						}
						RowOld = Row;
						ColumnOld = Column;
				}
				//获取坐标位置并在TFT上进行绘制
				if(Back)   
				{
						//只在触摸显示窗口有效
						if(Row < 275)
							LCD_DisASquare(Row, Column, 2, TouchColor);
						//点击“CLR“执行清屏
						if(Row > 276 && Row < 320 && Column > 180 && Column < 240)
						{
								LCD_DisARectangular(276,180,319,239,BLACK);
								delay_ms(100);
								LCD_DisARectangular(276,180,319,239,GRAY);
								LCD_DisARectangular(0,0,275,239,WHITE);
								LCD_DisASCString16x8(292, 198, (u8*)"CLR", BLACK, GRAY);
						}	
						//选择RED
						if(Row > 276 && Row < 320 && Column > 0 && Column < 60)
							Touch_ChoseColor(RED);
						//选择GREEN
						if(Row > 276 && Row < 320 && Column > 60 && Column < 120)
							Touch_ChoseColor(GREEN);
						//选择BLUE
						if(Row > 276 && Row < 320 && Column > 120 && Column < 180)
							Touch_ChoseColor(BLUE);
				}
		}
}
//在指定的坐标位置画一个十字，用于表示校准的触摸光标
void Touch_DrawCross(TOUCH_VAL_TYPE CalibraSamplePoint,u16 Color)
{
	LCD_DisALine(CalibraSamplePoint.x ,CalibraSamplePoint.y-10,1,20,Color);
	LCD_DisALine(CalibraSamplePoint.x-10,CalibraSamplePoint.y,2,20,Color);	

}

//选择触摸颜色并更新显示
void Touch_ChoseColor(u16 Val)
{
		u16 a = RED, b = GREEN, c = BLUE;
		switch(Val)
		{
				case RED:  	TouchColor = RED;   a = BLACK;  break;
			  case GREEN: TouchColor = GREEN; b = BLACK;  break;
				case BLUE:  TouchColor = BLUE;  c = BLACK;  break;
		}
		//显示选择框
		LCD_DisABorder(276, 0, 319, 59, a);
		LCD_DisABorder(277, 1, 318, 58, a);
		LCD_DisABorder(276, 60, 319, 119, b);
		LCD_DisABorder(277, 61, 318, 118, b);
		LCD_DisABorder(276, 120, 319, 179, c);
		LCD_DisABorder(277, 121, 318, 178, c);
}

//往SPI接口发送数据
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

//从SPI接口接收数据
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

//读一次触摸检测值
void Touch_GetVal(TOUCH_VAL_TYPE *Point)
{
    Driver_SPICS(0);
    Touch_SPIWrite(0x90);   //读取Y轴 检测值
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    (*Point).y = Touch_SPIRead();
    Touch_SPIWrite(0xd0);  //读取X轴 检测值
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
//导入经校准过的触摸屏幕 结果值
//！！如觉得不够准确 请执行屏幕触摸调校 然后将调校显示结果 赋值给下方变量
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
//以Row行 Column列处为中心点，显示一个Color颜色的十字星 十字星横竖长度为16个像素点
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
//触摸面板初始化
void Touch_FaceInit(void)
{	
		//显示触摸画面
		LCD_ClrScr(WHITE);
		LCD_DisARectangular(276,0,319,59,RED);
		LCD_DisARectangular(276,60,319,119,GREEN);
		LCD_DisARectangular(276,120,319,179,BLUE);
		LCD_DisARectangular(276,180,319,239,GRAY);
		LCD_DisASCString16x8(292, 198, "CLR", BLACK, GRAY);
		//初始化选择框
		Touch_ChoseColor(RED);
}
//应用层画线处理
void TouchDrawHandle(void)
{
		u16 Row, Column;
		#define KEY_LONGTIME 2000  /*长按外部按键1 进入触摸调校界面 时间值设定 */
		//static u16 KeyTimer = KEY_LONGTIME;   
		while(1)
		{
				//执行触摸读值处理
				GetSiteHandle();
				//执行画线处理
				if(TouchPointNewFlag == True)   
				{
						//获得触摸行列值
						Row = TouchRow;
						Column = TouchColumn;
						//只在触摸显示窗口有效
						if(Row < 275)
							LCD_DisASquare(Row, Column, 2, TouchColor);
						//点击“CLR“执行清屏
						if(Row > 276 && Row < 320 && Column > 180 && Column < 240)
						{
								LCD_DisARectangular(276,180,319,239,BLACK);
								delay_ms(100);
								LCD_DisARectangular(276,180,319,239,GRAY);
								LCD_DisARectangular(0,0,275,239,WHITE);
								LCD_DisASCString16x8(292, 198, "CLR", BLACK, GRAY);
						}	
						//选择RED
						if(Row > 276 && Row < 320 && Column > 0 && Column < 60)
							Touch_ChoseColor(RED);
						//选择GREEN
						if(Row > 276 && Row < 320 && Column > 60 && Column < 120)
							Touch_ChoseColor(GREEN);
						//选择BLUE
						if(Row > 276 && Row < 320 && Column > 120 && Column < 180)
							Touch_ChoseColor(BLUE);
						TouchPointNewFlag = False;
				}
				delay_ms(1);
		}
}

//获取当前的触摸位置值 每隔1ms令其运行一次
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
				//等待检测
				case READ_WAIT:
					{
							if(TouchPointNewFlag == True)
								break;
							else
								TickHStep = 0;
					}
				//先读一个点
				case REDA_FIRST:
					{
							Touch_GetVal_(&TouchPoint1X, &TouchPoint1Y);   /* 获取触摸X Y值 */
							if(GetScrSite_(TouchPoint1X, TouchPoint1Y, &TouchSiteRow1, &TouchSiteColumn1))  /* 将触摸X Y值转化为屏幕行列值 */
								TickHStep++;
							else
								break;
					} 
				//再读一个点
				case READ_SECOND:
					{
							Touch_GetVal_(&TouchPoint2X, &TouchPoint2Y);   /* 获取触摸X Y值 */
							if(GetScrSite_(TouchPoint2X, TouchPoint2Y, &TouchSiteRow2, &TouchSiteColumn2))  /* 将触摸X Y值转化为屏幕行列值 */
								TickHStep++;
							else
								break;
					}
        //判断点的有效性 跳跃则为噪点 去除				
				case READ_CHECK: 
					{	
							//行跳跃重检测
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
							//列跳跃重检测
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
							//得到有效的触摸检测值 进行平均值处理
							TouchRow = (TouchSiteRow1+TouchSiteRow2)/2;
							TouchColumn = (TouchSiteColumn1+TouchSiteColumn2)/2;
							TouchPointNewFlag = True;
							TickHStep++;							
					}	
					break;
		}	
}
//读一次触摸检测值
void Touch_GetVal_(u16 *X, u16 *Y)
{
    Driver_SPICS(0);
    Touch_SPIWrite(0x90);   //读取Y轴 检测值
    Driver_SPISCK(1);
    __NOP();
		__NOP();
		__NOP();
    Driver_SPISCK(0);
    __NOP();
		__NOP();
		__NOP();
    *Y = Touch_SPIRead();
    Touch_SPIWrite(0xd0);  //读取X轴 检测值
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
//通过触摸AD获得的X Y值, 计算屏幕所指向的行列位置值
//函数返回1 表示获得有效的位置 返回0 表示触摸位置无效 或者空按
bool GetScrSite_(u16 X, u16 Y, u16 *Row, u16 *Column)
{
		if(RowChgXYType == X_TYPE)
		{
				if((X > RowChgNumMIN && X < RowChgNumMAX) && (Y > ColumnChgNumMIN && Y < ColumnChgNumMAX))
				{
						//计算ROW
						if(RowChgNumType == NUM_ADD)
						{
								*Row = (X-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
						}
						else
						{
								*Row = (X-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
								*Row = LCD_ROWNUM - *Row;
						}
						//计算COLUMN
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
						//计算ROW
						if(RowChgNumType == NUM_ADD)
						{
								*Row = (Y-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
						}
						else
						{
								*Row = (Y-RowChgNumMIN)/((RowChgNumMAX - RowChgNumMIN)/LCD_ROWNUM);
								*Row = LCD_ROWNUM - *Row;
						}
						//计算COLUMN
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