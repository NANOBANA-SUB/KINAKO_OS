#pragma once
// #include <assert.h>
#include "types.h"

struct trap_frame
{
    uint64_t ra, gp, tp;
    uint64_t t0, t1, t2, t3, t4, t5, t6;
    uint64_t a0, a1, a2, a3, a4, a5, a6, a7;
    uint64_t s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint64_t sp; // 元の sp（sscratch から持ってくる）
};
typedef struct trap_frame trap_frame;
// static_assert(sizeof(struct trap_frame) % sizeof(uint64_t) == 0, "aligned");

#define READ_CSR(reg)                                                          \
    ({                                                                         \
        unsigned long __tmp;                                                   \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));                  \
        __tmp;                                                                 \
    })

#define WRITE_CSR(reg, value)                                                  \
    do {                                                                       \
        uint64_t __tmp = (value);                                              \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp));                \
    } while (0)


void handle_trap(struct trap_frame *tf);
void kernel_entry(void);
