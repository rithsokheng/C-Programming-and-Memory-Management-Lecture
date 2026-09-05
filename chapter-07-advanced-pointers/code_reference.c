/**
 * Chapter 7: Advanced Pointers - Code Reference
 * 
 * This file contains reference implementations of advanced pointer concepts:
 * - Double pointers (pointer-to-pointer, T **)
 * - Allocating heap memory through double pointer parameters
 * - Arrays of pointers (e.g. array of string pointers)
 * - Generic pointers (void *) and type casting
 * - Generic swap implementation using memcpy
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter07 && ./chapter07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- 1. Allocating Through Double Pointer --- */

/* Function mutates caller's pointer variable to point to newly allocated buffer */
bool allocate_buffer(char **out_buffer, size_t size) {
    if (out_buffer == NULL) {
        return false;
    }

    *out_buffer = malloc(size);
    if (*out_buffer == NULL) {
        return false;
    }

    return true;
}

/* --- 2. Generic void * Printing --- */

void print_generic_int(const void *data) {
    int value = *(const int *)data;
    printf("   Generic int: %d\n", value);
}

void print_generic_float(const void *data) {
    float value = *(const float *)data;
    printf("   Generic float: %.2f\n", value);
}

/* --- 3. Generic Swap with memcpy --- */

/* Swaps any two variables of arbitrary size using a heap buffer */
bool generic_swap(void *a, void *b, size_t size) {
    if (a == NULL || b == NULL || size == 0) {
        return false;
    }

    void *temp = malloc(size);
    if (temp == NULL) {
        return false;
    }

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);

    free(temp);
    return true;
}

/* Struct for testing generic swap */
typedef struct {
    int id;
    char code[4];
} Item_t;

/* --- 4. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 7: Advanced Pointers Code Reference ===\n\n");

    /* 7.1 & 7.2 Pointer-to-Pointer & Allocation */
    printf("1. Pointer-to-Pointer & Double Pointer Allocation:\n");
    char *message = NULL;
    size_t msg_size = 32;

    printf("   Before allocation: message = %p\n", (void *)message);
    if (allocate_buffer(&message, msg_size)) {
        snprintf(message, msg_size, "Allocated via char **!");
        printf("   After allocation:  message = %p (\"%s\")\n\n", (void *)message, message);
        free(message);
        message = NULL;
    }

    /* 7.3 Arrays of Pointers */
    printf("2. Array of Pointers:\n");
    const char *languages[] = {
        "C",
        "Python",
        "SnakeLang",
        "Rust",
        "Go"
    };
    size_t lang_count = sizeof(languages) / sizeof(languages[0]);

    for (size_t i = 0; i < lang_count; i++) {
        /* languages[i] is a pointer (char *) */
        printf("   languages[%zu] points to %p -> \"%s\"\n",
               i, (const void *)languages[i], languages[i]);
    }
    printf("\n");

    /* 7.4 Generic void * Pointers */
    printf("3. Generic void * Dispatch:\n");
    int test_int = 1024;
    float test_float = 3.1415f;

    print_generic_int(&test_int);
    print_generic_float(&test_float);
    printf("\n");

    /* 7.5 & 7.6 Generic Swap with memcpy */
    printf("4. Generic Swap with memcpy:\n");
    int x = 100, y = 999;
    printf("   Before int swap:    x = %d, y = %d\n", x, y);
    generic_swap(&x, &y, sizeof(int));
    printf("   After int swap:     x = %d, y = %d\n\n", x, y);

    Item_t itemA = { .id = 1, .code = "AAA" };
    Item_t itemB = { .id = 2, .code = "BBB" };
    printf("   Before struct swap: itemA={%d, %s}, itemB={%d, %s}\n",
           itemA.id, itemA.code, itemB.id, itemB.code);
    generic_swap(&itemA, &itemB, sizeof(Item_t));
    printf("   After struct swap:  itemA={%d, %s}, itemB={%d, %s}\n\n",
           itemA.id, itemA.code, itemB.id, itemB.code);

    printf("=== Chapter 7 reference executed successfully! ===\n");
    return 0;
}
