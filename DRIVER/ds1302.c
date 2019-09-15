/*-----------------------------------------------------------------------------

	项  目：读写DS1302数据
	开发者：杨春宇
	内  容：DS1302初始化后，随时对其内部数据进行读取，并通过串口发送至电脑
	时  间：2017-07-06

-----------------------------------------------------------------------------*/

#include "ds1302.h"

/*******************************************************************************
* 函数名    : Ds1302_Write_Byte()
* 函数功能	: DS1302字节写入函数
* 输入参数  : addr -- 地址
			  d    -- 字节数据
* 返回值    : 无
*******************************************************************************/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{
	unsigned char i;
	RST_SET;
	
	//写入目标地址：addr
	addr = addr & 0xFE;     //最低位置零
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
	
	//写入数据：d
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
	RST_CLR;					//停止DS1302总线
}

/*******************************************************************************
* 函数名    : Ds1302_Read_Byte()
* 函数功能	: DS1302字节读取函数
* 输入参数  : addr -- 地址
* 返回值    : temp -- 读出的字节数据
*******************************************************************************/
unsigned char Ds1302_Read_Byte(unsigned char addr)
 {
	unsigned char i;
	unsigned char temp;
	RST_SET;

	//写入目标地址：addr
	addr = addr | 0x01;//最低位置高
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
	
	//输出数据：temp
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
	RST_CLR;	//停止DS1302总线
	return temp;
}

/*******************************************************************************
* 函数名    : Ds1302_Write_Time()
* 函数功能	: 向DS1302中写入时间数据
* 输入参数  : 无
* 返回值    : 无
*******************************************************************************/
void Ds1302_Write_Time(unsigned char *buf_time)
{
    unsigned char i,temp;
	unsigned char buf[8];
	for(i=0;i<8;i++)
	{
		temp=buf_time[i]/10;	//BCD处理
		buf[i]=buf_time[i]%10;
		buf[i]=buf[i]+temp*16;
	}
	Ds1302_Write_Byte(ds1302_control_add,0x00);		//关闭写保护
	Ds1302_Write_Byte(ds1302_sec_add,0x80);			//暂停
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);	//涓流充电
	Ds1302_Write_Byte(ds1302_year_add,buf[1]);		//年
	Ds1302_Write_Byte(ds1302_month_add,buf[2]);		//月
	Ds1302_Write_Byte(ds1302_date_add,buf[3]);		//日
	Ds1302_Write_Byte(ds1302_day_add,buf[7]);		//周
	Ds1302_Write_Byte(ds1302_hr_add,buf[4]);		//时
	Ds1302_Write_Byte(ds1302_min_add,buf[5]);		//分
	Ds1302_Write_Byte(ds1302_sec_add,buf[6]);		//秒
	Ds1302_Write_Byte(ds1302_day_add,buf[7]);		//周
	Ds1302_Write_Byte(ds1302_control_add,0x80);		//打开写保护
}

/*******************************************************************************
* 函数名    : Ds1302_Read_Time()
* 函数功能	: 从DS1302中读取时间数据
* 输入参数  : 无
* 返回值    : 无
*******************************************************************************/
void Ds1302_Read_Time(unsigned char *buf_time)
{
   	unsigned char i,temp;
	unsigned char buf[8];
	buf[1]=Ds1302_Read_Byte(ds1302_year_add);			//年
	buf[2]=Ds1302_Read_Byte(ds1302_month_add);			//月
	buf[3]=Ds1302_Read_Byte(ds1302_date_add);			//日
	buf[4]=Ds1302_Read_Byte(ds1302_hr_add);			//时
	buf[5]=Ds1302_Read_Byte(ds1302_min_add);			//分
	buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;	//秒
	buf[7]=Ds1302_Read_Byte(ds1302_day_add);			//周

	for(i=0;i<8;i++)
	{           //BCD处理
		temp=buf[i]/16;
		buf_time[i]=buf[i]%16;
		buf_time[i]=buf_time[i]+temp*10;
	}
}

/*******************************************************************************
* 函数名    : Ds1302_Init()
* 函数功能	: DS1302初始化函数
* 输入参数  : 无
* 返回值    : 无
*******************************************************************************/
void Ds1302_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
  //initial LED1(PC0)
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			//上拉输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);			
  GPIO_SetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5);//GPIOF9,F10设置高，灯灭
	RST_CLR;												//RST脚置低
	SCK_ds_CLR;												//SCK_ds脚置低
	Ds1302_Write_Byte(ds1302_sec_add,0x00);
}
