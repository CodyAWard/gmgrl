//
// gmgrl
// cody ward 2025
//
// A terminal based screen viewer for gmgrl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "../src/types.h"
#include "../src/trace.h"

// ANSI escape codes for terminal control
#define CLEAR_SCREEN "\033[2J"
#define MOVE_CURSOR_TOP "\033[H"
#define CLEAR_TO_END "\033[J"

#define BUFFER_SIZE 160 * 144

// Global flag to handle Ctrl+C
volatile sig_atomic_t running = 1;

void signal_handler(i32 sig) {
    running = 0;
}

u32 ticks = 0;
void display_file(const char *filename) {
    ticks++;
    if (ticks > 999999) { // HACK this just reduces flashes but is very magic, and not perfect
        ticks = 0;
    } else { return; } 

    FILE *file = fopen(filename, "r");
    if (!file) {
        trace_err("failed to open file: %s", filename);
        return;
    }

    // Move cursor to top and clear screen
    printf("%s%s", CLEAR_SCREEN, MOVE_CURSOR_TOP);

    // Read and display file
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE - 1, file)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate
        printf("%s", buffer);
    }

    // Clear to end of screen to remove old content
    printf("%s", CLEAR_TO_END);
    fflush(stdout);

    if (ferror(file)) {
        trace_err("failed to read file: %s", filename);
    }

    fclose(file);
}

i32 main(i32 arg_count, char *args[]) {
    if (arg_count != 2) {
        trace_err("usage: %s <filename>", args[0]);
        return 1;
    }

    const char *filename = args[1];

    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler);

    // Main loop
    while (running) {
        display_file(filename);
    }

    return 0;
}
