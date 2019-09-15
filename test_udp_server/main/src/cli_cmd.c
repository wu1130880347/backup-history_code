#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "cli_cmd.h"
#include "uart_udp.h"

static const char *TAG = "cli_cmd";

const uint8_t cmd_info1[] = "\
\r\n        the wifi uart system transmission baud_rate set example: \
\r\n        set bps 115200 \
\r\n        set bps 57600 \
\r\n        set bps 9600 \
\r\n        ...\
\r\n        set bps 1200\
\r\n        notes !!!\
\r\n        the max bps = 115200\
\r\n        the min bps = 1200\
\r\n        [ the default bps ] = 115200\
\r\n                                \
\r\n        please input the above strings....\
";

const uint8_t cmd_info2[] = "\
\r\n                            \
\r\n        set bps successful, please reset the client device...\
\r\n                           \
";
const uint8_t cmd_info3[] = "\
\r\n                            \
\r\n        the client bps reset ok, please change the master consule bps... \
\r\n                              \
";
const uint8_t cmd_info4[] = "\
\r\n                            \
\r\n        the client bps set fialed, please retry... \
\r\n                            \
";

uint8_t cli_get_flag = 0;

uint8_t cmd_line[40] = {0};
uint8_t cli_init(void);
uint8_t cli_help_log(uint8_t log_flag);
static void set_bps(uint8_t *cmd_line);
static void cli_task(void *pvParameters);

uint8_t cli_init(void)
{
    xTaskCreate(cli_task, "cli_task", 2048, NULL, 12, NULL);
    return 0;
}
uint8_t cli_help_log(uint8_t log_flag)
{
    switch (log_flag)
    {
    case SET_EXAMPLE:
        uart_write_bytes(EX_UART_NUM, (const char *)cmd_info1, (int)sizeof(cmd_info1));
        break;
    case SET_OK:
        uart_write_bytes(EX_UART_NUM, (const char *)cmd_info2, (int)sizeof(cmd_info2));
        break;
    case SET_STATE:
        uart_write_bytes(EX_UART_NUM, (const char *)cmd_info3, (int)sizeof(cmd_info3));
        break;
    case SET_FIALED:
        uart_write_bytes(EX_UART_NUM, (const char *)cmd_info4, (int)sizeof(cmd_info4));
        break;
    default:
        break;
    }
    return 0;
}

static void cli_task(void *pvParameters)
{
    vSemaphoreCreateBinary(udp_cli_sem);
    if (udp_cli_sem == NULL)
    {
        ESP_LOGE(TAG, "vSemaphoreCreateBinary create failed");
    }
    xSemaphoreTake(udp_cli_sem, (TickType_t)10);
    ESP_LOGE(TAG, "xSemaphoreTake udp_cli_sem");

    for (;;)
    {
        if (xSemaphoreTake(udp_cli_sem, (TickType_t)50) == pdTRUE)
        {
            ESP_LOGI(TAG, "cli_line = %s", cmd_line);
            if (*cmd_line == 's' && *(cmd_line + 1) == 'e')
                set_bps(cmd_line);
            else
            {
                cli_help_log(SET_FIALED);
                cli_get_flag = 0;
            }
            //ESP_LOGI(TAG, "cmd_info1 = %s", cmd_info1);
            //ESP_LOGI(TAG, "cmd_info2 = %s", cmd_info2);
            //ESP_LOGI(TAG, "cmd_info3 = %s", cmd_info3);
        }
        vTaskDelay(60 / portTICK_PERIOD_MS);
    }
}

static void set_bps(uint8_t *cmd_line)
{
    volatile uint8_t i = 0;
    uint8_t *temp = cmd_line;
    uint32_t bps = 0;
    for (i = 0; i < 40; i++)
        if (*(cmd_line + i) >= '0' && *(cmd_line + i) <= '9')
            break;
    cmd_line += i;
    //send bps to the client and it need to reset geting the bps data
    for (i = 0; *(cmd_line + i) >= '0' && *(cmd_line + i) <= '9'; i++)
    {
        *(str_init_bps + i + 2) = *(cmd_line + i);
    }
    *(str_init_bps + i + 2) = 0;
    ESP_LOGE(TAG, "str_init_bps = %s", str_init_bps);

    for (i = 0; i < 40; i++)
        if (!(*(cmd_line + i) >= '0' && *(cmd_line + i) <= '9'))
            break;

    while (i--)
    {
        bps = bps * 10 + *(cmd_line++) - '0';
    }

    cli_help_log(SET_OK);
    if (xSemaphoreTake(client_reset_sem, (TickType_t)50000) == pdTRUE)
    {
        cli_help_log(SET_STATE);
        ESP_LOGE(TAG, "uart_set_baudrate = %d", bps);
        uart_set_baudrate(EX_UART_NUM, bps);
        uart_config.baud_rate = bps;
        cli_get_flag = 0;
        cmd_line = temp;
    }
    else
    {
        cli_help_log(SET_FIALED);
    }
    return;
}