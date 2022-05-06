#pragma once
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_server.h"
static const char* TAG = "REST";
/*
    DEBUG:
    GUI:
    Insomnia
    Authenticated request from CLI:
    me@pc$ curl --user user:passwd http://x.x.x.x/api/
*/
/*
    @brief API Call List
    @param uri URI  example: "/api/var1"
    @param next Next element in list
*/
typedef struct rest_api_t{
    httpd_uri_t *uri;
    struct rest_api_t* next;
} rest_api_t;

//Insert at the first position
rest_api_t * rest_api_add(rest_api_t **head, httpd_uri_t *uri);
//Purge the entire list
void rest_api_delete(rest_api_t **head);
//Dump it
void rest_api_print(rest_api_t *head);
/* Function for starting the webserver */
httpd_handle_t rest_api_start_server(rest_api_t *head);