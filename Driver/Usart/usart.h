#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"
#define __ON__    1
#define __OFF__   0

//调试信息颜色设置
#define BLACK 0x81
#define RED 0x41
#define BLUE 0x05
#define GREEN 0x01
#define WHITE 0x21
#define YELLOW 0x03
#define PINK 0x09
#define CYAN 0x11

#define USART_DEBUG     __ON__
//程序LOG打印
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
    extern u8 Dprintf(u8 enable_flag, const char *tag, const char *format, ...);
#else
    #define Dprintf(en, tag, ...) 0
    #define EN_LOG 0
    #define TAG ""
#endif
extern void uart_init(u32 bound);
#endif


