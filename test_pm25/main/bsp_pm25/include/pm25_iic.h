#ifndef __PM25_IIC_H_
#define __PM25_IIC_H_

#include "bsp_drivers.h"
#include "bsp_includes.h"
extern volatile gpio_struct_t GPIO;

#ifndef SOFT_IIC
#define SOFT_IIC TRUE
#endif

#ifndef HARD_IIC
#define HARD_IIC FALSE
#endif

#define IIC_DECTION TRUE
#define DEBUG_UART TRUE


#if (defined SOFT_IIC) && (SOFT_IIC == TRUE)

//IO��������
#define SDA_IN()  GPIO.enable_w1tc |= I2C_SDA_MASK;
#define SDA_OUT() GPIO.enable_w1ts |= I2C_SDA_MASK;

//IO��������
//λ������Ӧ��IO��	 
#define IIC_SCL_1  {GPIO.out_w1ts |= I2C_SCL_MASK;}   //SCL
#define IIC_SCL_0  {GPIO.out_w1tc |= I2C_SCL_MASK;}   //SCL
#define IIC_SDA_1  {GPIO.out_w1ts |= I2C_SDA_MASK;}   //SDA	 
#define IIC_SDA_0  {GPIO.out_w1tc |= I2C_SDA_MASK;}   //SDA
#define READ_SDA   (GPIO.in & I2C_SDA_MASK) //����SDA 

//IIC���в�������
extern void IIC_Start(void);				//����IIC��ʼ�ź�
extern void IIC_Stop(void);

extern void IIC_Init(void);                //��ʼ��IIC��IO��
extern s8 test_i2c(void *arg);
extern u8 IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#elif (defined HARD_IIC) && (HARD_IIC == TRUE)

extern void IIC_Init(void);                //��ʼ��IIC��IO��	
extern u8 IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
extern s8 test_i2c(void *arg);

#else

#warning "NOT Use I2C Drivers Successful... ";

#endif

#endif

















