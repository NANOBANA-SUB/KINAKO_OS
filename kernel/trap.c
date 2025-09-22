#include "trap.h"
#include "panic.h"

void handle_trap(struct trap_frame *tf)
{
    uint64_t scause = READ_CSR(scause);
    uint64_t stval = READ_CSR(stval);
    uint64_t user_pc = READ_CSR(sepc);

    PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}