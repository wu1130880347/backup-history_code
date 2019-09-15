#include "pm25_drivers.h"
#include "bsp_drivers.h"
#include "pm25_iic.h"
#include "lcd_12864.h"

static const char *TAG = "pm25_drv";

//extern funs interfaces
s8 boards_init(void *arg);
s8 bsp_init(void *arg);

//local funs

s8 boards_init(void *arg)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULL_DOWN_PIN_SEL;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULL_UP_PIN_SEL;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULL_UP_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULL_DOWN_PIN_SEL;
    gpio_config(&io_conf);

    gpio_set_level(PM_RST_IO,1);
    gpio_set_level(LEDB_IO,1);
    gpio_set_level(LEDG_IO,1);

    ESP_LOGI(TAG, "board init successful ... \n");

    return TRUE;
}

s8 bsp_init(void *arg)
{
    IIC_Init();
    test_i2c(NULL);
    OLED_Init();
    OLED_ShowString(0,0,(u8*)"hello",16);
    OLED_DrawBMP(0,0,127,7,NULL);
    //Picture();
    return TRUE;
}
