#include "http_client_app.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"


static const char *TAG = "HTTP_CLIENT";

static esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
        break;
    default:
        break;

    }
    return ESP_OK;
}

void post_rest_function(char* data, int len)
{
    esp_http_client_config_t config_post = {
        .url = "http://192.168.1.3:3000/api/loradata/store",
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = client_event_post_handler};
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char  *post_data = data;
    esp_http_client_set_post_field(client, post_data, len);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_err_t err = esp_http_client_open(client, strlen(post_data));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        esp_http_client_write(client, post_data, strlen(post_data));
    }
    esp_http_client_perform(client);
     int status_code = esp_http_client_get_status_code(client);
      printf("Status code: %d\n", status_code);
    esp_http_client_cleanup(client);
}