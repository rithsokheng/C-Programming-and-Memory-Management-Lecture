/**
 * Chapter 4: Enums - Code Reference
 * 
 * This file contains reference implementations of C enumerations:
 * - Basic enum declarations and implicit values (0, 1, 2...)
 * - Explicit integer assignments
 * - switch statement dispatch with enums
 * - Enum underlying representation and sizeof(enum)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter04 && ./chapter04
 */

#include <stdio.h>

/* --- 1. Enum Definitions --- */

/* Implicit enumeration: starts at 0 and increments */
typedef enum {
    LOG_DEBUG,   /* 0 */
    LOG_INFO,    /* 1 */
    LOG_WARN,    /* 2 */
    LOG_ERROR    /* 3 */
} LogLevel_t;

/* Explicit enumeration: custom values */
typedef enum {
    HTTP_OK                    = 200,
    HTTP_CREATED               = 201,
    HTTP_BAD_REQUEST           = 400,
    HTTP_UNAUTHORIZED          = 401,
    HTTP_NOT_FOUND             = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500
} HttpStatus_t;

/* SnakeLang object type tag (used in Chapter 9 & 10) */
typedef enum {
    SNAKE_INT,
    SNAKE_FLOAT,
    SNAKE_STRING,
    SNAKE_VECTOR3,
    SNAKE_ARRAY
} SnakeType_t;

/* --- 2. Helper Functions --- */

/* String representation via switch-case */
const char *log_level_string(LogLevel_t level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default:        return "UNKNOWN";
    }
}

const char *http_status_message(HttpStatus_t status) {
    switch (status) {
        case HTTP_OK:                    return "200 OK";
        case HTTP_CREATED:               return "201 Created";
        case HTTP_BAD_REQUEST:           return "400 Bad Request";
        case HTTP_UNAUTHORIZED:          return "401 Unauthorized";
        case HTTP_NOT_FOUND:             return "404 Not Found";
        case HTTP_INTERNAL_SERVER_ERROR: return "500 Internal Server Error";
        default:                         return "Unknown Status Code";
    }
}

/* --- 3. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 4: Enums Code Reference ===\n\n");

    /* 4.1 Implicit Enum Values */
    printf("1. Implicit Enum Values:\n");
    printf("   LOG_DEBUG: %d\n", LOG_DEBUG);
    printf("   LOG_INFO:  %d\n", LOG_INFO);
    printf("   LOG_WARN:  %d\n", LOG_WARN);
    printf("   LOG_ERROR: %d\n\n", LOG_ERROR);

    /* 4.2 Explicit Enum Values */
    printf("2. Explicit Enum Values:\n");
    printf("   HTTP_OK:                    %d\n", HTTP_OK);
    printf("   HTTP_CREATED:               %d\n", HTTP_CREATED);
    printf("   HTTP_BAD_REQUEST:           %d\n", HTTP_BAD_REQUEST);
    printf("   HTTP_NOT_FOUND:             %d\n", HTTP_NOT_FOUND);
    printf("   HTTP_INTERNAL_SERVER_ERROR: %d\n\n", HTTP_INTERNAL_SERVER_ERROR);

    /* 4.3 switch with Enums */
    printf("3. Dispatching via switch:\n");
    LogLevel_t current_level = LOG_WARN;
    printf("   Current log level: %s\n", log_level_string(current_level));

    HttpStatus_t response_code = HTTP_NOT_FOUND;
    printf("   Server response:   %s\n\n", http_status_message(response_code));

    /* 4.4 Enum Size */
    printf("4. Enum Size:\n");
    printf("   sizeof(LogLevel_t):   %zu bytes (typically sizeof(int))\n", sizeof(LogLevel_t));
    printf("   sizeof(HttpStatus_t): %zu bytes\n", sizeof(HttpStatus_t));
    printf("   sizeof(SnakeType_t):  %zu bytes\n\n", sizeof(SnakeType_t));

    printf("=== Chapter 4 reference executed successfully! ===\n");
    return 0;
}
