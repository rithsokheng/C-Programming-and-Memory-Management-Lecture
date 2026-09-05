# Chapter 5 - Unions

> **Navigation:** [Previous: Chapter 4 - Enums](../chapter-04-enums/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 6 - Stack and Heap](../chapter-06-stack-and-heap/README.md)

---

## Table of Contents

- [5.1 What Is a Union?](#51-what-is-a-union)
- [5.2 Shared Memory](#52-shared-memory)
- [5.3 Union Size](#53-union-size)
- [5.4 Tagged-Unions Pattern](#54-tagged-unions-pattern)
- [5.5 Packet Header Example](#55-packet-header-example)

---

## 5.1 What Is a Union?

A `union` allows several fields to share the same memory.

Example:

```c
union Value {
    int age;
    char *name;
};
```

A union can hold one representation at a time.

---

## 5.2 Shared Memory

Consider:

```c
union Value {
    int age;
    char *name;
};

union Value value;
value.age = 29;
```

The same memory is used by `age` and `name`.

Reading the inactive field is generally unsafe and can produce meaningless results.

Conceptually:

```text
+----------------------+
|      shared memory   |
+----------------------+
       ^          ^
       |          |
     age        name
```

---

## 5.3 Union Size

The size of a union is large enough to hold its largest member, plus any alignment requirements.

Example:

```c
union SensorData {
    int value;
    char raw[256];
};
```

The union must be able to store the 256-byte array, so its size is at least large enough for that member.

---

## 5.4 Tagged-Unions Pattern

A very important pattern is:

```text
enum  -> tells us what the value is
union -> stores the value
```

Example:

```c
typedef enum {
    INTEGER,
    STRING
} ObjectKind;

typedef union {
    int v_int;
    char *v_string;
} ObjectData;
```

Then:

```c
typedef struct {
    ObjectKind kind;
    ObjectData data;
} Object;
```

Use the enum before reading the union field:

```c
switch (object.kind) {
    case INTEGER:
        printf("%d\n", object.data.v_int);
        break;

    case STRING:
        printf("%s\n", object.data.v_string);
        break;

    default:
        break;
}
```

This tagged-union design becomes the foundation of the SnakeLang object system.

---

## 5.5 Packet Header Example

Overlapping packet header representation:

```c
#include <stdint.h>

typedef struct {
    uint16_t source_port;
    uint16_t destination_port;
    uint32_t sequence_number;
} TcpHeader_t;

typedef union {
    TcpHeader_t tcp;
    uint8_t raw[8];
} PacketHeader_t;
```

Both views occupy the same eight-byte region:

```text
TCP view:
+----------+----------+----------------+
| src port | dst port | sequence no.   |
| 2 bytes  | 2 bytes  | 4 bytes        |
+----------+----------+----------------+

Raw view:
+--------------------------------------+
|             8 raw bytes              |
+--------------------------------------+
```

This demonstrates two compatible views of the same memory.

---

> **Navigation:** [Previous: Chapter 4 - Enums](../chapter-04-enums/README.md) | [Course Index](../README.md) | [Code Reference](code_reference.c) | [Next: Chapter 6 - Stack and Heap](../chapter-06-stack-and-heap/README.md)
