#include "rest_default_handlers.h"
//Users
extern rest_user_t *rest_api_users;

// JSON Helper
static const char* API_RESET_JSON_TEMPLATE = "{\"status\":%d, \"reason\":\"%s\", \"uptime\":%jd}";
esp_err_t rest_default_reset_handler(httpd_req_t *req){
    /*
        Receives HTTP header data + does some Error handling
    */
    if(!rest_api_recv(req))
        return ESP_FAIL;
    /*
        A single function that handles authentication and error replies towards the client 
    */
    if(!rest_api_authenticate(req, rest_api_users, REST_USER_PERMISSION_RO))
        return ESP_FAIL;
    /*
        Actual API resource function
    */
    //Get uprtime in seconds
    time_t uptime;
    time(&uptime);
    // Malloc json
    size_t resplen = strlen(API_RESET_JSON_TEMPLATE) + 36;
    char *resp = (char*)malloc(resplen);
    if(resp){
        // Important: Clear
        memset(resp, 0, resplen);
        uint8_t status = 0;
        switch (esp_reset_reason()){
            case ESP_RST_UNKNOWN:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "UNKNOWN", (intmax_t)uptime);  break;
            case ESP_RST_POWERON:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Power On", (intmax_t)uptime);  break;
            case ESP_RST_EXT:       sprintf(resp, API_RESET_JSON_TEMPLATE, status, "EXT", (intmax_t)uptime);  break;
            case ESP_RST_SW:        sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Software", (intmax_t)uptime);  break;
            case ESP_RST_PANIC:     sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Panic!", (intmax_t)uptime);  break;
            case ESP_RST_INT_WDT:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Int Watchdog", (intmax_t)uptime);  break;
            case ESP_RST_TASK_WDT:  sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Task Watchdog", (intmax_t)uptime);  break;
            case ESP_RST_WDT:       sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Watchdog", (intmax_t)uptime);  break;
            case ESP_RST_DEEPSLEEP: sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Deepsleep", (intmax_t)uptime);  break;
            case ESP_RST_BROWNOUT:  sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Brownout", (intmax_t)uptime);  break;
            case ESP_RST_SDIO:      sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Std IO", (intmax_t)uptime);  break;
            default:                return ESP_FAIL;                  break;
        }

        httpd_resp_set_type(req, "text/json");
        httpd_resp_send(req, resp, strlen(resp));
        return ESP_OK;
    }else{
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    return ESP_OK;
}