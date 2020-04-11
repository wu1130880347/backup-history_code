#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"

extern void delay_init(u8 SYSCLK);
extern void delay_us(u32 nus);
extern void delay_ms(u32 nms);
extern void delay_xms(u32 nms);
#endif





























