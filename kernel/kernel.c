#include "types.h"
#include "kstring.h"
#include "kprintf.h"
#include "exception.h"

extern char __bss[], __bss_end[], __stack_top[];

void kernel_main(void) 
{
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    kprintf("\n\nHello KINAKO_OS!!\n");

    WRITE_CSR(stvec, (uint64_t) kernel_entry);
    __asm__ __volatile__("unimp"); // 無効な命令
    
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
