/* HTTP Server Example

	 This example code is in the Public Domain (or CC0 licensed, at your option.)

	 Unless required by applicable law or agreed to in writing, this
	 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	 CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
//#include <sys/stat.h>
//#include <mbedtls/base64.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_err.h"
//#include "esp_vfs.h"
//#include "esp_spiffs.h"
//#include "esp_chip_info.h"
#include "esp_http_server.h"
#include "cJSON.h"

static const char *TAG = "HTTP";

extern QueueHandle_t xQueueKey;

/* Handler for root get */
static esp_err_t root_get_handler(httpd_req_t *req)
{
	ESP_LOGI(TAG, "root_get_handler req->uri=[%s]", req->uri);
	if (strcmp(req->uri, "/favicon.ico") == 0) return ESP_OK;

	if (strlen(req->uri) > 2) {
		uint16_t key = req->uri[1];
		ESP_LOGI(TAG, "key=0x%x", key);
		//if (xQueueSend(xQueueKey, &key, portMAX_DELAY) != pdPASS) {
		if (xQueueSendFromISR(xQueueKey, &key, NULL) != pdPASS) {
			ESP_LOGE(TAG, "xQueueSend Fail");
		}
	}

	/* Send response with custom headers and body set as the
	 * string passed in user context*/
	const char* resp_str = (const char*) req->user_ctx;
	httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

	return ESP_OK;
}

/* favicon get handler */
static esp_err_t favicon_get_handler(httpd_req_t *req)
{
	ESP_LOGI(TAG, "favicon_get_handler req->uri=[%s]", req->uri);
	return ESP_OK;
}

/* Function to start the file server */
esp_err_t start_server(int port)
{
	httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	
	// Purge "Least Recently Used”connection
	config.lru_purge_enable = true;
	// TCP Port number for receiving and transmitting HTTP traffic
	config.server_port = port;

	/* Use the URI wildcard matching function in order to
	 * allow the same handler to respond to multiple different
	 * target URIs which match the wildcard scheme */
	config.uri_match_fn = httpd_uri_match_wildcard;

	// Start the httpd server
	if (httpd_start(&server, &config) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to start file server!");
		return ESP_FAIL;
	}

	/* URI handler for root */
	httpd_uri_t root = {
		.uri		 = "/*", // Match all URIs of type /*
		.method		 = HTTP_GET,
		.handler	 = root_get_handler,
		//.user_ctx    = "{\'result\':\'OK\'}"
		.user_ctx	 = "OK\n"
	};
	httpd_register_uri_handler(server, &root);

	/* URI handler for favicon.ico */
	httpd_uri_t _favicon_get_handler = {
		.uri		 = "/favicon.ico",
		.method		 = HTTP_GET,
		.handler	 = favicon_get_handler,
		//.user_ctx  = server_data	// Pass server data as context
	};
	httpd_register_uri_handler(server, &_favicon_get_handler);

	return ESP_OK;
}

#define WEB_PORT 8080

void http_server(void *pvParameters)
{
	char *task_parameter = (char *)pvParameters;
	ESP_LOGI(TAG, "Start task_parameter=%s", task_parameter);
	char url[64];
	sprintf(url, "http://%s:%d", task_parameter, WEB_PORT);
	ESP_LOGI(TAG, "Starting HTTP server on %s", url);
	ESP_ERROR_CHECK(start_server(WEB_PORT));

	while(1) {
		vTaskDelay(1);
	}

	// Never reach here
	ESP_LOGI(TAG, "finish");
	vTaskDelete(NULL);
}
