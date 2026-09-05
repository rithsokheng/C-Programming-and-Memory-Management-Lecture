# Chapter 2 - Structs

> **Navigation:** [Previous: Chapter 1 - C Basics](../chapter-01-c-basics/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 3 - Pointers and Arrays](../chapter-03-pointers-and-arrays/README.md)

---

## Table of Contents

- [2.1 What Is a Struct?](#21-what-is-a-struct)
- [2.2 Defining and Initializing Structs](#22-defining-and-initializing-structs)
- [2.3 Designated Initializers](#23-designated-initializers)
- [2.4 Returning and Modifying Structs](#24-returning-and-modifying-structs)
- [2.5 `typedef`](#25-typedef)
- [2.6 Struct Memory Layout and Padding](#26-struct-memory-layout-and-padding)

---

## 2.1 What Is a Struct?

A `struct` groups multiple related data fields.

Example:

```c
struct Coordinate {
    int x;
    int y;
    int z;
};
```

Conceptually:

```text
Coordinate
+---------+---------+---------+
|    x    |    y    |    z    |
+---------+---------+---------+
```

Structs are useful when several values belong together.

---

## 2.2 Defining and Initializing Structs

### Create a struct object

```c
struct Coordinate c = {0};
```

Using `{0}` initializes the fields to zero values.

### Positional initialization

```c
struct City {
    char *name;
    int latitude;
    int longitude;
};

struct City city = {
    "San Francisco",
    37,
    -122
};
```

The positional values correspond to field order.

---

## 2.3 Designated Initializers

C provides designated initializers:

```c
struct Coordinate c = {
    .x = 10,
    .y = 20,
    .z = 30
};
```

This is especially useful because it makes field/value relationships explicit.

Example function:

```c
struct Coordinate make_coordinate(int x, int y, int z) {
    struct Coordinate c = {
        .x = x,
        .y = y,
        .z = z
    };

    return c;
}
```

---

## 2.4 Returning and Modifying Structs

Structs can be copied into functions and returned from functions.

Example:

```c
struct Coordinate scale_coordinate(struct Coordinate c, int scale) {
    c.x *= scale;
    c.y *= scale;
    c.z *= scale;

    return c;
}
```

The important point is that the argument is copied into the function.

Changes to the local copy do not automatically modify the original caller's object.

---

## 2.5 `typedef`

A `typedef` creates a convenient alias for a type.

Without `typedef`:

```c
struct Coordinate c;
```

With `typedef`:

```c
typedef struct Coordinate {
    int x;
    int y;
    int z;
} Coordinate;
```

Now:

```c
Coordinate c = {
    .x = 1,
    .y = 2,
    .z = 3
};
```

The course also uses a `_t` convention:

```c
typedef struct Coordinate {
    int x;
    int y;
    int z;
} Coordinate_t;
```

---

## 2.6 Struct Memory Layout and Padding

Struct fields are stored in memory according to a layout that can include padding.

Example:

```c
struct Example {
    char a;
    int b;
};
```

It is tempting to assume:

```text
sizeof(struct Example)
==
sizeof(char) + sizeof(int)
```

but padding may make the struct larger.

### Why padding exists

Alignment can make hardware access more efficient, so the compiler may insert unused padding bytes between fields.

The order of fields therefore matters.

A common memory-layout optimization discussed in the lecture is to put larger fields together rather than creating avoidable padding.

---

> **Navigation:** [Previous: Chapter 1 - C Basics](../chapter-01-c-basics/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 3 - Pointers and Arrays](../chapter-03-pointers-and-arrays/README.md)
