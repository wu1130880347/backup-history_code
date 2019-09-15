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

//user include file
#include "uart_udp.h"
#include "my_leds.h"
#include "cli_cmd.h"

/* The examples use simple WiFi configuration that you can set via
   'make menuconfig'.
   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_MODE_AP TRUE //TRUE:AP FALSE:STA
#define EXAMPLE_ESP_WIFI_SSID "esp_usb_uart"
#define EXAMPLE_ESP_WIFI_PASS "usb_uart"
#define EXAMPLE_MAX_STA_CONN 6
#define AP_LOCAL_IP "192.168.4.1"

#define PORT 1009

extern u8_t new_ip_ok[4];
static uint8_t leds_flag = 0;

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;
const int IPV4_GOTIP_BIT = BIT0;
const int WIFI_CONNECTED_BIT = BIT0;

static const char *TAG = "udp_server";
static void udp_send_task(void *pvParameters);

static struct sockaddr_in sourceAddr = {
    .sin_addr.s_addr = (u32_t)0xc0a80402,
    .sin_family = AF_INET,
    .sin_port = (u16_t)(PORT),
};
static socklen_t socklen = sizeof(sourceAddr);
static int sock = 0;
static uint8_t udp_tx_flag = 0;
static uint8_t rx_buffer[UDP_BUF_SIZE] = {0};

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:" MACSTR " join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);

        ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip);
        sourceAddr.sin_addr.s_addr = (u32_t)(new_ip_ok[0] | new_ip_ok[1] << 8 | new_ip_ok[2] << 16 | new_ip_ok[3] << 24);
        sourceAddr.sin_family = AF_INET;
        sourceAddr.sin_port = htons(PORT);
        ESP_LOGI(TAG, "connect device ip:%s", ip4addr_ntoa((const ip4_addr_t *)&sourceAddr.sin_addr.s_addr));
        leds_flag = 1;
        udp_tx_flag = 0;

        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:" MACSTR "leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        leds_flag = 0;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGE(TAG, "wifi_init_softap finished.SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}
//default  client  bps  115200
uint8_t str_init_bps[10] = "OK115200";
static void udp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;
    int err = 0;
    uint8_t recall_flag = 0;
    uint8_t first_sem = 0;

    vSemaphoreCreateBinary(client_reset_sem);
    if (client_reset_sem == NULL)
    {
        ESP_LOGE(TAG, "vSemaphoreCreateBinary create failed");
    }
    xSemaphoreTake(client_reset_sem, (TickType_t)10);

    while (1)
    {

        struct sockaddr_in destAddr;
        destAddr.sin_addr.s_addr = inet_addr(AP_LOCAL_IP);
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_UDP;
        inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

        ESP_LOGI(TAG, "Open the Socket...\n");
        sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");
        err = bind(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
        if (err < 0)
        {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket binded");

        ESP_LOGI(TAG, "Waiting for data");

        while (1)
        {
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&sourceAddr, &socklen);
            if (len < 0)
            {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else
            {
                leds_on(LEDG_IO);
                ESP_LOGI(TAG, " recv data num = %d ", len);
                rx_buffer[len] = 0;
                udp_put_tx_data(rx_buffer, &len);
                leds_off(LEDG_IO);
                //uart_config.baud_rate;
                if (udp_tx_flag == 0 && leds_flag == 1)
                {
                    str_init_bps[0] = 'O';
                    str_init_bps[1] = 'K';
                    ESP_LOGE(TAG, " str_init_bps = %s ", str_init_bps);
                    err = sendto(sock, str_init_bps, 10, 0, (struct sockaddr *)&sourceAddr, sizeof(sourceAddr));
                    udp_tx_flag = 1;
                    recall_flag = 1;
                }

                if (recall_flag)
                {
                    if (*rx_buffer == 'c' && first_sem == 1)
                    {
                        ESP_LOGE(TAG, " the client reset bps successful ... ");
                        xSemaphoreGive(client_reset_sem);
                    }
                    recall_flag = 0;
                    first_sem = 1;
                }

                if (err < 0)
                {
                    ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                    break;
                }
            }
        }
        if (sock != -1)
        {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
            break;
        }
    }
    vTaskDelete(NULL);
}

static void udp_send_task(void *pvParameters)
{
    uint16_t len = 0;
    uint8_t *rx_buffer = NULL;
    uint8_t led_f = 0;
    int err = 0;
    while (1)
    {

        //ESP_LOGI(TAG, "udp_txrx_sem send data");

        if (leds_flag)
        {
            if (xSemaphoreTake(udp_uart_sem, (TickType_t)10) == pdTRUE)
            {
                rx_buffer = udp_get_tx_data(&len);
                *(rx_buffer + len) = 0;
                err = sendto(sock, rx_buffer, len, 0, (struct sockaddr *)&sourceAddr, sizeof(sourceAddr));
                ESP_LOGI(TAG, " send data num = %d", len);
            }
            //ESP_LOGI(TAG, " send data num = %d", 2);
            if ((led_f & 0x01) != 1)
            {
                leds_blink(NULL + 1);
                leds_on(LEDR_IO);
                ESP_LOGI(TAG, " send data num = %d", 1);
                led_f |= 0x01;
                led_f &= ~0x02;
            }
        }
        else
        {
            if (xSemaphoreTake(udp_uart_sem, (TickType_t)10) == pdTRUE)
            {
                rx_buffer = udp_get_tx_data(&len);
                *(rx_buffer + len) = 0;
                ESP_LOGI(TAG, " clear send data num = %d", len);
            }
            if ((led_f & 0x02) != 0x02)
            {
                leds_blink(NULL);
                led_f |= 0x02;
                led_f &= ~0x01;
            }
        }

        if (err < 0)
        {
            ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
            break;
        }

        vTaskDelay(60 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    Init_leds();
    leds_blink(NULL);

    initialise_wifi();
    ESP_LOGI(TAG, "udp_server_task created");

    ESP_LOGI(TAG, "uart0_init created");
    uart0_init();

    xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);

    ESP_LOGI(TAG, "udp_send_task created");
    xTaskCreate(udp_send_task, "udp_send_task", 4096, NULL, 2, NULL);

    ESP_LOGI(TAG, "cli_init created");
    cli_init();

    ESP_LOGI(TAG, "app_main end");
    leds_blink(NULL + 1);
    leds_on(LEDR_IO);
    ESP_LOGI(TAG, "leds_blink off");
}
