# Chapter 8 - Building a Stack Data Structure

> **Navigation:** [Previous: Chapter 7 - Advanced Pointers](../chapter-07-advanced-pointers/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 9 - Building the SnakeLang Object System](../chapter-09-snakelang-object-system/README.md)

---

## Table of Contents

- [8.1 Stack Representation](#81-stack-representation)
- [8.2 `stack_new`](#82-stack_new)
- [8.3 `stack_push`](#83-stack_push)
- [8.4 `stack_pop`](#84-stack_pop)
- [8.5 `stack_free`](#85-stack_free)
- [8.6 The `void **` Trade-off](#86-the-void--trade-off)

---

## 8.1 Stack Representation

The course next builds a stack abstraction using dynamic memory and generic pointers.

A conceptual definition:

```c
typedef struct Stack {
    size_t count;
    size_t capacity;
    void **data;
} Stack_t;
```

Meaning:

- `count` = number of elements currently stored
- `capacity` = number of pointer slots allocated
- `data` = array of generic object pointers

---

## 8.2 `stack_new`

```c
#include <stdlib.h>

Stack_t *stack_new(size_t capacity) {
    Stack_t *stack = malloc(sizeof(Stack_t));

    if (stack == NULL) {
        return NULL;
    }

    stack->count = 0;
    stack->capacity = capacity;

    stack->data = malloc(sizeof(void *) * capacity);

    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }

    return stack;
}
```

This:

1. allocates the stack object
2. initializes count to zero
3. stores the initial capacity
4. allocates space for the pointer array
5. cleans up properly if the second allocation fails

---

## 8.3 `stack_push`

When the stack is full, double its capacity:

A reconstructed implementation:

```c
#include <stdlib.h>

void stack_push(Stack_t *stack, void *object) {
    if (stack == NULL) {
        return;
    }

    if (stack->count == stack->capacity) {
        size_t new_capacity = stack->capacity * 2;

        void **new_data = realloc(
            stack->data,
            sizeof(void *) * new_capacity
        );

        if (new_data == NULL) {
            return;
        }

        stack->data = new_data;
        stack->capacity = new_capacity;
    }

    stack->data[stack->count] = object;
    stack->count++;
}
```

### Growth strategy

```text
capacity:
8 -> 16 -> 32 -> 64 -> ...
```

This avoids reallocating on every insertion.

---

## 8.4 `stack_pop`

Pop removes the most recently pushed item.

```c
void *stack_pop(Stack_t *stack) {
    if (stack == NULL || stack->count == 0) {
        return NULL;
    }

    stack->count--;

    return stack->data[stack->count];
}
```

This uses last-in, first-out behavior.

Example:

```text
push(A)
push(B)
push(C)

pop() -> C
pop() -> B
pop() -> A
```

---

## 8.5 `stack_free`

A cleanup function:

```c
void stack_free(Stack_t *stack) {
    if (stack == NULL) {
        return;
    }

    if (stack->data != NULL) {
        free(stack->data);
    }

    free(stack);
}
```

This frees the stack's own allocation and the pointer array.

It does **not** automatically free the objects referenced by `data`.

Ownership of those objects must be defined separately.

---

## 8.6 The `void **` Trade-off

Because the stack stores `void *`, it can hold different kinds of pointers.

Example:

```c
int *ip = malloc(sizeof(int));
float *fp = malloc(sizeof(float));
char *sp = malloc(10);
```

All can be placed into a `void **`-based container.

However, the compiler cannot automatically protect you from confusing:

```text
integer value
```

with:

```text
pointer to integer
```

---

> **Navigation:** [Previous: Chapter 7 - Advanced Pointers](../chapter-07-advanced-pointers/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 9 - Building the SnakeLang Object System](../chapter-09-snakelang-object-system/README.md)
