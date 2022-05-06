#include "rest_users.h"
static const char* TAG = "REST_USER";
//Insert at the first position
rest_user_t * insertrest_user_t(rest_user_t **head, const char *username, const char *password, rest_permissions_t permission){
    if(strlen(username) < REST_USER_LOGIN_LENGTH && strlen(username) < REST_USER_LOGIN_LENGTH){
        rest_user_t *elem = (rest_user_t*) malloc(sizeof(rest_user_t));
        if(elem == NULL)
            return elem;
        //Assign data
        memset(elem->user, 0, REST_USER_LOGIN_LENGTH);
        strcpy(elem->user, username);
        memset(elem->passwd, 0, REST_USER_LOGIN_LENGTH);
        strcpy(elem->passwd, password);
        elem->permission = permission;
        //Next node is head
        elem->next = *head;
        // this is the new First node
        *head = elem;
        //Just to get a reference...
        return elem;
    }else{
        ESP_LOGE(TAG, "Username or Password too long. User not added!");
        return NULL;
    }
}
//Purge the entire list
void deleteAllrest_user_t(rest_user_t **head){
    while(*head){
        rest_user_t *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    *head = NULL;
}
//Dump it
void printrest_user_t(rest_user_t *head){
    rest_user_t *elem = head;
    while(elem){
        ESP_LOGI(TAG, "%s", elem->user);
        elem = elem->next;
    }
}