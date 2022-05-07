# esp32-REST-API
REST api library for esp32
# Example
This example shows how to create an API resource that can be used to query the last reset reason of the device.
## Code to crate the resource
```c
static const char* API_RESET_JSON_TEMPLATE = "{\"status\":%d, \"reason\":\"%s\"}";
esp_err_t rest_reset_handler(httpd_req_t *req){
    // Static buffer for request
    char content[100];
    /* Truncate if content length larger than the buffer */
    size_t recv_size = min(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    // On error
    if (ret != ESP_OK) {
        switch (ret){
            case HTTPD_SOCK_ERR_TIMEOUT:
                httpd_resp_send_408(req);
                break;
            default:
                ESP_LOGI(TAG, "Error code: %d", ret);
                break;
        }
        /* in case of error exit */
        return ESP_FAIL;
    }
    /*
        A single function that handles authentication and error replies towards the client 
    */
    if(!rest_api_authenticate(req, users, REST_USER_PERMISSION_RW))
        return ESP_FAIL;
    
    size_t resplen = strlen(API_RESET_JSON_TEMPLATE) + 24;
    char *resp = (char*)malloc(resplen);
    if(resp){
        // Important: Clear
        memset(resp, 0, resplen);
        uint8_t status = 0;
        switch (esp_reset_reason()){
            case ESP_RST_UNKNOWN:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "UNKNOWN");  break;
            case ESP_RST_POWERON:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Power On");  break;
            case ESP_RST_EXT:       sprintf(resp, API_RESET_JSON_TEMPLATE, status, "EXT");  break;
            case ESP_RST_SW:        sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Software");  break;
            case ESP_RST_PANIC:     sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Panic!");  break;
            case ESP_RST_INT_WDT:   sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Int Watchdog");  break;
            case ESP_RST_TASK_WDT:  sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Task Watchdog");  break;
            case ESP_RST_WDT:       sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Watchdog");  break;
            case ESP_RST_DEEPSLEEP: sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Deepsleep");  break;
            case ESP_RST_BROWNOUT:  sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Brownout");  break;
            case ESP_RST_SDIO:      sprintf(resp, API_RESET_JSON_TEMPLATE, status, "Std IO");  break;
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
/*
    The Reset Reason Resource
*/
httpd_uri_t uri_reset = {
    .uri      = "/api/reset",
    .method   = HTTP_GET,
    .handler  = rest_reset_handler,
    .user_ctx = NULL
};
```
## Setup routine
This will usually be added to your `app_main(void)` function:
```c
rest_api_t *api = NULL;
rest_api_add(&api, &uri_reset);
```
The API resources are managed as a linked list. Hence, his can be done as often you (or the device memory) like.

# HTTP Request
Example without variables:
```HTTP
GET /api/reset HTTP/1.1
Host: 10.0.0.10
User-Agent: insomnia/2022.2.1
Authorization: Basic YWRtaW46cGFzc3dvcmQ=
Accept: */*
```
Example with variables:
```HTTP
GET /api/reset HTTP/1.1
Host: 10.0.0.10
User-Agent: insomnia/2022.2.1
Authorization: Basic YWRtaW46cGFzc3dvcmQ=
myvar: myvalue
Accept: */*
```
Variables need to be handled in the uri handler function