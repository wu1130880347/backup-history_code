/* gpio example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

#include "pm25_drivers.h"

static const char *TAG = "main";

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO15: output
 * GPIO16: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO15 with GPIO4
 * Connect GPIO16 with GPIO5
 * Generate pulses on GPIO15/16, that triggers interrupt on GPIO4/5
 *
 */

void app_main(void)
{
    int cnt = 0;
    boards_init(NULL);
    gpio_set_level(EN_OLED_IO, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    bsp_init(NULL);

    while (1)
    {
       // ESP_LOGI(TAG, "cnt: %d\n", cnt++);
        vTaskDelay(1000 / portTICK_RATE_MS);
        // gpio_set_level(LEDR_IO, 0);
        // gpio_set_level(LEDG_IO, 1);
        // gpio_set_level(LEDB_IO, 1);
        // vTaskDelay(1000 / portTICK_RATE_MS);
        // gpio_set_level(LEDR_IO, 1);
        // gpio_set_level(LEDG_IO, 0);
        // gpio_set_level(LEDB_IO, 1);
        // vTaskDelay(1000 / portTICK_RATE_MS);
        // gpio_set_level(LEDR_IO, 1);
        // gpio_set_level(LEDG_IO, 1);
        // gpio_set_level(LEDB_IO, 0);
    }
}
