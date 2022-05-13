#pragma once
#include "esp_log.h"
#include "esp_http_server.h"
#include "rest.h"
/*
    This file is used, to provide some basic handlers to the API
*/
/*
    @brief Usage: Get Reset reason, Method: GET, Permission: RO
    @param req Http request
    @returns ESP_OK on sucess
*/
esp_err_t rest_default_reset_handler(httpd_req_t *req);