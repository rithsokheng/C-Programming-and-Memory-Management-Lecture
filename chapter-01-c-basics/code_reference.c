/**
 * Chapter 1: C Basics - Code Reference
 * 
 * This file contains reference implementations of core C fundamentals:
 * - Hello World program structure
 * - Variables, basic data types, and const
 * - Function declarations and type casting
 * - void parameter and return types
 * - Arithmetic and increment/decrement operators
 * - Conditionals (if-else, ternary)
 * - sizeof operator
 * - Loops (for, while, do-while)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter01 && ./chapter01
 */

#include <stdio.h>
#include <stdbool.h>

/* --- 1. Functions & Casting --- */

/* Function taking two ints and casting the result to float */
float add_and_cast(int x, int y) {
    return (float)(x + y);
}

/* Function taking void (no arguments) and returning void */
void print_separator(void) {
    printf("----------------------------------------\n");
}

/* SnakeLang memory estimation example from lecture:
   Convert bytes to megabytes using float casting */
float bytes_to_megabytes(int bytes) {
    return (float)bytes / (1024.0f * 1024.0f);
}

/* --- 2. Conditionals --- */

int check_threshold(int x) {
    if (x > 10) {
        return 1;
    } else if (x == 10) {
        return 0;
    } else {
        return -1;
    }
}

/* Ternary operator example */
const char *get_status_label(bool is_active) {
    return is_active ? "ACTIVE" : "INACTIVE";
}

/* --- 3. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 1: C Basics Code Reference ===\n\n");

    /* 1.1 Hello World */
    printf("1. Hello World:\n");
    printf("   Hello, World from C!\n\n");

    /* 1.4 & 1.7 Basic Data Types & Variables & const */
    print_separator();
    printf("2. Variables and Basic Data Types:\n");
    int age = 20;
    char permission = 'r';
    float pi = 3.14159f;
    const char *name = "Alice";
    const int max_threads = 8;

    printf("   int:         %d\n", age);
    printf("   char:        '%c' (ASCII: %d)\n", permission, permission);
    printf("   float:       %.2f\n", pi);
    printf("   string:      \"%s\"\n", name);
    printf("   const int:   %d\n\n", max_threads);

    /* 1.8 Type Casting & Functions */
    print_separator();
    printf("3. Functions and Type Casting:\n");
    int a = 7, b = 2;
    float sum_f = add_and_cast(a, b);
    float int_div = (float)a / (float)b;
    printf("   add_and_cast(%d, %d) = %.2f\n", a, b, sum_f);
    printf("   %d / %d with float cast = %.2f\n", a, b, int_div);

    int memory_bytes = 10485760; // 10 MB in bytes
    printf("   %d bytes = %.2f MB\n\n", memory_bytes, bytes_to_megabytes(memory_bytes));

    /* 1.11 Arithmetic & Increment/Decrement */
    print_separator();
    printf("4. Arithmetic & Increment Operators:\n");
    int counter = 5;
    printf("   Initial:  %d\n", counter);
    printf("   Post-inc: %d (evaluates then increments)\n", counter++);
    printf("   After:    %d\n", counter);
    printf("   Pre-inc:  %d (increments then evaluates)\n\n", ++counter);

    /* 1.12 & 1.13 Conditionals & Ternary */
    print_separator();
    printf("5. Conditionals:\n");
    printf("   check_threshold(15) = %d\n", check_threshold(15));
    printf("   check_threshold(10) = %d\n", check_threshold(10));
    printf("   check_threshold(3)  = %d\n", check_threshold(3));
    printf("   Ternary status: %s\n\n", get_status_label(true));

    /* 1.14 sizeof */
    print_separator();
    printf("6. Type Sizes (sizeof):\n");
    printf("   sizeof(char):   %zu byte(s)\n", sizeof(char));
    printf("   sizeof(int):    %zu byte(s)\n", sizeof(int));
    printf("   sizeof(float):  %zu byte(s)\n", sizeof(float));
    printf("   sizeof(double): %zu byte(s)\n", sizeof(double));
    printf("   sizeof(void *): %zu byte(s)\n\n", sizeof(void *));

    /* 1.15 Loops */
    print_separator();
    printf("7. Loops:\n");
    printf("   For loop (0..4):   ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    printf("   While loop (0..4): ");
    int w = 0;
    while (w < 5) {
        printf("%d ", w);
        w++;
    }
    printf("\n");

    printf("   Do-while loop:     ");
    int dw = 0;
    do {
        printf("%d ", dw);
        dw++;
    } while (dw < 3);
    printf("\n");

    printf("   Reverse loop:      ");
    for (int rev = 4; rev >= 0; rev--) {
        printf("%d ", rev);
    }
    printf("\n\n");

    printf("=== Chapter 1 reference executed successfully! ===\n");
    return 0;
}
