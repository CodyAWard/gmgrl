// 
// gmgrl
// cody ward 2025
//
// string structs and functions
//

#ifndef gmgrl_string_h
#define gmgrl_string_h

#include "types.h"

typedef struct {
    char *data;
    u32 size;
} string;

string* string_new(const u32 len);
string* string_from_cstr(const char *data);
void string_free(string *str);

bool string_equals(string *a, string *b);
bool string_equals_cstr(string *a, const char *b);

#endif /* gmgrl_string_h */
