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

//IO方向设置
#define SDA_IN()  GPIO.enable_w1tc |= I2C_SDA_MASK;
#define SDA_OUT() GPIO.enable_w1ts |= I2C_SDA_MASK;

//IO操作函数
//位操作对应的IO口	 
#define IIC_SCL_1  {GPIO.out_w1ts |= I2C_SCL_MASK;}   //SCL
#define IIC_SCL_0  {GPIO.out_w1tc |= I2C_SCL_MASK;}   //SCL
#define IIC_SDA_1  {GPIO.out_w1ts |= I2C_SDA_MASK;}   //SDA	 
#define IIC_SDA_0  {GPIO.out_w1tc |= I2C_SDA_MASK;}   //SDA
#define READ_SDA   (GPIO.in & I2C_SDA_MASK) //输入SDA 

//IIC所有操作函数
extern void IIC_Start(void);				//发送IIC开始信号
extern void IIC_Stop(void);

extern void IIC_Init(void);                //初始化IIC的IO口
extern s8 test_i2c(void *arg);
extern u8 IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#elif (defined HARD_IIC) && (HARD_IIC == TRUE)

extern void IIC_Init(void);                //初始化IIC的IO口	
extern u8 IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
extern s8 test_i2c(void *arg);

#else

#warning "NOT Use I2C Drivers Successful... ";

#endif

#endif

















