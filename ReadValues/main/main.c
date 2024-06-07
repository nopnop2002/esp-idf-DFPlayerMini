/* The example of DFPlayerMini for ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "DFRobotDFPlayerMini.h"

static const char *TAG = "MAIN";

void app_main()
{
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

	int value = 0;
	char text[64];
	for (int request=0;request<7;request++) {
		if (request == 0) {
			strcpy(text, "Query current status");
			value = DF_readState();
		} else if (request == 1) {
			strcpy(text, "Query current volume");
			value = DF_readVolume();
		} else if (request == 2) {
			strcpy(text, "Query current EQ");
			value = DF_readEQ();
		} else if (request == 3) {
			strcpy(text, "Query number of tracks in the root of micro SD card");
			value = DF_readFileCounts(DFPLAYER_DEVICE_SD);
		} else if (request == 4) {
			strcpy(text, "Query current track in the micro SD Card");
			value = DF_readCurrentFileNumber(DFPLAYER_DEVICE_SD);
		} else if (request == 5) {
			strcpy(text, "Query number of tracks in a folder /01");
			value = DF_readFileCountsInFolder(1); // Query number of tracks in a folder
		} else if (request == 6) {
			strcpy(text, "Query number of tracks in a folder /02");
			value = DF_readFileCountsInFolder(2); // Query number of tracks in a folder
		}
		uint8_t type = DF_readType();
		ESP_LOGD(TAG, "type=%d", type);
		if (type != DFPlayerFeedBack) { //Error while Reading.
			ESP_LOGE(TAG, "Error while Reading. [%s]", text);
		} else { //Successfully get the result.
			ESP_LOGI(TAG, "%s is 0x%x", text, value);
		}
	}
	ESP_LOGI(TAG, "All Done.");
}

