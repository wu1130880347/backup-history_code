#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timer.h"		 	 
#include "tpad.h"
#include "oled.h"		 	 
#include "lcd.h"
#include "usmart.h"	
#include "rtc.h"	 	 
#include "wkup.h"	
#include "adc.h" 	 
#include "dac.h" 	 
#include "dma.h" 	 
#include "24cxx.h" 	 
#include "flash.h" 	 
#include "rs485.h" 	 
#include "can.h" 	 
#include "touch.h" 	 
#include "remote.h" 	 
#include "joypad.h"
#include "adxl345.h"
 	 
//ALIENTEK战舰STM32开发板实验29
//加速度传感器 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

//x,y:开始显示的坐标位置
//num:要显示的数据
//mode:0,显示加速度值;1,显示角度值;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//显示加速度值
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//显示负号
			num=-num;						//转为正数
		}else LCD_ShowChar(x,y,' ',16,0);	//去掉负号
		LCD_ShowNum(x+8,y,num,4,16);		//显示值		    
 	}else 		//显示角度值
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//显示负号
			num=-num;						//转为正数
		}else LCD_ShowChar(x,y,' ',16,0);	//去掉负号
 		LCD_ShowNum(x+8,y,num/10,2,16);		//显示整数部分		    
		LCD_ShowChar(x+24,y,'.',16,0);		//显示小数点
		LCD_ShowNum(x+32,y,num%10,1,16); 	//显示小数部分		    
	}
}		   						   	 
int main(void)
{		
	u8 key;
	u8 t=0;	
	short x,y,z;  	    
	short angx,angy,angz;  	    
  
 	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,9600);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   	//初始化LCD
	usmart_dev.init(72);	//初始化USMART		
 	KEY_Init();				//按键初始化

 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"3D TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/12");
 	LCD_ShowString(60,130,200,16,16,"WK_UP:Auto Adjust");
 	while(ADXL345_Init())	//3D加速度传感器初始化	
	{
		LCD_ShowString(60,150,200,16,16,"ADXL345 Error");
		delay_ms(200);
		LCD_Fill(60,150,239,150+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(60,150,200,16,16,"ADXL345 OK");
 	LCD_ShowString(60,170,200,16,16,"X VAL:");
	LCD_ShowString(60,190,200,16,16,"Y VAL:");
	LCD_ShowString(60,210,200,16,16,"Z VAL:");
  	LCD_ShowString(60,230,200,16,16,"X ANG:");
	LCD_ShowString(60,250,200,16,16,"Y ANG:");
	LCD_ShowString(60,270,200,16,16,"Z ANG:");
	POINT_COLOR=BLUE;//设置字体为红色 
	while(1)
	{		  	    
 		if(t%10==0)//每100ms读取一次
		{
			//得到X,Y,Z轴的加速度值(原始值)
			ADXL345_Read_Average(&x,&y,&z,10);	//读取X,Y,Z三个方向的加速度值 
			Adxl_Show_Num(60+48,170,x,0);		//显示加速度原始值
			Adxl_Show_Num(60+48,190,y,0);
			Adxl_Show_Num(60+48,210,z,0);
 			//得到角度值,并显示
			angx=ADXL345_Get_Angle(x,y,z,1);    
			angy=ADXL345_Get_Angle(x,y,z,2);   
			angz=ADXL345_Get_Angle(x,y,z,0); 
			Adxl_Show_Num(60+48,230,angx,1);		//显示角度值
			Adxl_Show_Num(60+48,250,angy,1);
			Adxl_Show_Num(60+48,270,angz,1);
		}
		key=KEY_Scan(0);
		if(key==KEY_UP)
		{
			LED1=0;//绿灯亮,提示校准中
			ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//自动校准
			LED1=1;//绿灯灭,提示校准完成
		}
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
			LED0=!LED0;
		}
	}
}







