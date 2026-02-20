#include "led_strip.h"

led_strip_handle_t led_strip;

void init_led() {
    led_strip_config_t strip_config = {
        .strip_gpio_num = 27, 
        .max_leds = 1, 
        .led_model = LED_MODEL_WS2812,
    };
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
	led_strip_set_pixel(led_strip, 0, 128, 128, 128);
	led_strip_refresh(led_strip);
}

void change_led_color(int r, int g, int b) {
	led_strip_set_pixel(led_strip, 0, g, r, b);
	led_strip_refresh(led_strip);
}

