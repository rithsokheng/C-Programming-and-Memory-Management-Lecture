/**
 * Chapter 5: Unions - Code Reference
 * 
 * This file contains reference implementations of C unions:
 * - Union definition and shared memory semantics
 * - Overwriting overlapping members
 * - sizeof(union) vs sizeof(struct)
 * - Tagged-Union (discriminated union) pattern
 * - Network packet header parsing example
 * 
 * Compile:
 *     gcc -Wall -Wextra -pedantic code_reference.c -o chapter05 && ./chapter05
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --- 1. Basic Union --- */

/* Members share the exact same memory location */
typedef union {
    int as_int;
    float as_float;
    char bytes[4];
} ValueUnion_t;

/* Equivalent struct for size comparison */
typedef struct {
    int as_int;
    float as_float;
    char bytes[4];
} ValueStruct_t;

/* --- 2. Tagged-Union Pattern --- */

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} TagType_t;

typedef union {
    int v_int;
    float v_float;
    const char *v_string;
} TagData_t;

typedef struct {
    TagType_t type;
    TagData_t data;
} TaggedValue_t;

void print_tagged_value(const TaggedValue_t *val) {
    if (val == NULL) return;

    switch (val->type) {
        case TYPE_INT:
            printf("   [Integer] %d\n", val->data.v_int);
            break;
        case TYPE_FLOAT:
            printf("   [Float]   %.2f\n", val->data.v_float);
            break;
        case TYPE_STRING:
            printf("   [String]  \"%s\"\n", val->data.v_string);
            break;
    }
}

/* --- 3. Packet Header Example --- */

/* View raw 8 bytes as either structured header fields or raw bytes */
typedef union {
    struct {
        uint16_t packet_id;   /* 2 bytes */
        uint8_t  flags;       /* 1 byte  */
        uint8_t  channel;     /* 1 byte  */
        uint32_t payload_len; /* 4 bytes */
    } fields;
    uint8_t raw_bytes[8];
} PacketHeader_t;

/* --- 4. Main Demonstration --- */

int main(void) {
    printf("=== Chapter 5: Unions Code Reference ===\n\n");

    /* 5.1 & 5.2 Shared Memory & Overwriting */
    printf("1. Shared Memory Semantics:\n");
    ValueUnion_t u;
    u.as_int = 0x12345678;

    printf("   Set u.as_int = 0x12345678\n");
    printf("   u.as_int: 0x%08X\n", u.as_int);
    printf("   Address of u.as_int:   %p\n", (void *)&u.as_int);
    printf("   Address of u.as_float: %p (identical address!)\n", (void *)&u.as_float);
    printf("   Address of u.bytes:    %p (identical address!)\n\n", (void *)&u.bytes);

    /* Overwrite with float */
    u.as_float = 3.14159f;
    printf("   After writing u.as_float = 3.14159f:\n");
    printf("   u.as_float: %f\n", u.as_float);
    printf("   u.as_int:   0x%08X (raw IEEE-754 bit pattern)\n\n", u.as_int);

    /* 5.3 Size Comparison */
    printf("2. Size Comparison:\n");
    printf("   sizeof(ValueUnion_t):  %zu bytes (largest member: 4 bytes)\n", sizeof(ValueUnion_t));
    printf("   sizeof(ValueStruct_t): %zu bytes (sum of members + padding: 12 bytes)\n\n", sizeof(ValueStruct_t));

    /* 5.4 Tagged Union Pattern */
    printf("3. Tagged Union Pattern:\n");
    TaggedValue_t v1 = { .type = TYPE_INT,    .data = { .v_int = 42 } };
    TaggedValue_t v2 = { .type = TYPE_FLOAT,  .data = { .v_float = 99.5f } };
    TaggedValue_t v3 = { .type = TYPE_STRING, .data = { .v_string = "SnakeLang" } };

    print_tagged_value(&v1);
    print_tagged_value(&v2);
    print_tagged_value(&v3);
    printf("\n");

    /* 5.5 Packet Header Example */
    printf("4. Packet Header Parsing:\n");
    PacketHeader_t packet;
    packet.fields.packet_id = 0x0102;
    packet.fields.flags = 0x80;
    packet.fields.channel = 0x05;
    packet.fields.payload_len = 1024;

    printf("   Structured fields: ID=0x%04X, flags=0x%02X, channel=%d, len=%u\n",
           packet.fields.packet_id, packet.fields.flags, packet.fields.channel, packet.fields.payload_len);
    printf("   Raw wire bytes:    ");
    for (size_t i = 0; i < sizeof(packet.raw_bytes); i++) {
        printf("%02X ", packet.raw_bytes[i]);
    }
    printf("\n\n");

    printf("=== Chapter 5 reference executed successfully! ===\n");
    return 0;
}
