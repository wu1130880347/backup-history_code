#include "sys.h"
#include "usart.h"
#include "lcd_12864.h"
#include "LED.h"


u16 set_pwm = 0;
s16 set_temper = 250;
s16 now_temp = 0;
u8 set_T = 0;
u8 P = 0;
u8 set_I = 0;
u8 set_D = 0;
u8 start = 0;

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1  
#if defined ( __ICCARM__ )  //IAR��ʹ��  
//�ض���fputc����   
u8 fputc(u8 ch)  
{        
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������     
    USART1->DR = (u8) ch;        
    return ch;  
}  
#else  //��IAR��ʹ�� 

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//}; 
//
//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit(u16 x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{
   //GPIO�˿�����
        GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
        

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
        USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
        USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
//#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


u8 Res = '0';                  //������ձ���
u8 rec[10] = 0;                //�������֡����
u8 num10 = 0;                  //�����ϱ���������

/*******************************************************/
/********   ��ȡ��λ���·�ָ��ͨ��Э��    **************/
/*******************************************************/
/*******************************************************/
/*******************************************************/


void USART1_IRQHandler(void)                	     //����1�жϷ������
{
  
          static u8 sum_f = 0;             //������ռ�����̬����            

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //��֤����״̬
		{
                  Res = USART_ReceiveData(USART1)&0xff;	       //��ȡ���յ�������
                  rec[sum_f++] = Res;                          
                  if(rec[sum_f-1] == 0xA5)                      //����֡У��
                  {
                    sum_f = 0;
                    LED0 ^= 1;                                 //��λ��LEDָʾ���ճɹ�
                  }
                } 
        USART_ClearFlag(USART1,USART_IT_RXNE);               //������ձ�־λ
        if(sum_f == 0 && rec[2] == 0x04)                     //���յ���λ�����ò�������
        {
          set_temper = (rec[3]<<8) + rec[4];   //��λ���¶�����
          OLED_ShowString(48,4,"      ",8);
          OLED_ShowFloat(48,4,(float)set_temper*0.1,1,8);   //OLED��ʾ��λ��ָ������
          P = rec[5];                       //��λ��P����
          OLED_ShowString(24,5,"   ",8);
           OLED_ShowNum(24,5,P,0,8);     //��ʾ��λ��P����
          while(num10<10)                //�ظ���λ��
                usart2_send_char(rec[num10++]);
          num10 = 0;
          
        }
        else if(sum_f == 0 && (rec[2] == 0x01 || rec[2] == 0x00))   //���յ���λ���������ӣ��Ͽ�����ָ�� 
        {
          rec[3] = (u8)now_temp>>8;
          rec[4] = (u8)now_temp;
          rec[0] = 0xAA;
          rec[9] = 0xAA;
          while(num10<10)
                usart2_send_char(rec[num10++]);
          num10 = 0;
        }
        else if(sum_f == 0 && rec[2] == 0x02)  //���յ���λ����ʼ����ָ��   
        {
          rec[3] = (u8)now_temp>>8;
          rec[4] = (u8)now_temp;
          start = 1;
          while(num10<10)
                usart2_send_char(rec[num10++]);
          num10 = 0;
        }
        else if(sum_f == 0 && rec[2] == 0x03)   //���յ���λ����������ָ��
        {
          rec[3] = (u8)now_temp>>8;
          rec[4] = (u8)now_temp;
          start = 0;
          while(num10<10)
                usart2_send_char(rec[num10++]);
          num10 = 0;
        }
  

}

void usart2_send_char(u8 c)      //�����ֽڷ���
{

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
        USART_SendData(USART1,c);   

}
void usart2_send_string(u8 *data)        //�����ַ�������
{
	while(*(data))
          usart2_send_char(*(data++));	//�������ݵ�����1
}


 



