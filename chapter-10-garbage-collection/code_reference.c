/**
 * Chapter 10: Garbage Collection - Code Reference
 * 
 * This file contains reference implementations of both automatic memory management models:
 * 1. Reference Counting:
 *    - Explicit ref_count field
 *    - ref_count_inc() and ref_count_dec()
 *    - Cascade deallocation on ref_count reaching 0
 *    - Demonstration of why reference counting leaks on reference cycles
 * 
 * 2. Mark-and-Sweep Garbage Collector:
 *    - Virtual Machine (VM_t) tracking all allocated objects
 *    - Root set (VM stack)
 *    - Marked flag on objects
 *    - Phase 1: Mark (roots)
 *    - Phase 2: Trace (depth-first traversal of reachable object graph)
 *    - Phase 3: Sweep (freeing unreachable objects, even cyclic ones!)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter10 && ./chapter10
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* =========================================================================
 * PART 1: Reference Counting Implementation
 * ========================================================================= */

typedef struct RefCountedNode RefCountedNode_t;

struct RefCountedNode {
    int id;
    int ref_count;
    RefCountedNode_t *next; /* Reference to another node */
};

RefCountedNode_t *rc_new_node(int id) {
    RefCountedNode_t *node = malloc(sizeof(RefCountedNode_t));
    if (node == NULL) return NULL;
    node->id = id;
    node->ref_count = 1; /* Initial reference held by creator */
    node->next = NULL;
    return node;
}

void rc_inc(RefCountedNode_t *node) {
    if (node == NULL) return;
    node->ref_count++;
}

void rc_dec(RefCountedNode_t *node) {
    if (node == NULL) return;
    node->ref_count--;

    if (node->ref_count == 0) {
        printf("     [RC Free] Node %d ref_count hit 0 -> freeing memory\n", node->id);
        if (node->next != NULL) {
            rc_dec(node->next); /* Decrement child */
        }
        free(node);
    }
}

void rc_set_next(RefCountedNode_t *parent, RefCountedNode_t *child) {
    if (parent->next != NULL) {
        rc_dec(parent->next);
    }
    parent->next = child;
    if (child != NULL) {
        rc_inc(child);
    }
}

/* =========================================================================
 * PART 2: Mark-and-Sweep Garbage Collector Implementation
 * ========================================================================= */

#define MAX_ROOTS 32

typedef struct GCObject GCObject_t;

struct GCObject {
    int id;
    bool marked;
    GCObject_t *next_in_vm;  /* Global linked list of all objects tracked by VM */
    GCObject_t *child_left;  /* Reference 1 */
    GCObject_t *child_right; /* Reference 2 */
};

typedef struct {
    GCObject_t *objects_head;          /* Head of tracked objects list */
    size_t total_objects;
    GCObject_t *roots[MAX_ROOTS];      /* Root stack (active stack frame references) */
    size_t root_count;
} VM_t;

/* Initialize VM */
VM_t *vm_new(void) {
    VM_t *vm = malloc(sizeof(VM_t));
    if (vm == NULL) return NULL;
    vm->objects_head = NULL;
    vm->total_objects = 0;
    vm->root_count = 0;
    return vm;
}

/* Push root reference onto VM stack */
void vm_push_root(VM_t *vm, GCObject_t *obj) {
    assert(vm->root_count < MAX_ROOTS);
    vm->roots[vm->root_count++] = obj;
}

/* Pop root reference */
GCObject_t *vm_pop_root(VM_t *vm) {
    assert(vm->root_count > 0);
    return vm->roots[--vm->root_count];
}

/* Allocate new GC object and register it with the VM */
GCObject_t *vm_alloc_object(VM_t *vm, int id) {
    GCObject_t *obj = malloc(sizeof(GCObject_t));
    if (obj == NULL) return NULL;

    obj->id = id;
    obj->marked = false;
    obj->child_left = NULL;
    obj->child_right = NULL;

    /* Add to VM's tracked list */
    obj->next_in_vm = vm->objects_head;
    vm->objects_head = obj;
    vm->total_objects++;

    return obj;
}

/* Trace reachable objects starting from node (DFS) */
static void trace_object(GCObject_t *obj) {
    if (obj == NULL || obj->marked) {
        return; /* Already marked or null */
    }

    obj->marked = true;
    trace_object(obj->child_left);
    trace_object(obj->child_right);
}

/* 1. Mark phase: trace all objects reachable from roots */
void gc_mark_and_trace(VM_t *vm) {
    for (size_t i = 0; i < vm->root_count; i++) {
        trace_object(vm->roots[i]);
    }
}

/* 2. Sweep phase: free unmarked objects and unmark live ones */
size_t gc_sweep(VM_t *vm) {
    size_t freed_count = 0;
    GCObject_t **curr = &vm->objects_head;

    while (*curr != NULL) {
        if (!(*curr)->marked) {
            /* Unreachable! Remove from list and free */
            GCObject_t *unreached = *curr;
            *curr = unreached->next_in_vm;

            printf("     [GC Sweep] Reclaiming unreachable Object %d\n", unreached->id);
            free(unreached);
            vm->total_objects--;
            freed_count++;
        } else {
            /* Reachable! Clear mark for the next cycle */
            (*curr)->marked = false;
            curr = &(*curr)->next_in_vm;
        }
    }
    return freed_count;
}

/* Full Garbage Collection cycle */
size_t vm_garbage_collect(VM_t *vm) {
    printf("   -> Running Mark-and-Sweep GC cycle (tracked objects: %zu, roots: %zu)...\n",
           vm->total_objects, vm->root_count);
    gc_mark_and_trace(vm);
    size_t freed = gc_sweep(vm);
    printf("   -> GC cycle complete: freed %zu objects, %zu remaining.\n", freed, vm->total_objects);
    return freed;
}

/* Free VM and all remaining objects */
void vm_free(VM_t *vm) {
    GCObject_t *curr = vm->objects_head;
    while (curr != NULL) {
        GCObject_t *next = curr->next_in_vm;
        free(curr);
        curr = next;
    }
    free(vm);
}

/* =========================================================================
 * Main Demonstration
 * ========================================================================= */

int main(void) {
    printf("=== Chapter 10: Garbage Collection Code Reference ===\n\n");

    /* ---------------------------------------------------------------------
     * 1. Reference Counting Demo
     * --------------------------------------------------------------------- */
    printf("1. Reference Counting (Normal deallocation):\n");
    RefCountedNode_t *n1 = rc_new_node(1);
    RefCountedNode_t *n2 = rc_new_node(2);
    rc_set_next(n1, n2);
    rc_dec(n2); /* n1 now uniquely owns the reference to n2 */

    printf("   Dropping root reference to Node 1:\n");
    rc_dec(n1); /* Both n1 and n2 will be freed recursively! */
    printf("   Cascade deallocation completed.\n\n");

    /* Reference counting cycle failure note */
    printf("2. Reference Counting Cycle Problem:\n");
    RefCountedNode_t *cycle_a = rc_new_node(10);
    RefCountedNode_t *cycle_b = rc_new_node(20);
    rc_set_next(cycle_a, cycle_b); /* a -> b */
    rc_set_next(cycle_b, cycle_a); /* b -> a (Cycle formed!) */
    printf("   Node 10 ref_count: %d, Node 20 ref_count: %d\n", cycle_a->ref_count, cycle_b->ref_count);
    printf("   If caller drops references, both counts remain at 1 -> LEAK!\n");
    /* Manual cycle break to avoid memory leak in test */
    cycle_b->next = NULL;
    cycle_a->ref_count--;
    rc_dec(cycle_a);
    rc_dec(cycle_b);
    printf("   (Cycle cleanly cleaned up for demo).\n\n");

    /* ---------------------------------------------------------------------
     * 3. Mark-and-Sweep GC Demo
     * --------------------------------------------------------------------- */
    printf("3. Mark-and-Sweep Garbage Collection:\n");
    VM_t *vm = vm_new();

    /* Create live objects */
    GCObject_t *root_obj = vm_alloc_object(vm, 100);
    GCObject_t *child_obj = vm_alloc_object(vm, 101);
    root_obj->child_left = child_obj;
    vm_push_root(vm, root_obj); /* Root set holds object 100 */

    /* Create a cyclic garbage graph NOT reachable from roots */
    GCObject_t *garbage_a = vm_alloc_object(vm, 201);
    GCObject_t *garbage_b = vm_alloc_object(vm, 202);
    garbage_a->child_left = garbage_b;
    garbage_b->child_right = garbage_a; /* Cycle: 201 <-> 202 */

    printf("   Created reachable graph (100 -> 101) and cyclic garbage (201 <-> 202).\n");
    vm_garbage_collect(vm); /* Reclaims 201 and 202 despite cycle! */
    printf("\n");

    /* Now drop root and run GC again */
    printf("   Dropping root reference to Object 100...\n");
    vm_pop_root(vm);
    vm_garbage_collect(vm); /* Reclaims 100 and 101! */
    printf("\n");

    vm_free(vm);
    printf("=== Chapter 10 reference executed successfully! ===\n");
    return 0;
}
