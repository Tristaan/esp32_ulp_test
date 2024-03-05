#include <esp_event.h>
#include <freeRTOS/event_groups.h>
#include <esp_wifi.h>
#include <esp32/ulp_common_defs.h>
#include <ulp_fsm_common.h>

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);