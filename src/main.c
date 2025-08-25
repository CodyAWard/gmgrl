//
// gmgrl
// cody ward 2025
//
// The main entry point for gmgrl

// gmgrl
#include "gmgrl.h"
#include "rom.h"
#include "strings.h"
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

    trace_out("gmgrl: asserting nintendo logo");
    if(!assert_nintendo_logo(&loaded_rom)) {
        trace_err("gmgrl: failed to assert nintendo logo");
        return 1;
    }

    string title;
    if(!get_rom_title(&loaded_rom, &title)) {
        trace_err("gmgrl: failed to get rom title");
        return 1;
    }

    string manufacturer_code;
    if(!get_rom_manufacturer_code(&loaded_rom, &manufacturer_code)) {
        trace_err("gmgrl: failed to get rom manufacturer code");
        return 1;
    }

    enum CGM_FLAGS cgm_flags;
    if(!get_rom_cgm_flags(&loaded_rom, &cgm_flags)) {
        trace_err("gmgrl: failed to get rom cgm flags");
        return 1;
    }

    trace_out("gmgrl: done");
    return 0;
}
