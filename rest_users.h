#pragma once
#include <string.h>
#include "esp_log.h"
#include "mbedtls/base64.h"
/*
    @brief This type can be asigned to an element, to set the required permissions
    @param REST_USER_PERMISSION_ANY Everyone
    @param REST_USER_PERMISSION_RO Must have read permission
    @param REST_USER_PERMISSION_RW Must have read and wirte permission
*/
typedef enum{
    REST_USER_PERMISSION_ANY=0,
    REST_USER_PERMISSION_RO=1,
    REST_USER_PERMISSION_RW=2,
} rest_permissions_t;

#define REST_USER_BASE64_LENGTH 65
#define REST_USER_LOGIN_LENGTH 32
/*
    @brief User
    @param auth Base64 encoded authentication data (max. 65 char)
    @param permission Permission level of a user
    @param next Next element in list
*/
typedef struct rest_user_t{
    char auth[REST_USER_BASE64_LENGTH];
    rest_permissions_t permission;
    struct rest_user_t *next;
} rest_user_t;
/*
    @brief Add ned user to list
    @param head List head
    @param username Username (max. 32 char)
    @param password Password (max. 32 char)
    @param permission Permission level of a user
*/
rest_user_t * rest_user_add(rest_user_t **head, const char *username, const char *password, rest_permissions_t permission);
/*
    @brief Delete user list
    @param head List head
*/
void rest_user_delete(rest_user_t **head);
/*
    @brief Dump list
    @param head List head
*/
void rest_user_print(rest_user_t *head);