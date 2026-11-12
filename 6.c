// Problem 3 — Extended: Memory-Efficient Typed Records and Safe Union Access
// Compiled with: -std=c11 -Wall -Wextra -Wpedantic

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// 1. Plain Union and Structure with matching member declarations
typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

// 4. Tagged Enumeration for tracking active union member type
typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

// 4. Tagged Record structure combining UnionData and ValueType tag
typedef struct {
    UnionData data;
    ValueType tag;
} TaggedRecord;

// 5. Validated setters with safety checks and null-terminator boundary control
int tagged_set_int(TaggedRecord *rec, int val) {
    if (rec == NULL) {
        return 0;
    }
    rec->data.i = val;
    rec->tag = VALUE_INT;
    return 1;
}

int tagged_set_float(TaggedRecord *rec, float val) {
    if (rec == NULL) {
        return 0;
    }
    rec->data.f = val;
    rec->tag = VALUE_FLOAT;
    return 1;
}

int tagged_set_text(TaggedRecord *rec, const char *text) {
    if (rec == NULL || text == NULL) {
        return 0;
    }
    // Ensure string fits in the 20-byte array including null terminator
    if (strlen(text) >= sizeof(rec->data.str)) {
        return 0;
    }
    strcpy(rec->data.str, text);
    rec->tag = VALUE_TEXT;
    return 1;
}

// 6. Safe display function reading only the active member based on its tag
void tagged_print(const TaggedRecord *rec) {
    if (rec == NULL) {
        return;
    }
    switch (rec->tag) {
        case VALUE_INT:
            printf("Active Type: INT   | Value: %d\n", rec->data.i);
            break;
        case VALUE_FLOAT:
            printf("Active Type: FLOAT | Value: %.2f\n", rec->data.f);
            break;
        case VALUE_TEXT:
            printf("Active Type: TEXT  | Value: %s\n", rec->data.str);
            break;
        default:
            printf("Active Type: UNKNOWN / INVALID\n");
            break;
    }
}

int main() {
    printf("=== Memory-Efficient Typed Records & Safe Union Access ===\n\n");

    UnionData u;
    StructData s;

    // 1. Size and Member Addresses
    printf("--- 1. Size & Member Addresses Analysis ---\n");
    printf("Size of UnionData:  %zu bytes\n", sizeof(UnionData));
    printf("Size of StructData: %zu bytes\n\n", sizeof(StructData));

    printf("Plain Union Member Addresses:\n");
    printf("  u.i:   %p\n", (void*)&u.i);
    printf("  u.f:   %p\n", (void*)&u.f);
    printf("  u.str: %p\n\n", (void*)&u.str);

    printf("Plain Structure Member Addresses:\n");
    printf("  s.i:   %p\n", (void*)&s.i);
    printf("  s.f:   %p\n", (void*)&s.f);
    printf("  s.str: %p\n\n", (void*)&s.str);

    // 2. Structure Offsets using offsetof
    printf("--- 2. Structure Field Offsets (offsetof) ---\n");
    printf("Offset of s.i:   %zu bytes\n", offsetof(StructData, i));
    printf("Offset of s.f:   %zu bytes\n", offsetof(StructData, f));
    printf("Offset of s.str: %zu bytes\n\n", offsetof(StructData, str));

    // 3. Memory Comparison for 100 records
    printf("--- 3. Memory Comparison for 100 Records ---\n");
    printf("Memory for 100 Struct records: %zu bytes\n", sizeof(StructData) * 100);
    printf("Memory for 100 Union records:  %zu bytes\n\n", sizeof(UnionData) * 100);

    // 7. Store at least five tagged records of mixed types and display safely
    printf("--- 7. Tagged Records Demonstration (5 Mixed Records) ---\n");
    TaggedRecord records[5];

    tagged_set_int(&records[0], 10);
    tagged_set_float(&records[1], 220.5f);
    tagged_set_text(&records[2], "Union Programming");
    tagged_set_int(&records[3], 42);
    tagged_set_text(&records[4], "Safe Record");

    for (int i = 0; i < 5; i++) {
        printf("Record %d -> ", i + 1);
        tagged_print(&records[i]);
    }

    printf("\nSize of TaggedRecord struct: %zu bytes\n\n", sizeof(TaggedRecord));

    // 8. Observation & Explanation Section
    printf("--- 8. Observation & Interpretation ---\n");
    printf("- Union members overlap in memory at the exact same base address, allowing space-saving when a record holds only one of several types at any given time.\n");
    printf("- Structure fields occupy separate, non-overlapping offsets with padding added for hardware alignment requirements.\n");
    printf("- Tagged records prevent unsafe member access by storing an explicit tag (`ValueType`), ensuring we never read stale or misinterpreted union bytes.\n");

    return 0;
}
