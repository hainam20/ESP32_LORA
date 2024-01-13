

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "string.h"
#include "cJson.h"

#include "lora.h"
#include "wifi.h"
#include "DHT22.h"
#include "esp_mqtt.h"

uint8_t buf[256];

void task_tx(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	uint8_t buf[256];
	int i = 0;
	while (1)
	{
		if (i < 3)
		{
			int send_len = sprintf((char *)buf, "GET_ID%d", i);
			lora_send_packet(buf, send_len);
			vTaskDelay(pdMS_TO_TICKS(5000));
			i++;
		}
		else
		{
			i = 0;
		}

	} // end while
}
void task_rx(void *pvParameters)
{
	ESP_LOGI(pcTaskGetName(NULL), "Start");
	int count = 0;
	while (1)
	{
		lora_receive(); // put into receive mode
		if (lora_received())
		{
			int receive_len = lora_receive_packet(buf, sizeof(buf));
			buf[receive_len] = 0;
			count++;
			printf("Received Packed: %d\n", count);
			Publisher_Task((char *)buf, sizeof(buf));
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", receive_len, receive_len, buf);
		}
		vTaskDelay(1); // Avoid WatchDog alerts
	}
}
void app_main()
{

	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	int cr = 5;
	int sf = 7;
	if (lora_init() == 0)
	{
		ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
		while (1)
		{
			vTaskDelay(1);
		}
	}
	lora_set_frequency(433e6);
	lora_enable_crc();
	lora_set_coding_rate(cr);
	lora_set_bandwidth(125E3);
	lora_set_spreading_factor(sf);
	// lora_set_sync_word(0xF3);
	//
	wifi_init_sta();
	mqtt_start();
	vTaskDelay(pdMS_TO_TICKS(5000));
	xTaskCreate(&task_rx, "task_rx", 1024 * 3, NULL, 5, NULL);
	xTaskCreate(&task_tx, "task_tx", 1024 * 3, NULL, 5, NULL);
}