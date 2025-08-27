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

// terminal
#include <termios.h>
#include <unistd.h>

i32 main(i32 arg_count, char *args[]) {
    trace_out("gmgrl: starting");

    const char *rom_path = NULL;
    bool manual_step = false;

    if (arg_count > 1) {
        for (i32 i = 1; i < arg_count; i++) {
            if (strlen(args[i]) <= 0) continue;

            if (args[i][0] == '-') {
                if (strcmp(args[i], "-h") == 0 || strcmp(args[i], "--help") == 0) {
                    trace_out("gmgrl: usage: gmgrl [rom path]");
                    return 0;
                }
                else if (strcmp(args[i], "-s") == 0 || strcmp(args[i], "--step") == 0) {
                    manual_step = true;
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
    if(!rom_assert_nintendo_logo(&loaded_rom)) {
        trace_err("gmgrl: failed to assert nintendo logo");
        return 1;
    }

    string title;
    if(!rom_get_title(&loaded_rom, &title)) {
        trace_err("gmgrl: failed to get rom title");
        return 1;
    }

    string manufacturer_code;
    if(!rom_get_manufacturer_code(&loaded_rom, &manufacturer_code)) {
        trace_out("gmgrl: failed to get rom manufacturer code");
    }

    enum CGM_FLAGS cgm_flags;
    if(!rom_get_cgm_flags(&loaded_rom, &cgm_flags)) {
        trace_out("gmgrl: failed to get rom cgm flags");
    }

    enum CART_TYPE cart_type;
    if(!rom_get_cart_type(&loaded_rom, &cart_type)) {
        trace_out("gmgrl: failed to get rom cart type");
    }

    cpu *cpu = cpu_new();

    if (!manual_step) {
        while(cpu_step(cpu, &loaded_rom)) {
            trace_out("gmgrl: cpu stepped");
            cpu_print(cpu);
        }
    } else {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        char c;
        while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
            if (c == ' ' || c == 'n') {
                cpu_step(cpu, &loaded_rom);
                cpu_print(cpu);
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    trace_out("gmgrl: done");
    return 0;
}
