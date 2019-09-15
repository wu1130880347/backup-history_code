#ifndef __BSP_DRIVERS__
#define __BSP_DRIVERS__

#include "bsp_includes.h"

#ifndef u8
#define u8 unsigned char
#else
#undef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#else
#undef u16
#define u16 unsigned short
#endif

#ifndef u32
#define u32 unsigned long
#else
#undef u32
#define u32 unsigned long
#endif

#ifndef s8
#define s8 char
#else
#undef s8
#define s8 char
#endif

#ifndef s16
#define s16 short
#else
#undef s16
#define s16 short
#endif

#ifndef s32
#define s32 long
#else
#undef s32
#define s32 long
#endif

#ifndef TRUE
#define TRUE 1
#else
#undef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#else
#undef FALSE
#define FALSE 0
#endif


#endif