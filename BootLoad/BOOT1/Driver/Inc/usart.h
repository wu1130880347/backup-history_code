#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h"
#include "DriverConfig.h"

//≥Ã–ÚLOG¥Ú”°
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
    extern u8 Dprintf(u8 enable_flag, const char *tag, const char *format, ...);
#else
    #define Dprintf(en, tag, ...) 0
    #define EN_LOG 0
    #define TAG ""
#endif
extern void uart_init(u32 bound);
#endif


