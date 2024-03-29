#include "rest.h"
static const char* TAG = "REST";

//Insert at the first position
rest_api_t * rest_api_add(rest_api_t **head, httpd_uri_t *uri){
    if(!uri)
        return NULL;
    // Malloc
    rest_api_t *elem = (rest_api_t*) malloc(sizeof(rest_api_t));
    if(elem == NULL)
        return elem;
    //Assign data
    elem->uri = uri;
    //Next node is head
    elem->next = *head;
    // this is the new First node
    *head = elem;
    //Just to get a reference...
    return elem;
}
//Purge the entire list
void rest_api_delete(rest_api_t **head){
    while(*head){
        rest_api_t *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    *head = NULL;
}
//Dump it
void rest_api_print(rest_api_t *head){
    rest_api_t *elem = head;
    while(elem){
        ESP_LOGI(TAG, "%s ", elem->uri->uri);
        elem = elem->next;
    }
}

httpd_handle_t rest_api_start_server(rest_api_t *head)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        rest_api_t *elem = head;
        while(elem){
            httpd_register_uri_handler(server, elem->uri);
            elem = elem->next;
        }
    }
    /* If server failed to start, handle will be NULL */
    return server;
}

bool rest_api_recv(httpd_req_t *req){
    // if req is 0. Give it plenty of memory
    size_t recv_size = req->content_len ? req->content_len : 128;
    char* content = (char*)malloc(recv_size);
    if(content == NULL){
        httpd_resp_send_500(req);
        return false;
    }
    // Start
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
        return false;
    }
    free(content);
    return true;
}

static const char* HTTP_HEADER_AUTH_FIELD = "Authorization";
bool rest_api_authenticate(httpd_req_t *req, rest_user_t *users, rest_permissions_t min_permission){
    // If no permission or user is set then it's fine: return true
    if(users == NULL || min_permission == REST_USER_PERMISSION_ANY)
        return true;
        
    size_t alen = httpd_req_get_hdr_value_len(req, HTTP_HEADER_AUTH_FIELD);
    char* b64 = (char*)malloc(++alen);
    if(!b64){
        httpd_resp_send_500(req);
        return false;
    }
    memset(b64, 0, alen);
    esp_err_t err = httpd_req_get_hdr_value_str(req, HTTP_HEADER_AUTH_FIELD, b64, alen);
    if(ESP_OK == err){
        rest_user_t *elem = users;
        while (elem){
            if(strstr(b64, elem->auth)){
                if(min_permission <= elem->permission){
                    free(b64);
                    return true;
                }else{
                    httpd_resp_set_type(req, "text/json");
                    httpd_resp_send_err(req, HTTPD_403_FORBIDDEN, REST_403_JSON);
                    free(b64);
                    return false;
                }
            }
            elem = elem->next;
        }
        // Invalid login
        httpd_resp_set_type(req, "text/json");
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, REST_401_JSON);
    }else if(err == ESP_ERR_NOT_FOUND){
        httpd_resp_set_type(req, "text/json");
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, REST_401_JSON);
    }else{
        httpd_resp_set_type(req, "text/json");
        httpd_resp_send_500(req);
    }
    free(b64);
    return false;
}