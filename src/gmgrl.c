//
// gmgrl
// cody ward 2025
//
// The main gmgrl implementation

#include "gmgrl.h"

#include "rom.h"
#include "trace.h"

cpu* cpu_new() {
    trace_out("cpu_new");
    // here we are assuming that the boot sequence executed
    // correctly and the cpu is in the correct state
    cpu *new = mem_alloc(sizeof(cpu));
    new->regs.pc = 0x0100;

    new->regs.af = 0x01B0;
    new->regs.bc = 0x0013;
    new->regs.de = 0x00D8;
    new->regs.hl = 0x014D;
    new->regs.sp = 0xFFFE;

    return new;
}

bool cpu_step(cpu *cpu, rom *rom) {
    trace_out("cpu_step");
    u8 op_code = rom->data[cpu->regs.pc];
    trace_out("    %04X -> 0x%02X", cpu->regs.pc, op_code);

    cpu->regs.pc++;

    if (op_code == 0x00) { // NOP
        trace_out("    nop");
        return true;
    }
    if (op_code == 0x0E) { // LD C U8
        trace_out("    ld c u8");
        u8 n8 = cpu_get_u8(cpu, rom);
        cpu->regs.c = n8;
        cpu->regs.pc++;
        return true;
    }
    if (op_code == 0x21) { // LD HL U16
        trace_out("    ld hl u16");
        u16 n16 = cpu_get_u16(cpu, rom);
        cpu->regs.hl = n16;
        cpu->regs.pc += 2;
        return true;
    }
    if (op_code == 0xC3) { // JP U16
        u16 n16 = cpu_get_u16(cpu, rom);
        trace_out("    jp u16: %04X", n16);
        cpu->regs.pc = n16;
        return true;
    }
    if (op_code == 0xAF) { // XOR A A
        trace_out("    xor a a: %02X", cpu->regs.a);
        cpu->regs.a ^= cpu->regs.a;

        cpu_set_flag_z(cpu, cpu->regs.a == 0);
        cpu_set_flag_n(cpu, false);
        cpu_set_flag_h(cpu, false);
        cpu_set_flag_c(cpu, false);
        return true;
    }
    if (op_code == 0xFF) { // RST 38
        trace_out("    rst 38");
        // push the address of the next instruction onto the stack
        cpu->regs.sp -= 2; // decrement the stack pointer
        cpu->memory[cpu->regs.sp] = cpu->regs.pc >> 8; // store the high byte of the next instruction
        cpu->memory[cpu->regs.sp + 1] = cpu->regs.pc & 0xFF; // store the low byte of the next instruction
        // set the program counter to the address of the RST instruction
        cpu->regs.pc = cpu->memory[0x0038];
        return true;
    }

    trace_err("    invalid op code: %02X", op_code);
    trace_char('-'); trace_char(' ');
    // to binary
    for (i32 i = 0; i < 8; i++) {
        trace_char(check_bit(op_code, i) ? '1' : '0');
    }
    trace_char('\n');


    return false;
}

u8 cpu_get_u8(cpu *cpu, rom *rom) {
    return rom->data[cpu->regs.pc];
}

u16 cpu_get_u16(cpu *cpu, rom *rom) {
    u8 lsb = rom->data[cpu->regs.pc];
    u8 msb = rom->data[cpu->regs.pc + 1];

    return (msb << 8) | lsb;
}

void cpu_print(cpu *cpu) {
    trace_out("    --------");
    trace_out("    pc: %04X", cpu->regs.pc);
    trace_out("    af: %02X %02X", cpu->regs.a, cpu->regs.f);
    trace_out("    bc: %02X %02X", cpu->regs.b, cpu->regs.c);
    trace_out("    de: %02X %02X", cpu->regs.d, cpu->regs.e);
    trace_out("    hl: %02X %02X", cpu->regs.h, cpu->regs.l);
    trace_out("    sp: %04X", cpu->regs.sp);
}

void cpu_set_flag_z(cpu *cpu, bool value) {
    if (value) {
        cpu->regs.f |= 0b10000000;
    } else {
        cpu->regs.f &= 0b01111111;
    }
}

void cpu_set_flag_n(cpu *cpu, bool value) {
    if (value) {
        cpu->regs.f |= 0b01000000;
    } else {
        cpu->regs.f &= 0b10111111;
    }
}

void cpu_set_flag_h(cpu *cpu, bool value) {
    if (value) {
        cpu->regs.f |= 0b00100000;
    } else {
        cpu->regs.f &= 0b11011111;
    }
}

void cpu_set_flag_c(cpu *cpu, bool value) {
    if (value) {
        cpu->regs.f |= 0b00010000;
    } else {
        cpu->regs.f &= 0b11101111;
    }
}
