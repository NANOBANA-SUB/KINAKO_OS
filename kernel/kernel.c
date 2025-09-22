#include "types.h"
#include "kstring.h"
#include "kprintf.h"
#include "memory_alloc.h"
#include "trap.h"
#include "proc.h"
#include "util.h"
#include "panic.h"

extern char __bss[], __bss_end[], __stack_top[];

void delay(void) {
    for (int i = 0; i < 30000000; i++)
        __asm__ __volatile__("nop"); // 何もしない命令
}

struct proc *proc_a;
struct proc *proc_b;

void proc_a_entry(void) {
    kprintf("starting process A\n");
    while (1) {
        kputchar('A');
        context_switch(&proc_a->sp, &proc_b->sp);
        delay();
    }
}

void proc_b_entry(void) {
    kprintf("starting process B\n");
    while (1) {
        kputchar('B');
        context_switch(&proc_b->sp, &proc_a->sp);
        delay();
    }
}

void kernel_main(void) 
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    kprintf("\n\nHello KINAKO_OS!!\n");

    WRITE_CSR(stvec, (uint64_t) kernel_entry);
    
    proc_a = create_process((uint64_t) proc_a_entry);
    proc_b = create_process((uint64_t) proc_b_entry);
    proc_a_entry();

    PANIC("unreachable here!");
    
    for (;;) {
        __asm__ __volatile__("wfi");
    }
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "j kernel_main\n"
        :
        : [stack_top] "r" (__stack_top)
    );
}
