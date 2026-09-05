# C Programming and Memory Management

> A structured, GitHub-ready course repository based on the comprehensive lecture **“C Programming and Memory Management - Full Course”**.
> Each chapter has its own folder containing complete lecture notes and a standalone, compilable C code reference file.


## Course Overview

The course uses C as a practical way to understand how programs interact with memory. The main progression is:

- C fundamentals
- `struct`
- pointers
- arrays
- enums
- unions
- stack and heap memory
- dynamic allocation
- advanced pointers
- stack data structures
- a small object system
- reference-counting garbage collection
- mark-and-sweep garbage collection

The project throughout the course is **SnakeLang**, a fictional language used as a vehicle for implementing a small object system and memory-management algorithms.

The goal is not simply to learn C syntax, but to build a solid mental model of how values, addresses, memory regions, object lifetimes, and garbage collectors work.

## Learning Prerequisites

You should already understand basic programming ideas such as:

- variables
- functions
- return values
- loops
- basic control flow

No previous C experience is required.


## Course Map & Mental Model

```text
C Syntax & Basics
       │
       ▼
Structs & Memory Layout
       │
       ▼
Addresses, Pointers & Arrays
       │
       ▼
Enums & Unions (Tagged Unions)
       │
       ▼
Stack Frames vs. Dynamic Heap (malloc / free)
       │
       ▼
Advanced Pointers (Double Pointers, void *, memcpy)
       │
       ▼
Generic Data Structures (Dynamic Stack)
       │
       ▼
SnakeLang Tagged Object System
       │
       ▼
Automatic Memory Management (Reference Counting & Mark-and-Sweep GC)
```


## Chapters & Curriculum

| Chapter | Notes | Code Reference | Core Topics Covered |
|:---|:---:|:---:|:---|
| **01. C Basics** | [Lecture Notes](chapter-01-c-basics/README.md) | [code_reference.c](chapter-01-c-basics/code_reference.c) | Hello World, compilation model, variables, data types, `const`, functions, casting, conditionals, loops, `sizeof`, header guards |
| **02. Structs** | [Lecture Notes](chapter-02-structs/README.md) | [code_reference.c](chapter-02-structs/code_reference.c) | Struct definitions, positional vs designated initializers, pass-by-value, `typedef`, memory alignment, padding, `offsetof` |
| **03. Pointers and Arrays** | [Lecture Notes](chapter-03-pointers-and-arrays/README.md) | [code_reference.c](chapter-03-pointers-and-arrays/code_reference.c) | Address operator `&`, dereferencing `*`, arrow `->`, pointer arithmetic, array decay, 2D arrays, C strings, concatenation |
| **04. Enums** | [Lecture Notes](chapter-04-enums/README.md) | [code_reference.c](chapter-04-enums/code_reference.c) | Enumeration types, implicit vs explicit values, `switch` pattern matching with enums, enum sizes in memory |
| **05. Unions** | [Lecture Notes](chapter-05-unions/README.md) | [code_reference.c](chapter-05-unions/code_reference.c) | Shared memory semantics, overlapping fields, tagged unions / discriminated unions, network packet header parsing |
| **06. Stack and Heap** | [Lecture Notes](chapter-06-stack-and-heap/README.md) | [code_reference.c](chapter-06-stack-and-heap/code_reference.c) | Stack frames, stack overflow, dangling pointers, heap allocation (`malloc`, `calloc`, `realloc`, `free`), memory leaks, endianness |
| **07. Advanced Pointers** | [Lecture Notes](chapter-07-advanced-pointers/README.md) | [code_reference.c](chapter-07-advanced-pointers/code_reference.c) | Pointer-to-pointer (`**`), allocating through double pointers, arrays of pointers, generic `void *` pointers, generic swap with `memcpy` |
| **08. Building a Stack Data Structure** | [Lecture Notes](chapter-08-stack-data-structure/README.md) | [code_reference.c](chapter-08-stack-data-structure/code_reference.c) | Dynamic generic Stack implementation, geometric resizing (2x), `stack_new`, `stack_push`, `stack_pop`, `stack_free`, `void **` trade-offs |
| **09. SnakeLang Object System** | [Lecture Notes](chapter-09-snakelang-object-system/README.md) | [code_reference.c](chapter-09-snakelang-object-system/code_reference.c) | Tagged object model, integer/float/string/Vector3 objects, dynamic object arrays, polymorphic length and add operations |
| **10. Garbage Collection** | [Lecture Notes](chapter-10-garbage-collection/README.md) | [code_reference.c](chapter-10-garbage-collection/code_reference.c) | Automatic memory management, Reference Counting, cycle failure, Mark-and-Sweep GC, VM root set, tracing, sweeping |


## How to Run the Code References

Every chapter includes a standalone, fully compilable C code reference file tested with `gcc` (with `-Wall -Wextra -pedantic`).

To compile and run any chapter's code:

```bash
# Example: Compile and run Chapter 1 (C Basics)
gcc -Wall -Wextra -pedantic chapter-01-c-basics/code_reference.c -o chapter01
./chapter01

# Example: Compile and run Chapter 8 (Dynamic Stack Data Structure)
gcc -Wall -Wextra -pedantic chapter-08-stack-data-structure/code_reference.c -o chapter08
./chapter08

# Example: Compile and run Chapter 10 (Garbage Collection & Mark-and-Sweep)
gcc -Wall -Wextra -pedantic chapter-10-garbage-collection/code_reference.c -o chapter10
./chapter10
```

To run all chapter code references in sequence:

```bash
for dir in chapter-*; do
    echo "=== Running $dir ==="
    gcc -Wall -Wextra -pedantic "$dir/code_reference.c" -o "$dir/runner"
    "./$dir/runner"
    rm "$dir/runner"
    echo ""
done
```

---

# Key Takeaways

## 1. Everything eventually becomes memory

At the lowest practical level, program data occupies bytes in memory.


## 2. Variables are names; pointers are addresses

```c
int x = 42;
int *p = &x;
```

- `x` gives a value
- `&x` gives its address
- `p` stores that address
- `*p` accesses the value at that address


## 3. C is strongly tied to data layout

Struct order, padding, array layout, pointer arithmetic, and unions all expose details that higher-level languages often hide.


## 4. Stack memory has predictable function-scoped lifetimes

Good for:

- local variables
- function parameters
- fixed-size temporary data


## 5. Heap memory gives dynamic lifetime and size

Good for:

- runtime-sized collections
- objects that must survive beyond a function
- dynamic data structures

But the programmer must manage ownership.


## 6. `malloc` and `free` form a basic manual-management pair

```c
int *p = malloc(sizeof(int));

if (p != NULL) {
    *p = 42;
}

free(p);
```

Every allocation should have a clear ownership story.


## 7. C strings are null-terminated

Remember:

```text
"hello"
```

is stored conceptually as:

```text
h e l l o \0
```


## 8. `struct` groups data

```c
typedef struct {
    int x;
    int y;
} Point_t;
```


## 9. `enum + union + struct` is a powerful runtime representation

The pattern:

```c
typedef enum {
    INTEGER,
    STRING
} Kind;

typedef union {
    int v_int;
    char *v_string;
} Data;

typedef struct {
    Kind kind;
    Data data;
} Object;
```

is the foundation of many dynamically typed runtime designs.


## 10. Reference counting is simple but cannot detect cycles

```text
A -> B
^    |
|    v
+----+
```

The objects can keep each other's counts above zero forever.


## 11. Mark-and-sweep uses reachability

Start from roots.

Mark everything reachable.

Sweep everything else.


## 12. Garbage collection is not free

Automatic memory management moves complexity rather than eliminating it.

The runtime still performs work to determine which objects are live.

---

# Common Memory-Management Bugs

## Memory leak

```text
allocate
   |
   v
lose pointer
   |
   v
memory can no longer be released
```


## Use-after-free

```text
allocate
   |
   v
free
   |
   v
use pointer again  <-- BUG
```


## Returning a pointer to a local variable

```c
int *bad(void) {
    int x = 42;
    return &x;
}
```

The pointed-to object no longer has a valid lifetime after the function returns.


## Buffer overflow

Writing more characters than a destination buffer can hold:

```c
char buffer[8];

strcpy(buffer, "this string is too long");
```

Always ensure that string buffer capacity is strictly respected.


## Invalid array access

```c
int values[5];

values[5] = 10;  // out of bounds
```

Valid indexes are:

```text
0 1 2 3 4
```


## Misusing `void *`

A generic pointer can hide type information:

```c
void *data = ...;
```

Casting it incorrectly can make the program interpret bytes as the wrong type.


## Reading an inactive union member

A union's fields share storage.

Do not assume every field contains a valid value simultaneously.

---

# Final Mental Model

The whole course can be reduced to one chain of ideas:

```text
C syntax
   |
   v
Values
   |
   v
Memory
   |
   v
Addresses
   |
   v
Pointers
   |
   v
Dynamic allocation
   |
   v
Data structures
   |
   v
Objects
   |
   v
Object references
   |
   v
Garbage collection
```

Or, in one sentence:

> **To understand memory management, understand where data lives, how it is addressed, how long it remains valid, what references keep it alive, and what mechanism eventually releases it.**
