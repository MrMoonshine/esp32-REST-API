#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "rest_users.h"

static const char* TAG = "REST";
#define REST_401_JSON "{\"error\":401, \"description\":\"Unauthorized\"}"
#define REST_403_JSON "{\"error\":403, \"description\":\"Forbidden\"}"
/*
    DEBUG:
    GUI:
    Insomnia
    Authenticated request from CLI:
    me@pc$ curl --user user:passwd http://x.x.x.x/api/
*/
/*
    @brief API Call List
    @param uri URI. See espressif doc
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
/*
    @brief initial recv and error handling
*/
bool rest_api_recv(httpd_req_t *req);
/* 
    @brief Function for Checking Authentication
    @param req Request structure. see esp doc
    @param users Linked list of users
    @param min_permission Minimum permission level to access the resource
    @return true on sucess false on failure
*/
bool rest_api_authenticate(httpd_req_t *req, rest_user_t *users, rest_permissions_t min_permission);