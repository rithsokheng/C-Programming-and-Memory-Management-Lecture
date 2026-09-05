# Chapter 7 - Advanced Pointers

> **Navigation:** [Previous: Chapter 6 - Stack and Heap](../chapter-06-stack-and-heap/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 8 - Building a Stack Data Structure](../chapter-08-stack-data-structure/README.md)

---

## Table of Contents

- [7.1 Pointer-to-Pointer](#71-pointer-to-pointer)
- [7.2 Allocating Through a Double Pointer](#72-allocating-through-a-double-pointer)
- [7.3 Arrays of Pointers](#73-arrays-of-pointers)
- [7.4 `void *` Pointers](#74-void--pointers)
- [7.5 Swapping Values](#75-swapping-values)
- [7.6 Generic Swap with `memcpy`](#76-generic-swap-with-memcpy)

---

## 7.1 Pointer-to-Pointer

A pointer-to-pointer is simply a pointer whose stored value is itself the address of another pointer.

Example:

```c
int x = 5;
int *p = &x;
int **pp = &p;
```

Conceptually:

```text
pp
 |
 v
 p
 |
 v
 x
 |
 5
```

Dereference once:

```c
*p
```

Dereference twice:

```c
**pp
```

---

## 7.2 Allocating Through a Double Pointer

A common use is allowing a function to change a caller's pointer.

Example:

```c
#include <stdlib.h>

void allocate_int(int **pointer, int value) {
    int *new_pointer = malloc(sizeof(int));

    if (new_pointer == NULL) {
        return;
    }

    *new_pointer = value;
    *pointer = new_pointer;
}
```

Usage:

```c
int *p = NULL;

allocate_int(&p, 42);

printf("%d\n", *p);

free(p);
```

Why `int **`?

Because the function needs to modify `p` itself, not only the integer pointed to by `p`.

---

## 7.3 Arrays of Pointers

An array of strings is naturally represented as an array of `char *` pointers.

Example:

```c
char *names[] = {
    "Alice",
    "Bob",
    "Charlie"
};
```

Conceptually:

```text
names
 |
 +----> "Alice"
 |
 +----> "Bob"
 |
 +----> "Charlie"
```

If dynamically allocating the pointer array:

```c
char **names = malloc(3 * sizeof(char *));
```

The type `char **` means:

> pointer to pointer to character

---

## 7.4 `void *` Pointers

A `void *` is a generic object pointer.

Example:

```c
int x = 42;

void *data = &x;
```

The compiler no longer knows the pointed-to object's specific type through the `void *`.

To access the data as an `int`:

```c
int value = *(int *)data;
```

### Important idea

`void *` gives flexibility, but removes type information.

The programmer must keep track of the real type.

---

## 7.5 Swapping Values

### Integer swap

```c
void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

Example:

```c
int a = 10;
int b = 20;

swap_ints(&a, &b);
```

The temporary variable prevents the original value from being lost.

---

## 7.6 Generic Swap with `memcpy`

For a generic swap, the function needs to work with arbitrary-sized data.

```c
#include <stdlib.h>
#include <string.h>

void swap(void *vp1, void *vp2, size_t size) {
    void *temp = malloc(size);

    if (temp == NULL) {
        return;
    }

    memcpy(temp, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, temp, size);

    free(temp);
}
```

### Example

```c
int a = 10;
int b = 20;

swap(&a, &b, sizeof(int));
```

The same function can operate on other types:

```c
float x = 3.14f;
float y = 9.81f;

swap(&x, &y, sizeof(float));
```

The generic swap pattern:

1. allocate temporary storage
2. copy first value into temporary storage
3. copy second value into first
4. copy temporary value into second
5. free the temporary storage

It also emphasizes that the size must be known at runtime for a generic version.

---

> **Navigation:** [Previous: Chapter 6 - Stack and Heap](../chapter-06-stack-and-heap/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 8 - Building a Stack Data Structure](../chapter-08-stack-data-structure/README.md)
