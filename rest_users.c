#include "rest_users.h"
static const char* TAG = "REST_USER";
//Insert at the first position
rest_user_t * rest_user_add(rest_user_t **head, const char *username, const char *password, rest_permissions_t permission){
    if(strlen(username) < REST_USER_LOGIN_LENGTH && strlen(username) < REST_USER_LOGIN_LENGTH){
        rest_user_t *elem = (rest_user_t*) malloc(sizeof(rest_user_t));
        if(elem == NULL)
            return elem;
        //Assign data
        char *authbasicplain;
        authbasicplain = (char*)malloc(REST_USER_BASE64_LENGTH);
        if(authbasicplain == NULL)
            return NULL;
        strcpy(authbasicplain, "");
        strcat(authbasicplain, username);
        strcat(authbasicplain, ":");
        strcat(authbasicplain, password);
        // This is now user:password
        // Encode this to base64:
        size_t bufflen = REST_USER_BASE64_LENGTH;
        mbedtls_base64_encode((unsigned char*)elem->auth, sizeof(elem->auth), &bufflen, (unsigned char*)authbasicplain, strlen(authbasicplain));
        free(authbasicplain);

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
void rest_user_delete(rest_user_t **head){
    while(*head){
        rest_user_t *tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    *head = NULL;
}
//Dump it
void rest_user_print(rest_user_t *head){
    rest_user_t *elem = head;
    while(elem){
        ESP_LOGI(TAG, "%s", elem->auth);
        elem = elem->next;
    }
}