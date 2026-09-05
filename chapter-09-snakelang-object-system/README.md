# Chapter 9 - Building the SnakeLang Object System

> **Navigation:** [Previous: Chapter 8 - Building a Stack Data Structure](../chapter-08-stack-data-structure/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 10 - Garbage Collection](../chapter-10-garbage-collection/README.md)

---

## Table of Contents

- [9.1 Object Model](#91-object-model)
- [9.2 Integer Objects](#92-integer-objects)
- [9.3 Float Objects](#93-float-objects)
- [9.4 String Objects](#94-string-objects)
- [9.5 Vector3 Objects](#95-vector3-objects)
- [9.6 Dynamically Sized Arrays](#96-dynamically-sized-arrays)
- [9.7 Array `set` and `get`](#97-array-set-and-get)
- [9.8 Generic `length`](#98-generic-length)
- [9.9 Generic `add`](#99-generic-add)

---

## 9.1 Object Model

SnakeLang needs one unified representation for different kinds of values.

The object system uses:

1. an enum describing the kind
2. a union storing the data
3. a struct combining both

A simplified first version:

```c
typedef enum {
    INTEGER,
    FLOAT,
    STRING,
    VECTOR_3,
    ARRAY
} SnakeObjectKind_t;
```

Data:

```c
typedef union {
    int v_int;
    float v_float;
    char *v_string;
} SnakeObjectData_t;
```

Object:

```c
typedef struct {
    SnakeObjectKind_t kind;
    SnakeObjectData_t data;
} SnakeObject_t;
```

---

## 9.2 Integer Objects

Constructor:

```c
#include <stdlib.h>

SnakeObject_t *new_snake_integer(int value) {
    SnakeObject_t *object = malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->kind = INTEGER;
    object->data.v_int = value;

    return object;
}
```

Example:

```c
SnakeObject_t *object = new_snake_integer(42);
```

---

## 9.3 Float Objects

Add a float kind and data field:

```c
typedef enum {
    INTEGER,
    FLOAT
} SnakeObjectKind_t;

typedef union {
    int v_int;
    float v_float;
} SnakeObjectData_t;
```

Constructor:

```c
SnakeObject_t *new_snake_float(float value) {
    SnakeObject_t *object = malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->kind = FLOAT;
    object->data.v_float = value;

    return object;
}
```

---

## 9.4 String Objects

Strings require two allocations:

1. the object itself
2. the string buffer

```c
#include <stdlib.h>
#include <string.h>

SnakeObject_t *new_snake_string(const char *value) {
    SnakeObject_t *object = malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->kind = STRING;

    object->data.v_string = malloc(strlen(value) + 1);

    if (object->data.v_string == NULL) {
        free(object);
        return NULL;
    }

    strcpy(object->data.v_string, value);

    return object;
}
```

The extra `+ 1` is required for `'\0'`.

---

## 9.5 Vector3 Objects

A container object can store references to other SnakeLang objects.

Definition:

```c
typedef struct {
    SnakeObject_t *x;
    SnakeObject_t *y;
    SnakeObject_t *z;
} SnakeVector3_t;
```

Add it to the union:

```c
typedef union {
    int v_int;
    float v_float;
    char *v_string;
    SnakeVector3_t v_vector3;
} SnakeObjectData_t;
```

Constructor:

```c
SnakeObject_t *new_snake_vector3(
    SnakeObject_t *x,
    SnakeObject_t *y,
    SnakeObject_t *z
) {
    if (x == NULL || y == NULL || z == NULL) {
        return NULL;
    }

    SnakeObject_t *object = malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->kind = VECTOR_3;
    object->data.v_vector3.x = x;
    object->data.v_vector3.y = y;
    object->data.v_vector3.z = z;

    return object;
}
```

---

## 9.6 Dynamically Sized Arrays

Define the array structure:

```c
typedef struct {
    size_t size;
    SnakeObject_t **elements;
} SnakeArray_t;
```

Add it to the union:

```c
typedef union {
    int v_int;
    float v_float;
    char *v_string;
    SnakeVector3_t v_vector3;
    SnakeArray_t v_array;
} SnakeObjectData_t;
```

Constructor:

```c
SnakeObject_t *new_snake_array(size_t size) {
    SnakeObject_t *object = malloc(sizeof(SnakeObject_t));

    if (object == NULL) {
        return NULL;
    }

    object->kind = ARRAY;

    object->data.v_array.size = size;

    object->data.v_array.elements =
        calloc(size, sizeof(SnakeObject_t *));

    if (object->data.v_array.elements == NULL) {
        free(object);
        return NULL;
    }

    return object;
}
```

---

## 9.7 Array `set` and `get`

### Set

```c
#include <stdbool.h>

bool snake_array_set(
    SnakeObject_t *object,
    size_t index,
    SnakeObject_t *value
) {
    if (object == NULL || value == NULL) {
        return false;
    }

    if (object->kind != ARRAY) {
        return false;
    }

    if (index >= object->data.v_array.size) {
        return false;
    }

    object->data.v_array.elements[index] = value;

    return true;
}
```

### Get

```c
SnakeObject_t *snake_array_get(
    SnakeObject_t *object,
    size_t index
) {
    if (object == NULL) {
        return NULL;
    }

    if (object->kind != ARRAY) {
        return NULL;
    }

    if (index >= object->data.v_array.size) {
        return NULL;
    }

    return object->data.v_array.elements[index];
}
```

---

## 9.8 Generic `length`

A dynamic runtime can implement a generic `length` operation by inspecting an object's kind.

```c
int snake_length(SnakeObject_t *object) {
    if (object == NULL) {
        return -1;
    }

    switch (object->kind) {
        case INTEGER:
            return 1;

        case FLOAT:
            return 1;

        case STRING:
            return (int)strlen(object->data.v_string);

        case VECTOR_3:
            return 3;

        case ARRAY:
            return (int)object->data.v_array.size;

        default:
            return -1;
    }
}
```

This is analogous to dynamic dispatch based on runtime type information.

---

## 9.9 Generic `add`

The runtime can dispatch `add` based on the two operand kinds.

### Integer + integer

```c
return new_snake_integer(
    a->data.v_int +
    b->data.v_int
);
```

### Integer + float

```c
return new_snake_float(
    (float)a->data.v_int +
    b->data.v_float
);
```

### Float + integer

```c
return new_snake_float(
    a->data.v_float +
    (float)b->data.v_int
);
```

### Float + float

```c
return new_snake_float(
    a->data.v_float +
    b->data.v_float
);
```

### String + string

A complete reconstructed pattern:

```c
SnakeObject_t *snake_add(
    SnakeObject_t *a,
    SnakeObject_t *b
) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    switch (a->kind) {
        case INTEGER:
            switch (b->kind) {
                case INTEGER:
                    return new_snake_integer(
                        a->data.v_int +
                        b->data.v_int
                    );

                case FLOAT:
                    return new_snake_float(
                        (float)a->data.v_int +
                        b->data.v_float
                    );

                default:
                    return NULL;
            }

        case FLOAT:
            switch (b->kind) {
                case INTEGER:
                    return new_snake_float(
                        a->data.v_float +
                        (float)b->data.v_int
                    );

                case FLOAT:
                    return new_snake_float(
                        a->data.v_float +
                        b->data.v_float
                    );

                default:
                    return NULL;
            }

        case STRING: {
            if (b->kind != STRING) {
                return NULL;
            }

            size_t length =
                strlen(a->data.v_string) +
                strlen(b->data.v_string) +
                1;

            char *new_string = malloc(length);

            if (new_string == NULL) {
                return NULL;
            }

            new_string[0] = '\0';
            strcat(new_string, a->data.v_string);
            strcat(new_string, b->data.v_string);

            SnakeObject_t *object =
                new_snake_string(new_string);

            free(new_string);

            return object;
        }

        default:
            return NULL;
    }
}
```

### Vector + vector

Recursively reuse the same `snake_add` logic for each component:

```c
return new_snake_vector3(
    snake_add(a->data.v_vector3.x, b->data.v_vector3.x),
    snake_add(a->data.v_vector3.y, b->data.v_vector3.y),
    snake_add(a->data.v_vector3.z, b->data.v_vector3.z)
);
```

### Array + array

The pattern:

1. allocate an array large enough for both
2. copy all elements from the first array
3. append all elements from the second array
4. return the new array object

Conceptually:

```c
size_t size =
    a->data.v_array.size +
    b->data.v_array.size;

SnakeObject_t *result = new_snake_array(size);

for (size_t i = 0; i < a->data.v_array.size; i++) {
    snake_array_set(
        result,
        i,
        snake_array_get(a, i)
    );
}

for (size_t i = 0; i < b->data.v_array.size; i++) {
    snake_array_set(
        result,
        i + a->data.v_array.size,
        snake_array_get(b, i)
    );
}
```

---

> **Navigation:** [Previous: Chapter 8 - Building a Stack Data Structure](../chapter-08-stack-data-structure/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 10 - Garbage Collection](../chapter-10-garbage-collection/README.md)
