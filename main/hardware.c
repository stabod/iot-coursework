#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "music.h"

#define PIEZO_PIN       GPIO_NUM_7
#define BUTTON_PIN      GPIO_NUM_8
#define ESP_INTR_FLAG_DEFAULT 0

static TaskHandle_t music_task_handle = NULL;
extern volatile uint8_t current_song_idx;

static const char *TAG = "HARDWARE";

static void IRAM_ATTR button_isr_handler(void* arg) {
  static int64_t last_interrupt_time = 0;
  int64_t now = esp_timer_get_time();

  if (now - last_interrupt_time > 200000) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(music_task_handle, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken) {
	  portYIELD_FROM_ISR();
	}
  }
  last_interrupt_time = now;
}

static void play_tone(uint32_t freq, uint32_t duration) {
  if (freq > 0) {
	ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
        
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 4096);
	ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
  } else {
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
	ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
  }
  vTaskDelay(pdMS_TO_TICKS(duration));

  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    
  vTaskDelay(pdMS_TO_TICKS(10));
}

static void play_song(int index) {
  if (index < 0 || index > music_size - 1) return;
  const note* current = music_lookup[index];
  while (current->ms != 0) {
	play_tone(current->hertz, current->ms);
	current++;
  }
}

void music_task(void *pvParameters) {
  while (1) {
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	ESP_LOGI(TAG, "Playing song ID: %d\n", current_song_idx);
	play_song(current_song_idx);
  }
}

void init_hw(void) {
  ledc_timer_config_t ledc_timer = {
	.speed_mode       = LEDC_LOW_SPEED_MODE,
	.timer_num        = LEDC_TIMER_0,
	.duty_resolution  = LEDC_TIMER_13_BIT,
	.freq_hz          = 4000,
	.clk_cfg          = LEDC_AUTO_CLK
  };
  ledc_timer_config(&ledc_timer);

  ledc_channel_config_t ledc_channel = {
	.speed_mode     = LEDC_LOW_SPEED_MODE,
	.channel        = LEDC_CHANNEL_0,
	.timer_sel      = LEDC_TIMER_0,
	.intr_type      = LEDC_INTR_DISABLE,
	.gpio_num       = PIEZO_PIN,
	.duty           = 0,
	.hpoint         = 0
  };
  ledc_channel_config(&ledc_channel);

  gpio_config_t io_conf = {
	.intr_type = GPIO_INTR_NEGEDGE, 
	.pin_bit_mask = (1ULL << BUTTON_PIN),
	.mode = GPIO_MODE_INPUT,
	.pull_up_en = 1,
	.pull_down_en = 0,
  };
  gpio_config(&io_conf);

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);
  xTaskCreate(music_task, "music_task", 2048, NULL, 5, &music_task_handle);
  ESP_LOGI(TAG, "Hardware initialized.");
}

