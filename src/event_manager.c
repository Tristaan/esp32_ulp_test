#include <string.h>
#include <esp_sleep.h>
#include <esp_err.h>
#include <esp_log.h>
#include "event_manager.h"
#include "ulp_main.h"

const char* TAG = "event_manager";
void deliver_data(ip_event_got_ip_t* event) {
    // Do something with the IP address
    ESP_LOGI(TAG, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
}

void start_ulp_program() {
    esp_sleep_enable_ulp_wakeup();

    ESP_ERROR_CHECK(ulp_run(&ulp_entry - RTC_SLOW_MEM));
}

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                esp_wifi_stop();
                break;
            case WIFI_EVENT_STA_STOP:
                esp_wifi_deinit();
                start_ulp_program();
                esp_deep_sleep_start();
                break;
            default:
                ESP_LOGE(TAG, "Unhandled WiFi event %ld", event_id);
                break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
                deliver_data(event);
                esp_wifi_disconnect();
                break;
            case IP_EVENT_STA_LOST_IP:
                esp_wifi_connect();
                break;
            default:
                ESP_LOGE(TAG, "Unhandled IP event %ld", event_id);
                break;
        }
    }
}