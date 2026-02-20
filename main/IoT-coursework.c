#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_ap.h"
#include "http_srv.h"
#include "hardware.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MAIN";

void init_nvs_flash(void)
{
	esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
	ESP_LOGI(TAG, "nvs_flash initialized.");

}
void app_main(void)
{
   init_nvs_flash();
   wifi_init_softap();
   start_http_srv();
   init_hw();
}
