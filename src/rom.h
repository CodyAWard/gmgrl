//
// gmgrl
// cody ward 2025
//
// rom loading and related data structures
//

#ifndef gmgrl_rom_h
#define gmgrl_rom_h

#include "strings.h"
#include "types.h"

typedef struct {
    byte *data;
    u32 size;
} rom;

bool rom_try_load(const char *path, rom *out);


bool rom_assert_nintendo_logo(rom* rom);

bool rom_get_title(rom* rom, string* out);
bool rom_get_manufacturer_code(rom* rom, string* out);

enum CGM_FLAGS {
    CGM_SUPPORTED = 0,
    GGM_ONLY = 1,
};

bool rom_get_cgm_flags(rom* rom, enum CGM_FLAGS *out);

enum CART_TYPE {
    ROM_ONLY,
    MBC1,
    MBC1_RAM,
    MBC1_RAM_BATTERY,
    MBC2,
    MBC2_BATTERY,
    ROM_RAM_9,
    ROM_RAM_BATTERY_9,
    MMM01,
    MMM01_RAM,
    MMM01_RAM_BATTERY,
    MBC3_TIMER_BATTERY,
    MBC3_TIMER_RAM_BATTERY_10,
    MBC3,
    MBC3_RAM_10,
    MBC3_RAM_BATTERY_10,
    MBC5,
    MBC5_RAM,
    MBC5_RAM_BATTERY,
    MBC5_RUMBLE,
    MBC5_RUMBLE_RAM,
    MBC5_RUMBLE_RAM_BATTERY,
    MBC6,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY,
    POCKET_CAMERA,
    BANDAI_TAMA5,
    HUC3,
    HUC1_RAM_BATTERY,
};

bool rom_getcart_type(rom* rom, enum CART_TYPE *out);


#endif /* gmgrl_rom_h */
