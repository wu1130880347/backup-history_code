#ifndef __PM25_DRIVERS__
#define __PM25_DRIVERS__
#include "bsp_drivers.h"

extern s8 boards_init(void *arg);
extern s8 bsp_init(void *arg);
extern s8 test_i2c(void *arg);
extern s8 test_uart(void *arg);

#endif