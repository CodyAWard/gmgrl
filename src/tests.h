//
// gmgrl
// cody ward 2025
//
// Unit test framework for
// implementing unit tests directly
// next to our source code

#ifndef gmgrl_tests_h
#define gmgrl_tests_h

// std
#include <math.h>
// gmgrl
#include "strings.h"
#include "types.h"
#include "trace.h"

typedef struct {
    bool did_succeed;
    const char* error_message;
} test_result;

typedef struct {
    const char* name;
    const char* description;
    test_result (*fn)(void);
} test_definition;

#define TEST_ASSERT(assertion, message) \
if(!(assertion)) {  \
    return (test_result){ false, message }; \
}

#define TEST_EQUAL_F32(a, b) \
if(a != b) {  \
    return (test_result){ false, string_from_format("%f != %f", (f32)a, (f32)b) }; \
}

#define TEST_EPSILON 0.0001
#define TEST_APPROX_F32(a, b) \
if(fabs((f32)(a) - (f32)(b)) >= TEST_EPSILON) {  \
    return (test_result){ false, string_from_format("%f !~= %f\n -- err.%f", a, b, fabs((f32)(a) - (f32)(b))) }; \
}

#define TEST_STRING_EQUAL(a, b) \
if(a.length != b.length) { \
    return (test_result){ false, string_from_format("'%s' != '%s', different lengths %i and %i", a.data, b.data, a.length, b.length) }; \
} else if(!strings_are_equal(&a, &b)) { \
    return (test_result){ false, string_from_format("'%s' != '%s', different strings", a.data, b.data) }; \
}

#define TEST_SUCCEED() return (test_result){ true, "" }; 
#define TEST_FAIL(msg) return (test_result){ false, msg };

#define TEST(test, desc) \
    test_result test(void); \
    const test_definition def_##test = { \
		.name = #test, \
		.description = #desc, \
		.fn = test \
	}; \
    __attribute__((used, section("__DATA, gmgrltst"))) \
	const test_definition* defptr_##test = &def_##test; \
    test_result test(void) \

bool tests_run_all();

#endif /* gmgrl_tests_h */

