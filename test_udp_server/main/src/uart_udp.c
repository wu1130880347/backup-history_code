#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "semphr.h"

#include "uart_udp.h"
#include "cli_cmd.h"

static const char *TAG = "uart_udp";
static QueueHandle_t uart0_queue;

SemaphoreHandle_t udp_uart_sem = NULL;
SemaphoreHandle_t udp_cli_sem = NULL;
SemaphoreHandle_t xMutex_buffer = NULL;
SemaphoreHandle_t client_reset_sem = NULL;
uint8_t udp_buffer[UDP_BUF_SIZE] = {0};
uint8_t temp_buffer[UDP_BUF_SIZE] = {0};

void uart0_init(void);
static void uart_event_task(void *pvParameters);

//static void uart_irq_deal(void *pvParameters);

static void add_rxbuf_to_udpbuf(uint8_t *udp_buffer, uint8_t *dtmp, uint16_t len);
uint16_t udp_put_tx_data(uint8_t *data, int *len);
uint8_t *udp_get_tx_data(uint16_t *len);

/* static void uart_irq_deal(void *pvParameters)
{
    static uint32_t uart_irq_flag = 0;
    uart_irq_flag = READ_PERI_REG(UART_INT_ST(0));
    if(uart_irq_flag & UART_RXFIFO_FULL_INT_ST == UART_RXFIFO_FULL_INT_ST)
    {
        add_rxbuf_to_udpbuf(udp_buffer, dtmp, event.size);
        UART_INT_CLR(UART_RXFIFO_FULL_INT_CLR);
        return ;
    }
    else if(uart_irq_flag & UART_RXFIFO_TOUT_INT_ST == UART_RXFIFO_TOUT_INT_ST)
    {
        add_rxbuf_to_udpbuf(udp_buffer, dtmp, event.size);
        UART_INT_CLR(UART_RXFIFO_TOUT_INT_CLR);
        return ;
    }
} */


uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

void uart0_init(void)
{
    // Configure parameters of an UART driver,
    // communication pins and install the driver

    uart_param_config(EX_UART_NUM, &uart_config);

    // uart_intr_config_t uart_config_int = {
    //     .rx_timeout_thresh = 9;
    // }
    //uart_intr_config(EX_UART_NUM, &uart_config_int);
    //uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL);

    //uart_enable_intr_mask(EX_UART_NUM, UART_RXFIFO_FULL_INT_ST | UART_RXFIFO_TOUT_INT_ST);
    //uart_isr_register(EX_UART_NUM, uart_irq_deal, NULL);
    //uart_enable_rx_intr(EX_UART_NUM);

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
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
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
        //ESP_LOGE(TAG,"cli_get_flag = %d",cli_get_flag);
        if (cli_get_flag)
        {
            //  ESP_LOGE(TAG,"len = %d",cli_get_flag);
            if (len < 40)
            {
                for (i = 0; i < len; i++)
                    cmd_line[i] = *(dtmp + i);
                cmd_line[i] = 0;
                xSemaphoreGive(udp_cli_sem);
            }
        }

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