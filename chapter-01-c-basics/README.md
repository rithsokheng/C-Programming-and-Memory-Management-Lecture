# Chapter 1 - C Basics

> **Navigation:** [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 2 - Structs](../chapter-02-structs/README.md)

---

## Table of Contents

- [1.1 Hello World](#11-hello-world)
- [1.2 Compiled vs. Interpreted Languages](#12-compiled-vs-interpreted-languages)
- [1.3 Comments](#13-comments)
- [1.4 Basic Data Types](#14-basic-data-types)
- [1.5 C Strings](#15-c-strings)
- [1.6 Printing Variables with `printf`](#16-printing-variables-with-printf)
- [1.7 Variables, Assignment, and `const`](#17-variables-assignment-and-const)
- [1.8 Functions and Type Casting](#18-functions-and-type-casting)
- [1.9 The `void` Type](#19-the-void-type)
- [1.10 Unit Testing](#110-unit-testing)
- [1.11 Arithmetic and Increment/Decrement](#111-arithmetic-and-incrementdecrement)
- [1.12 Conditional Statements](#112-conditional-statements)
- [1.13 The Ternary Operator](#113-the-ternary-operator)
- [1.14 `sizeof`](#114-sizeof)
- [1.15 Loops](#115-loops)
- [1.16 Header Guards and `#pragma once`](#116-header-guards-and-pragma-once)

---

## 1.1 Hello World

A minimal C program contains an entry point called `main`.

```c
#include <stdio.h>

int main(void) {
    printf("Program in C\n");
    return 0;
}
```

### Important pieces

| Code | Meaning |
|---|---|
| `#include <stdio.h>` | Includes declarations from the standard I/O library |
| `int main(void)` | Program entry point returning an integer |
| `{ ... }` | Function body |
| `printf(...)` | Formatted output |
| `\n` | Newline |
| `return 0;` | Successful program termination |

C statements normally end with semicolons.

---

## 1.2 Compiled vs. Interpreted Languages

Key differences between compiled and interpreted languages:

- **Interpreted languages:** Python, JavaScript
- **Compiled languages:** C, Go, Rust

A simplified mental model:

```text
Source code
    |
    v
 Compiler
    |
    v
Executable / binary
    |
    v
CPU executes program
```

The important practical differences discussed are:

### Interpreted model

An interpreter executes code through a runtime environment. Errors can appear when execution reaches the problematic statement.

### Compiled model

A compiler performs checks before execution and produces an executable.

Advantages discussed:

- many errors can be detected earlier
- the executable can run without shipping the entire language implementation
- compiled programs are often faster in practice

Trade-offs discussed:

- compilation can take time
- strict compile-time checks can slow rapid experimentation

---

## 1.3 Comments

C supports two common comment forms.

### Single-line comment

```c
// This is a comment
```

### Multi-line comment

```c
/*
   This is a
   multi-line comment.
*/
```

---

## 1.4 Basic Data Types

The first basic types covered are:

- `int`
- `float`
- `char`
- `char *`

### Integer

```c
int age = 20;
```

### Floating-point number

```c
float pi = 3.14f;
```

### Character

```c
char permission = 'r';
```

### Character pointer / C string

```c
char *title = "SnakeLang";
```

---

## 1.5 C Strings

Unlike languages with a built-in high-level string object, C represents strings as character sequences ending with a null terminator.

Example:

```c
char *name = "Alice";
```

Conceptually:

```text
A l i c e \0
```

The `\0` byte marks the end of the string.

### Character vs. string

These are different:

```c
char a = 'A';
char *b = "A";
```

`'A'` is one character.

`"A"` is a string containing the character `A` plus the terminating null byte.

---

## 1.6 Printing Variables with `printf`

C uses format specifiers.

Common specifiers from the lecture:

| Specifier | Intended value |
|---|---|
| `%d` | integer |
| `%c` | character |
| `%f` | floating-point value |
| `%s` | string |
| `%p` | pointer |
| `%zu` | `size_t` |

Example:

```c
#include <stdio.h>

int main(void) {
    char *name = "Alice";
    int age = 20;

    printf("Hello %s, age %d\n", name, age);

    return 0;
}
```

Another example:

```c
int max_threads = 8;
char permissions = 'r';
float pi = 3.14f;
char *title = "SnakeLang";

printf("max threads: %d\n", max_threads);
printf("permissions: %c\n", permissions);
printf("pi: %f\n", pi);
printf("title: %s\n", title);
```

---

## 1.7 Variables, Assignment, and `const`

C variables have a fixed declared type within their scope.

This is valid:

```c
int x = 5;
x = 10;
```

This is not valid as a redeclaration in the same scope:

```c
int x = 5;
int x = 10;   // invalid redeclaration
```

Instead:

```c
int x = 5;
x = 10;
```

### Constant values

Use `const` when a value should not be reassigned:

```c
const int answer = 42;
```

Attempting this:

```c
answer = 99;
```

should be rejected by the compiler.

The lecture distinguishes a variable's **type** from its **value**: the type is fixed after declaration, but a normal variable's value can change.

---

## 1.8 Functions and Type Casting

A C function specifies its return type and parameter types.

Example:

```c
float add(int x, int y) {
    return (float)(x + y);
}
```

Here:

- return type: `float`
- function name: `add`
- parameter 1: `int x`
- parameter 2: `int y`

### Type casting

A cast has this form:

```c
(type) expression
```

Example:

```c
float result = (float)(x + y);
```

This is useful when integer arithmetic would otherwise produce integer division.

For example:

```c
float get_average(int x, int y, int z) {
    return (float)(x + y + z) / 3.0f;
}
```

### SnakeLang memory example

The exercise described in the lecture can be represented as:

```c
int max_snake_lang_memory(int max_threads, int memory_per_thread) {
    return max_threads * memory_per_thread;
}
```

---

## 1.9 The `void` Type

`void` is used in two important situations.

### Function takes no arguments

```c
int main(void) {
    return 0;
}
```

### Function returns nothing

```c
void print_hello(void) {
    printf("Hello\n");
}
```

There is no value of type `void` that you can store like an ordinary variable.

---

## 1.10 Unit Testing

The course introduces a small unit-testing framework and uses a pattern similar to:

```text
Test case
   |
   v
Assertions
   |
   v
Run all test cases
```

A simplified conceptual structure:

```c
test_function() {
    // arrange
    // act
    // assert
}
```

---

## 1.11 Arithmetic and Increment/Decrement

Standard arithmetic operators:

```c
+   // addition
-   // subtraction
*   // multiplication
/   // division
```

Compound assignment:

```c
x += 1;
x -= 1;
```

Increment/decrement:

```c
x++;
x--;
```

The course discusses both postfix and prefix forms:

```c
x++;
++x;

x--;
--x;
```

The main practical use shown is in loops.

### Example

```c
for (int i = 0; i < 5; i++) {
    printf("%d\n", i);
}
```

---

## 1.12 Conditional Statements

Basic form:

```c
if (condition) {
    // body
}
```

Multiple branches:

```c
if (condition1) {
    // ...
} else if (condition2) {
    // ...
} else {
    // ...
}
```

### SnakeLang temperature example

```c
char *get_temperature_status(int temp) {
    if (temp < 70) {
        return "cold";
    } else if (temp > 90) {
        return "hot";
    } else {
        return "just right";
    }
}
```

### Style recommendation

Prefer braces:

```c
if (x > 10) {
    return 0;
}
```

instead of relying on a single unbraced statement:

```c
if (x > 10)
    return 0;
```

---

## 1.13 The Ternary Operator

C provides a compact conditional expression:

```c
condition ? value_if_true : value_if_false
```

Example:

```c
int max = (a > b) ? a : b;
```

It is useful when the expression remains simple. Avoid deeply nested ternary expressions.

---

## 1.14 `sizeof`

`sizeof` reports the size of a type or object.

Example:

```c
printf("%zu\n", sizeof(int));
printf("%zu\n", sizeof(float));
printf("%zu\n", sizeof(double));
printf("%zu\n", sizeof(char));
```

Important points:

- `sizeof(char)` is `1`
- sizes of other types can depend on the platform
- `sizeof` is an operator, not a normal function

Example:

```c
printf("int: %zu bytes\n", sizeof(int));
printf("float: %zu bytes\n", sizeof(float));
printf("double: %zu bytes\n", sizeof(double));
```

---

## 1.15 Loops

### `for` loop

A C `for` loop contains:

1. initialization
2. condition
3. final expression

```c
for (int i = 0; i < 5; i++) {
    printf("%d\n", i);
}
```

### Printing a range

```c
void print_numbers(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("%d\n", i);
    }
}
```

### `while`

```c
int i = 0;

while (i < 5) {
    printf("%d\n", i);
    i++;
}
```

### Reverse loop

```c
void print_numbers_reverse(int start, int end) {
    int i = end;

    while (i >= start) {
        printf("%d\n", i);
        i--;
    }
}
```

### `do while`

A `do while` loop executes its body at least once.

```c
int i = 1;

do {
    printf("%d\n", i);
    i++;
} while (i <= 5);
```

---

## 1.16 Header Guards and `#pragma once`

C header files are often included with:

```c
#include "exercise.h"
```

The preprocessor effectively inserts the header's contents into the source file.

Repeated inclusion can produce redefinition problems.

A common modern approach:

```c
#pragma once
```

Example header:

```c
#pragma once

int get_average(int x, int y, int z);
```

Traditional header guards can also be written as:

```c
#ifndef EXERCISE_H
#define EXERCISE_H

int get_average(int x, int y, int z);

#endif
```

---

> **Navigation:** [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 2 - Structs](../chapter-02-structs/README.md)
