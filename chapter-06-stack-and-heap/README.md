# Chapter 6 - Stack and Heap

> **Navigation:** [Previous: Chapter 5 - Unions](../chapter-05-unions/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 7 - Advanced Pointers](../chapter-07-advanced-pointers/README.md)

---

## Table of Contents

- [6.1 The Stack](#61-the-stack)
- [6.2 Stack Frames](#62-stack-frames)
- [6.3 Why the Stack Is Fast](#63-why-the-stack-is-fast)
- [6.4 Stack Overflow](#64-stack-overflow)
- [6.5 Dangling Pointers to Stack Data](#65-dangling-pointers-to-stack-data)
- [6.6 The Heap](#66-the-heap)
- [6.7 `malloc`](#67-malloc)
- [6.8 `calloc`](#68-calloc)
- [6.9 `free`](#69-free)
- [6.10 Memory Leaks and Use-After-Free](#610-memory-leaks-and-use-after-free)
- [6.11 Endianness](#611-endianness)

---

## 6.1 The Stack

The stack is a region used for short-lived function-related data.

A useful mental model:

```text
Higher memory
+------------------+
| caller frame     |
+------------------+
| current frame    |
+------------------+
| callee frame     |
+------------------+
Lower memory
```

---

## 6.2 Stack Frames

When a function is called, a stack frame is created.

A simplified frame can contain:

- function arguments
- local variables
- return information
- other implementation-specific state

Example:

```c
void create_typist(int uses_neovim) {
    int words_per_minute = 100;
    char name[] = "TJ";
}
```

When the function returns, the stack space associated with the frame becomes available again.

---

## 6.3 Why the Stack Is Fast

The lecture identifies several reasons the stack is attractive:

- allocation/deallocation is simple
- memory is contiguous
- data is often close together
- function-local lifetime makes management easy
- no explicit `free` is needed for ordinary local variables

Conceptually, “allocation” can be thought of as moving a stack pointer.

---

## 6.4 Stack Overflow

Because stack space is limited, excessive recursion or large stack allocations can overflow the stack.

Typical source:

```c
void recurse(void) {
    recurse();
}
```

A recursive function with no terminating case can continue creating stack frames until stack space is exhausted.

---

## 6.5 Dangling Pointers to Stack Data

A dangerous mistake is returning a pointer to a local variable.

Bad:

```c
struct Coordinate *make_coordinate(int x, int y) {
    struct Coordinate c = {
        .x = x,
        .y = y
    };

    return &c;  // BUG
}
```

Why?

`c` lives in the function's stack frame.

When the function returns, that stack frame is no longer valid for this object.

A pointer returned from the function can therefore point into invalid/reusable stack memory.

Safer alternatives include returning the struct by value:

```c
struct Coordinate make_coordinate(int x, int y) {
    return (struct Coordinate){
        .x = x,
        .y = y
    };
}
```

or allocating long-lived storage dynamically when appropriate.

---

## 6.6 The Heap

The heap is used when:

- the lifetime must outlive the creating function
- the size is only known at runtime
- dynamic data structures are needed

Example use cases:

- dynamic arrays
- trees
- linked structures
- objects managed by a runtime

---

## 6.7 `malloc`

`malloc` dynamically allocates a contiguous block of memory.

Example:

```c
int *numbers = malloc(5 * sizeof(int));

if (numbers == NULL) {
    return 1;
}

for (int i = 0; i < 5; i++) {
    numbers[i] = i * 10;
}

free(numbers);
```

### Key properties

- memory is allocated dynamically
- returned memory is uninitialized
- the caller is responsible for eventually calling `free`
- `NULL` indicates allocation failure

---

## 6.8 `calloc`

`calloc` allocates memory and initializes it to zero.

Example:

```c
int *numbers = calloc(5, sizeof(int));

if (numbers == NULL) {
    return 1;
}

free(numbers);
```

This is useful when you want freshly allocated elements to begin at zero.

---

## 6.9 `free`

`free` releases dynamically allocated memory.

```c
int *numbers = malloc(10 * sizeof(int));

/* use numbers */

free(numbers);
```

After `free`, do not continue using the memory.

Bad:

```c
free(numbers);
printf("%d\n", numbers[0]);  // invalid
```

Freeing memory does not automatically make the pointer variable become `NULL`.

A common cleanup pattern is:

```c
free(numbers);
numbers = NULL;
```

when keeping the variable around.

---

## 6.10 Memory Leaks and Use-After-Free

### Memory leak

Memory is allocated and never released.

```c
void leak(void) {
    int *p = malloc(100 * sizeof(int));

    /* forgot: free(p); */
}
```

Repeated leaks can consume available memory.

### Use-after-free

Memory is accessed after it has been freed.

```c
int *p = malloc(sizeof(int));

*p = 42;

free(p);

printf("%d\n", *p);  // invalid
```

---

## 6.11 Endianness

**Endianness** determines byte order for multi-byte values.

Suppose:

```text
0xA1B2C3D4
```

### Big-endian

Most significant byte first:

```text
A1 B2 C3 D4
```

### Little-endian

Least significant byte first:

```text
D4 C3 B2 A1
```

This usually does not matter for ordinary local arithmetic, but it becomes important when working with:

- binary files
- network protocols
- serialization
- cross-platform binary formats

---

> **Navigation:** [Previous: Chapter 5 - Unions](../chapter-05-unions/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 7 - Advanced Pointers](../chapter-07-advanced-pointers/README.md)
