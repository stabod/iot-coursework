idf_component_register(
	SRCS
		"src/IoT-coursework.c"
		"src/wifi_ap.c"
		"src/http_srv.c"
		"src/hardware.c"
	PRIV_REQUIRES
		esp_wifi nvs_flash esp_http_server esp_driver_gpio esp_driver_ledc esp_timer led_strip
	INCLUDE_DIRS
		"include"
	EMBED_FILES
		"web/index.html"
		"web/led.html"
		"web/music.html"
)
