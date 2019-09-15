#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

#include "my_leds.h"
#include "cli_cmd.h"
static const char *TAG = "my_leds";

static xQueueHandle gpio_evt_queue = NULL;

Leds ledr = {LEDR_IO, 0, 5, 0};

void Init_leds(void);
void leds_on(uint32_t led);
void leds_off(uint32_t led);
void leds_blink(void *st);
static void gpio_task_irq(void *arg);

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_irq(void *arg)
{
    uint32_t io_num;
    uint8_t btn_flag = 0;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            btn_flag = gpio_get_level(io_num);
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d", io_num, btn_flag);
            vTaskDelay(30 / portTICK_PERIOD_MS);
            if (!btn_flag)
            {
                leds_off(LEDR_IO);
                cli_get_flag = 1;
                cli_help_log(SET_EXAMPLE);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                leds_on(LEDR_IO);
            }
        }
    }
}

void Init_leds(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.
    io_conf.pin_bit_mask = LEDR_MASK | LEDG_MASK | LEDB_MASK;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    gpio_set_level(LEDR_IO, HIGH);
    gpio_set_level(LEDG_IO, HIGH);
    gpio_set_level(LEDB_IO, HIGH);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = BTN0_MASK;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(BTN0_IO, GPIO_INTR_NEGEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_irq, "gpio_task_irq", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(BTN0_IO, gpio_isr_handler, (void *)BTN0_IO);

    return;
}

void leds_on(uint32_t led)
{
    if (led == LEDR_IO)
        gpio_set_level(LEDR_IO, LOW);

    if (led == LEDG_IO)
        gpio_set_level(LEDG_IO, LOW);
    return;
}
void leds_off(uint32_t led)
{
    if (led == LEDR_IO)
        gpio_set_level(LEDR_IO, HIGH);
    if (led == LEDG_IO)
        gpio_set_level(LEDG_IO, HIGH);
    return;
}

static uint8_t leds_flag = 0;
static void leds_blink_task(void *pvParameters)
{
    Leds *arg = &ledr;
    ESP_LOGE(TAG, "gpio = %d mode = %d freq = %d duty = %d", arg->gpio, arg->mode, arg->freq, arg->duty);
    arg->freq = portTICK_PERIOD_MS / arg->freq;
    arg->freq >>= 1;
    ESP_LOGE(TAG, "arg->freq = %d", arg->freq);
    arg->freq = portTICK_PERIOD_MS;
    ESP_LOGE(TAG, "arg->freq = %d", arg->freq);
    arg->freq = 200;
    for (;;)
    {
        gpio_set_level(arg->gpio, HIGH);
        vTaskDelay(arg->freq / portTICK_PERIOD_MS);
        gpio_set_level(arg->gpio, LOW);
        vTaskDelay(arg->freq / portTICK_PERIOD_MS);
        if (leds_flag)
            break;
    }
    vTaskDelete(NULL);
}

void leds_blink(void *st)
{
    if (st == NULL)
    {
        leds_flag = 0;
        xTaskCreate(leds_blink_task, "leds_blink_task", 1024, NULL, 13, NULL);
    }
    else
        leds_flag = 1;
}
