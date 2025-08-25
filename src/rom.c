//
// gmgrl
// cody ward 2025
//
// rom loading and related data structures
//

#include "rom.h"

#include "trace.h"

bool rom_try_load(const char *path, rom *out) {
    trace_out("rom_try_load at %s", path);
    FILE *file = fopen(path, "rb");
    if (!file) {
        trace_err("rom_try_load failed to open rom file: %s", path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    rewind(file);

    byte *data = mem_alloc(size);
    if (!data) {
        fclose(file);
        trace_err("rom_try_load failed to allocate memory for rom. attempted to allocate %d bytes", size);
        return false;
    }

    if (fread(data, 1, size, file) != size) {
        fclose(file);
        mem_free(data);
        trace_err("rom_try_load failed to read rom file: %s", path);
        return false;
    }

    fclose(file);
    out->data = data;
    out->size = size;

    trace_out("rom_try_load success");
    return true;
}

bool rom_assert_nintendo_logo(rom* rom) {
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

bool rom_get_title(rom* rom, string* out) {
    trace_out("get_rom_title");
    

    // The title is contained between the bytes $0134-$0143
    i32 title_start = 0x0134;
    i32 title_end   = 0x0143;
    i32 title_size  = title_end - title_start;
    
    out = string_new(title_size);
    for (i32 i = title_start; i < title_end; i++) {
        out->data[i - title_start] = rom->data[i];
    }

    trace_out("get_rom_title success: %s", out->data);
    return true;
}

bool rom_get_manufacturer_code(rom* rom, string* out) {
    trace_out("get_rom_manufacturer_code");
    
    // The manufacturer code is contained between the bytes $0144-$0145
    i32 manufacturer_start = 0x013f;
    i32 manufacturer_end   = 0x0142;
    i32 manufacturer_size  = manufacturer_end - manufacturer_start;
    
    out = string_new(manufacturer_size);
    for (i32 i = manufacturer_start; i < manufacturer_end; i++) {
        out->data[i - manufacturer_start] = rom->data[i];
    }

    if(out->data[0] <= 0) {
        trace_err("get_rom_manufacturer_code failed: invalid manufacturer code");
        return false;
    }

    trace_out("get_rom_manufacturer_code success: %s", out->data);
    return true;
}

bool rom_get_cgm_flags(rom* rom, enum CGM_FLAGS *out) {
    trace_out("get_rom_cgm_flags");

    i32 flag_index = 0x0143;
    if (rom->data[flag_index] == 0x80) {
        *out = CGM_SUPPORTED;
        trace_out("get_rom_cgm_flags success: CGM_SUPPORTED");
        return true;
    }

    if (rom->data[flag_index] == 0xC0) {
        *out = GGM_ONLY;
        trace_out("get_rom_cgm_flags success: GGM_ONLY");
        return true;
    }

    trace_err("get_rom_cgm_flags failed: invalid flag value: %02X", rom->data[flag_index]);

    return false;
}

bool rom_get_cart_type(rom* rom, enum CART_TYPE *out) {
    trace_out("get_rom_cart_type");
    
    i32 cart_type_start = 0x0147;
    
    byte cart_type = rom->data[cart_type_start];

    switch (cart_type) {
         case 0x00: *out = ROM_ONLY;                         break;
         case 0x01: *out = MBC1;                             break;
         case 0x02: *out = MBC1_RAM;                         break;
         case 0x03: *out = MBC1_RAM_BATTERY;                 break;
         case 0x05: *out = MBC2;                             break;
         case 0x06: *out = MBC2_BATTERY;                     break;
         case 0x08: *out = ROM_RAM_9;                        break;
         case 0x09: *out = ROM_RAM_BATTERY_9;                break;
         case 0x0B: *out = MMM01;                            break;
         case 0x0C: *out = MMM01_RAM;                        break;
         case 0x0D: *out = MMM01_RAM_BATTERY;                break; 
         case 0x0F: *out = MBC3_TIMER_BATTERY;               break;
         case 0x10: *out = MBC3_TIMER_RAM_BATTERY_10;        break;
         case 0x11: *out = MBC3;                             break;
         case 0x12: *out = MBC3_RAM_10;                      break;
         case 0x13: *out = MBC3_RAM_BATTERY_10;              break;
         case 0x19: *out = MBC5;                             break;
         case 0x1A: *out = MBC5_RAM;                         break;
         case 0x1B: *out = MBC5_RAM_BATTERY;                 break;
         case 0x1C: *out = MBC5_RUMBLE;                      break;
         case 0x1D: *out = MBC5_RUMBLE_RAM;                  break;
         case 0x1E: *out = MBC5_RUMBLE_RAM_BATTERY;          break;
         case 0x20: *out = MBC6;                             break;
         case 0x22: *out = MBC7_SENSOR_RUMBLE_RAM_BATTERY;   break;
         case 0xFC: *out = POCKET_CAMERA;                    break;
         case 0xFD: *out = BANDAI_TAMA5;                     break;
         case 0xFE: *out = HUC3;                             break;
         case 0xFF: *out = HUC1_RAM_BATTERY;                 break;
    }

    trace_out("get_rom_cart_type success: %02X", *out);
    return true;
}
