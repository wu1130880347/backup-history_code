/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "semphr.h"

#include "uart_udp.h"
#include "my_leds.h"

/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_WIFI_SSID "esp_usb_uart"
#define EXAMPLE_WIFI_PASS "usb_uart"
#define HOST_IP_ADDR "192.168.4.1"

#define PORT 1009
static uint8_t leds_flag = 0;

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int WIFI_CONNECTED_BIT = BIT0;

static const char *TAG = "udp_client";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:

        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED");
        leds_flag = 1;
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        leds_flag = 1;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, IPV4_GOTIP_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        leds_flag = 0;
        break;
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void wait_for_ip()
{
    uint32_t bits = IPV4_GOTIP_BIT; // | IPV6_GOTIP_BIT ;

    ESP_LOGI(TAG, "Waiting for AP connection...");
    xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP");
}

static struct sockaddr_in destAddr;
static int sock = 0;
static uint8_t rx_buffer[UDP_BUF_SIZE] = {0};

static void udp_client_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1)
    {

        destAddr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_UDP;
        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

        sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        ESP_LOGI(TAG, "Socket created");
        ESP_LOGI(TAG, "Message sent");
        struct sockaddr_in sourceAddr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(sourceAddr);

        int err = sendto(sock, "client", 6, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err < 0)
        {
            ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
            break;
        }
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&sourceAddr, &socklen);
        if (len < 0)
        {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            break;
        }
        else
        {
            rx_buffer[len] = 0;
            ESP_LOGE(TAG, "rece data = %s", rx_buffer);
            if (rx_buffer[0] == 'O' && rx_buffer[1] == 'K')
            {
                ESP_LOGE(TAG, "leds_blink off");
                ESP_LOGE(TAG,"rece = %s",rx_buffer);
                reset_uart(rx_buffer);
                leds_blink(NULL + 1);
                leds_on(LEDR_IO);
            }
        }

        while (1)
        {

            len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&sourceAddr, &socklen);
            // Error occured during receiving
            if (len < 0)
            {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else
            {
                leds_on(LEDG_IO);
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "recv data num = %d", len);
                udp_put_tx_data(rx_buffer, &len);
                leds_off(LEDG_IO);
            }

            //vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

static void udp_send_task(void *pvParameters)
{
    uint16_t len = 0;
    uint8_t *rx_buffer = NULL;
    uint8_t led_f = 0;
    while (1)
    {
        if (leds_flag)
        {
            if ((led_f & 0x01) != 0x01)
            {
                leds_blink(NULL + 1);
                leds_on(LEDR_IO);
                led_f |= 0x01;
                led_f &= ~0x02;
            }

            if (xSemaphoreTake(udp_uart_sem, (TickType_t)10) == pdTRUE)
            {
                rx_buffer = udp_get_tx_data(&len);
                *(rx_buffer + len) = 0;
                int err = sendto(sock, rx_buffer, len, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
                ESP_LOGI(TAG, " send data num = %d", len);
                if (err < 0)
                {
                    ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                    break;
                }
            }
        }
        else
        {
            if ((led_f & 0x02) != 0x02)
            {
                leds_blink(NULL);
                led_f |= 0x02;
                led_f &= ~0x01;
            }

            if (xSemaphoreTake(udp_uart_sem, (TickType_t)10) == pdTRUE)
            {
                rx_buffer = udp_get_tx_data(&len);
                *(rx_buffer + len) = 0;
                ESP_LOGI(TAG, " clear send data num = %d", len);
            }
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    initialise_wifi();
    Init_leds();
    leds_blink(NULL);
    wait_for_ip();
    ESP_LOGI(TAG, "udp_client_task created");
    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "uart0_init created");
    uart0_init();
    ESP_LOGI(TAG, "udp_send_task created");
    xTaskCreate(udp_send_task, "udp_send_task", 4096, NULL, 2, NULL);

    ESP_LOGI(TAG, "app_main end");
}
