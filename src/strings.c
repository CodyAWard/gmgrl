//
// gmgrl
// cody ward 2025
//
// string structs and functions
//

// gmgrl includes
#include "strings.h"
#include "trace.h"

// std includes
#include <string.h>

string* string_new(const u32 len) {
    trace_out("string_new");
    string *str = mem_alloc(sizeof(string));
    str->data = mem_alloc(len + 1);
    for (u32 i = 0; i < len; i++) {
        str->data[i] = '\0';
    }
    str->size = len;
    return str;
}

string* string_from_cstr(const char *data) {
    trace_out("string_from_cstr");
    string *str = mem_alloc(sizeof(string));
    str->data = mem_alloc(strlen(data) + 1);
    strcpy(str->data, data);
    str->data[strlen(data)] = '\0';
    str->size = strlen(data);
    return str;
}

void string_free(string *str) {
    trace_out("string_free");
    mem_free(str->data);
    mem_free(str);
}

bool string_equals(string *a, string *b) {
    trace_out("string_equals");
    if (a->size != b->size) return false;
    return memcmp(a->data, b->data, a->size) == 0;
}   

bool string_equals_cstr(string *a, const char *b) {
    trace_out("string_equals_cstr");
    if (a->size != strlen(b)) return false;
    return memcmp(a->data, b, a->size) == 0;
}

