/**
 * Chapter 9: Building the SnakeLang Object System - Code Reference
 * 
 * This file contains a complete implementation of SnakeLang's tagged object system:
 * - SnakeObjectKind_t enum and SnakeObjectData_t union
 * - Tagged SnakeObject_t representation
 * - Constructors: integer, float, string, vector3, and dynamic array
 * - Array indexing (get/set)
 * - Polymorphic length operation (strings and arrays)
 * - Polymorphic add operation (integers, floats, string concatenation, vector addition)
 * - Memory deallocation (recursive cleanup)
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter09 && ./chapter09
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/* Forward declaration */
typedef struct SnakeObject SnakeObject_t;

/* --- 1. Type Definitions --- */

typedef enum {
    SNAKE_INTEGER,
    SNAKE_FLOAT,
    SNAKE_STRING,
    SNAKE_VECTOR_3,
    SNAKE_ARRAY
} SnakeObjectKind_t;

typedef struct {
    float x;
    float y;
    float z;
} Vector3_t;

typedef struct {
    size_t length;
    size_t capacity;
    SnakeObject_t **elements;
} SnakeArray_t;

typedef union {
    int v_int;
    float v_float;
    char *v_string;
    Vector3_t v_vec3;
    SnakeArray_t v_array;
} SnakeObjectData_t;

struct SnakeObject {
    SnakeObjectKind_t kind;
    SnakeObjectData_t data;
};

/* --- 2. Constructors & Destructors --- */

void snake_object_free(SnakeObject_t *obj);

SnakeObject_t *new_snake_integer(int value) {
    SnakeObject_t *obj = malloc(sizeof(SnakeObject_t));
    if (obj == NULL) return NULL;
    obj->kind = SNAKE_INTEGER;
    obj->data.v_int = value;
    return obj;
}

SnakeObject_t *new_snake_float(float value) {
    SnakeObject_t *obj = malloc(sizeof(SnakeObject_t));
    if (obj == NULL) return NULL;
    obj->kind = SNAKE_FLOAT;
    obj->data.v_float = value;
    return obj;
}

SnakeObject_t *new_snake_string(const char *value) {
    SnakeObject_t *obj = malloc(sizeof(SnakeObject_t));
    if (obj == NULL) return NULL;
    obj->kind = SNAKE_STRING;
    obj->data.v_string = malloc(strlen(value) + 1);
    if (obj->data.v_string == NULL) {
        free(obj);
        return NULL;
    }
    strcpy(obj->data.v_string, value);
    return obj;
}

SnakeObject_t *new_snake_vector3(float x, float y, float z) {
    SnakeObject_t *obj = malloc(sizeof(SnakeObject_t));
    if (obj == NULL) return NULL;
    obj->kind = SNAKE_VECTOR_3;
    obj->data.v_vec3 = (Vector3_t){ x, y, z };
    return obj;
}

SnakeObject_t *new_snake_array(size_t capacity) {
    if (capacity == 0) capacity = 4;
    SnakeObject_t *obj = malloc(sizeof(SnakeObject_t));
    if (obj == NULL) return NULL;
    obj->kind = SNAKE_ARRAY;
    obj->data.v_array.length = 0;
    obj->data.v_array.capacity = capacity;
    obj->data.v_array.elements = malloc(sizeof(SnakeObject_t *) * capacity);
    if (obj->data.v_array.elements == NULL) {
        free(obj);
        return NULL;
    }
    return obj;
}

void snake_object_free(SnakeObject_t *obj) {
    if (obj == NULL) return;

    if (obj->kind == SNAKE_STRING) {
        free(obj->data.v_string);
    } else if (obj->kind == SNAKE_ARRAY) {
        for (size_t i = 0; i < obj->data.v_array.length; i++) {
            snake_object_free(obj->data.v_array.elements[i]);
        }
        free(obj->data.v_array.elements);
    }
    free(obj);
}

/* --- 3. Array Operations --- */

bool snake_array_set(SnakeObject_t *arr_obj, size_t index, SnakeObject_t *val) {
    if (arr_obj == NULL || arr_obj->kind != SNAKE_ARRAY) return false;
    SnakeArray_t *arr = &arr_obj->data.v_array;

    if (index >= arr->capacity) {
        size_t new_cap = (index + 1) * 2;
        SnakeObject_t **new_elems = realloc(arr->elements, sizeof(SnakeObject_t *) * new_cap);
        if (new_elems == NULL) return false;
        arr->elements = new_elems;
        arr->capacity = new_cap;
    }

    arr->elements[index] = val;
    if (index >= arr->length) {
        arr->length = index + 1;
    }
    return true;
}

SnakeObject_t *snake_array_get(const SnakeObject_t *arr_obj, size_t index) {
    if (arr_obj == NULL || arr_obj->kind != SNAKE_ARRAY) return NULL;
    if (index >= arr_obj->data.v_array.length) return NULL;
    return arr_obj->data.v_array.elements[index];
}

/* --- 4. Polymorphic Operations --- */

int snake_object_length(const SnakeObject_t *obj) {
    if (obj == NULL) return -1;
    switch (obj->kind) {
        case SNAKE_STRING:
            return (int)strlen(obj->data.v_string);
        case SNAKE_ARRAY:
            return (int)obj->data.v_array.length;
        default:
            return -1; /* Objects without length semantics */
    }
}

SnakeObject_t *snake_object_add(const SnakeObject_t *a, const SnakeObject_t *b) {
    if (a == NULL || b == NULL || a->kind != b->kind) {
        return NULL; /* Incompatible types */
    }

    switch (a->kind) {
        case SNAKE_INTEGER:
            return new_snake_integer(a->data.v_int + b->data.v_int);
        case SNAKE_FLOAT:
            return new_snake_float(a->data.v_float + b->data.v_float);
        case SNAKE_STRING: {
            size_t len_a = strlen(a->data.v_string);
            size_t len_b = strlen(b->data.v_string);
            char *buf = malloc(len_a + len_b + 1);
            if (buf == NULL) return NULL;
            strcpy(buf, a->data.v_string);
            strcat(buf, b->data.v_string);
            SnakeObject_t *res = new_snake_string(buf);
            free(buf);
            return res;
        }
        case SNAKE_VECTOR_3:
            return new_snake_vector3(
                a->data.v_vec3.x + b->data.v_vec3.x,
                a->data.v_vec3.y + b->data.v_vec3.y,
                a->data.v_vec3.z + b->data.v_vec3.z
            );
        default:
            return NULL;
    }
}

/* --- 5. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 9: SnakeLang Object System Code Reference ===\n\n");

    /* 9.2 - 9.5 Primitive and Composite Objects */
    printf("1. Creating Objects:\n");
    SnakeObject_t *i1 = new_snake_integer(42);
    SnakeObject_t *i2 = new_snake_integer(58);
    SnakeObject_t *f1 = new_snake_float(3.14f);
    SnakeObject_t *s1 = new_snake_string("Hello, ");
    SnakeObject_t *s2 = new_snake_string("SnakeLang!");
    SnakeObject_t *v1 = new_snake_vector3(1.0f, 2.0f, 3.0f);
    SnakeObject_t *v2 = new_snake_vector3(10.0f, 20.0f, 30.0f);

    printf("   Integer 1: %d\n", i1->data.v_int);
    printf("   Float 1:   %.2f\n", f1->data.v_float);
    printf("   String 1:  \"%s\"\n", s1->data.v_string);
    printf("   Vector 1:  (%.1f, %.1f, %.1f)\n\n", v1->data.v_vec3.x, v1->data.v_vec3.y, v1->data.v_vec3.z);

    /* 9.8 Polymorphic Length */
    printf("2. Polymorphic length:\n");
    printf("   length(\"%s\") = %d\n", s2->data.v_string, snake_object_length(s2));
    printf("   length(integer)  = %d (not supported)\n\n", snake_object_length(i1));

    /* 9.9 Polymorphic Add */
    printf("3. Polymorphic add:\n");
    SnakeObject_t *i_sum = snake_object_add(i1, i2);
    printf("   %d + %d = %d\n", i1->data.v_int, i2->data.v_int, i_sum->data.v_int);

    SnakeObject_t *s_concat = snake_object_add(s1, s2);
    printf("   \"%s\" + \"%s\" = \"%s\"\n", s1->data.v_string, s2->data.v_string, s_concat->data.v_string);

    SnakeObject_t *v_sum = snake_object_add(v1, v2);
    printf("   Vector addition = (%.1f, %.1f, %.1f)\n\n",
           v_sum->data.v_vec3.x, v_sum->data.v_vec3.y, v_sum->data.v_vec3.z);

    /* 9.6 & 9.7 Dynamic Object Arrays */
    printf("4. Dynamic Object Array:\n");
    SnakeObject_t *arr = new_snake_array(2);
    snake_array_set(arr, 0, new_snake_integer(100));
    snake_array_set(arr, 1, new_snake_string("ArrayElement"));
    snake_array_set(arr, 2, new_snake_float(9.99f)); /* Triggers resize */

    printf("   Array length: %d\n", snake_object_length(arr));
    for (size_t idx = 0; idx < (size_t)snake_object_length(arr); idx++) {
        SnakeObject_t *elem = snake_array_get(arr, idx);
        if (elem->kind == SNAKE_INTEGER) printf("     [%zu] Integer: %d\n", idx, elem->data.v_int);
        else if (elem->kind == SNAKE_STRING) printf("     [%zu] String:  \"%s\"\n", idx, elem->data.v_string);
        else if (elem->kind == SNAKE_FLOAT) printf("     [%zu] Float:   %.2f\n", idx, elem->data.v_float);
    }
    printf("\n");

    /* Cleanup */
    snake_object_free(i1);
    snake_object_free(i2);
    snake_object_free(i_sum);
    snake_object_free(f1);
    snake_object_free(s1);
    snake_object_free(s2);
    snake_object_free(s_concat);
    snake_object_free(v1);
    snake_object_free(v2);
    snake_object_free(v_sum);
    snake_object_free(arr); /* Recursively frees its elements */

    printf("=== Chapter 9 reference executed successfully! ===\n");
    return 0;
}
