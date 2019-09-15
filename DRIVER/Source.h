//P1：P1.0~P1.3     外部中断;
//P1：P1.6          OUT_TA1;
//P2：P2.2~P2.3     CAPD_OUT/CAPD_IN  比较0.5Vcc
//P3：P3.4--P3.5    TXD/RXD
//P4：P4.0~P4.3     LCD12_864;
//P4：P4.7          TBCLK_IN;
//P5：P5.4~P5.6     MCLK SMCLK ACLK:8MHz 1MHz  32KHz;
//P6：P6.0          ADC12_IN_1;

#ifndef __SOURCE_H_
#define __SOURCE_H_
#include "lcd_12864.h"
#include "usart.h"
#include "sys.h"
#include "adc.h"
#include "exti.h"
#include "pwm.h"
#include "timer.h"
#include "dac.h"
#include "spi.h"
#include "rtc.h"
#include "stm32f4xx.h"
#include "driver.h"
#include "ProHead.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "24l01.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
//#include "pid.h"
#include "adxl345.h"
#include "ProHead.h"
#include "lcd_driver.h" 
#include "Touch.h"
#include "lcdlib.h"
//#include "work_touch.h"
#include "bsp_ov7725.h"
#include "fsmc_driver.h"
//#include "lcd.h"
#include "FILE_SYS.H"
#include "ds1302.h"
#include "ds18b20.h"

void Osc_Init(void);
void Osc_Show(void);
void Init_F4(void);
void rf24l01_RX_TX(void);
void mpu6050_Init(void);
void adxl345_Init(void);
void Tft3_2_Init(void);
void ov7670_init(void);
void Dis_AXPoint(u16 Row, u16 Column, u16 Color);
void usart1_send_char(u8 c);
void SD_file_Init(void);
extern u8 buf_time[8];


#endif