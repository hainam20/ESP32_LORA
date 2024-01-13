#ifndef _HTTP_SEVER_APP_H
#define _HTTP_SEVER_APP_H
#include <stdint.h>

typedef void (*http_get_callback_t)(void);

void start_webserver(void);
void stop_webserver(void);
void http_set_json_callback(void *cb); 
void json_response(char* data, int len); 
#endif