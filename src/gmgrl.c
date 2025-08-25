//
// gmgrl
// cody ward 2025
//
// The main gmgrl implementation

#include "gmgrl.h"

#include "trace.h"

bool assert_nintendo_logo(rom* rom) {
    byte expexted_nintendo_logo[] = {
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
        0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
        0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
    };

    i32 logo_start = 0x0104;
    i32 logo_end   = 0x0133;
    
    for (i32 i = logo_start; i < logo_end; i++) {
        if (rom->data[i] != expexted_nintendo_logo[i - logo_start]) {
            trace_err("gmgrl: invalid nintendo logo at %i. expected 0x%02X, got 0x%02X", i, expexted_nintendo_logo[i - logo_start], rom->data[i]);
            return false;
        }
    }

    // print the logo
    //
    // The way the pixels are encoded is as follows: (more visual aid)
    // - The logo is encoded as a 48-pixel “grid” of 4 pixels per nibble.
    // - The bytes $0104—$011B encode the top half of the logo while the bytes $011C–$0133 encode the bottom half.
    i32 logo_top_start = 0x0104;
    i32 logo_top_end   = 0x011B;
    i32 logo_bottom_start = 0x011C;
    i32 logo_bottom_end   = 0x0133;
    
    // - For each half, each nibble encodes 4 pixels (the MSB corresponds to the leftmost pixel, the LSB to the rightmost); a pixel is lit if the corresponding bit is set.
    // - The 4-pixel “groups” are laid out top to bottom, left to right.

    // Upper Half
    // First Row
    for (i32 i = logo_top_start; i < logo_top_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_msb_nibble(rom->data[i]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');
    // Second Row
    for (i32 i = logo_top_start; i < logo_top_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_lsb_nibble(rom->data[i]), j) ? '#' : ' ');
        }
    } 
    trace_char('\n');
    // Third Row
    for (i32 i = logo_top_start; i < logo_top_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_msb_nibble(rom->data[i + 1]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');
    // Fourth Row
    for (i32 i = logo_top_start; i < logo_top_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_lsb_nibble(rom->data[i + 1]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');

    // Lower Half
    // First Row
    for (i32 i = logo_bottom_start; i < logo_bottom_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_msb_nibble(rom->data[i]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');
    // Second Row
    for (i32 i = logo_bottom_start; i < logo_bottom_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_lsb_nibble(rom->data[i]), j) ? '#' : ' ');
        }
    } 
    trace_char('\n');
    // Third Row
    for (i32 i = logo_bottom_start; i < logo_bottom_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_msb_nibble(rom->data[i + 1]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');
    // Fourth Row
    for (i32 i = logo_bottom_start; i < logo_bottom_end; i+=2) {
        for (i32 j = 3; j >= 0; j--) {
            trace_char(check_bit(get_lsb_nibble(rom->data[i + 1]), j) ? '#' : ' ');
        }
    }
    trace_char('\n');

    return true;
}


