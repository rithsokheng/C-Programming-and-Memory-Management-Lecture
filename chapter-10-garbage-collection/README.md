# Chapter 10 - Garbage Collection

> **Navigation:** [Previous: Chapter 9 - Building the SnakeLang Object System](../chapter-09-snakelang-object-system/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c)

---

## Table of Contents

- [10.1 Automatic Memory Management](#101-automatic-memory-management)
- [10.2 Reference Counting](#102-reference-counting)
- [10.3 Reference Count Increment](#103-reference-count-increment)
- [10.4 Reference Count Decrement](#104-reference-count-decrement)
- [10.5 Freeing Reference-Counted Objects](#105-freeing-reference-counted-objects)
- [10.6 Reference Counting Container Objects](#106-reference-counting-container-objects)
- [10.7 Why Reference Counting Fails on Cycles](#107-why-reference-counting-fails-on-cycles)
- [10.8 Mark-and-Sweep](#108-mark-and-sweep)
- [10.9 The Virtual Machine](#109-the-virtual-machine)
- [10.10 Tracking Objects](#1010-tracking-objects)
- [10.11 Marking Root Objects](#1011-marking-root-objects)
- [10.12 Tracing Reachable Objects](#1012-tracing-reachable-objects)
- [10.13 Sweeping Unmarked Objects](#1013-sweeping-unmarked-objects)
- [10.14 Running a Full GC Cycle](#1014-running-a-full-gc-cycle)

---

## 10.1 Automatic Memory Management

Manual memory management requires the programmer to know when memory is no longer needed.

Garbage collection shifts this responsibility to the runtime.

Three broad memory management models:

### Manual management

```text
programmer
   |
   +--> allocate
   |
   +--> use
   |
   +--> free
```

### Rust-style ownership/borrow checking

The compiler helps determine whether values may still be used.

### Garbage collection

The runtime determines whether objects are still reachable/live.

---

## 10.2 Reference Counting

Reference counting adds a counter to every object.

Example:

```c
typedef struct {
    SnakeObjectKind_t kind;
    SnakeObjectData_t data;
    int ref_count;
} SnakeObject_t;
```

Basic idea:

```text
object
 |
 +-- ref_count = 1
```

When another reference is created:

```text
ref_count++
```

When a reference is removed:

```text
ref_count--
```

When it reaches zero:

```text
free(object)
```

---

## 10.3 Reference Count Increment

When creating a new object:

```c
SnakeObject_t *snake_new_object(void) {
    SnakeObject_t *object =
        malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->ref_count = 1;

    return object;
}
```

Increment:

```c
void ref_count_inc(SnakeObject_t *object) {
    if (object == NULL) {
        return;
    }

    object->ref_count++;
}
```

---

## 10.4 Reference Count Decrement

```c
void ref_count_dec(SnakeObject_t *object) {
    if (object == NULL) {
        return;
    }

    object->ref_count--;

    if (object->ref_count == 0) {
        ref_count_free(object);
    }
}
```

The key transition is:

```text
ref_count > 0
     |
     | decrement
     v
ref_count == 0
     |
     v
free object
```

---

## 10.5 Freeing Reference-Counted Objects

Different object kinds may have different cleanup requirements.

```c
void ref_count_free(SnakeObject_t *object) {
    if (object == NULL) {
        return;
    }

    switch (object->kind) {
        case INTEGER:
            free(object);
            break;

        case FLOAT:
            free(object);
            break;

        case STRING:
            free(object->data.v_string);
            free(object);
            break;

        default:
            free(object);
            break;
    }
}
```

The important principle is:

> Free nested allocations before freeing the outer object that owns them.

For strings:

```text
SnakeObject
    |
    +--> char buffer
```

Therefore:

```text
free(buffer)
free(object)
```

---

## 10.6 Reference Counting Container Objects

### Vector3

If a `Vector3` object stores references to three objects:

```text
Vector3
  |
  +--> X
  +--> Y
  +--> Z
```

Creating the vector means incrementing all three:

```c
ref_count_inc(x);
ref_count_inc(y);
ref_count_inc(z);
```

Freeing the vector means decrementing all three:

```c
ref_count_dec(object->data.v_vector3.x);
ref_count_dec(object->data.v_vector3.y);
ref_count_dec(object->data.v_vector3.z);
```

### Arrays

When replacing an existing array element:

```c
ref_count_inc(new_value);

if (array->elements[index] != NULL) {
    ref_count_dec(array->elements[index]);
}

array->elements[index] = new_value;
```

When destroying the array:

```c
for (size_t i = 0; i < array->size; i++) {
    ref_count_dec(array->elements[i]);
}

free(array->elements);
```

This keeps the counts consistent with the references stored by containers.

---

## 10.7 Why Reference Counting Fails on Cycles

Consider:

```text
A -> B
^    |
|    v
+----+
```

Suppose:

```text
A.ref_count = 1
B.ref_count = 1
```

Even when no outside variable refers to either object, they still reference one another.

Therefore:

```text
A.ref_count != 0
B.ref_count != 0
```

and naive reference counting will not collect them.

This creates a cycle leak.

---

## 10.8 Mark-and-Sweep

Mark-and-sweep solves the cycle problem by asking:

> Which objects are reachable from live roots?

The process has two major phases:

### Mark

Start from root objects and mark all reachable objects.

### Sweep

Find objects that were not marked and free them.

Conceptual graph:

```text
Roots
  |
  +--> A --> B
  |
  +--> C

Unreachable:
D --> E
```

Marked:

```text
A, B, C
```

Swept:

```text
D, E
```

---

## 10.9 The Virtual Machine

The course introduces a small virtual-machine structure.

Conceptually:

```c
typedef struct {
    Stack_t *references;
} Frame_t;

typedef struct {
    Stack_t *frames;
    Stack_t *objects;
} VM_t;
```

### Meaning

- `frames` stores active stack frames
- `objects` stores all allocated SnakeLang objects
- each frame stores references to objects

This gives the garbage collector a place to discover root references.

---

## 10.10 Tracking Objects

Every object is registered with the VM.

Conceptual function:

```c
void vm_track_object(VM_t *vm, SnakeObject_t *object) {
    if (vm == NULL || object == NULL) {
        return;
    }

    stack_push(vm->objects, object);
}
```

The reason is simple:

> The collector must know about every allocated object so it can later determine which ones remain reachable.

---

## 10.11 Marking Root Objects

Replace the reference count field with a mark bit:

```c
typedef struct {
    SnakeObjectKind_t kind;
    SnakeObjectData_t data;
    bool marked;
} SnakeObject_t;
```

New objects begin unmarked:

```c
object->marked = false;
```

A root is an object directly referenced by an active stack frame.

Mark roots:

```c
void mark(VM_t *vm) {
    if (vm == NULL) {
        return;
    }

    for (size_t i = 0; i < vm->frames->count; i++) {
        Frame_t *frame = vm->frames->data[i];

        for (size_t j = 0; j < frame->references->count; j++) {
            SnakeObject_t *object =
                frame->references->data[j];

            object->marked = true;
        }
    }
}
```

---

## 10.12 Tracing Reachable Objects

Marking roots is not enough.

If:

```text
root --> vector --> integer
```

the integer must also be marked.

So the collector recursively follows container references.

### Vector3

```c
void trace_object(SnakeObject_t *object) {
    if (object == NULL || object->marked) {
        return;
    }

    object->marked = true;

    switch (object->kind) {
        case INTEGER:
            break;

        case FLOAT:
            break;

        case STRING:
            break;

        case VECTOR_3:
            trace_object(object->data.v_vector3.x);
            trace_object(object->data.v_vector3.y);
            trace_object(object->data.v_vector3.z);
            break;

        default:
            break;
    }
}
```

### Arrays

For arrays, iterate over all elements:

```c
for (size_t i = 0; i < object->data.v_array.size; i++) {
    trace_object(
        object->data.v_array.elements[i]
    );
}
```

---

## 10.13 Sweeping Unmarked Objects

After the mark/trace phase:

```text
marked   -> alive
unmarked -> garbage
```

Sweep:

```c
void sweep(VM_t *vm) {
    if (vm == NULL) {
        return;
    }

    for (size_t i = 0; i < vm->objects->count; i++) {
        SnakeObject_t *object =
            vm->objects->data[i];

        if (object->marked) {
            object->marked = false;
        } else {
            snake_object_free(object);
            vm->objects->data[i] = NULL;
        }
    }

    /* compact object stack here */
}
```

### Important detail

Marked objects are reset to `false` for the next GC cycle.

Unmarked objects are freed.

Null entries can then be removed by compacting the object list.

---

## 10.14 Running a Full GC Cycle

The final collector pipeline is:

```c
void garbage_collect(VM_t *vm) {
    mark(vm);
    trace(vm);
    sweep(vm);
}
```

Conceptually:

```text
                Garbage Collection
                        |
                +-------+-------+
                |               |
              MARK            TRACE
                |               |
                +-------+-------+
                        |
                      SWEEP
                        |
                        v
                  Free garbage
```

---

> **Navigation:** [Previous: Chapter 9 - Building the SnakeLang Object System](../chapter-09-snakelang-object-system/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c)
