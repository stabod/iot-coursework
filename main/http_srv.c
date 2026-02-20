#include <stdio.h>
#include "esp_log.h"
#include "esp_http_server.h"
#include "led_control.h"

static const char *TAG = "HTTP_SRV";

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

extern const uint8_t led_change_html_start[] asm("_binary_led-change_html_start");
extern const uint8_t led_change_html_end[] asm("_binary_led-change_html_end");

extern const uint8_t music_change_html_start[] asm("_binary_music-change_html_start");
extern const uint8_t music_change_html_end[] asm("_binary_music-change_html_end");

esp_err_t get_index(httpd_req_t *req) {
  const size_t index_html_len = index_html_end - index_html_start;
    
  httpd_resp_send(req, (const char *)index_html_start, index_html_len);
  return ESP_OK;
}

esp_err_t get_led(httpd_req_t *req) {
  const size_t led_html_len = led_change_html_end - led_change_html_start;

  httpd_resp_send(req, (const char*)led_change_html_start, led_chnage_html_len);
  return ESP_OK;
}

esp_err_t get_music(httpd_req_t *req) {
  const size_t music_html_len = music_change_html_end - music_change_html_start;

  httpd_resp_send(req, (const char*)music_change_html_start, music_chnage_html_len);
  return ESP_OK;
}

esp_err_t color_post(httpd_req_t *req) {
  char buf[256]; /* Will be much smaller than this */
  int ret, remaining = req->content_len;

  while (remaining > 0) {
	if ((ret = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)))) <= 0) {
	  return ESP_FAIL;
	}
	remaining -= ret;
  }
  buf[req->content_len] = '\0';

  int r, g, b;
  if (sscanf(buf, "r=%d&g=%d&b=%d", &r, &g, &b) == 3) {
	ESP_LOGI(TAG, "Setting color: R:%d, G:%d, B:%d.", r, g, b);
	change_led_color(r, g, b);
  }

  httpd_resp_send_chunk(req, NULL, 0);
  return ESP_OK;
}

httpd_uri_t uri_index = { .uri = "/", .method = HTTP_GET, .handler = get_index };
httpd_uri_t uri_set_color = { .uri = "/set_color", .method = HTTP_POST, .handler = color_post, .user_ctx = NULL };

void start_http_srv(void) {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server = NULL;

  if (httpd_start(&server, &config) == ESP_OK) {
	init_led();
	httpd_register_uri_handler(server, &uri_index);
	httpd_register_uri_handler(server, &uri_set_color);
	ESP_LOGI(TAG, "HTTP server started.");
  }
}
