#include "proc.h"
#include "panic.h"
#include "kstring.h"

struct proc procs[NPROC];

struct proc *create_process(uint64_t pc)
{
    // 利用可能なプロセスを探す
    struct proc *proc = NULL;
    int i;

    for (i = 0; i < NPROC; i++)
    {
        if (procs[i].state == P_UNUSED)
        {
            proc = &procs[i];
            break;
        }
    }

    if (!proc)
        PANIC("no free process slots");

    uint64_t *sp = (uint64_t *) &proc->kstack[sizeof(proc->kstack)];
    *--sp = 0;                      // s11
    *--sp = 0;                      // s10
    *--sp = 0;                      // s9
    *--sp = 0;                      // s8
    *--sp = 0;                      // s7
    *--sp = 0;                      // s6
    *--sp = 0;                      // s5
    *--sp = 0;                      // s4
    *--sp = 0;                      // s3
    *--sp = 0;                      // s2
    *--sp = 0;                      // s1
    *--sp = 0;                      // s0
    *--sp = (uint64_t) pc;          // ra

    proc->pid = i + 1;
    proc->state = P_RUNNABLE;
    proc->sp = (uint64_t) sp;
    return proc;
}