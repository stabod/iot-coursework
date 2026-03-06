idf_component_register(SRCS "IoT-coursework.c" "wifi_ap.c" "http_srv.c" "led_control.c" "hardware.c"
					PRIV_REQUIRES esp_wifi nvs_flash esp_http_server esp_driver_gpio esp_driver_ledc esp_timer
                    INCLUDE_DIRS "."
					EMBED_FILES "index.html" "led.html" "music.html")
