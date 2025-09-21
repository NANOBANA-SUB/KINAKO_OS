#pragma once
#include "types.h"

#define PAGE_SIZE 4096

paddr_t alloc_pages(uint64_t n);