/* The example of DFPlayerMini for ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "DFRobotDFPlayerMini.h"

#define TAG "MAIN"

QueueHandle_t xQueueKey;

void gpio(void *pvParameter)
{
	ESP_LOGI(pcTaskGetName(0), "Start CONFIG_GPIO_INPUT=%d", CONFIG_GPIO_INPUT);

	// set the GPIO as a input
	gpio_reset_pin(CONFIG_GPIO_INPUT);
	gpio_set_direction(CONFIG_GPIO_INPUT, GPIO_MODE_DEF_INPUT);

#if CONFIG_GPIO_PULLUP
	ESP_LOGI(pcTaskGetName(0), "GPIO%d is PULL UP", CONFIG_GPIO_INPUT);
	int push = 0;
	int release = 1;
#endif
#if CONFIG_GPIO_PULLDOWN
	ESP_LOGI(pcTaskGetName(0), "GPIO%d is PULL DOWN", CONFIG_GPIO_INPUT);
	int push = 1;
	int release = 0;
#endif
	ESP_LOGI(pcTaskGetName(0), "push=%d release=%d", push, release);

	uint16_t key = 0x6E;
	while(1) {
		int level = gpio_get_level(CONFIG_GPIO_INPUT);
		if (level == push) {
			ESP_LOGI(pcTaskGetName(0), "Push Button");
			while(1) {
				level = gpio_get_level(CONFIG_GPIO_INPUT);
				if (level == release) break;
				vTaskDelay(1);
			}
			ESP_LOGI(pcTaskGetName(0), "Release Button");
			if (xQueueSend(xQueueKey, &key, 10) != pdPASS) {
				ESP_LOGE(pcTaskGetName(0), "xQueueSend fail");
			}
		}
		vTaskDelay(1);
	}

	/* Never reach */
	vTaskDelete( NULL );
}

void play(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");

	while(1) {
		bool ret = DF_begin(CONFIG_TX_GPIO, CONFIG_RX_GPIO, true);
		ESP_LOGI(TAG, "DF_begin=%d", ret);
		if (ret) break;
		vTaskDelay(200);
	}
	ESP_LOGI(TAG, "DFPlayer Mini online.");
	DF_volume(30); //Set volume value. From 0 to 30

	uint16_t key;
	while(1) {
		// Wait key input
		ESP_LOGI(pcTaskGetName(0),"Waitting ....");
		xQueueReceive(xQueueKey, &key, portMAX_DELAY);
		ESP_LOGI(pcTaskGetName(0), "key=0x%x", key);

		// Show DFPlayer event
		while(1) {
			if (DF_waitAvailable(1000)) {
				uint8_t type = DF_readType();
				int value = DF_read();
				//Print the detail message from DFPlayer to handle different errors and states.
				DF_printDetail(type, value);
			} else {
				break;
			}
		}

		if (key == 0x6E) { // 'n'
			ESP_LOGI(pcTaskGetName(0), "Play next");
			DF_next(); //Play the next mp3
		} else if (key == 0x70) { // 'p'
			ESP_LOGI(pcTaskGetName(0), "Play first");
			DF_play(1); //Play the first mp3
		} else if (key == 0x73) { // 's'
			ESP_LOGI(pcTaskGetName(0), "Stop play");
			DF_stop(); //Stop
		} else if (key == 0x71) { // 'q'
			int value = DF_readState();
			uint8_t type = DF_readType();
			ESP_LOGI(pcTaskGetName(0), "type=%d value=%x", type, value);
			if (type != DFPlayerFeedBack) { //Error while Reading.
				ESP_LOGE(pcTaskGetName(0), "Error while Reading.");
			}
		} else {
			ESP_LOGW(pcTaskGetName(0), "Type p to play. Type s to stop. Type n to next. Type q to status.");
		}
	}

	/* Never reach */
	vTaskDelete( NULL );
}

void app_main()
{
	/* Create Queue */
	xQueueKey = xQueueCreate( 1, sizeof(uint16_t) );
	configASSERT( xQueueKey );

	/* Create task */
	xTaskCreate(gpio, "GPIO", 1024*4, NULL, 2, NULL);
	xTaskCreate(play, "PLAY", 1024*4, NULL, 2, NULL);
}

