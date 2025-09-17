//
// gmgrl
// cody ward 2025
//
// The meta build program for gmgrl

// gmgrl
#include "src/trace.h"
#include "src/types.h"

// std
#include <stdlib.h>
#include <string.h>

i32 main(i32 arg_count, char* args[]) {
    trace_out("build: starting");

    bool should_run = false;
    i32 last_used_arg = 1;
    if(arg_count > 1) {
        trace_out("build: with args");
        for(i32 i = 1; i < arg_count; i++) {
            trace_out("build: arg: %s", args[i]);

            if(strcmp(args[i], "--run") == 0) {
                should_run = true;
                last_used_arg = i;
            } else {
                break;
            }
        }
    }
    
    i32 result = system("cc src/*.c -o bin/gmgrl");
    if(result != 0) {
        trace_err("build: failed to build gmgrl");
        return 1;
    }
    
    trace_out("build: built gmgrl");

    if(should_run) {
        // pass the args to the program
        char* concatenated_args;
        i32 concatenated_args_len = 0;
        for(i32 i = last_used_arg; i < arg_count; i++) {
            concatenated_args_len += strlen(args[i]) + 1;
        }
        concatenated_args = mem_alloc(concatenated_args_len);
        for(i32 i = last_used_arg; i < arg_count; i++) {
            strcat(concatenated_args, args[i]);
            if(i != arg_count - 1) {
                strcat(concatenated_args, " ");
            }
        }

        char* run_command = mem_alloc(strlen("./bin/gmgrl") + strlen(concatenated_args) + 1);
        sprintf(run_command, "./bin/gmgrl %s", concatenated_args);
        trace_out("gmgrl: running command: %s", run_command);

        result = system(run_command);
        if(result != 0) {
            trace_err("gmgrl: failed");
            return 1;
        }
        trace_out("gmgrl: done");
    }

    return 0;
}
