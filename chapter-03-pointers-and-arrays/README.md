# Chapter 3 - Pointers and Arrays

> **Navigation:** [Previous: Chapter 2 - Structs](../chapter-02-structs/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 4 - Enums](../chapter-04-enums/README.md)

---

## Table of Contents

- [3.1 The Memory Model](#31-the-memory-model)
- [3.2 Addresses and the Address-of Operator](#32-addresses-and-the-address-of-operator)
- [3.3 Virtual Memory](#33-virtual-memory)
- [3.4 What Is a Pointer?](#34-what-is-a-pointer)
- [3.5 Dereferencing](#35-dereferencing)
- [3.6 Pass-by-Value in C](#36-pass-by-value-in-c)
- [3.7 Pointer Parameters and the Arrow Operator](#37-pointer-parameters-and-the-arrow-operator)
- [3.8 Arrays](#38-arrays)
- [3.9 Pointer Arithmetic](#39-pointer-arithmetic)
- [3.10 Multidimensional Arrays / Arrays of Structs](#310-multidimensional-arrays--arrays-of-structs)
- [3.11 Array Decay](#311-array-decay)
- [3.12 C Strings in Memory](#312-c-strings-in-memory)
- [3.13 The `string.h` Library](#313-the-stringh-library)
- [3.14 Implementing String Concatenation](#314-implementing-string-concatenation)
- [3.15 Bounded String Appending](#315-bounded-string-appending)
- [3.16 Forward Declarations](#316-forward-declarations)

---

## 3.1 The Memory Model

A useful mental model is:

> Memory is a large sequence of bytes, and variables provide readable names for locations in that memory.

Conceptually:

```text
Memory

address     value
-------     -----
0x1000      ...
0x1001      ...
0x1002      ...
0x1003      ...
...
```

A variable:

```c
int x = 5;
```

can be thought of as:

```text
name:    x
address: some memory address
value:   5
```

---

## 3.2 Addresses and the Address-of Operator

Use `&` to obtain an object's address.

```c
int age = 20;

printf("%p\n", (void *)&age);
```

The pointer-format specifier is `%p`.

The address is generally displayed in hexadecimal.

Conceptually:

```text
age
 |
 | address-of (&)
 v
0x7ffd1234...
```

---

## 3.3 Virtual Memory

Modern operating systems give processes their own virtual address spaces.

Simplified:

```text
Program
   |
   v
Virtual address space
   |
   v
Operating system
   |
   v
Physical memory
```

This separation helps isolate processes from one another and allows the operating system to manage memory mappings.

---

## 3.4 What Is a Pointer?

A pointer is a value that stores an address.

Example:

```c
int x = 5;
int *p = &x;
```

Conceptually:

```text
x: 5
address of x: 0x1000

p: 0x1000
```

So:

```c
int *p
```

means:

> `p` is a pointer to an `int`.

---

## 3.5 Dereferencing

Use `*` to dereference a pointer.

```c
int x = 5;
int *p = &x;

int y = *p;
```

Now:

```text
p  ---> x
         5
```

So `*p` means:

> Go to the memory location stored in `p` and obtain the value there.

Dereferencing can also modify the pointed-to value:

```c
*p = 10;
```

Now `x` is `10`.

---

## 3.6 Pass-by-Value in C

C passes function arguments by value.

Example:

```c
void change(int x) {
    x = 100;
}

int main(void) {
    int n = 5;
    change(n);

    printf("%d\n", n);  // 5
}
```

The function receives a copy.

For structs:

```c
struct Coordinate {
    int x;
    int y;
};

void change_x(struct Coordinate c, int new_x) {
    c.x = new_x;
}
```

The original struct remains unchanged because `c` is a copy.

To modify the original object, use a pointer.

---

## 3.7 Pointer Parameters and the Arrow Operator

Pointer parameter:

```c
void change_x(struct Coordinate *c, int new_x) {
    c->x = new_x;
}
```

Calling code:

```c
struct Coordinate c = {
    .x = 1,
    .y = 2
};

change_x(&c, 99);
```

### `.` vs `->`

For a normal struct:

```c
c.x
```

For a pointer to a struct:

```c
c->x
```

Conceptually:

```c
ptr->field
```

is equivalent to:

```c
(*ptr).field
```

---

## 3.8 Arrays

A C array:

- has a fixed size at declaration
- stores elements of one type
- is indexed from zero
- stores elements contiguously

Example:

```c
int numbers[5] = {1, 2, 3, 4, 5};
```

Access:

```c
printf("%d\n", numbers[0]);
printf("%d\n", numbers[2]);
```

Modify:

```c
numbers[2] = 100;
```

C does not provide a built-in dynamic list abstraction in the same style as Python.

---

## 3.9 Pointer Arithmetic

Arrays and pointers are closely related.

Given:

```c
int numbers[] = {1, 2, 3, 4, 5};
```

we can write:

```c
int *p = numbers;
```

Then:

```c
numbers[2]
```

is equivalent to:

```c
*(numbers + 2)
```

Conceptually:

```text
numbers
   |
   v
[1][2][3][4][5]
 ^
 |
base
```

Adding `1` to an `int *` advances by `sizeof(int)` bytes.

---

## 3.10 Multidimensional Arrays / Arrays of Structs

Suppose:

```c
struct Coordinate {
    int x;
    int y;
    int z;
};

struct Coordinate points[3];
```

Each `Coordinate` occupies a contiguous block, and the array repeats that layout.

Conceptually:

```text
points[0] -> x y z
points[1] -> x y z
points[2] -> x y z
```

Pointer arithmetic advances by the full size of the element type.

---

## 3.11 Array Decay

One of the important C rules discussed is **array-to-pointer decay**.

An array:

```c
int numbers[8];
```

has an actual array size that `sizeof` can know.

Inside many expressions and function calls, however, the array is converted to a pointer to its first element.

### Example

```c
void print_size(int numbers[]) {
    printf("%zu\n", sizeof(numbers));
}
```

Inside the function, `numbers` is treated as a pointer parameter, so its size is the pointer size rather than the full array size.

Compare:

```c
int numbers[8];

printf("%zu\n", sizeof(numbers));
```

with a pointer:

```c
int *p = numbers;

printf("%zu\n", sizeof(p));
```

---

## 3.12 C Strings in Memory

A C string is an array of characters followed by `'\0'`.

Example:

```c
char message[] = "Hello";
```

Memory conceptually:

```text
H e l l o \0
```

The string length excludes the terminator.

```c
strlen("Hello") == 5
```

---

## 3.13 The `string.h` Library

Important standard string functions:

### `strcpy`

```c
strcpy(destination, source);
```

Copies a string.

### `strcat`

```c
strcat(destination, source);
```

Appends one string to another.

### `strlen`

```c
size_t len = strlen(text);
```

Returns the length excluding the null terminator.

### `strcmp`

```c
int result = strcmp(a, b);
```

Compares two strings.

### `strncpy`

Copies up to a specified number of characters.

### `strncat`

Appends up to a specified number of characters.

### `strchr`

Finds the first occurrence of a character.

---

## 3.14 Implementing String Concatenation

The course first demonstrates how to concatenate manually rather than using `strcat`.

Example:

```c
void concat_string(char *dest, const char *src) {
    int idx = 0;

    // Find the null terminator in dest.
    while (dest[idx] != '\0') {
        idx++;
    }

    // Copy src, including its null terminator.
    int src_idx = 0;

    while (src[src_idx] != '\0') {
        dest[idx] = src[src_idx];
        idx++;
        src_idx++;
    }

    dest[idx] = '\0';
}
```

Conceptually:

```text
destination:
S n a k e \0

source:
L a n g \0

after concat:

S n a k e L a n g \0
```

---

## 3.15 Bounded String Appending

Example of a fixed 64-byte destination buffer with safe appending behavior:

Conceptual implementation:

```c
#include <string.h>

#define MAX_BUFFER_SIZE 64

int smart_append(struct TextBuffer *dest, const char *source) {
    if (dest == NULL || source == NULL) {
        return 1;
    }

    size_t source_len = strlen(source);
    size_t remaining = sizeof(dest->buffer) - dest->length - 1;

    if (source_len > remaining) {
        strncat(dest->buffer, source, remaining);
        dest->buffer[sizeof(dest->buffer) - 1] = '\0';
        dest->length = sizeof(dest->buffer) - 1;
        return 1;
    }

    strcat(dest->buffer, source);
    dest->length += source_len;

    return 0;
}
```

Example structure:

```c
typedef struct TextBuffer {
    char buffer[64];
    size_t length;
} TextBuffer;
```

### Core rule

Always reserve space for the null terminator:

```text
usable characters = buffer capacity - 1
```

---

## 3.16 Forward Declarations

Forward declarations let C know about a type before its full definition appears.

Example:

```c
typedef struct Node Node_t;

struct Node {
    int value;
    Node_t *next;
};
```

The forward declaration is especially useful for self-referential structures.

### Mutually referencing structs

Example:

```c
typedef struct Employee Employee_t;
typedef struct Department Department_t;

struct Employee {
    int id;
    char *name;
    Department_t *department;
};

struct Department {
    char *name;
    Employee_t *manager;
};
```

Pointers are essential here because directly embedding two recursively sized structs would create an impossible infinite-size layout.

---

> **Navigation:** [Previous: Chapter 2 - Structs](../chapter-02-structs/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 4 - Enums](../chapter-04-enums/README.md)
