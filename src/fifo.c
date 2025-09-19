//
// gmgrl
// cody ward 2025
//
// The fifo queue implementation

#include "fifo.h"
#include "tests.h"

fifo_u8* fifo_u8_new(u8 capacity) {
    fifo_u8 *new = mem_alloc(sizeof(fifo_u8));
    new->data = mem_alloc(capacity);
    new->front = 0;
    new->rear = -1;
    new->size = 0;
    new->capacity = capacity;

    return new;
}

void fifo_u8_free(fifo_u8* fifo) {
    mem_free(fifo->data);
    mem_free(fifo);
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

// tests

TEST(fifo_u8_is_empty_test, "ensures the fifo is empty") {
    fifo_u8* fifo = fifo_u8_new(10);
    TEST_ASSERT(fifo_u8_is_empty(fifo), "fifo_u8_is_empty failed to return true for empty fifo");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}

TEST(fifo_u8_is_full_test, "ensures the fifo is full") {
    fifo_u8* fifo = fifo_u8_new(10);
    TEST_ASSERT(!fifo_u8_is_full(fifo), "fifo_u8_is_full failed to return false for full fifo");
    for (i32 i = 0; i < 10; i++) {
        TEST_ASSERT(fifo_u8_try_push(fifo, i), "fifo_u8_try_push failed to return true for pushing to fifo");
    }
    TEST_ASSERT(fifo_u8_is_full(fifo), "fifo_u8_is_full failed to return true for full fifo");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}

TEST(fifo_u8_try_push_test, "ensures the fifo pushes at the correct data") {
    fifo_u8* fifo = fifo_u8_new(10);
    for (i32 i = 0; i < 10; i++) {
        TEST_ASSERT(fifo_u8_try_push(fifo, i), "fifo_u8_try_push failed to return true for pushing to fifo");
        TEST_ASSERT(fifo->data[i] == i, "fifo_u8_try_push failed to push data");
    }
    TEST_ASSERT(!fifo_u8_try_push(fifo, 10), "fifo_u8_try_push should have failed to push to full fifo");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}

TEST(fifo_u8_try_pop_test, "ensures the fifo pops at the correct data") {
    fifo_u8* fifo = fifo_u8_new(10);
    fifo_u8_try_push(fifo, 1);
    TEST_ASSERT(fifo_u8_try_pop(fifo, &fifo->data[0]), "fifo_u8_try_pop failed to return true for popping from non-empty fifo");
    TEST_ASSERT(fifo->size == 0, "fifo_u8_try_pop failed to decrement size");
    TEST_ASSERT(fifo->data[0] == 1, "fifo_u8_try_pop failed to pop data");
    TEST_ASSERT(!fifo_u8_try_pop(fifo, &fifo->data[0]), "fifo_u8_try_pop failed to return false for popping from empty fifo");
    TEST_ASSERT(fifo->size == 0, "fifo_u8_try_pop failed to not decrement size");
    TEST_ASSERT(fifo->data[0] == 1, "fifo_u8_try_pop failed to not pop data");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}

TEST(fifo_u8_peek_test, "ensures the fifo peeks at the correct data") {
    fifo_u8* fifo = fifo_u8_new(10);
    fifo_u8_try_push(fifo, 2);
    fifo_u8_try_push(fifo, 1);
    TEST_ASSERT(fifo_u8_peek(fifo) == 2, "fifo_u8_peek failed to return data");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}

TEST(fifo_u8_reset_test, "ensures the fifo is reset to empty") {
    fifo_u8* fifo = fifo_u8_new(10);
    fifo_u8_try_push(fifo, 1);
    fifo_u8_reset(fifo);
    TEST_ASSERT(fifo->size == 0, "fifo_u8_reset failed to reset size");
    TEST_ASSERT(fifo->front == 0, "fifo_u8_reset failed to reset front");
    TEST_ASSERT(fifo->rear == -1, "fifo_u8_reset failed to reset rear");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}   

TEST(fifo_u8_overflow_test, "ensures the fifo overflows") {
    fifo_u8* fifo = fifo_u8_new(10);
    for (i32 i = 0; i < 10; i++) {
        TEST_ASSERT(fifo_u8_try_push(fifo, i), "fifo_u8_try_push failed to return true for pushing to fifo");
    }
    TEST_ASSERT(!fifo_u8_try_push(fifo, 10), "fifo_u8_try_push should have failed to push to full fifo");
    fifo_u8_free(fifo);

    TEST_SUCCEED();
}
