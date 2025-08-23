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

    // Nintendo logo assertion
    {
        byte expexted_nintendo_logo[] = {
            0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
            0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
            0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
        };

        i32 logo_start = 0x0104;
        i32 logo_end   = 0x0133;
        i32 logo_len   = logo_end - logo_start;
        
        for (i32 i = logo_start; i < logo_end; i++) {
            if (loaded_rom.data[i] != expexted_nintendo_logo[i - logo_start]) {
                trace_err("gmgrl: invalid nintendo logo at %i. expected 0x%02X, got 0x%02X", i, expexted_nintendo_logo[i - logo_start], loaded_rom.data[i]);
                return 1;
            }
        }

        trace_out("gmgrl: valid nintendo logo");
    }

    trace_out("gmgrl: done");
    return 0;
}
