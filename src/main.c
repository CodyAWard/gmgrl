//
// gmgrl
// cody ward 2025
//
// The main entry point for gmgrl

// gmgrl
#include "gmgrl.h"

#include "trace.h"
#include "types.h"

i32 main() {
    trace_out("gmgrl: starting");

    registers regs = {0};
    regs.af = 0x0123;
    regs.bc = 0x4567;
    regs.de = 0x89AB;
    regs.hl = 0xCDEF;
    regs.sp = 0xF000;
    regs.pc = 0x1000;

    trace_out("regs: af: %04X bc: %04X de: %04X hl: %04X sp: %04X pc: %04X", regs.af, regs.bc, regs.de, regs.hl, regs.sp, regs.pc);
    trace_out("regs: a: %02X b: %02X c: %02X d: %02X e: %02X h: %02X l: %02X", regs.a, regs.b, regs.c, regs.d, regs.e, regs.h, regs.l);

    u8 value = 0x0000;
    trace_out("regs: checking bit: %u", check_bit(value, 0));
    trace_out("regs: checking bit: %u", check_bit(value, 1));
    trace_out("regs: checking bit: %u", check_bit(value, 2));
    trace_out("regs: checking bit: %u", check_bit(value, 3));
    trace_out("regs: checking bit: %u", check_bit(value, 4));
    trace_out("regs: checking bit: %u", check_bit(value, 5));
    trace_out("regs: checking bit: %u", check_bit(value, 6));
    trace_out("regs: checking bit: %u", check_bit(value, 7));


    value = set_bit(value, 7);
    trace_out("regs: set bit: %u", value);
    trace_out("regs: checking bit: %u", (bool)check_bit(value, 7));

    trace_out("gmgrl: done");
    return 0;
}
