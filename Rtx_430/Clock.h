#ifndef __CLOCK_H_
#define __CLOCK_H_

#include "msp430x14x.h"
#include "lcd_12864.h"

void Clock_Init();
void CAPD_Start(void);
void ADC_12Start(void);
void InitUART(void);
void DAC12_Init(void);
void P1_Init(void);
void Wdt_1s(void);
void Ago_Init(void);

#endif