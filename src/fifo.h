//
// gmgrl
// cody ward 2025
//
// The fifo queue implementation

#include "types.h"

typedef struct {
    u8* data;
    u8 front;
    i8 rear;
    u8 size;
    u8 capacity;
} fifo_u8;

fifo_u8* fifo_u8_new(u8 capacity);
void fifo_u8_free(fifo_u8* fifo);

bool fifo_u8_is_empty(fifo_u8* fifo);

bool fifo_u8_is_full(fifo_u8* fifo);

bool fifo_u8_try_push(fifo_u8* fifo, u8 value);

bool fifo_u8_try_pop(fifo_u8* fifo, u8* value);

u8 fifo_u8_peek(fifo_u8* fifo);

void fifo_u8_reset(fifo_u8* fifo);

