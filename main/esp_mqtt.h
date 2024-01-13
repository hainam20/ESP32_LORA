#ifndef _ESP_MQTT_H_
#define _ESP_MQTT_H_

#define URI "mqtt://mqtt.flespi.io"
#define ClientId "ESP32"
#define Username "r5sB4Lo4EbSi6siLzlNkXx6waBsHRU56OnxIWLAoLjuFzCgYLR6zc0TdrbK6WF0A"
#define Password ""

void mqtt_start(void);
void mqtt_stop(void);
void Publisher_Task(char *data, int lenght);
#endif