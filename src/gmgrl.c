//
// gmgrl
// cody ward 2025
//
// The main gmgrl implementation

#include "gmgrl.h"

#include "rom.h"
#include "trace.h"

// macro wich appends a ; to the end of a switch
// label, avoiding the issue of a declaration
// immediately following a case statement
#define CASE(x) case x:;

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

    new->ime = false;

    new->ticks = 0;

    return new;
}

bool cpu_tick(cpu *cpu, rom *rom) {
    trace_out("cpu_tick %llu", cpu->ticks);
    u8 op_code = rom->data[cpu->regs.pc];
    trace_out("    %04X -> 0x%02X", cpu->regs.pc, op_code);

    cpu->ticks++;
    cpu->regs.pc++;

    if (op_code == 0x00) { // NOP
        trace_out("    nop");
        return true;
    }

    if (op_code == 0xF3) { // DI
        trace_out("    di");
        cpu->ime = false;
        return true;
    }
    if (op_code == 0xF8) { // EI
        trace_out("    ei");
        cpu->ime = true;
        return true;
    }

    if (op_code == 0x05) { // DEC B
        trace_out("    dec b");
        bool requires_borrow = get_lsb_nibble(cpu->regs.b) == 0x00;
        cpu->regs.b--;
        cpu_set_flag_z(cpu, cpu->regs.b == 0);
        cpu_set_flag_n(cpu, true);
        cpu_set_flag_h(cpu, requires_borrow);
        return true;
    }
    if (op_code == 0x0D) { // DEC C
        trace_out("    dec c");
        bool requires_borrow = get_lsb_nibble(cpu->regs.c) == 0x00;
        cpu->regs.c--;
        cpu_set_flag_z(cpu, cpu->regs.c == 0);
        cpu_set_flag_n(cpu, true);
        cpu_set_flag_h(cpu, requires_borrow);
        return true;
    }


    if (op_code == 0x3E) { // LD A U8
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    ld a u8: %02X", n8);
        cpu->regs.a = n8;
        cpu->regs.pc++;
        return true;
    }
    if (op_code == 0x06) { // LD B U8
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    ld b u8: %02X", n8);
        cpu->regs.b = n8;
        cpu->regs.pc++;
        return true;
    }
    if (op_code == 0x0E) { // LD C U8
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    ld c u8: %02X", n8);
        cpu->regs.c = n8;
        cpu->regs.pc++;
        return true;
    }

    if (op_code == 0x0F) { // RRCA
        trace_out("    rrca");
        u8 lsb = cpu->regs.a & 0x01; 
        cpu->regs.a = (cpu->regs.a >> 1) | (lsb << 7);         
        cpu_set_flag_z(cpu, false); // Zero flag is always reset
        cpu_set_flag_n(cpu, false); 
        cpu_set_flag_h(cpu, false); 
        cpu_set_flag_c(cpu, lsb); 
        return true;
    }

    if (op_code == 0x20) { // JR NZ U8
        i8 n8 = cpu_get_i8(cpu, rom);
        u16 jmp_addr_adjusted = cpu->regs.pc + n8 + 1;
        trace_out("    jr nz i8: %04X", jmp_addr_adjusted);
        if (cpu_get_flag_z(cpu) == false) {
            cpu->regs.pc += n8;
        } 

        cpu->regs.pc++;
        return true;
    }

    if (op_code == 0x21) { // LD HL U16
        u16 n16 = cpu_get_u16(cpu, rom);
        trace_out("    ld hl u16: %04X", n16);
        cpu->regs.hl = n16;
        cpu->regs.pc += 2;
        return true;
    }
    if (op_code == 0x32) { // LD HL- A
        trace_out("    ld hl- a");
        cpu->memory[cpu->regs.hl] = cpu->regs.a;
        cpu->regs.hl--;
        return true;
    }

    if (op_code == 0xE0) { // LDH U8 A
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    ldh u8 a: %02X", n8);
        cpu->memory[0xFF00 + n8] = cpu->regs.a;
        cpu->regs.pc++;
        return true;
    }
    if (op_code == 0xF0) { // LDH A U8
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    ldh a u8: %02X", n8);
        cpu->regs.a = cpu->memory[0xFF00 + n8];
        cpu->regs.pc++;
        return true;
    }

    if (op_code == 0xFE) { // CP A U8
        u8 n8 = cpu_get_u8(cpu, rom);
        trace_out("    cp a u8: %02X", n8);
        u8 result = cpu->regs.a - n8;
        cpu_set_flag_z(cpu, result == 0);
        cpu_set_flag_n(cpu, true);
        cpu_set_flag_h(cpu, ((cpu->regs.a & 0x0F) - (n8 & 0x0F)) & 0x10);
        cpu_set_flag_c(cpu, n8 > cpu->regs.a);
        cpu->regs.pc++;
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

i8 cpu_get_i8(cpu *cpu, rom *rom) {
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
    trace_out("    af: %02X %02X (%c%c%c%c)", cpu->regs.a, cpu->regs.f, cpu_get_flag_z(cpu) ? 'Z' : '-', cpu_get_flag_n(cpu) ? 'N' : '-', cpu_get_flag_h(cpu) ? 'H' : '-', cpu_get_flag_c(cpu) ? 'C' : '-');
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

bool cpu_get_flag_z(cpu *cpu) {
    return check_bit(cpu->regs.f, 7);
}

bool cpu_get_flag_n(cpu *cpu) {
    return check_bit(cpu->regs.f, 6);
}

bool cpu_get_flag_h(cpu *cpu) {
    return check_bit(cpu->regs.f, 5);
}

bool cpu_get_flag_c(cpu *cpu) {
    return check_bit(cpu->regs.f, 4);
}

// pixel fetcher
pixel_fetcher* pixel_fetcher_new() {
    pixel_fetcher *new = mem_alloc(sizeof(pixel_fetcher));
    new->state = FETCHER_READ_TILE_ID;
    new->ticks = 0;
    new->fifo = fifo_u8_new(16);

    return new;
}

void pixel_fetcher_start(
    pixel_fetcher* fetcher, 
    u16 map_address, 
    u8 tile_line) {
    fetcher->map_address = map_address;
    fetcher->tile_line = tile_line;
    fetcher->state = FETCHER_READ_TILE_ID;
    fetcher->ticks = 0;

    fifo_u8_reset(fetcher->fifo);
}

void pixel_fetcher_tick(pixel_fetcher* fetcher, u8* memory) {
    fetcher->ticks++;
    if (fetcher->ticks < 2) {
        return; // The fetcher runs at 1/2 the rate of the PPU
    }
    fetcher->ticks = 0; // reset every time we actually tick

    switch (fetcher->state) {
        CASE(FETCHER_READ_TILE_ID)
            fetcher->current_tile_id = memory[
                fetcher->map_address + fetcher->tile_line];
            fetcher->state = FETCHER_READ_TILE_DATA_01;
            break;
        CASE(FETCHER_READ_TILE_DATA_01)
            // the graphical data for a tile
            // is 16 bytes (8 pixels per row)
            // tile data starts at address 0x8000 
            // so we need to compute an offset
            u16 offset = 0x8000 + fetcher->current_tile_id * 16;

            // from the starting offset, we compute the 
            // final address to read by finding out which 
            // of the 8-pixel rows of the tile we need
            u16 address = offset + (u16)fetcher->tile_line * 2;

            u16 data = memory[address];
            for (u8 bit_pos = 0; bit_pos <= 7; bit_pos++) {
                // store the first bit of pixel color in the pixel data buffer
                fetcher->data[bit_pos] = (data >> bit_pos) & 1;
            }
            fetcher->state = FETCHER_READ_TILE_DATA_02;
            break;
        CASE(FETCHER_READ_TILE_DATA_02) {
            // this is similar to the above state, but the address
            // is offset by 1 to the right, reading the second byte
            // instead of overwriting the data, we append to
            // the data buffer
            u16 offset = 0x8000 + fetcher->current_tile_id * 16;
            u16 address = offset + (u16)fetcher->tile_line * 2;
            u16 data = memory[address + 1];
            for (u8 bit_pos = 0; bit_pos <= 7; bit_pos++) {
                fetcher->data[bit_pos] |= ((data >> bit_pos) & 1) << 1;
            }

            fetcher->state = FETCHER_PUSH_FIFO;
            }
            break;
        CASE(FETCHER_PUSH_FIFO)
            if (fetcher->fifo->size <= 8) {
                // Note: we are pushing in reverse order
                for (i8 i = 7; i >= 0; i--) { 
                    if(!fifo_u8_try_push(
                        fetcher->fifo, 
                        fetcher->data[i])) {
                    exit(1);
                        trace_err("pixel fetcher: failed to push fifo");
                    }
                }
                // Advance to the next tile in the map's row.
                fetcher->tile_line++;
                fetcher->state = FETCHER_READ_TILE_ID;
            }            
            break;
    }
}

// ppu
ppu* ppu_new() {
    ppu *new = mem_alloc(sizeof(ppu));
    new->state = PPU_STATE_OAM_SEARCH;
    new->x = 0;
    new->y = 0;
    new->ticks = 0;
    new->fetcher = *pixel_fetcher_new();

    return new;
}

void ppu_tick(ppu *ppu, screen *screen, u8 *memory) {
    ppu->ticks++;

    switch (ppu->state) {
        case PPU_STATE_HBLANK:
           // A full scanline will take 456 ticks to complete.
           // At the end of each scanline, the PPU goes back to the
           // initial OAM Search state. When we reach line 144
           // (the end of the screen), we will switch back 
           // to VBlank state instead.
           if (ppu->ticks == 456) {
               ppu->ticks = 0;
               ppu->y++;
               if (ppu->y == SCREEN_HEIGHT) {
                   ppu->state = PPU_STATE_VBLANK;
               } else {
                   ppu->state = PPU_STATE_OAM_SEARCH;
               }
           }

            screen_hblank(screen);
           break;
        case PPU_STATE_VBLANK:
            // When we get to the end of the screen,
            // we wait 10 more scanlines before starting over.
            if (ppu->ticks == 456) {
                ppu->ticks = 0;
                ppu->y++;
                if (ppu->y == SCREEN_HEIGHT + 9) { // TODO: is this right?
                    ppu->y = 0;
                    ppu->state = PPU_STATE_OAM_SEARCH;
                }
            }

            screen_vblank(screen);
            break;
        case PPU_STATE_OAM_SEARCH:
            // OAM (Object Attribute Memory) is a memory area
            // that contains information about 40 sprites.
            // Each sprite requires 2 memory reads, at a 
            // tick each. So we require 80 ticks to read
            // all 40 sprites.
            if (ppu->ticks == 80) {
                ppu->x = 0;
                u8 tile_line = ppu->y % 8;
                u16 tile_address = 0x9800 + (ppu->y / 8) * 32;
                pixel_fetcher_start(&ppu->fetcher, tile_address, tile_line);
                ppu->state = PPU_STATE_PIXEL_TRANSFER;
            }
            ppu->state = PPU_STATE_PIXEL_TRANSFER;
            break;
        case PPU_STATE_PIXEL_TRANSFER: 
            pixel_fetcher_tick(&ppu->fetcher, memory);
            u8 col;
            if (fifo_u8_try_pop(ppu->fetcher.fifo, &col)) {
                screen_write_next(screen, col);
                ppu->x++;
            }
            if (ppu->x == SCREEN_WIDTH) { // Scanline is complete when we reach the end of the screen
                ppu->state = PPU_STATE_HBLANK;
            }
            break;
    }
}

// screen
screen* screen_new() {
    screen *new = mem_alloc(sizeof(screen));
    return new;
}

#define SCREEN_PIXEL_PAIR(top, bot) 

void screen_print(screen *screen) {
    u8 screen_width = 160;
    u8 screen_height = 144;

    for (u8 y = 0; y < screen_height; y+=2) {
        for (u8 x = 0; x < screen_width; x++) {
            u8 top = screen->pixels[x][y];
            u8 bot = screen->pixels[x][y + 1];
            printf("\x1b[%u;%um▀\x1b[0m", 30 + top, 40 + bot);
        }
        printf("\n");
    }

    
    printf("\033[2J\033[1;1H");
}

void screen_set_pixel(screen *screen, u8 x, u8 y, u8 color) {
    screen->pixels[x][y] = color;
}

void screen_write_next(screen *screen, u8 color) {
    screen->pixels[screen->current_x][screen->current_y] = color;
    screen->current_x++;
    if (screen->current_x == SCREEN_WIDTH) {
        screen->current_x = 0;
        screen->current_y++;
        if (screen->current_y == SCREEN_HEIGHT) {
            screen->current_y = 0;
        }
    }
}

void screen_hblank(screen *screen) {

    screen_print(screen);

    screen->current_x = 0;
    screen->current_y++;
}

void screen_vblank(screen *screen) {
    screen->current_x = 0;
    screen->current_y = 0;
}


