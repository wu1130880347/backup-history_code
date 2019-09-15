#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "semphr.h"

#include "uart_udp.h"

/* FreeRTOS event group to signal when we are connected & ready to make a request */

static const char *TAG = "uart_udp";

static QueueHandle_t uart0_queue;
SemaphoreHandle_t udp_uart_tx_sem = NULL;
SemaphoreHandle_t udp_uart_sem = NULL;
SemaphoreHandle_t xMutex_buffer = NULL;
uint8_t udp_buffer[UDP_BUF_SIZE] = {0};
uint8_t temp_buffer[UDP_BUF_SIZE] = {0};

void uart0_init(void);
static void uart_event_task(void *pvParameters);
static void add_rxbuf_to_udpbuf(uint8_t *udp_buffer, uint8_t *dtmp, uint16_t len);
uint8_t *udp_get_tx_data(uint16_t *len);
uint16_t udp_put_tx_data(uint8_t *data, int *len);
void reset_uart(uint8_t *rx_buffer);

void uart0_init(void)
{
    // Configure parameters of an UART driver,
    // communication pins and install the driver
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(EX_UART_NUM, &uart_config);

    // Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 100, &uart0_queue);

    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 4, NULL);
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *)malloc(RD_BUF_SIZE);

    vSemaphoreCreateBinary(udp_uart_sem);

    if (udp_uart_sem == NULL)
    {
        ESP_LOGE(TAG, "vSemaphoreCreateBinary create failed");
    }
    xSemaphoreTake(udp_uart_sem, (TickType_t)10);

    vSemaphoreCreateBinary(udp_uart_tx_sem);
    if (udp_uart_tx_sem == NULL)
    {
        ESP_LOGE(TAG, "vSemaphoreCreateBinary create failed");
    }
    xSemaphoreTake(udp_uart_tx_sem, (TickType_t)10);

    ESP_LOGE(TAG, "xSemaphoreTake udp_uart_sem");
    // Create the mutex to guard a shared resource.
    xMutex_buffer = xSemaphoreCreateRecursiveMutex();

    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void *)&event, (portTickType)portMAX_DELAY))
        {
            bzero(dtmp, RD_BUF_SIZE);
            //ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);

            switch (event.type)
            {
            // Event of UART receving data
            // We'd better handler data event fast, there would be much more data events than
            // other types of events. If we take too much time on data event, the queue might be full.
            case UART_DATA:
                //ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                add_rxbuf_to_udpbuf(udp_buffer, dtmp, event.size);
                //uart_write_bytes(EX_UART_NUM, (const char *)dtmp, event.size);
                break;

            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGE(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;

            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGE(TAG, "ring buffer full");
                // If buffer full happened, you should consider encreasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;

            case UART_PARITY_ERR:
                ESP_LOGE(TAG, "uart parity error");
                break;

            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGE(TAG, "uart frame error");
                break;

            // Others
            default:
                ESP_LOGE(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }

    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

static uint16_t sum = 0;
static void add_rxbuf_to_udpbuf(uint8_t *udp_buffer, uint8_t *dtmp, uint16_t len)
{

    volatile uint16_t i = 0;

    // See if we can obtain the mutex.  If the mutex is not available
    // wait 50 ticks to see if it becomes free.
    if (xSemaphoreTakeRecursive(xMutex_buffer, (TickType_t)50) == pdTRUE)
    {
        //the udp transfer buffer no empty
        //ESP_LOGE(TAG,"sum = %d",sum + len);
        if (sum + len >= UDP_BUF_SIZE)
        {
            xSemaphoreGiveRecursive(xMutex_buffer);
            xSemaphoreGive(udp_uart_sem);
            return;
        }

        //ESP_LOGI(TAG, " udp transfer buffer = %d", sum);

        for (i = 0; i < len + sum; i++)
        {
            *(udp_buffer + sum + i) = *(dtmp + i);
        }
        sum += len;

        // ESP_LOGI(TAG, " udp buffer len = %d", len);
        xSemaphoreGiveRecursive(xMutex_buffer);
        xSemaphoreGive(udp_uart_sem);

        // Now the mutex can be taken by other tasks.
    }
    else
    {
        // We could not obtain the mutex and can therefore not access
        // the shared resource safely.
    }
}

uint8_t *udp_get_tx_data(uint16_t *len)
{
    if (xSemaphoreTakeRecursive(xMutex_buffer, (TickType_t)50) == pdTRUE)
    {
        memcpy(temp_buffer, udp_buffer, sum);
        *len = sum;
        sum = 0;
        xSemaphoreGiveRecursive(xMutex_buffer);
    }
    else
    {
        // We could not obtain the mutex and can therefore not access
        // the shared resource safely.
    }

    return temp_buffer;
}

uint16_t udp_put_tx_data(uint8_t *data, int *len)
{
    uart_write_bytes(EX_UART_NUM, (const char *)data, *len);
    return *len;
}

void reset_uart(uint8_t *rx_buffer)
{
    uint32_t bps = 0;
    uint8_t *temp = rx_buffer;
    uint8_t i = 0;
    rx_buffer += 2;
    for (i = 0; *(rx_buffer + i) >= '0' && *(rx_buffer + i) <= '9'; i++)
        ;
    for (; i > 0; i--)
    {
        bps = bps * 10 + *(rx_buffer++) - '0';
    }
    
    ESP_LOGE(TAG, "bps = %d", bps);
    rx_buffer = temp;
    uart_set_baudrate(EX_UART_NUM, bps);
    return;
}