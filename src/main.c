#include <stdio.h>
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_ulp.h"
#include "soc/rtc.h"
#include "ulp_main.h"
#include "esp32/ulp_common_defs.h"
#include "ulp_fsm_common.h"

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");

void start_ulp_program() {
    ESP_ERROR_CHECK(ulp_load_binary(0, ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t)));
    ESP_ERROR_CHECK(ulp_run(&ulp_entry - RTC_SLOW_MEM));
}

void app_main() {
    esp_sleep_enable_ulp_wakeup();
    start_ulp_program();

    printf("Hello world!\n");
    fflush(stdout);

    esp_deep_sleep_start();
}