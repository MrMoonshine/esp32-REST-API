# esp32-REST-API
REST api library for esp32
# Example
This example shows how to use an API resource. Each API resource is a `http_uri_t` structure. Each resource is defined by a handler. Check *rest_default_handlers.c* to get an idea how this is supposed to work.
## Code to use a resource
```c
extern rest_user_t *rest_api_users;
// Example
httpd_uri_t uri_reset = {
    .uri      = "/api/reset",
    .method   = HTTP_GET,
    .handler  = rest_default_reset_handler,
    .user_ctx = NULL
};

void app_main(void){
    // some setup code ...
    rest_api_mdns("esp32-oida")
    // Create some users
    rest_user_add(&rest_api_users, "admin", "password", REST_USER_PERMISSION_RW);
    rest_user_add(&rest_api_users, "guest", "password", REST_USER_PERMISSION_RO);
    rest_user_print(rest_api_users);
    // Create http server
    rest_api_t *api = NULL;
    rest_api_add(&api, &uri_reset);
    /*
        Do more:
        rest_api_add(&api, &...);
    */
    rest_api_print(api);
    // Start server
    rest_api_start_server(api);
    // Delete API object. Keep user object!!!
    rest_api_delete(&api);
    while(1){
        //Used to avoid watchdog errors
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}
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