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
//#include "pid.h"
#define   Num_of_Results   32                       //adc采集次数


#define CPU_F ((double)8000000)
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
static u32 fag = 0;                                  //TBR计数标记
static u16 set_Speed = 0;
static u8 pwm_on = 0;                             //pwm占空比
static u16 results[Num_of_Results];              //ADC数据
extern u8 Pi,Ii,Di;

void Trans_val(u16 Hex_Val);               //ADC_12_12864--show
void Time_s();                              //Time_Show
void Time_s_up();
void PutString(u8 *ptr);
void Osc_Init(void);
void work_pid(u16 now_Speed,u16 setspeed);
int PID_con(int Error,u8 P,u8 I,u8 D);

#endif