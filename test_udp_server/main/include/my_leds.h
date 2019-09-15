#ifndef __MY_LEDS__
#define __MY_LEDS__
#include "stdint.h"
#include "semphr.h"

#define HIGH 1
#define LOW 0

#define GPIO_0 0
#define GPIO_1 1
#define GPIO_2 2
#define GPIO_3 3
#define GPIO_4 4
#define GPIO_5 5
#define GPIO_6 6
#define GPIO_7 7
#define GPIO_8 8
#define GPIO_9 9
#define GPIO_10 10
#define GPIO_11 11
#define GPIO_12 12
#define GPIO_13 13
#define GPIO_14 14
#define GPIO_15 15
#define GPIO_16 16
//bps set button
#define BTN0_IO GPIO_5
//3 color led io control
#define LEDR_IO GPIO_16
#define LEDB_IO GPIO_2
#define LEDG_IO GPIO_4
#define LEDR_MASK (1ULL << LEDR_IO)
#define LEDG_MASK (1ULL << LEDG_IO)
#define LEDB_MASK (1ULL << LEDB_IO)

#define BTN0_MASK (1ULL << BTN0_IO)

typedef struct {
    uint32_t gpio;
    uint8_t mode;
    uint16_t freq;
    uint8_t duty;
}Leds;


extern void Init_leds(void);
extern void leds_blink(void *st);
extern void leds_on(uint32_t led);
extern void leds_off(uint32_t led);

#endif