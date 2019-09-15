/*-----------------------------------------------------------------------------

	��  Ŀ����дDS1302����
	�����ߣ����
	��  �ݣ�DS1302��ʼ������ʱ�����ڲ����ݽ��ж�ȡ����ͨ�����ڷ���������
	ʱ  �䣺2017-07-06

-----------------------------------------------------------------------------*/

#include "ds1302.h"

/*******************************************************************************
* ������    : Ds1302_Write_Byte()
* ��������	: DS1302�ֽ�д�뺯��
* �������  : addr -- ��ַ
			  d    -- �ֽ�����
* ����ֵ    : ��
*******************************************************************************/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{
	unsigned char i;
	RST_SET;
	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;     //���λ����
	for (i = 0; i < 8; i ++)
	{
		if (addr & 0x01)
		{
			IO_SET;
		}
		else
		{
			IO_CLR;
		}
		SCK_ds_SET;
		SCK_ds_CLR;
		addr = addr >> 1;
	}
	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++)
	{
		if (d & 0x01)
		{
			IO_SET;
		}
		else
		{
			IO_CLR;
		}
		SCK_ds_SET;
		SCK_ds_CLR;
		d = d >> 1;
	}
	RST_CLR;					//ֹͣDS1302����
}

/*******************************************************************************
* ������    : Ds1302_Read_Byte()
* ��������	: DS1302�ֽڶ�ȡ����
* �������  : addr -- ��ַ
* ����ֵ    : temp -- �������ֽ�����
*******************************************************************************/
unsigned char Ds1302_Read_Byte(unsigned char addr)
 {
	unsigned char i;
	unsigned char temp;
	RST_SET;

	//д��Ŀ���ַ��addr
	addr = addr | 0x01;//���λ�ø�
	for (i = 0; i < 8; i ++)
	{
		if (addr & 0x01)
		{
			IO_SET;
		}
		else
		{
			IO_CLR;
		}
		SCK_ds_SET;
		SCK_ds_CLR;
		addr = addr >> 1;
	}
	
	//������ݣ�temp
	for (i = 0; i < 8; i ++)
	{
		temp = temp >> 1;
		if (IO_R)
		{
			temp |= 0x80;
		}
		else
		{
			temp &= 0x7F;
		}
		SCK_ds_SET;
		SCK_ds_CLR;
	}
	RST_CLR;	//ֹͣDS1302����
	return temp;
}

/*******************************************************************************
* ������    : Ds1302_Write_Time()
* ��������	: ��DS1302��д��ʱ������
* �������  : ��
* ����ֵ    : ��
*******************************************************************************/
void Ds1302_Write_Time(unsigned char *buf_time)
{
    unsigned char i,temp;
	unsigned char buf[8];
	for(i=0;i<8;i++)
	{
		temp=buf_time[i]/10;	//BCD����
		buf[i]=buf_time[i]%10;
		buf[i]=buf[i]+temp*16;
	}
	Ds1302_Write_Byte(ds1302_control_add,0x00);		//�ر�д����
	Ds1302_Write_Byte(ds1302_sec_add,0x80);			//��ͣ
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);	//������
	Ds1302_Write_Byte(ds1302_year_add,buf[1]);		//��
	Ds1302_Write_Byte(ds1302_month_add,buf[2]);		//��
	Ds1302_Write_Byte(ds1302_date_add,buf[3]);		//��
	Ds1302_Write_Byte(ds1302_day_add,buf[7]);		//��
	Ds1302_Write_Byte(ds1302_hr_add,buf[4]);		//ʱ
	Ds1302_Write_Byte(ds1302_min_add,buf[5]);		//��
	Ds1302_Write_Byte(ds1302_sec_add,buf[6]);		//��
	Ds1302_Write_Byte(ds1302_day_add,buf[7]);		//��
	Ds1302_Write_Byte(ds1302_control_add,0x80);		//��д����
}

/*******************************************************************************
* ������    : Ds1302_Read_Time()
* ��������	: ��DS1302�ж�ȡʱ������
* �������  : ��
* ����ֵ    : ��
*******************************************************************************/
void Ds1302_Read_Time(unsigned char *buf_time)
{
   	unsigned char i,temp;
	unsigned char buf[8];
	buf[1]=Ds1302_Read_Byte(ds1302_year_add);			//��
	buf[2]=Ds1302_Read_Byte(ds1302_month_add);			//��
	buf[3]=Ds1302_Read_Byte(ds1302_date_add);			//��
	buf[4]=Ds1302_Read_Byte(ds1302_hr_add);			//ʱ
	buf[5]=Ds1302_Read_Byte(ds1302_min_add);			//��
	buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;	//��
	buf[7]=Ds1302_Read_Byte(ds1302_day_add);			//��

	for(i=0;i<8;i++)
	{           //BCD����
		temp=buf[i]/16;
		buf_time[i]=buf[i]%16;
		buf_time[i]=buf_time[i]+temp*10;
	}
}

/*******************************************************************************
* ������    : Ds1302_Init()
* ��������	: DS1302��ʼ������
* �������  : ��
* ����ֵ    : ��
*******************************************************************************/
void Ds1302_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��
  //initial LED1(PC0)
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//��������
  GPIO_Init(GPIOF, &GPIO_InitStructure);			
  GPIO_SetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5);//GPIOF9,F10���øߣ�����
	RST_CLR;												//RST���õ�
	SCK_ds_CLR;												//SCK_ds���õ�
	Ds1302_Write_Byte(ds1302_sec_add,0x00);
}
