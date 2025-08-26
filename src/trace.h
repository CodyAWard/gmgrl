//
// gmgrl
// cody ward 2025
// 
// A simple trace macro for gmgrl, logging from gmgrl 
// will be handled through these macros

#ifndef gmgrl_trace_h
#define gmgrl_trace_h

#define trace_out(...) do { \
    fprintf(stdout, "> "); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
} while (0)

#define trace_err(...) do { \
    fprintf(stderr, "! "); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
} while (0)

#define trace_char(c) do { \
    fprintf(stdout, "%c", c); \
} while (0)

#endif /* gmgrl_trace_h */
