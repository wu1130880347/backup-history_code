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
 	 
//ALIENTEKս��STM32������ʵ��29
//���ٶȴ����� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

//x,y:��ʼ��ʾ������λ��
//num:Ҫ��ʾ������
//mode:0,��ʾ���ٶ�ֵ;1,��ʾ�Ƕ�ֵ;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//��ʾ���ٶ�ֵ
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//��ʾ����
			num=-num;						//תΪ����
		}else LCD_ShowChar(x,y,' ',16,0);	//ȥ������
		LCD_ShowNum(x+8,y,num,4,16);		//��ʾֵ		    
 	}else 		//��ʾ�Ƕ�ֵ
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//��ʾ����
			num=-num;						//תΪ����
		}else LCD_ShowChar(x,y,' ',16,0);	//ȥ������
 		LCD_ShowNum(x+8,y,num/10,2,16);		//��ʾ��������		    
		LCD_ShowChar(x+24,y,'.',16,0);		//��ʾС����
		LCD_ShowNum(x+32,y,num%10,1,16); 	//��ʾС������		    
	}
}		   						   	 
int main(void)
{		
	u8 key;
	u8 t=0;	
	short x,y,z;  	    
	short angx,angy,angz;  	    
  
 	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD
	usmart_dev.init(72);	//��ʼ��USMART		
 	KEY_Init();				//������ʼ��

 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"3D TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/12");
 	LCD_ShowString(60,130,200,16,16,"WK_UP:Auto Adjust");
 	while(ADXL345_Init())	//3D���ٶȴ�������ʼ��	
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
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	while(1)
	{		  	    
 		if(t%10==0)//ÿ100ms��ȡһ��
		{
			//�õ�X,Y,Z��ļ��ٶ�ֵ(ԭʼֵ)
			ADXL345_Read_Average(&x,&y,&z,10);	//��ȡX,Y,Z��������ļ��ٶ�ֵ 
			Adxl_Show_Num(60+48,170,x,0);		//��ʾ���ٶ�ԭʼֵ
			Adxl_Show_Num(60+48,190,y,0);
			Adxl_Show_Num(60+48,210,z,0);
 			//�õ��Ƕ�ֵ,����ʾ
			angx=ADXL345_Get_Angle(x,y,z,1);    
			angy=ADXL345_Get_Angle(x,y,z,2);   
			angz=ADXL345_Get_Angle(x,y,z,0); 
			Adxl_Show_Num(60+48,230,angx,1);		//��ʾ�Ƕ�ֵ
			Adxl_Show_Num(60+48,250,angy,1);
			Adxl_Show_Num(60+48,270,angz,1);
		}
		key=KEY_Scan(0);
		if(key==KEY_UP)
		{
			LED1=0;//�̵���,��ʾУ׼��
			ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//�Զ�У׼
			LED1=1;//�̵���,��ʾУ׼���
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







