#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED0 PAout(8)       // DS0
extern void LED_Init(void); //初始化
#endif
