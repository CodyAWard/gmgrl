//
// gmgrl
// cody ward 2025
//
// The meta build program for gmgrl

// gmgrl
#include "trace.h"
#include "types.h"

// std
#include <stdlib.h>
#include <string.h>

i32 main(i32 arg_count, char* args[]) {
    trace_out("build: starting");

    bool should_run = false;

    if(arg_count > 1) {
        trace_out("build: with args");
        for(i32 i = 1; i < arg_count; i++) {
            trace_out("build: arg: %s", args[i]);

            if(strcmp(args[i], "--run") == 0) {
                should_run = true;
            }
        }
    }


    i32 result = system("cc main.c -o bin/gmgrl");
    if(result != 0) {
        trace_err("build: failed");
        return 1;
    }

    trace_out("build: done");

    if(should_run) {
        trace_out("gmgrl: running");
        result = system("./bin/gmgrl");
        if(result != 0) {
            trace_err("gmgrl: failed");
            return 1;
        }
        trace_out("gmgrl: done");
    }

    return 0;
}
