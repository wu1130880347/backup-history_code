#ifndef __UART_UDP_H__
#define __UART_UDP_H__
#include "semphr.h"
#include "stdint.h"
#define UDP_BUFFER_OK 1
#define UDP_BUFFER_BUSY 0

#define UDP_TXRX_OK 1
#define UDP_TXRX_BUSY 0

#define EX_UART_NUM UART_NUM_0
#define BUF_UDP_LEN 2
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (128)

#define UDP_BUF_SIZE (BUF_SIZE * BUF_UDP_LEN) //5k  udp transfer buffer


extern void uart0_init(void);
extern SemaphoreHandle_t udp_uart_sem;
extern SemaphoreHandle_t xMutex_buffer;
extern uint8_t udp_buffer[UDP_BUF_SIZE];

extern void reset_uart(uint8_t *rx_buffer);
extern uint8_t *udp_get_tx_data(uint16_t *len);
extern uint16_t udp_put_tx_data(uint8_t * data,int *len);


#endif