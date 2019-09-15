
#ifndef __DS1302_H__
#define __DS1302_H__

#include "Source.h"



//sbit SCK_ds=P2^2;		//最新电池供电
//sbit SDA_ds=P2^1;
//sbit RST=P2^0;
#define SCK_ds delay_us(2);PFout(1)	// DS1
#define SDA_ds delay_us(2);PFout(3)	// DS1
#define RST_ds delay_us(2);PFout(5)	// DS1
//复位脚
#define RST_CLR	RST_ds=0//电平置低
#define RST_SET	RST_ds=1//电平置高


//双向数据
#define IO_CLR	SDA_ds=0//电平置低
#define IO_SET	SDA_ds=1//电平置高
#define IO_R	PFin(3)  //电平读取


//时钟信号
#define SCK_ds_CLR	SCK_ds=0//时钟信号
#define SCK_ds_SET	SCK_ds=1//电平置高


#define ds1302_sec_add				0x80		//秒数据地址
#define ds1302_min_add				0x82		//分数据地址
#define ds1302_hr_add				0x84		//时数据地址
#define ds1302_date_add				0x86		//日数据地址
#define ds1302_month_add			0x88		//月数据地址
#define ds1302_day_add				0x8a		//星期数据地址
#define ds1302_year_add				0x8c		//年数据地址
#define ds1302_control_add			0x8e		//控制数据地址
#define ds1302_charger_add			0x90 					 
#define ds1302_clkburst_add			0xbe


/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);

/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr);

/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(unsigned char *buf_time);

/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(unsigned char *buf_time);

/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void);

#endif
