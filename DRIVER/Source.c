#include "Source.h"
#include "qqImage.h"
#include "DEBUG.H"
#include "fsmc_driver.h"
void EXTI8_Init(void);
void usart1_send_char(u8 c);
void DS18B20_F4(void);
//u8 buf_time[8] = 0;
void Init_F4(void)
{
        
        Driver_MCU_Init();
	delay_init(168);
	LED_Init();            //����LED��     ====>>PC0 and PD3                
        OLED_Init();       //                           ==>>PF8 and PF10
        OLED_ShowString(0,0,"..Temperature..",8);
        //Osc_Init();        //12864�����ʼ��    
        //TIM3_us_on();    //��ʱ�����ж�       
        //Adc_Init();         //��ʼ��ADC               ==>>PA5
        uart_init(10000);  //9600�����ʣ�����Ϊ���� ==>>PA9 and PA10
        //EXTIX_Init();                     //             ==>>PA0 and PF11
        //Tft3_2_Init();       //PD:0,1,4,5,8,9,10,14,15;PE:7,8,9,10,11,12,13,14,15
                             //PG:0,12;PB:0,1,2;PC:2,3,5;PF:8,9,10;
        //Ds1302_Init();
        //Ds1302_Read_Time(buf_time);
        //LCD_DisASCString16x8(100,0,buf_time,~WHITE,~GREEN);
        
        //ov7670_init();                     //           ==>>PC6,7,PB8~15,PG2~7  
        //Key_4x4_Init();                   //            ==>>PD8,10,12,14(IN)PB12,14 and PE12,14(OUT)
        //My_RTC_Init();		 		//��ʼ��RTC
	//RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��
        //SD_file_Init();                    // ==>>PF1,3,5,7
        
        TIM14_PWM_Init(1000-1,84-1);             //     ==>>PF9 (OUT:PWM)
        //mpu6050_Init();        //                     ==>>PB8,PB9
        //rf24l01_RX_TX();              //            ==>>PG6,7,8,PB14,3,4,5
        
        DS18B20_F4();
        //adxl345_Init(); 	  //                     ==>>PB8,PB9
        //TIM5_CH1_Cap_Init(0xffffffff,84-1);         //   ==>>  PA1 (CAP_PWM_TD) 
        //Dac1_Init();		//DACͨ��1��ʼ��	//   ==>>PA4  (OUT:DAC_V) 	 
        //Dac1_Set_Vol(2000);	//����ͨ��1�����ѹ
}

/*******************************************************/
/********      ����ʽP(����)�����㷨      **************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
void do_pid(s16 error)
{
  static float now_error;//,pre_error,d_error,i_error;   //��������     ���֣�΢�֣�δ��
  static float out_pid = 0;                     //PID�������
  static float f_d = 0;                        //�����м����
  now_error = error;                           //����ƫ���
  //ierror += now_error;                       //����ƫ��� 
  //d_error = now_error - pre_error;           //΢��ƫ���
  //pre_error = now_error;                     //������һ��ƫ��
  //    PID���
  out_pid = (P * now_error)*0.1; //+ (I * i_error)/1000 + (D * d_error)/100;
  //d_error = 0;                              //΢������
  f_d += (s8)out_pid;                          //����任
  F_D = (u16)f_d;
  
  if(F_D < 50){F_D = 50;f_d = 50;}             //����޷������������  5%
  if(F_D > 950){F_D = 950;f_d = 950;}          //����޷������������  95%
  now_error = 0;                               //ƫ������
  TIM_SetCompare1(TIM14,F_D);                  //PWM�������   F_DΪռ�ձȱ���
  
}


/*******************************************************/
/********           �������߼����        **************/
/*******************************************************/
/*******************************************************/
/*******************************************************/

void DS18B20_F4(void)
{

  u8 num10 = 0;                    //�����ϱ�����������10�ֽ�һ֡
  s16 temper = 0;                  //��ǰ�¶�ֵ
  u8 my_data[10] = {0x5A,0x01,0x08,0x03,0x00,0x00,0x00,0x00,0x32,0xA5};  //ͨ��Э��֡����
  //                ֡ͷ �ϱ�������  �¶�����              PWM����  ֡β
  while(DS18B20_Init())	                      //DS18B20��ʼ��
	{
                OLED_ShowString(0,1,"Sensor:ERROR...",8);        //OLED��ʾ������״̬
		delay_ms(200);
                OLED_ShowString(60,1,"              ",8);
 		delay_ms(200);
	}
  OLED_ShowString(0,1,"Sensor:...OK...",8);       //��ʼ���ɹ�
  OLED_ShowString(0,3,"Now_T:",8);                //��ʾOLED
  OLED_ShowString(0,4,"Set_T:",8);
  OLED_ShowString(0,5,"P : ",8);
  s8 now_error = 0;
  
  while(1)                                        //ѭ������
  {
    delay_ms(50);                                //50ms����ʱ�䣨û��20�Σ�
    temper = DS18B20_Get_Temp();                 //��ȡ�������¶�
    OLED_ShowFloat(48,3,(float)temper*0.1,1,8);  //OLED��ʾ��ǰ�¶�ֵ
    now_temp = temper;                         //���õ�ǰ�¶�ֵ
    
    if(start)                                    //��ʼִ��PID���Ʊ�־λ
          {
                num10 = 0;                      //����ϱ�����
                now_error = temper - set_temper;       //Ԥ�����
                if(now_error > 0)now_error += 7;
                if(now_error < 0)now_error -= 7;
                do_pid(now_error);    //PID����
                my_data[3] = (u8)(temper>>8);   //�¶����ݴ���
                my_data[4] = (u8)temper;
                my_data[8] = (u8)(F_D/10);      //PWM���ݴ���
                                                //��λ�������ϱ�
                while(num10<10)                 //ʮ�ֽ�û֡
                usart2_send_char(my_data[num10++]);
                
                LED1 ^= 1;                      //LEDָʾ����״̬
          }
          else 
          {
            delay_ms(10);                       //��ִ�п��ƣ��ȴ�״̬
            TIM_SetCompare2(TIM14,0);           //�ر�PWM
          }
  }
}

















void ov7670_init()
{
  OV7725_GPIO_Config();
  
  while(OV7725_Init() != SUCCESS)
	{
                OLED_ShowString(0,0, "OV7725 Error!!",8);
		delay_ms(200);
	         OLED_ShowString(0,0, "OV7725        ",8);
		delay_ms(200);
	}
   OLED_ShowString(0,0, "OV7670 OK     ",8);

      
//	OV7725_Light_Mode(cam_mode.light_mode);
//	OV7725_Color_Saturation(cam_mode.saturation);
//	OV7725_Brightness(cam_mode.brightness);
//	OV7725_Contrast(cam_mode.contrast);
//	OV7725_Special_Effect(cam_mode.effect);
//	OV7725_Window_Set(cam_mode.cam_sx,
//					cam_mode.cam_sy,
//					cam_mode.cam_width,
//					cam_mode.cam_height,
//					cam_mode.QVGA_VGA);
                                OV7725_Light_Mode(0);
		                OV7725_Color_Saturation(0);
		                OV7725_Brightness(0);
		                OV7725_Contrast(0);
		                OV7725_Special_Effect(1);
		                OV7725_Window_Set(0,0,320,240,1);
	Ov7725_vsync = 0;
        //uart_init(117500); ////115200
        //usart1_send_char(0xaa);
        //fputc(0x22);
        //printf("hello World....\n");
        while(1)
	{
		
		if( Ov7725_vsync == 2)
		{
			FIFO_PREPARE;  							
			ImagDisp();			
			Ov7725_vsync = 0;
                        LED0^=1;

		}		
	}
						
	
  	
}
void Osc_Init(void)
{
//   OLED_DrawLine(0,0,87,0,1);
//   OLED_DrawLine(0,55,87,55,1);
//   OLED_DrawLine(0,0,0,55,1);
//   OLED_DrawLine(87,0,87,55,1);

//   OLED_DrawLine(43,0,43,55,2);
//   OLED_DrawLine(3,0,3,55,L);
//   OLED_DrawLine(13,0,13,55,L);
//   OLED_DrawLine(23,0,23,55,L);
//   OLED_DrawLine(33,0,33,55,L);
//   OLED_DrawLine(53,0,53,55,L);
//   OLED_DrawLine(63,0,63,55,L);
//   OLED_DrawLine(73,0,73,55,L);
//   OLED_DrawLine(83,0,83,55,L);

  // OLED_DrawLine(0,27,87,27,2);
//   OLED_DrawLine(0,37,87,37,L);
//   OLED_DrawLine(0,47,87,47,L);
//   OLED_DrawLine(0,17,87,17,L);
//   OLED_DrawLine(0,7,87,7,L);
  OLED_ShowChar(16,0,':',8);
  OLED_ShowChar(40,0,':',8);
//  OLED_ShowNum(0,1,60035354,0,8);
//  OLED_ShowNum(0,2,654,6,8);
//  OLED_Show0Num(0,3,15,4,8);
//  OLED_ShowFloat(0,4,8,0,8);
  OLED_ShowFloat(0,7,-80.67,2,8);
//  OLED_ShowFloat(0,6,7,0,8);
//  OLED_ShowFloat(0,7,-9,0,8);
//   OLED_Refresh_Gram();
//   OLED_ShowString(88,1,"T:1ms",8);
//   OLED_ShowString(88,3,"V:1mV",8);
//   OLED_ShowString(88,5,"VPP:2",8);
//   OLED_ShowString(88,7,"Td:50",8);

}
/*
void Osc_Show(void)
{
    u8 x,y;
    for(x=1;x<87;x++)
    for(y=1;y<55;y++)OLED_DrawPoint(x,y,0);

  u8 i;
  for(i=1;i<87;i++)
  {
	OLED_DrawPoint(i,data_adc[i]+7,1);
	if(data_adc[i]<data_adc[i+1])OLED_DrawLine(i+1,data_adc[i]+7,i+1,data_adc[i+1]+7,1);
	else OLED_DrawLine(i+1,data_adc[i+1]+7,i+1,data_adc[i]+7,1);
  }
   OLED_DrawLine(0,0,87,0,1);
   OLED_DrawLine(0,55,87,55,1);
   //OLED_DrawLine(0,0,0,55,1);
   //OLED_DrawLine(87,0,87,55,1);
   //OLED_DrawLine(43,0,43,55,3);
   //OLED_DrawLine(0,27,87,27,3);
   OLED_Refresh_Gram();

}*/

void rf24l01_RX_TX(void)
{
        u16 t=0;			 
	u8 tmp_buf[9];	
 	NRF24L01_Init();    		//��ʼ��NRF24L01 
         
	OLED_ShowString(0,1,"STM32F4",8);	
	OLED_ShowString(0,2,"NRF24L01 TEST",8);	
	OLED_ShowString(0,3,"ALIENTEK",8);
	OLED_ShowString(0,4,"2017/5/12",8);
        
	while(NRF24L01_Check())
	{
		OLED_ShowString(0,5,"NRF24L01 Error",8);
		delay_ms(200);
	}
	        OLED_ShowString(0,5,"NRF24L01 OK",8); 	 	
		 		
			     		  	   	
		NRF24L01_RX_Mode();
                delay_ms(5);
                OLED_ShowString(0,6,"RX_Mode",8); //��˸��ʾ��ʾ��Ϣ
		while(1)
		{	
                        NRF24L01_RX_Mode();
                        delay_ms(50);
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
			{
				tmp_buf[8]=0;   //�����ַ���������
				OLED_ShowString(0,7,tmp_buf,8);
                              //  NRF24L01_Write_Reg(NRF_READ_REG+STATUS,0XFF);
			}
                        t++;
                        if(t==20){
                          OLED_ShowString(0,7,"        ",8);t=0;  }
                        LED0 ^= 1;
                                                      				    
		}
}

void usart1_send_char(u8 c)
{

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
        USART_SendData(USART1,c);   

}
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
void mpu6050_send_data(int aacx,int aacy,int aacz,int gyrox,int gyroy,int gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(int aacx,int aacy,int aacz,int gyrox,int gyroy,int gyroz,int roll,int pitch,int yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
} 
void mpu6050_Init(void)
{
	//u8 report=1;			//Ĭ�Ͽ����ϱ�
	//u8 key;
	float pitch,roll,yaw; 		//ŷ����
        //float angx,angy,angz;
	//int aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	//int gyrox,gyroy,gyroz;	//������ԭʼ����
	//float temp;					//�¶�
	uart_init(10000);		//��ʼ�����ڲ�����Ϊ500000
	while(MPU_Init())
        {
          delay_ms(100);					//��ʼ��MPU6050
          OLED_ShowString(0,1,"MPU6050 Error",8);
        }
	while(mpu_dmp_init())
	{
		OLED_ShowString(0,1,"MPU6050 Error",8);
		delay_ms(400);
	}
	OLED_ShowString(0,1,"MPU6050 OK   ",8);
 	OLED_ShowString(0,2,"UPLOAD ON ",8);	 
 	OLED_ShowString(0,3," Temp:      C",8);	
 	OLED_ShowString(0,4,"Pitch:      C",8);	
 	OLED_ShowString(0,5," Roll:      C",8);	 
 	OLED_ShowString(0,6," Yaw :      C",8);	
 	while(1)
	{
		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
                //MPU_Get_Accelerometer(&pitch,&roll,&yaw);
		{ 
			
//                  if(pitch>0||(u8)pitch==0){OLED_ShowChar(48,4,' ',8);OLED_ShowNum(56,4,(u8)pitch,3,8);}
//                  else {OLED_ShowChar(48,4,'-',8);OLED_ShowNum(56,4,255-(u8)pitch,3,8);}
//                  if(roll>0||(u8)roll==0){OLED_ShowChar(48,5,' ',8);OLED_ShowNum(56,5,(u8)roll,3,8);}
//                  else {OLED_ShowChar(48,5,'-',8);OLED_ShowNum(56,5,255-(u8)roll,3,8);}
//                  if(yaw>0||(u8)yaw==0){OLED_ShowChar(48,6,' ',8);OLED_ShowNum(56,6,(u8)yaw,3,8);}
//                  else {OLED_ShowChar(48,6,'-',8);OLED_ShowNum(56,6,255-(u8)yaw,3,8);}
                        OLED_ShowFloat(48,4,pitch-85,2,8);
                        OLED_ShowFloat(48,5,roll,2,8);
                        OLED_ShowFloat(48,6,yaw,2,8);
                 // break;
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������                                         
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//			if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
//			if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
                }
//                        temp=MPU_Get_Temperature();	//�õ��¶�ֵ
//                        OLED_ShowFloat(56,3,temp,1,8);
                delay_ms(20);
                LED1 ^= 1;
	} 	

}

void adxl345_Init(void)
{
		   						   	 
	short x,y,z;  	    
	float angx,angy,angz;  
        uart_init(10000);		//��ʼ�����ڲ�����Ϊ500000
 	while(ADXL345_Init())	//3D���ٶȴ�������ʼ��	
	{
		OLED_ShowString(0,1,"ADXL345 Error",8);
		delay_ms(200);
	}								   
	OLED_ShowString(0,1,"ADXL345 OK   ",8);
 	OLED_ShowString(0,2,"X VAL: ",8);
	OLED_ShowString(0,3,"Y VAL:",8);
	OLED_ShowString(0,4,"Z VAL:",8);
  	OLED_ShowString(0,5,"X ANG:",8);
	OLED_ShowString(0,6,"Y ANG:",8);
	OLED_ShowString(0,7,"Z ANG:",8);
        
        //�Զ�У׼
        LED1=0;//�̵���,��ʾУ׼��
	  ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//�Զ�У׼
        LED1=1;//�̵���,��ʾУ׼���
	while(1)
	{		  	    
 		
			//�õ�X,Y,Z��ļ��ٶ�ֵ(ԭʼֵ)
			ADXL345_Read_Average(&x,&y,&z,10);	//��ȡX,Y,Z��������ļ��ٶ�ֵ 
                        x = ~x + 1;
                        y = ~y + 1;
                        z = ~z + 1;
//                        if(x>=0) {OLED_ShowChar(48,2,' ',8);OLED_ShowNum(56,2,x,4,8);}
//                        else {OLED_ShowChar(48,2,'-',8);OLED_ShowNum(56,2,65536-x,4,8);}
//                        if(y>=0) {OLED_ShowChar(48,3,' ',8);OLED_ShowNum(56,3,y,4,8);}
//                        else {OLED_ShowChar(48,3,'-',8);OLED_ShowNum(56,3,65536-y,4,8);}
//                        if(z>=0) {OLED_ShowChar(48,4,' ',8);OLED_ShowNum(56,4,z,4,8);}
//                        else {OLED_ShowChar(48,4,'-',8);OLED_ShowNum(56,4,65536-z,4,8);}
//                          OLED_ShowNum(48,3,y,5,8);
//                        OLED_ShowNum(48,4,z,5,8);
 			//�õ��Ƕ�ֵ,����ʾ
			angx=ADXL345_Get_Angle((float)x,(float)y,(float)z,1);    
			angy=ADXL345_Get_Angle((float)x,(float)y,(float)z,2);   
			angz=ADXL345_Get_Angle((float)x,(float)y,(float)z,0); 
//                        if(angx>=0){OLED_ShowChar(48,5,' ',8);OLED_ShowNum(56,5,angx,4,8);}
//                        else {OLED_ShowChar(48,5,'-',8);OLED_ShowNum(56,5,65536-angx,4,8);}
//                        if(angy>=0){OLED_ShowChar(48,6,' ',8);OLED_ShowNum(56,6,angy,4,8);}
//                        else {OLED_ShowChar(48,6,'-',8);OLED_ShowNum(56,6,65536-angy,4,8);}
//                        if(angz>=0){OLED_ShowChar(48,7,' ',8);OLED_ShowNum(56,7,angz,4,8);}
//                        else {OLED_ShowChar(48,7,'-',8);OLED_ShowNum(56,7,65536-angz,4,8);}
                        OLED_ShowFloat(48,5,angx,1,8);
                        OLED_ShowFloat(48,6,angy,1,8);
                        OLED_ShowFloat(48,7,angz,1,8);
                        //mpu6050_send_data(x,y,z,angx,angy,angz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
		
	 	//delay_ms(10);
		LED0=!LED0;
	}
}

void Tft3_2_Init(void)
{
	LCD_Init();                 //������ ����
	Driver_LcdBacklight(True);
	LCD_ClrScr(BLACK);         //����LOGO
        //LCD_ClrScr(WHITE);
//        LCD_DisARectangular(70+46, 20+29, 70+30+46, 10+60+29, YELLOW);
//	LCD_DisARectangular(63+46, 28+29, 63+32+46, 28+50+29, GREEN);
//	LCD_DisABorder(63+46, 28+29, 63+32+46, 28+50+29, WHITE);
//	LCD_DisASCString16x8(72+46, 31+29, "EU", WHITE, GREEN);
//	LCD_DisGB2312String16x16(72+46, 47+29, "����", WHITE, GREEN);
//	LCD_DisGB2312String16x16(72+46, 90+29, "��ת����", RED, YELLOW);
//	LCD_DisGB2312String16x16(100+46, 50+29, "�Ȱ�Ƕ��ʽ����", WHITE, GREEN);
//	LCD_DisASCString16x8(120+46, 10+29, "http://euse.taobao.com", BLACK, GREEN);
        //LCD_DisASC_Char16x8(0,0,'X',WHITE,BLACK);
        //LCD_DisASC_Char16x8(0,16,'X',WHITE,BLACK);
//        LCD_Show_Num(0,0,123456789,WHITE,BLACK); 
//        LCD_Show_0Num(16,0,1234678,9,WHITE,BLACK); 
//        LCD_DisASCString16x8(32,0, "abcdefghijklmnopqrstuvwxyz", WHITE, BLACK);
//        LCD_DisASCString16x8(48,0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", WHITE, BLACK);
//        LCD_DisASCString16x8(64,0, "1234567890+-./~|=-><", WHITE, BLACK);
//        LCD_Show_Float(100,0,3.1415234,0,WHITE,BLACK); 
//        LCD_Show_Float(116,0,-5.055234,0,WHITE,BLACK); 
//        LCD_Show_Float(132,0,-0.102515234,0,WHITE,BLACK); 
//        LCD_Show_Float(148,0,-4.0985234,0,WHITE,BLACK); 
//        LCD_Show_Float(164,0,-12482.985234,0,WHITE,BLACK);
//        LCD_Show_Float(180,0,0.012515234,0,WHITE,BLACK); 
        
                //������Ļ������ʼ��
		LoadTouchFaceVal();
		//��������ʼ��
                Touch_FaceInit();
		//���������ߴ���
		TouchDrawHandle();
}
void SD_file_Init(void)
{
        u8		buf[64]="sajdgahsgdh123";
        UINT8	i, s;
	//P_FAT_DIR_INFO	pDir;
	 //u8	SrcName[64];
	 //u8	TarName[64];
         
	mDelaymS( 100 );  /* ��ʱ100���� */
	OLED_ShowString(0,1, "Start...",8 );

	
	while(mInitCH376Host( )!=USB_INT_SUCCESS )
        {
                OLED_ShowString(0,1, "CH376 Error!!",8);
		delay_ms(200);
	         OLED_ShowString(0,1, "CH376        ",8);
		delay_ms(200);
        }

	while ( 1 ) {
		OLED_ShowString( 0,2,"Wait Udisk/SD" ,8);
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );  /* ��ʱ,��ѡ����,�е�USB�洢����Ҫ��ʮ�������ʱ */

/* ���ڼ�⵽USB�豸��,���ȴ�100*50mS,��Ҫ�����ЩMP3̫��,���ڼ�⵽USB�豸��������DISK_MOUNTED��,���ȴ�5*50mS,��Ҫ���DiskReady������ */
		for ( i = 0; i < 100; i ++ ) {  /* ��ȴ�ʱ��,100*50mS */
			mDelaymS( 50 );
			OLED_ShowString(0,3 ,"Ready ?",8 );
			s = CH376DiskMount( );  /* ��ʼ�����̲����Դ����Ƿ���� */
			if ( s == USB_INT_SUCCESS ) break;  /* ׼���� */
			else if ( s == ERR_DISK_DISCON ) break;  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) break;  /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
		}
		if ( s == ERR_DISK_DISCON ) {  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			OLED_ShowString( 0,3,"Device gone" ,8);
			continue;
		}
		if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) {  /* δ֪USB�豸,����USB���̡���ӡ���� */
			OLED_ShowString( 0,4,"Unknown device" ,8);
		}
		i = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
		if ( i == sizeof( INQUIRY_DATA ) ) {  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
			buf[ i ] = 0;
			OLED_ShowString(0,5,buf+8,8);
		}

/* ��ȡԭ�ļ� */
		
		OLED_ShowString( 0,6,"Open....",8 );
		s = CH376FileOpen( "/MY_ADC.TXT" );  /* ���ļ�,���ļ��ڸ�Ŀ¼�� */
		if ( s == USB_INT_SUCCESS ) {  /* �ļ����ڲ����Ѿ�����,�ƶ��ļ�ָ�뵽β���Ա�������� */
			OLED_ShowNum(0,1,CH376GetFileSize( ),0,8);  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
			
			OLED_ShowString(0,3, "Locate tail",8 );
			s = CH376SecLocate( 0xFFFFFFFF );  /* ������Ϊ��λ�Ƶ��ļ�β��,����ļ�β���в���һ����������ͷ�����򱻺���,�������������ô��ͷ���ݽ����ܱ�д�����ݸ��� */
			mStopIfError( s );
			CH376ByteWrite("asdf12345...",12,NULL);
			CH376FileClose(TRUE);break;
		}
		else if ( s == ERR_MISS_FILE ) {  /* û���ҵ��ļ�,�����½��ļ� */
			OLED_ShowString( 0,6,"Create" ,8);
			s = CH376FileCreate( NULL );  /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½�,�������ṩ�ļ���,�ղ��Ѿ��ṩ��CH376FileOpen */
			mStopIfError( s );
			
		}
		else mStopIfError( s );  /* ���ļ�ʱ���� */
        }
			
}