/*
	Example using WEB Socket.
	This example code is in the Public Domain (or CC0 licensed, at your option.)
	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "cJSON.h"

#include "DFRobotDFPlayerMini.h"

static const char *TAG = "client_task";

#include "websocket_server.h"

extern MessageBufferHandle_t xMessageBufferToClient;

void client_task(void* pvParameters) {
	ESP_LOGI(TAG, "Start");
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
	int currentVolume = 15;
	DF_volume(currentVolume); //Set volume value. From 0 to 30

	char cRxBuffer[512];
	char DEL = 0x04;
	char outBuffer[64];

	int currentFileNumber = 0;
	int totalFileCounts = 0;
	bool currentLoop = false;
	while (1) {
		size_t readBytes = xMessageBufferReceive(xMessageBufferToClient, cRxBuffer, sizeof(cRxBuffer), 10 );
		ESP_LOGD(TAG, "readBytes=%d", readBytes);
		if (readBytes == 0) {
			if (DF_waitAvailable(1)) {
				uint8_t type = DF_readType();
				int value = DF_read();
				//Print the detail message from DFPlayer to handle different errors and states.
				DF_printDetail(type, value);
				if (type == DFPlayerPlayFinished) {
					if (currentLoop == false) {
						sprintf(outBuffer,"FINISH%c%d", DEL, value);
						//ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
						ws_server_send_text_all(outBuffer,strlen(outBuffer));
					} else {
						currentFileNumber++;
						if (currentFileNumber > totalFileCounts) currentFileNumber=1;
						ESP_LOGI(TAG, "currentFileNumber=%d", currentFileNumber);
						sprintf(outBuffer,"CURRENT%c%d", DEL, currentFileNumber);
						ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
						ws_server_send_text_all(outBuffer,strlen(outBuffer));
					}
				}
			}
			continue;
		}

		ESP_LOGI(TAG, "cRxBuffer=[%.*s]", readBytes, cRxBuffer);
		cJSON *root = cJSON_Parse(cRxBuffer);
		if (cJSON_GetObjectItem(root, "id")) {
			char *id = cJSON_GetObjectItem(root,"id")->valuestring;
			ESP_LOGI(TAG, "id=[%s]",id);

			if ( strcmp (id, "init") == 0) {
				totalFileCounts = DF_readFileCounts(DFPLAYER_DEVICE_SD);
				ESP_LOGI(TAG, "totalFileCounts=%d", totalFileCounts);
				currentFileNumber = DF_readCurrentFileNumber(DFPLAYER_DEVICE_SD);
				ESP_LOGI(TAG, "currentFileNumber=%d", currentFileNumber);
				sprintf(outBuffer,"LIST%c%d", DEL, totalFileCounts);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));

				sprintf(outBuffer,"CURRENT%c%d", DEL, currentFileNumber);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));

				// test code
				//sprintf(outBuffer,"CURRENT%c%d", DEL, 2);
				//ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				//ws_server_send_text_all(outBuffer,strlen(outBuffer));
			} // end init

			if ( strcmp (id, "play") == 0) {
				//int state = DF_readState();
				//uint8_t type = DF_readType();
				ESP_LOGI(TAG, "currentFileNumber=%d", currentFileNumber);
				DF_play(currentFileNumber); // Play the current file
			} // end play

			if ( strcmp (id, "pause") == 0) {
				DF_pause(); // Pause the current file
			} // end pause

			if ( strcmp (id, "volume") == 0) {
				char *value = cJSON_GetObjectItem(root,"value")->valuestring;
				ESP_LOGI(TAG, "value=[%s]",value);
				currentVolume = atoi(value);
				ESP_LOGI(TAG, "currentVolume=[%d]",currentVolume);
				DF_volume(currentVolume); //Set volume value. From 0 to 30
			} // end volume

			if ( strcmp (id, "mute") == 0) {
				char *value = cJSON_GetObjectItem(root,"value")->valuestring;
				ESP_LOGI(TAG, "value=[%s]",value);
				if (strcmp(value, "ON") == 0) {
					DF_volume(0); //Set volume value. From 0 to 30
				} else {
					DF_volume(currentVolume); //Set volume value. From 0 to 30
				}
			} // end mute

			if ( strcmp (id, "previous") == 0) {
				if (currentFileNumber > 1) currentFileNumber--;
				ESP_LOGI(TAG, "currentFileNumber=%d", currentFileNumber);
				sprintf(outBuffer,"CURRENT%c%d", DEL, currentFileNumber);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));
			} // end previous

			if ( strcmp (id, "next") == 0) {
				currentFileNumber++;
				if (currentFileNumber > totalFileCounts) currentFileNumber=1;
				ESP_LOGI(TAG, "currentFileNumber=%d", currentFileNumber);
				sprintf(outBuffer,"CURRENT%c%d", DEL, currentFileNumber);
				ESP_LOGD(TAG, "outBuffer=[%s]", outBuffer);
				ws_server_send_text_all(outBuffer,strlen(outBuffer));
			} // end next

			if ( strcmp (id, "loop") == 0) {
				char *value = cJSON_GetObjectItem(root,"value")->valuestring;
				ESP_LOGI(TAG, "value=[%s]",value);
				if (strcmp(value, "ON") == 0) {
					//DF_enableLoop(); // Loop start current file
					DF_enableLoopAll(); // Loop start all file
					currentLoop = true;
				} else {
					//DF_disableLoop(); // Loop stop current file
					DF_disableLoopAll(); // Loop stop all file
					currentLoop = false;
				}
			} // end loop

		} // end if

		// Delete a cJSON structure
		cJSON_Delete(root);

	} // end while

	// Never reach here
	vTaskDelete(NULL);
}
