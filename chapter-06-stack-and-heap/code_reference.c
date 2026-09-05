/**
 * Chapter 6: Stack and Heap - Code Reference
 * 
 * This file contains reference implementations of memory management:
 * - Dynamic allocation with malloc and free
 * - Zero-initialized allocation with calloc
 * - Safe reallocation pattern with realloc
 * - Preventing dangling pointers (free-and-NULL pattern)
 * - Detecting system endianness at runtime
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter06 && ./chapter06
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- 1. Heap Allocation & Dynamic Arrays --- */

/* Allocates and initializes an array of integers */
int *create_int_array(size_t count, int initial_val) {
    /* 1. Allocate heap memory */
    int *arr = malloc(count * sizeof(int));
    if (arr == NULL) {
        perror("malloc failed");
        return NULL;
    }

    /* 2. Initialize values */
    for (size_t i = 0; i < count; i++) {
        arr[i] = initial_val + (int)i;
    }

    return arr;
}

/* Demonstrates safe dynamic resizing using realloc */
bool resize_int_array(int **arr_ptr, size_t *capacity) {
    size_t new_cap = (*capacity) * 2;

    /* Always store realloc return in a temporary pointer first */
    int *temp = realloc(*arr_ptr, new_cap * sizeof(int));
    if (temp == NULL) {
        /* Allocation failed; original *arr_ptr is still valid! */
        return false;
    }

    *arr_ptr = temp;
    *capacity = new_cap;
    return true;
}

/* --- 2. Endianness Detection --- */

bool is_little_endian(void) {
    uint32_t val = 0x01020304;
    /* Inspect the lowest memory address */
    uint8_t *first_byte = (uint8_t *)&val;
    return (*first_byte == 0x04);
}

/* --- 3. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 6: Stack and Heap Code Reference ===\n\n");

    /* 6.7 malloc and free */
    printf("1. Heap Allocation with malloc & free:\n");
    size_t count = 5;
    int *numbers = create_int_array(count, 10);
    if (numbers == NULL) {
        return 1;
    }

    printf("   Allocated %zu ints at %p:\n   ", count, (void *)numbers);
    for (size_t i = 0; i < count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");

    /* Dynamic resizing with realloc */
    printf("2. Dynamic Resizing with realloc:\n");
    if (resize_int_array(&numbers, &count)) {
        printf("   Resized array capacity to %zu elements (new address: %p)\n", count, (void *)numbers);
        /* Initialize the new elements */
        for (size_t i = 5; i < count; i++) {
            numbers[i] = (int)(i * 10);
        }
        printf("   Full array: ");
        for (size_t i = 0; i < count; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n\n");
    }

    /* 6.9 & 6.10 Free and NULL cleanup */
    printf("3. Memory Cleanup & Preventing Use-After-Free:\n");
    free(numbers);
    numbers = NULL; /* Invalidate pointer */
    printf("   Memory freed and pointer set to %p\n\n", (void *)numbers);

    /* 6.8 calloc (zero-initialized) */
    printf("4. Zero-Initialized Memory with calloc:\n");
    size_t zero_count = 4;
    int *zeros = calloc(zero_count, sizeof(int));
    if (zeros != NULL) {
        printf("   calloc allocated 4 ints: ");
        for (size_t i = 0; i < zero_count; i++) {
            printf("%d ", zeros[i]);
        }
        printf("\n");
        free(zeros);
        zeros = NULL;
    }
    printf("\n");

    /* 6.11 Endianness */
    printf("5. System Endianness:\n");
    if (is_little_endian()) {
        printf("   Architecture: Little-Endian (Least significant byte stored at lowest address)\n");
    } else {
        printf("   Architecture: Big-Endian (Most significant byte stored at lowest address)\n");
    }
    printf("\n");

    printf("=== Chapter 6 reference executed successfully! ===\n");
    return 0;
}
