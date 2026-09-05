/**
 * Chapter 2: Structs - Code Reference
 * 
 * This file contains reference implementations of C structs:
 * - Defining structs and using typedef
 * - Positional vs designated initializers
 * - Pass-by-value behavior and returning modified structs
 * - Memory layout, alignment, and padding (offsetof, sizeof)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter02 && ./chapter02
 */

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/* --- 1. Struct Definitions --- */

/* Basic struct with typedef */
typedef struct {
    int x;
    int y;
    int z;
} Coordinate_t;

/* Struct demonstrating user data */
typedef struct {
    int id;
    const char *username;
    bool is_admin;
} User_t;

/* Struct demonstrating memory alignment and padding:
   char (1 byte) + 3 bytes padding + int (4 bytes) + double (8 bytes) = 16 bytes */
typedef struct {
    char flag;      /* offset 0 */
    int count;      /* offset 4 (3 bytes padding before this) */
    double weight;  /* offset 8 */
} PaddedStruct_t;

/* Packed or reordered struct to minimize padding */
typedef struct {
    double weight;  /* offset 0 (8 bytes) */
    int count;      /* offset 8 (4 bytes) */
    char flag;      /* offset 12 (1 byte) + 3 bytes trailing padding = 16 bytes */
} OptimizedStruct_t;

/* --- 2. Functions Returning Structs (Value Semantics) --- */

/* In C, structs are passed and returned by value (copied) */
Coordinate_t move_x(Coordinate_t coord, int delta_x) {
    coord.x += delta_x;
    return coord; /* Returns a copy with modified x */
}

/* --- 3. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 2: Structs Code Reference ===\n\n");

    /* 2.2 Defining and Initializing Structs */
    printf("1. Positional Initialization:\n");
    Coordinate_t p1 = { 10, 20, 30 };
    printf("   p1 = (x: %d, y: %d, z: %d)\n\n", p1.x, p1.y, p1.z);

    /* 2.3 Designated Initializers */
    printf("2. Designated Initializers:\n");
    Coordinate_t p2 = {
        .z = 100,
        .x = 25,
        .y = 50
    };
    printf("   p2 = (x: %d, y: %d, z: %d)\n", p2.x, p2.y, p2.z);

    User_t admin = {
        .id = 1,
        .username = "root",
        .is_admin = true
    };
    printf("   User #%d: %s (Admin: %s)\n\n",
           admin.id, admin.username, admin.is_admin ? "yes" : "no");

    /* 2.4 Pass-by-Value & Returning Modified Structs */
    printf("3. Pass-by-Value Semantics:\n");
    Coordinate_t moved = move_x(p1, 5);
    printf("   Original p1.x: %d (unchanged)\n", p1.x);
    printf("   Moved    p1.x: %d (returned copy)\n\n", moved.x);

    /* 2.6 Memory Layout, Padding, and offsetof */
    printf("4. Struct Memory Layout and Padding:\n");
    printf("   sizeof(Coordinate_t):  %zu bytes (3 x 4-byte ints)\n", sizeof(Coordinate_t));
    printf("   sizeof(PaddedStruct_t): %zu bytes\n", sizeof(PaddedStruct_t));
    printf("     - offsetof(flag):    %zu\n", offsetof(PaddedStruct_t, flag));
    printf("     - offsetof(count):   %zu (padded for 4-byte alignment)\n", offsetof(PaddedStruct_t, count));
    printf("     - offsetof(weight):  %zu (aligned to 8-byte boundary)\n", offsetof(PaddedStruct_t, weight));

    printf("   sizeof(OptimizedStruct_t): %zu bytes\n", sizeof(OptimizedStruct_t));
    printf("     - offsetof(weight):  %zu\n", offsetof(OptimizedStruct_t, weight));
    printf("     - offsetof(count):   %zu\n", offsetof(OptimizedStruct_t, count));
    printf("     - offsetof(flag):    %zu\n\n", offsetof(OptimizedStruct_t, flag));

    printf("=== Chapter 2 reference executed successfully! ===\n");
    return 0;
}
