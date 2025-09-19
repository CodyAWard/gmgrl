//
// gmgrl
// cody ward 2025
//
// The test runner implementation

// std
#include <stdlib.h>
// gmgrl
#include "tests.h"

extern const test_definition* const __start_gmgrltst __asm("section$start$__DATA$gmgrltst");
extern const test_definition* const __stop_gmgrltst __asm("section$end$__DATA$gmgrltst");
#define GMGRLTST_START (&__start_gmgrltst)
#define GMGRLTST_STOP (&__stop_gmgrltst)

bool tests_run_all() {
    u32 success_count = 0;
    u32 test_count = 0;
	for (const test_definition* const* it = GMGRLTST_START; it != GMGRLTST_STOP; it++) {
		if (*it == NULL) {
			continue;
		}
        
        test_count++;

		test_definition test = **it;

        trace_char('\n');
        trace_out("test | %s", test.name);
        trace_out("     | %s", test.description);
        test_result result = test.fn();
        if(result.did_succeed) {
            success_count++;
            trace_out(" [✓] | PASSED"); 
        } else { 
            trace_err(" [✗] | FAILED"); 
            trace_err("     | %s", result.error_message); 
        }
        trace_char('\n');
    }

    u32 failure_count = test_count - success_count;
    bool had_failures = failure_count > 0;
    string* result_title = string_from_cstr(had_failures ? "FAILED" : "PASSED");
    trace_out("results | %s", result_title->data);
    const char* msg = string_from_format("        | %i / %i tests passed.", success_count, test_count)->data;

    if(success_count == test_count) {    
        trace_out("%s", msg);
    } else {
        trace_err("%s", msg);
    }

    return success_count == test_count;
}

