/* The example of DFPlayerMini for ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "DFRobotDFPlayerMini.h"

static const char *TAG = "MAIN";

QueueHandle_t xQueueKey;

void keyin(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");

	uint16_t key;
	while (1) {
		key = fgetc(stdin);
		if (key == 0xffff) {
			vTaskDelay(10);
			continue;
		}
		ESP_LOGI(pcTaskGetName(0), "key=0x%x", key);
		if (xQueueSend(xQueueKey, &key, 10) != pdPASS) {
			ESP_LOGE(pcTaskGetName(0), "xQueueSend fail");
		}
	}

	/* Never reach */
	vTaskDelete( NULL );
}

void play(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");

	bool debug = false;
#if CONFIG_DEBUG_MODE
	debug = true;
#endif
	bool ret = DF_begin(CONFIG_TX_GPIO, CONFIG_RX_GPIO, true, true, debug);
	ESP_LOGI(TAG, "DF_begin=%d", ret);
	if (!ret) {
		ESP_LOGE(TAG, "DFPlayer Mini not online.");
		while(1) {
			vTaskDelay(1);
		}
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
	xTaskCreate(keyin, "KEYIN", 1024*4, NULL, 2, NULL);
	xTaskCreate(play, "PLAY", 1024*4, NULL, 2, NULL);
}

