/**
 * Chapter 8: Building a Stack Data Structure - Code Reference
 * 
 * This file contains a complete, generic, dynamically-resizing Stack data structure:
 * - Stack_t struct with void ** data array
 * - stack_new(size_t capacity)
 * - stack_push(Stack_t *stack, void *object) with geometric resizing (2x)
 * - stack_pop(Stack_t *stack)
 * - stack_free(Stack_t *stack)
 * - Discussion of the void ** trade-offs (flexibility vs type safety)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter08 && ./chapter08
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* --- 1. Stack Definition --- */

typedef struct {
    size_t count;
    size_t capacity;
    void **data;
} Stack_t;

/* --- 2. Stack Operations --- */

/* Allocate and initialize a new Stack */
Stack_t *stack_new(size_t capacity) {
    if (capacity == 0) {
        capacity = 4; /* Default fallback */
    }

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

/* Push an item onto the stack, doubling capacity if full */
bool stack_push(Stack_t *stack, void *object) {
    if (stack == NULL) {
        return false;
    }

    if (stack->count == stack->capacity) {
        size_t new_capacity = stack->capacity * 2;
        void **new_data = realloc(stack->data, sizeof(void *) * new_capacity);

        if (new_data == NULL) {
            return false; /* Allocation failure; original stack preserved */
        }

        stack->data = new_data;
        stack->capacity = new_capacity;
    }

    stack->data[stack->count] = object;
    stack->count++;
    return true;
}

/* Pop the top item from the stack, or NULL if empty */
void *stack_pop(Stack_t *stack) {
    if (stack == NULL || stack->count == 0) {
        return NULL;
    }

    stack->count--;
    return stack->data[stack->count];
}

/* Free stack internals and stack struct itself */
void stack_free(Stack_t *stack) {
    if (stack == NULL) {
        return;
    }

    free(stack->data);
    free(stack);
}

/* --- 3. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 8: Stack Data Structure Code Reference ===\n\n");

    /* 8.2 Create stack with small initial capacity to test resizing */
    size_t initial_cap = 2;
    printf("1. Creating Stack with initial capacity = %zu:\n", initial_cap);
    Stack_t *s = stack_new(initial_cap);
    assert(s != NULL);
    printf("   Stack created at %p (count: %zu, capacity: %zu)\n\n",
           (void *)s, s->count, s->capacity);

    /* 8.3 Push items and observe geometric growth */
    printf("2. Pushing Items (triggering geometric resizing):\n");
    const char *items[] = {
        "First (index 0)",
        "Second (index 1)",
        "Third (index 2 - triggered resize to 4)",
        "Fourth (index 3)",
        "Fifth (index 4 - triggered resize to 8)"
    };

    for (size_t i = 0; i < 5; i++) {
        stack_push(s, (void *)items[i]);
        printf("   Pushed: \"%s\" -> count: %zu, capacity: %zu\n",
               items[i], s->count, s->capacity);
    }
    printf("\n");

    /* 8.4 Pop items (LIFO order) */
    printf("3. Popping Items (Last-In, First-Out):\n");
    while (s->count > 0) {
        const char *popped = (const char *)stack_pop(s);
        printf("   Popped: \"%s\" (remaining count: %zu)\n", popped, s->count);
    }

    /* Pop on empty stack returns NULL */
    void *empty_pop = stack_pop(s);
    printf("   Popping from empty stack returns: %p\n\n", empty_pop);
    assert(empty_pop == NULL);

    /* 8.5 Free the stack */
    printf("4. Freeing Stack:\n");
    stack_free(s);
    s = NULL;
    printf("   Stack freed cleanly.\n\n");

    printf("=== Chapter 8 reference executed successfully! ===\n");
    return 0;
}
