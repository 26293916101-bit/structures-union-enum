#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Enum for Data Types including TYPE_EMPTY
typedef enum {
    TYPE_EMPTY,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING
} DataType;

// 2. Union for Data Storage
typedef union {
    int i_val;
    float f_val;
    char c_val;
    char *s_val;
} DataValue;

// 3. Container Struct
typedef struct {
    DataType type;
    DataValue data;
} Container;

// Helper: Get type name as string
const char* container_type_name(DataType type) {
    switch (type) {
        case TYPE_EMPTY: return "EMPTY";
        case TYPE_INT:   return "INT";
        case TYPE_FLOAT: return "FLOAT";
        case TYPE_CHAR:  return "CHAR";
        case TYPE_STRING:return "STRING";
        default:         return "UNKNOWN";
    }
}

// Initialize container
void container_init(Container *c) {
    if (c != NULL) {
        c->type = TYPE_EMPTY;
        c->data.s_val = NULL;
    }
}

// Clear container (free memory if string)
void container_clear(Container *c) {
    if (c == NULL) return;
    if (c->type == TYPE_STRING && c->data.s_val != NULL) {
        free(c->data.s_val);
        c->data.s_val = NULL;
    }
    c->type = TYPE_EMPTY;
}

// Setters with memory safety
void container_set_int(Container *c, int val) {
    if (c == NULL) return;
    container_clear(c);
    c->type = TYPE_INT;
    c->data.i_val = val;
}

void container_set_float(Container *c, float val) {
    if (c == NULL) return;
    container_clear(c);
    c->type = TYPE_FLOAT;
    c->data.f_val = val;
}

void container_set_char(Container *c, char val) {
    if (c == NULL) return;
    container_clear(c);
    c->type = TYPE_CHAR;
    c->data.c_val = val;
}

void container_set_string(Container *c, const char *val) {
    if (c == NULL) return;
    container_clear(c);
    if (val == NULL) {
        c->type = TYPE_EMPTY;
        return;
    }
    c->data.s_val = strdup(val);
    if (c->data.s_val != NULL) {
        c->type = TYPE_STRING;
    } else {
        c->type = TYPE_EMPTY;
    }
}

// Status-based Getters (returns 1 on success, 0 on failure)
int container_get_int(const Container *c, int *out_val) {
    if (c == NULL || out_val == NULL || c->type != TYPE_INT) return 0;
    *out_val = c->data.i_val;
    return 1;
}

int container_get_float(const Container *c, float *out_val) {
    if (c == NULL || out_val == NULL || c->type != TYPE_FLOAT) return 0;
    *out_val = c->data.f_val;
    return 1;
}

int container_get_char(const Container *c, char *out_val) {
    if (c == NULL || out_val == NULL || c->type != TYPE_CHAR) return 0;
    *out_val = c->data.c_val;
    return 1;
}

int container_get_string(const Container *c, const char **out_val) {
    if (c == NULL || out_val == NULL || c->type != TYPE_STRING) return 0;
    *out_val = c->data.s_val;
    return 1;
}

// Deep Copy function
void container_copy(Container *dest, const Container *src) {
    if (dest == NULL || src == NULL) return;
    container_clear(dest);
    dest->type = src->type;
    if (src->type == TYPE_STRING && src->data.s_val != NULL) {
        dest->data.s_val = strdup(src->data.s_val);
    } else {
        dest->data = src->data;
    }
}

// Print Container
void container_print(const Container *c) {
    if (c == NULL) return;
    printf("Container Type: %-7s | Value: ", container_type_name(c->type));
    switch (c->type) {
        case TYPE_EMPTY:
            printf("(empty)\n");
            break;
        case TYPE_INT:
            printf("%d\n", c->data.i_val);
            break;
        case TYPE_FLOAT:
            printf("%.2f\n", c->data.f_val);
            break;
        case TYPE_CHAR:
            printf("%c\n", c->data.c_val);
            break;
        case TYPE_STRING:
            printf("%s\n", c->data.s_val ? c->data.s_val : "(null)");
            break;
    }
}

// Main Function with 6+ containers and type transitions
int main() {
    printf("=== Safe Heterogeneous Data Container Tests ===\n\n");
    
    Container containers[6];
    for (int i = 0; i < 6; i++) {
        container_init(&containers[i]);
    }

    // Set initial values
    container_set_int(&containers[0], 100);
    container_set_float(&containers[1], 45.5f);
    container_set_char(&containers[2], 'Z');
    container_set_string(&containers[3], "Hello Faculty of Technology");

    // Test type transitions on container 0: INT -> STRING -> FLOAT
    printf("[*] Testing type transitions on Container 0...\n");
    container_set_string(&containers[0], "Now I am a string");
    container_set_float(&containers[0], 3.14159f);

    // Test Deep Copy
    printf("[*] Testing deep copy from Container 3 to Container 4...\n");
    container_copy(&containers[4], &containers[3]);
    container_set_string(&containers[3], "Modified Original String"); // Should not affect Container 4

    // Print all containers
    printf("\n--- Final Container States ---\n");
    for (int i = 0; i < 6; i++) {
        printf("Container %d -> ", i);
        container_print(&containers[i]);
    }

    // Cleanup memory
    for (int i = 0; i < 6; i++) {
        container_clear(&containers[i]);
    }

    return 0;
}
