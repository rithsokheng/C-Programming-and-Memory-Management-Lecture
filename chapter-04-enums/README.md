# Chapter 4 - Enums

> **Navigation:** [Previous: Chapter 3 - Pointers and Arrays](../chapter-03-pointers-and-arrays/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 5 - Unions](../chapter-05-unions/README.md)

---

## Table of Contents

- [4.1 Enumerations](#41-enumerations)
- [4.2 Explicit Enum Values](#42-explicit-enum-values)
- [4.3 `switch` and Enums](#43-switch-and-enums)
- [4.4 Enum Size](#44-enum-size)

---

## 4.1 Enumerations

An enum gives names to a set of integer constants.

Example:

```c
typedef enum {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} DayOfWeek;
```

By default:

```text
MONDAY    = 0
TUESDAY   = 1
WEDNESDAY = 2
...
```

---

## 4.2 Explicit Enum Values

Values can be assigned manually:

```c
typedef enum {
    RED = 55,
    GREEN = 176,
    BLUE = 38
} Color;
```

The explicit values do not need to be sequential.

Enums are especially useful when the actual numeric value has meaning, such as protocol or status codes.

---

## 4.3 `switch` and Enums

Example:

```c
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void print_log_level(LogLevel level) {
    switch (level) {
        case LOG_DEBUG:
            printf("debug enabled\n");
            break;

        case LOG_INFO:
            printf("info enabled\n");
            break;

        case LOG_WARNING:
            printf("warning enabled\n");
            break;

        case LOG_ERROR:
            printf("error enabled\n");
            break;

        default:
            printf("unknown log level\n");
            break;
    }
}
```

### Important: `break`

Without `break`, execution can continue into later cases.

Fallthrough can be intentional, but accidental fallthrough is a common source of bugs.

### HTTP status example

```c
typedef enum {
    HTTP_BAD_REQUEST,
    HTTP_UNAUTHORIZED,
    HTTP_NOT_FOUND,
    HTTP_IM_A_TEAPOT,
    HTTP_INTERNAL_SERVER_ERROR
} HttpRequest;

int http_status_code(HttpRequest request) {
    switch (request) {
        case HTTP_BAD_REQUEST:
            return 400;

        case HTTP_UNAUTHORIZED:
            return 401;

        case HTTP_NOT_FOUND:
            return 404;

        case HTTP_IM_A_TEAPOT:
            return 418;

        case HTTP_INTERNAL_SERVER_ERROR:
            return 500;

        default:
            return -1;
    }
}
```

---

## 4.4 Enum Size

Enums are generally represented using an integer type capable of holding their values.

For ordinary values, an enum often behaves like an integer.

For unusually large values, the representation may require a larger integer type.

Example:

```c
typedef enum {
    ERROR_SMALL = 1,
    ERROR_LARGE = 4294967296ULL
} ErrorCode;

printf("%zu\n", sizeof(ErrorCode));
```

The exact size is platform/compiler dependent.

---

> **Navigation:** [Previous: Chapter 3 - Pointers and Arrays](../chapter-03-pointers-and-arrays/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 5 - Unions](../chapter-05-unions/README.md)
