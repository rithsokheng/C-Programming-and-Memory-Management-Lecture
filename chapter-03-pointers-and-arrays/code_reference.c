/**
 * Chapter 3: Pointers and Arrays - Code Reference
 * 
 * This file contains reference implementations of pointers and arrays:
 * - Address-of operator (&) and dereferencing (*)
 * - Passing pointers to functions and struct arrow operator (->)
 * - Arrays, pointer arithmetic, and array decay
 * - Multidimensional arrays and arrays of structs
 * - C strings, null termination, and string.h operations
 * - Custom string concatenation and bounded string appending
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter03 && ./chapter03
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/* --- Struct Definitions --- */

typedef struct {
    int x;
    int y;
    int z;
} Coordinate_t;

/* --- Pointer and Function Demonstrations --- */

/* Mutates struct directly via pointer using the arrow operator */
void update_coordinate(Coordinate_t *point, int new_x, int new_y, int new_z) {
    if (point == NULL) return;
    point->x = new_x;
    point->y = new_y;
    point->z = new_z;
}

/* Array decay: 'int arr[]' decays to 'int *arr', so length must be passed */
int sum_array(const int *arr, size_t length) {
    int total = 0;
    for (size_t i = 0; i < length; i++) {
        /* Array indexing arr[i] is identical to *(arr + i) */
        total += *(arr + i);
    }
    return total;
}

/* Custom string concatenation copying into destination buffer */
bool concat_strings(char *dest, size_t dest_size, const char *src1, const char *src2) {
    size_t len1 = strlen(src1);
    size_t len2 = strlen(src2);

    /* +1 for null terminator */
    if (len1 + len2 + 1 > dest_size) {
        return false; /* Buffer too small */
    }

    /* Copy first string */
    for (size_t i = 0; i < len1; i++) {
        dest[i] = src1[i];
    }

    /* Append second string */
    for (size_t j = 0; j < len2; j++) {
        dest[len1 + j] = src2[j];
    }

    dest[len1 + len2] = '\0';
    return true;
}

/* Bounded string append */
bool append_string(char *dest, size_t dest_size, const char *src) {
    size_t cur_len = strlen(dest);
    size_t src_len = strlen(src);

    if (cur_len + src_len + 1 > dest_size) {
        return false;
    }

    strcat(dest, src);
    return true;
}

/* --- Main Demonstration --- */

int main(void) {
    printf("=== Chapter 3: Pointers and Arrays Code Reference ===\n\n");

    /* 3.2 & 3.4 & 3.5 Addresses, Pointers, and Dereferencing */
    printf("1. Addresses, Pointers, and Dereferencing:\n");
    int x = 42;
    int *p = &x;

    printf("   Value of x:       %d\n", x);
    printf("   Address of x (&x): %p\n", (void *)&x);
    printf("   Pointer value (p): %p\n", (void *)p);
    printf("   Dereferenced (*p): %d\n", *p);

    *p = 99; /* Modify x through pointer */
    printf("   Value after *p=99: %d\n\n", x);

    /* 3.7 Pointer Parameters and Arrow Operator */
    printf("2. Pointer Parameters & Arrow Operator (->):\n");
    Coordinate_t coord = { .x = 1, .y = 2, .z = 3 };
    printf("   Before update: (%d, %d, %d)\n", coord.x, coord.y, coord.z);
    update_coordinate(&coord, 100, 200, 300);
    printf("   After update:  (%d, %d, %d)\n\n", coord.x, coord.y, coord.z);

    /* 3.8 & 3.9 & 3.11 Arrays, Pointer Arithmetic, and Array Decay */
    printf("3. Arrays and Pointer Arithmetic:\n");
    int numbers[5] = { 10, 20, 30, 40, 50 };
    printf("   Base address:     %p\n", (void *)numbers);
    printf("   Address of index 1 (&numbers[1]): %p (offset: +%zu bytes)\n",
           (void *)&numbers[1], (size_t)((char *)&numbers[1] - (char *)numbers));

    printf("   Traversal using *(ptr + i): ");
    for (size_t i = 0; i < 5; i++) {
        printf("%d ", *(numbers + i));
    }
    printf("\n");

    int sum = sum_array(numbers, 5);
    printf("   Sum via array-decay function: %d\n\n", sum);

    /* 3.10 Multidimensional Arrays / Arrays of Structs */
    printf("4. Array of Structs:\n");
    Coordinate_t points[3] = {
        { .x = 1, .y = 2, .z = 3 },
        { .x = 4, .y = 5, .z = 6 },
        { .x = 7, .y = 8, .z = 9 }
    };
    for (size_t i = 0; i < 3; i++) {
        printf("   Point %zu: (%d, %d, %d)\n", i, points[i].x, points[i].y, points[i].z);
    }
    printf("\n");

    /* 3.12 & 3.14 & 3.15 C Strings and Concatenation */
    printf("5. Strings & Safe Appending:\n");
    char buffer[64];
    if (concat_strings(buffer, sizeof(buffer), "Hello, ", "SnakeLang!")) {
        printf("   concat_strings result: \"%s\"\n", buffer);
    }

    if (append_string(buffer, sizeof(buffer), " v1.0")) {
        printf("   append_string result:  \"%s\"\n", buffer);
        printf("   Length: %zu characters\n", strlen(buffer));
    }
    printf("\n");

    printf("=== Chapter 3 reference executed successfully! ===\n");
    return 0;
}
