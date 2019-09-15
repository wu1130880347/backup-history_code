
#include "lcd_12864.h"
//#include "picture.h"
#include "font.h"
#include "math.h"
//#include "bmp.h"

 //OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127


/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
   SCL_1;
   SDA_1;
   SDA_0;
   SCL_0;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
   SCL_0;
   SDA_0;
   SCL_1;
   SDA_1;
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
		{SDA_1;}
		else
		{SDA_0;}
		SCL_1;
		SCL_0;
		IIC_Byte<<=1;
	}
	SDA_1;
	SCL_1;
	SCL_0;
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   Write_IIC_Byte(0x00);			//write command
   Write_IIC_Byte(IIC_Command);
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
   Write_IIC_Byte(0x40);			//write data
   Write_IIC_Byte(IIC_Data);
   IIC_Stop();
}
/********************************************
// fill_Picture
********************************************/
//void fill_picture(unsigned char fill_Data)
//{
//	unsigned char m,n;
//	for(m=0;m<8;m++)
//	{
//		Write_IIC_Command(0xb0+m);		//page0-page1
//		Write_IIC_Command(0x00);		//low column start address
//		Write_IIC_Command(0x10);		//high column start address
//		for(n=0;n<132;n++)
//			{
//				Write_IIC_Data(fill_Data);
//			}
//	}
//}
/******************************************
// picture用来显示一个图片
******************************************/
//void Picture()
//{
//  unsigned char x,y;
//  unsigned int i=0;
//  for(y=0;y<8;y++)
//    {
//      Write_IIC_Command(0xb0+y);
//      Write_IIC_Command(0x0);
//      Write_IIC_Command(0x10);
//      for(x=0;x<132;x++)
//        {
//          Write_IIC_Data(show[i++]);
//        }
//    }
//}
void delay_ms(unsigned int ms)
{
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}
/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{
		for(j=0;j<123;j++);
	}
}



//void Initial_M096128x64_ssd1306()
//{
//	Write_IIC_Command(0xAE);   //display off
//	Write_IIC_Command(0x20);	//Set Memory Addressing Mode
//	Write_IIC_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
//	Write_IIC_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
//	Write_IIC_Command(0xc8);	//Set COM Output Scan Direction
//	Write_IIC_Command(0x00);//---set low column address
//	Write_IIC_Command(0x10);//---set high column address
//	Write_IIC_Command(0x40);//--set start line address
//	Write_IIC_Command(0x81);//--set contrast control register
//	Write_IIC_Command(0xdf);
//	Write_IIC_Command(0xa1);//--set segment re-map 0 to 127
//	Write_IIC_Command(0xa6);//--set normal display
//	Write_IIC_Command(0xa8);//--set multiplex ratio(1 to 64)
//	Write_IIC_Command(0x3F);//
//	Write_IIC_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
//	Write_IIC_Command(0xd3);//-set display offset
//	Write_IIC_Command(0x00);//-not offset
//	Write_IIC_Command(0xd5);//--set display clock divide ratio/oscillator frequency
//	Write_IIC_Command(0xf0);//--set divide ratio
//	Write_IIC_Command(0xd9);//--set pre-charge period
//	Write_IIC_Command(0x22); //
//	Write_IIC_Command(0xda);//--set com pins hardware configuration
//	Write_IIC_Command(0x12);
//	Write_IIC_Command(0xdb);//--set vcomh
//	Write_IIC_Command(0x20);//0x20,0.77xVcc
//	Write_IIC_Command(0x8d);//--set DC-DC enable
//	Write_IIC_Command(0x14);//
//	Write_IIC_Command(0xaf);//--turn on oled panel
//}


//void IIC_Wait_Ack()
//{
//
//	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
//	//GPIOB->CRH |= 0x00080000;
////	OLED_SDA = 1;
////	delay_us(1);
//	//OLED_SCL = 1;
//	//delay_us(50000);
///*	while(1)
//	{
//		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
//		{
//			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
//			//GPIOB->CRH |= 0x00030000;
//			return;
//		}
//	}
//*/
//	SCL_1 ;
//	SCL_0;
//}
/**********************************************
// IIC Write byte
**********************************************/

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd){Write_IIC_Data(dat);}
	else {Write_IIC_Command(dat);}
}



//坐标设置

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{

	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;

}
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 t)
{
   u8 i = 0;
   if(!(y1-y2))
   for(i=x1;i<x2;i+=t)
	 OLED_DrawPoint(i,y1,1);
   else for(i=y1;i<y2;i+=t)OLED_DrawPoint(x1,i,1);

}

//更新显存到LCD
void OLED_Refresh_Gram(void)
{

	u8 i,n;
	for(i=1;i<8;i++)
	{

	OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
	OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置―列低地址
	OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置―列高地址
	for(n=0;n<88;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);

	}

}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{

u8 x,y;
for(x=x1;x<=x2;x++)
{

for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);

}
OLED_Refresh_Gram();//更新显示

}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
	} //更新显示
}
void OLED_On(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);

			}
}
//m^n函数
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}
u8 oled_len(u32 n)
{
  u8 num = 0;
  if(n==0)return 1;
  while(n)
  { 
    num++;
    n/=10;
  }
  return num;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
        len = oled_len(num);
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size)*t,y,' ',size);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size)*t,y,temp+'0',size);
	}
}
void OLED_Show0Num(u8 x,u8 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
	 	OLED_ShowChar(x+(size)*t,y,temp+'0',size);
	}
}
void OLED_ShowFloat(u8 x,u8 y,float num,u8 len,u8 size)
{
  u8 l_float;
  long f_num;
  f_num = (long)num;
  
  if(num>=0)
  {
    OLED_ShowChar(x,y,' ',8);
    OLED_ShowNum(x+8,y,(u32)num,0,8);
  }
  else 
  {
    OLED_ShowChar(x,y,'-',8);
    num = fabs(num);
    f_num = (long)num;
    OLED_ShowNum(x+8,y,(u32)num,0,8);
  }
  
  l_float = oled_len(f_num);
  if(len)OLED_ShowChar(x+(l_float+1)*8,y,'.',8);
  f_num = (u32)((num - (long)num)*oled_pow(10,len));
  OLED_Show0Num(x+(l_float+2)*8,y,(u32)f_num,len,8);
}
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字
//void OLED_ShowCHinese(u8 x,u8 y,u8 no)
//{
//	u8 t,adder=0;
//	OLED_Set_Pos(x,y);
//    for(t=0;t<16;t++)
//		{
//				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
//				adder+=1;
//     }
//		OLED_Set_Pos(x,y+1);
//    for(t=0;t<16;t++)
//			{
//				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
//				adder+=1;
//      }
//}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP1)
//{
// unsigned int j=0;
// unsigned char x,y;
//
//  if(y1%8==0) y=y1/8;
//  else y=y1/8+1;
//	for(y=y0;y<y1;y++)
//	{
//		OLED_Set_Pos(x0,y);
//    for(x=x0;x<x1;x++)
//	    {
//	    	OLED_WR_Byte(BMP_0[BMP1][j++],OLED_DATA);
//	    }
//	}
//}

//初始化SSD1306
void OLED_Init(void)
{
    //LCD_12864 Control
    P2DIR = 0xf0;
    P2OUT |= BIT4;
    P2OUT &= ~BIT5;

    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128
	OLED_WR_Byte(0xA0,OLED_CMD);//set segment remap
	//OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	//OLED_WR_Byte(0xA7,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC0,OLED_CMD);//Com scan direction
	//OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//

	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//

	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//

	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//

	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//

	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//

	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//

	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel

	OLED_Clear();          //cleaning
}


