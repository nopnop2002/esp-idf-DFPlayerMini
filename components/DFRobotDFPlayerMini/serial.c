/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include "serial.h"

#define RX_BUF_SIZE 1024
#define TX_BUF_SIZE 1024

typedef struct {
	TaskHandle_t parent_task_handle;
	TaskHandle_t tx_task_handle;
	uint8_t tx_buf[TX_BUF_SIZE];
	int tx_len;
	TaskHandle_t rx_task_handle;
	uint8_t rx_buf[RX_BUF_SIZE];
	int rx_len;
	int rx_save_idx;
	int rx_read_idx;
} uart_obj_t;

static uart_obj_t uart_obj;

static void tx_task(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	uart_obj.tx_len = 0;
	while (1) {
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		ESP_LOGD(pcTaskGetName(0), "uart_obj.tx_len=%d", uart_obj.tx_len);
		int txBytes = uart_write_bytes(UART_NUM_1, uart_obj.tx_buf, uart_obj.tx_len);
		ESP_LOGD(pcTaskGetName(0), "txBytes=%d", txBytes);
		assert(uart_obj.tx_len == txBytes);
		uart_obj.tx_len = 0;
		xTaskNotifyGive(uart_obj.parent_task_handle);
	}
}

static void rx_task(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(0), "Start");
	uart_obj.rx_len = 0;
	uart_obj.rx_save_idx = 0;
	uart_obj.rx_read_idx = 0;
	uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
	while (1) {
		//const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
		const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1);
		if (rxBytes > 0) {
			data[rxBytes] = 0;
#if CONFIG_DEBUG_MODE
			ESP_LOGI(pcTaskGetName(0), "Read %d bytes", rxBytes);
#endif
			//ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(0), data, rxBytes, ESP_LOG_INFO);
			for (int index=0; index<rxBytes; index++) {
#if CONFIG_DEBUG_MODE
				ESP_LOGI(pcTaskGetName(0), "data[%d]=0x%x", index, data[index]);
#endif
				uart_obj.rx_buf[uart_obj.rx_save_idx] = data[index];
				uart_obj.rx_len++;
				uart_obj.rx_save_idx++;
				if (uart_obj.rx_save_idx == RX_BUF_SIZE) uart_obj.rx_save_idx = 0;
			}
			ESP_LOGD(pcTaskGetName(0), "uart_obj.rx_len=%d", uart_obj.rx_len);
		}
	}
	free(data);
}

void serial_begin(int baudrate, int txd, int rxd) {
	const uart_config_t uart_config = {
		//.baud_rate = 115200,
		.baud_rate = baudrate,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
		.source_clk = UART_SCLK_DEFAULT,
#else
		.source_clk = UART_SCLK_APB,
#endif
	};
	// We won't use a buffer for sending data.
	uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, txd, rxd, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	// Start uart task
	UBaseType_t prio = uxTaskPriorityGet( NULL );
	ESP_LOGD(__FUNCTION__, "prio=%d configMAX_PRIORITIES=%d", prio, configMAX_PRIORITIES);
	if (prio != configMAX_PRIORITIES) prio++; 
	uart_obj.parent_task_handle = xTaskGetCurrentTaskHandle();
	xTaskCreate(rx_task, "uart_rx_task", 1024*3, NULL, prio, &(uart_obj.rx_task_handle));
	xTaskCreate(tx_task, "uart_tx_task", 1024*3, NULL, prio, &(uart_obj.tx_task_handle));

	// Wait for task start
	vTaskDelay(10);
}

void serial_end(void) {

}

int serial_available(void) {
	ESP_LOGD(__FUNCTION__, "uart_obj.rx_len=%d", uart_obj.rx_len);
	return (uart_obj.rx_len);
}

uint8_t serial_read(void) {
	uint8_t data = uart_obj.rx_buf[uart_obj.rx_read_idx];
	uart_obj.rx_read_idx++;
	if (uart_obj.rx_read_idx == RX_BUF_SIZE) uart_obj.rx_read_idx = 0;
	uart_obj.rx_len--;
	return data;
}

uint8_t serial_peek(void) {
	uint8_t data = uart_obj.rx_buf[uart_obj.rx_read_idx];
	//uart_obj.rx_read_idx++;
	//if (uart_obj.rx_read_idx == RX_BUF_SIZE) uart_obj.rx_read_idx = 0;
	//uart_obj.rx_len--;
	return data;
}

void serial_print(char *ch) {
	int len = strlen(ch);
	serial_write_buffer((uint8_t *)ch, len);
}

void serial_println(char *ch) {
	char _ch[128];
	int len = strlen(ch);
	strcpy(_ch, ch);
	_ch[len] = 0x0d;
	_ch[len+1] = 0x0a;
	serial_write_buffer((uint8_t *)_ch, len+2);
}

void serial_write(uint8_t ch) {
	uart_obj.tx_buf[0] = ch;
	uart_obj.tx_len=1;
	xTaskNotifyGive(uart_obj.tx_task_handle);
	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
}

void serial_write_buffer(uint8_t *ch, int len) {
	for (int i=0;i<len;i++) {
		uart_obj.tx_buf[i] = ch[i];
	}
	uart_obj.tx_len=len;
	xTaskNotifyGive(uart_obj.tx_task_handle);
	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
}

void serial_flash() {

}

