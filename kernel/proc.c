#include "proc.h"
#include "panic.h"
#include "kstring.h"

struct proc procs[NPROC];

struct proc *current_proc;  // 実行中プロセス
struct proc *idle_proc;     // アイドルプロセス

void yield(void)
{
    // 実行可能プロセスの探索
    struct proc *next = idle_proc;

    for (int i = 0; i < NPROC; i++)
    {
        struct proc *proc = &procs[(current_proc->pid + i) % NPROC];

        if (proc->state == P_RUNNABLE && proc->pid > 0)
        {
            next = proc;
            break;
        }
    }

    // 現在実行中のプロセス以外に実行可能プロセスが存在しない
    if (next == current_proc)
        return;

    __asm__ __volatile__(
        "csrw sscratch, %[sscratch]\n"
        :
        : [sscratch] "r" ((uint64_t) &next->kstack[sizeof(next->kstack)])
    );

    // コンテクストスイッチ
    struct proc *prev = current_proc;
    current_proc = next;
    context_switch(&prev->sp, &next->sp);
}

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