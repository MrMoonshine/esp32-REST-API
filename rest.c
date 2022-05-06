#include "rest.h"

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