//
// gmgrl
// cody ward 2025
//
// The main gmgrl implementation header

#ifndef gmgrl_h
#define gmgrl_h

#include "rom.h"
#include "strings.h"
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

bool assert_nintendo_logo(rom* rom);

bool get_rom_title(rom* rom, string* out);
bool get_rom_manufacturer_code(rom* rom, string* out);

enum CGM_FLAGS {
    CGM_SUPPORTED = 0,
    GGM_ONLY = 1,
};

bool get_rom_cgm_flags(rom* rom, enum CGM_FLAGS *out);

#endif /* gmgrl_h */
