/* The example of DFPlayerMini for ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "DFRobotDFPlayerMini.h"

#define TAG "MAIN"

void app_main()
{
	while(1) {
		bool ret = DF_begin(CONFIG_TX_GPIO, CONFIG_RX_GPIO, true);
		ESP_LOGI(TAG, "DF_begin=%d", ret);
		if (ret) break;
		vTaskDelay(200);
	}
	ESP_LOGI(TAG, "DFPlayer Mini online.");
	ESP_LOGI(TAG, "Play first track on 01 folder.");
	DF_volume(30); //Set volume value. From 0 to 30
	DF_play(1); //Play the first mp3

    /*
    Wait until play finish.
    For some reason, when the play finished, Play Finished event is notified twice.
    I don't know why.

    received:7E FF 6 3D 0 0 1 FE BD EF
    Number:1 Play Finished!
    received:7E FF 6 3D 0 0 1 FE BD EF
    Number:1 Play Finished!
    */

	while(1) {
		if (DF_available()) {
			uint8_t type = DF_readType();
			int value = DF_read();
			//Print the detail message from DFPlayer to handle different errors and states.
			DF_printDetail(type, value);
		}
		vTaskDelay(1);
	}
}

