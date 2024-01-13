

#include "http_sever_app.h"
#include "DHT22.h"
#include <stdint.h>

#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */
 
static const char *TAG = "wifi";
static httpd_handle_t http_sever_handle = NULL;
static httpd_req_t *REG;

extern const unsigned char login_html_start[]       asm("_binary_login_html_start");
extern const unsigned char login_html_end[]         asm("_binary_login_html_end");
extern const unsigned char login_css_start[]        asm("_binary_login_css_start");
extern const unsigned char login_css_end[]          asm("_binary_login_css_end");
extern const unsigned char login_js_start[]         asm("_binary_login_js_start");
extern const unsigned char login_js_end[]           asm("_binary_login_js_end");


extern const unsigned char dashboard_html_start[]      asm("_binary_dashboard_html_start");
extern const unsigned char dashboard_html_end[]        asm("_binary_dashboard_html_end");
extern const unsigned char dashboard_css_start[]       asm("_binary_dashboard_css_start");
extern const unsigned char dashboard_css_end[]         asm("_binary_dashboard_css_end");
extern const unsigned char dashboard_js_start[]        asm("_binary_dashboard_js_start");
extern const unsigned char dashboard_js_end[]          asm("_binary_dashboard_js_end");

extern const unsigned char favicon_ico_start[]         asm("_binary_favicon_ico_start");
extern const unsigned char favicon_ico_end[]           asm("_binary_favicon_ico_end");
extern const unsigned char log_svg_start[]             asm("_binary_log_svg_start");
extern const unsigned char log_svg_end[]               asm("_binary_log_svg_end");


/**
 * Set JSON callback  
*/
static http_get_callback_t http_get_json_callback = NULL;

/*
 *  LOGIN HTML GET handler 
 */
static esp_err_t login_html_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "login.html requested");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)login_html_start, login_html_end - login_html_start);
 
    return ESP_OK;
}
/*
 * DASHBOARD HTML GET handler 
*/
static esp_err_t dashboard_html_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "dashboard.html requested");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)dashboard_html_start, dashboard_html_end - dashboard_html_start);
    return ESP_OK;
}
/**
 * favio.ico GET handler
*/
static esp_err_t favicon_ico_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "favicon.ico requested");
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_end - favicon_ico_start);
 
    return ESP_OK;
}
/**
 * log.svg GET handler
*/

static esp_err_t log_svg_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "log.svg requested");
    httpd_resp_set_type(req, "image/svg+xml");
    httpd_resp_send(req, (const char *)log_svg_start, log_svg_end - log_svg_start);
 
    return ESP_OK;
}
/**
 * login.css GET handler
*/
static esp_err_t login_css_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "login.css requested");
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)login_css_start, login_css_end - login_css_start);
 
    return ESP_OK;
}
/**
 * dashboard.css GET handler
*/

static esp_err_t dashboard_css_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "dashboard.css requested");
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)dashboard_css_start, dashboard_css_end - dashboard_css_start);
 
    return ESP_OK;
}
/**
 * login.js get handler
*/
static esp_err_t login_js_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "login.js requested");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)login_js_start, login_js_end - login_js_start);
    return ESP_OK;
}
/**
 * dashbooard.js get handler
*/
static esp_err_t dashboard_js_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "dashboard.js requested");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)dashboard_js_start, dashboard_js_end - dashboard_js_start);
    return ESP_OK;
}
/**
 * dht_sensor_json Get handler
*/
void json_response(char* data, int len)
{
    
    httpd_resp_send(REG, data, len);
}
static esp_err_t dht_sensor_json_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "/dhtSensor.json requested");

	// char dhtSensorJSON[100];

		// sprintf(dhtSensorJSON, "{\"temp\":\"%.2f\",\"humidity\":\"%.2f\"}", getTemperature()/100, getHumidity()/100);

		httpd_resp_set_type(req, "application/json");
	// 	httpd_resp_send(req,dhtSensorJSON, strlen(dhtSensorJSON));
    REG = req;
    http_get_json_callback();
	return ESP_OK;

}

//  esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
// {
//     if (strcmp("/", req->uri) == 0) {
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
//         /* Return ESP_OK to keep underlying socket open */
//         return ESP_OK;
//     } else if (strcmp("/echo", req->uri) == 0) {
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
//         /* Return ESP_FAIL to close underlying socket */
//         return ESP_FAIL;
//     }
//     /* For any other URI send 404 and close socket */
//     httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
//     return ESP_FAIL;
// }



static httpd_handle_t http_sever_configure(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.core_id = 0;

	// Adjust the default priority to 1 less than the wifi application task
	config.task_priority = 4;

	// Bump up the stack size (default is 4096)
	config.stack_size = 8192;

	// Increase uri handlers
	config.max_uri_handlers = 20;

	// Increase the timeout limits
	config.recv_wait_timeout = 10;
	config.send_wait_timeout = 10;

	ESP_LOGI(TAG,
			"http_server_configure: Starting server on port: '%d' with task priority: '%d'",
			config.server_port,
			config.task_priority);
    // Start the httpd http_sever_handle
   
    if (httpd_start(&http_sever_handle, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");

        // register html handler
        httpd_uri_t login_html = {
             .uri       = "/",
             .method    = HTTP_GET,
             .handler   = login_html_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &login_html);

        httpd_uri_t dashboard_html = {
             .uri       = "/dashboard.html",
             .method    = HTTP_GET,
             .handler   = dashboard_html_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &dashboard_html);
        // register fivicon_ico handler
        httpd_uri_t favicon_ico = {
             .uri       = "/favicon.ico",
             .method    = HTTP_GET,
             .handler   = favicon_ico_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &favicon_ico);

        httpd_uri_t log_svg = {
             .uri       = "/img/log.svg",
             .method    = HTTP_GET,
             .handler   = log_svg_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &log_svg);
        // register login_css handler
        httpd_uri_t login_css = {
             .uri       = "/css/login.css",
             .method    = HTTP_GET,
             .handler   = login_css_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &login_css);
          httpd_uri_t dashboard_css = {
             .uri       = "/css/dashboard.css",
             .method    = HTTP_GET,
             .handler   = dashboard_css_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &dashboard_css);
        // register login.js handler
        httpd_uri_t login_js = {
             .uri       = "/js/login.js",
             .method    = HTTP_GET,
             .handler   = login_js_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &login_js);

        httpd_uri_t dashboard_js = {
             .uri       = "/js/dashboard.js",
             .method    = HTTP_GET,
             .handler   = dashboard_js_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &dashboard_js);

        // register dhtSensor.json handler
        httpd_uri_t dht_sensor_json = {
             .uri       = "/dhtsensor.json",
             .method    = HTTP_GET,
             .handler   = dht_sensor_json_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &dht_sensor_json);

        return http_sever_handle;
        
    }
    else {
        ESP_LOGI(TAG, "Error starting server!");
    }
    return NULL;
    
    
}
void start_webserver(void)
{
    if(http_sever_handle == NULL)
    {
        http_sever_handle = http_sever_configure();
    }
}
void stop_webserver(void)
{
    // Stop the httpd server
   httpd_stop(http_sever_handle);
}
void http_set_json_callback(void *cb)
{
    http_get_json_callback = cb;
}

