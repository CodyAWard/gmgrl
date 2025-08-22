//
// gmgrl
// cody ward 2025
//
// The main entry point for gmgrl

// gmgrl
#include "gmgrl.h"
#include "rom.h"
#include "trace.h"
#include "types.h"

// std
#include <string.h>

i32 main(i32 arg_count, char *args[]) {
    trace_out("gmgrl: starting");

    const char *rom_path = NULL;

    if (arg_count > 1) {
        for (i32 i = 1; i < arg_count; i++) {
            if (strlen(args[i]) <= 0) continue;

            if (args[i][0] == '-') {
                if (strcmp(args[i], "-h") == 0 || strcmp(args[i], "--help") == 0) {
                    trace_out("gmgrl: usage: gmgrl [rom path]");
                    return 0;
                }
            } else {
                rom_path = args[i];
            }
        }
    }

    if (!rom_path) {
        trace_err("gmgrl: no rom path specified");
        return 1;
    }
 
    rom loaded_rom;
    if (!rom_try_load(rom_path, &loaded_rom)) {
        trace_err("gmgrl: failed to load rom");
        return 1;
    }

    trace_out("gmgrl: done");
    return 0;
}
