//
// gmgrl
// cody ward 2025
//
// A collection of type aliases and macros for the gmgrl project

#ifndef gmgrl_types_h
#define gmgrl_types_h

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#include <stdio.h>
#include <stdlib.h>

// memory
// unit macros
#define GB(number) ((number) * 1000 * 1000 * 1000)
#define MB(number) ((number) * 1000 * 1000)
#define KB(number) ((number) * 1000)
// allocation
static void* mem_alloc(uint32_t size) {
    //printf("malloc: %u\n", size);
    void* ptr = malloc(size);
    if(ptr == NULL)
        printf("malloc failed!\n");
    return ptr;
}
// free
static void mem_free(void* ptr) { 
    //printf("free: %u\n", ptr);
    free(ptr);
}
// ban macros
#undef malloc
#define malloc(size) _banned__use_mem_alloc
#undef free
#define free(ptr) _banned__use_mem_free
// end memory

// type aliases
#define byte char

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8  int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define f16 _Float16
#define f32 float
#define f64 double
// end type aliases

// type helpers
#define check_bit(value, bit) ((value) & (1 << (bit)))
#define set_bit(value, bit) ((value) | (1 << (bit)))
#define clear_bit(value, bit) ((value) & ~(1 << (bit)))
// end type helpers

#endif /* gmgrl_types_h */
