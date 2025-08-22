//
// gmgrl
// cody ward 2025
//
// rom loading and related data structures
//

#ifndef gmgrl_rom_h
#define gmgrl_rom_h

#include "types.h"

typedef struct {
    byte *data;
    u32 size;
} rom;

bool rom_try_load(const char *path, rom *out);

#endif /* gmgrl_rom_h */
