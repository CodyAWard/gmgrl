//
// gmgrl
// cody ward 2025
//
// The fifo queue implementation

#include "fifo.h"

#include "trace.h"

fifo_u8* fifo_u8_new(u8 capacity) {
    fifo_u8 *new = mem_alloc(sizeof(fifo_u8));
    new->data = mem_alloc(capacity);
    new->front = 0;
    new->rear = -1;
    new->size = 0;
    new->capacity = capacity;

    return new;
}

bool fifo_u8_is_empty(fifo_u8* fifo) {
    return fifo->size == 0;
}

bool fifo_u8_is_full(fifo_u8* fifo) {
    return fifo->size == fifo->capacity;
}

bool fifo_u8_try_push(fifo_u8* fifo, u8 value) {
    if (fifo_u8_is_full(fifo)) {
        return false;
    }
    fifo->rear = (fifo->rear + 1) % fifo->capacity;
    fifo->data[fifo->rear] = value;
    fifo->size++;
    return true;
}

bool fifo_u8_try_pop(fifo_u8* fifo, u8* value) {
    if (fifo_u8_is_empty(fifo)) {
        return false;
    }
    *value = fifo->data[fifo->front];
    fifo->front = (fifo->front + 1) % fifo->capacity;
    fifo->size--;
    return true;
}

u8 fifo_u8_peek(fifo_u8* fifo) {
    if (fifo_u8_is_empty(fifo)) {
        return 0; 
    }
    return fifo->data[fifo->front];
}

void fifo_u8_reset(fifo_u8* fifo) {
    fifo->front = 0;
    fifo->rear = -1;
    fifo->size = 0;
}
