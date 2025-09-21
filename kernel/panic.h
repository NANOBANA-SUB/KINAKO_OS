#pragma once
#include "kprintf.h"

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        kprintf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)