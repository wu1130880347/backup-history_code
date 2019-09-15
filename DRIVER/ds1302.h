
#ifndef __DS1302_H__
#define __DS1302_H__

#include "Source.h"



//sbit SCK_ds=P2^2;		//���µ�ع���
//sbit SDA_ds=P2^1;
//sbit RST=P2^0;
#define SCK_ds delay_us(2);PFout(1)	// DS1
#define SDA_ds delay_us(2);PFout(3)	// DS1
#define RST_ds delay_us(2);PFout(5)	// DS1
//��λ��
#define RST_CLR	RST_ds=0//��ƽ�õ�
#define RST_SET	RST_ds=1//��ƽ�ø�


//˫������
#define IO_CLR	SDA_ds=0//��ƽ�õ�
#define IO_SET	SDA_ds=1//��ƽ�ø�
#define IO_R	PFin(3)  //��ƽ��ȡ


//ʱ���ź�
#define SCK_ds_CLR	SCK_ds=0//ʱ���ź�
#define SCK_ds_SET	SCK_ds=1//��ƽ�ø�


#define ds1302_sec_add				0x80		//�����ݵ�ַ
#define ds1302_min_add				0x82		//�����ݵ�ַ
#define ds1302_hr_add				0x84		//ʱ���ݵ�ַ
#define ds1302_date_add				0x86		//�����ݵ�ַ
#define ds1302_month_add			0x88		//�����ݵ�ַ
#define ds1302_day_add				0x8a		//�������ݵ�ַ
#define ds1302_year_add				0x8c		//�����ݵ�ַ
#define ds1302_control_add			0x8e		//�������ݵ�ַ
#define ds1302_charger_add			0x90 					 
#define ds1302_clkburst_add			0xbe


/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);

/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr);

/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(unsigned char *buf_time);

/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(unsigned char *buf_time);

/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void);

#endif
