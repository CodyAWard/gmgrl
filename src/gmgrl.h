//
// gmgrl
// cody ward 2025
//
// The main gmgrl implementation header

#ifndef gmgrl_h
#define gmgrl_h

#include "fifo.h"
#include "rom.h"
#include "types.h"

// The CPU registers
//
// 16 bit 	hi	lo	function/name
// ------------------------------
// af	    a	f	accumulator & flags (16 bit is virtual)
// bc	    b	c	bc (16 bit is virtual)
// de	    d	e	de (16 bit is virtual)
// hl	    h	l	hl (16 bit is virtual)
// sp	    -	-	stack pointer (16 bits)
// pc	    -	-	program counter (16 bits)
// 
// Since we have 16 bit virtual registers, we can store the actual
// 8 bit registers within a union. So accessing the virtual registers
// reads/writes into the actual registers at the proper offsets.
// Note: the order of the actual registers is "flipped" as we are
// assuming little endian.
typedef struct {
    union {
        struct {
            u8 f; // The flags register
            u8 a; // The accumulator
        };
        u16 af; // Virtual register: accumulator and flags
    };

    union {
        struct {
            u8 c; 
            u8 b;
        };
        u16 bc; // Virtual register: bc
    };

    union {
        struct {
            u8 e;
            u8 d;
        };
        u16 de; // Virtual register: de
    };

    union {
        struct {
            u8 l;
            u8 h;
        };
        u16 hl; // Virtual register: hl
    };
	
	u16 sp; // The stack pointer
	u16 pc; // The program counter
} registers;

typedef struct {
    registers regs;
    u16 pc;
    u8 memory[0xFFFF];
    bool ime;
    u64 ticks;
} cpu;

cpu* cpu_new();
bool cpu_tick(cpu *cpu, rom *rom);

u8 cpu_get_u8(cpu *cpu, rom *rom);
i8 cpu_get_i8(cpu *cpu, rom *rom);
u16 cpu_get_u16(cpu *cpu, rom *rom);

void cpu_print(cpu *cpu);

void cpu_set_flag_z(cpu *cpu, bool value);
void cpu_set_flag_n(cpu *cpu, bool value);
void cpu_set_flag_h(cpu *cpu, bool value);
void cpu_set_flag_c(cpu *cpu, bool value);

bool cpu_get_flag_z(cpu *cpu);
bool cpu_get_flag_n(cpu *cpu);
bool cpu_get_flag_h(cpu *cpu);
bool cpu_get_flag_c(cpu *cpu);

// pixel fetcher
enum PIXEL_FETCHER_STATES {
    FETCHER_READ_TILE_ID,
    FETCHER_READ_TILE_DATA_01,
    FETCHER_READ_TILE_DATA_02,
    FETCHER_PUSH_FIFO
};

typedef struct {
    enum PIXEL_FETCHER_STATES state;
    u8 ticks; // clock counter, so we can run at the correct speed
    fifo_u8* fifo;
    u16 map_address;
    u8 tile_line;
    u8 current_tile_id;
    u8 data[8];
} pixel_fetcher;

pixel_fetcher* pixel_fetcher_new();
void pixel_fetcher_start(
    pixel_fetcher* fetcher, 
    u16 map_address, 
    u8 tile_line);
void pixel_fetcher_tick(pixel_fetcher* fetcher, u8 *memory);

// screen
static const u8 SCREEN_WIDTH = 160;
static const u8 SCREEN_HEIGHT = 144;

typedef struct {
    u8 pixels[SCREEN_WIDTH][SCREEN_HEIGHT];
    u32 current_x;
    u32 current_y;
} screen;

// ppu
enum PPU_STATES {
    PPU_STATE_HBLANK,
    PPU_STATE_VBLANK,
    PPU_STATE_OAM_SEARCH,
    PPU_STATE_PIXEL_TRANSFER,
};

typedef struct {
    enum PPU_STATES state;
    u8 x; // distance across the current scanline
    u8 y; // the current scanline
    u32 ticks; // the number of ticks since the last hblank
    pixel_fetcher fetcher;
} ppu;

ppu* ppu_new();
void ppu_tick(ppu *ppu, screen *screen, u8 *memory);

// screen functions

screen* screen_new();
const char* screen_make_pixel_pair(u8 top, u8 bot);
void screen_print(screen *screen);
void screen_set_pixel(screen *screen, u8 x, u8 y, u8 color);

void screen_write_next(screen *screen, u8 color);
void screen_hblank(screen *screen);
void screen_vblank(screen *screen);
#endif /* gmgrl_h */
