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
