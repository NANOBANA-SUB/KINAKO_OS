#pragma once

#include "types.h"

// --- 調整パラメータ ---
#define NPROC        16
#define KSTACK_SIZE  4096
#define PNAME_LEN    16

// コンテクストスイッチのために保存するレジスタ群.
struct context
{
    uint64_t ra;   // x1: return address
    uint64_t sp;   // x2: stack pointer
    uint64_t gp;   // x3: global pointer
    uint64_t tp;   // x4: thread pointer
    uint64_t t0;   // x5
    uint64_t t1;   // x6
    uint64_t t2;   // x7
    uint64_t s0;   // x8 (frame pointer)
    uint64_t s1;   // x9
    uint64_t a0;   // x10
    uint64_t a1;   // x11
    uint64_t a2;   // x12
    uint64_t a3;   // x13
    uint64_t a4;   // x14
    uint64_t a5;   // x15
    uint64_t a6;   // x16
    uint64_t a7;   // x17
    uint64_t s2;   // x18
    uint64_t s3;   // x19
    uint64_t s4;   // x20
    uint64_t s5;   // x21
    uint64_t s6;   // x22
    uint64_t s7;   // x23
    uint64_t s8;   // x24
    uint64_t s9;   // x25
    uint64_t s10;  // x26
    uint64_t s11;  // x27
    uint64_t t3;   // x28
    uint64_t t4;   // x29
    uint64_t t5;   // x30
    uint64_t t6;   // x31

    uint64_t pc;   // program counter
    uint64_t mstatus; // machine status register (if必要)
};

// プロセス状態
enum procstate
{
    P_UNUSED = 0,
    P_EMBRYO,       // 生成直後
    P_RUNNABLE,     // 実行可能
    P_RUNNING,      // 実行中
    P_SLEEPING,     // スリープ
    P_ZOMBIE        // 終了待ち
};

// トラップフレーム
// 割り込みや例外で自動待避するCSRやレジスタ群を保存する
struct trapframe
{
    uint64_t mepc;
    uint64_t mstatus;
    uint64_t mcause;
    uint64_t mtval;
};

// プロセス制御構造体
struct proc
{
    int         pid;                // プロセスID
    procstate   state;              // プロセスの状態
    char        name[PNAME_LEN];    // プロセスの名前

    // スケジューリング情報
    int         time_slice;
    int         nice;               // 優先度

    // カーネルスタック
    void*       kstack;             // 低位アドレス(確保先)
    void*       kstack_top;         // 高位アドレス(sp 初期値)
    size_t      kstack_sz;

    // コンテクスト(コンテクストスイッチで保存・復元するレジスタ)
    context     ctx;

    // トラップフレーム
    trapframe*  tf;

    // 親子関係
    struct proc* parent;

    // ユーザデータ
    void*       arg;

    // リンク
    struct proc* next;
};

struct proctable
{
    proc    procs[NPROC];
    proc*   free_list;      // 未使用リスト
    proc*   run_queue_head; // 単純なFIFO
    proc*   run_queue_tail;
    proc*   current;        // 実行中
    int     next_pid;
};

extern proctable g_proctab;

// API
void     proc_init(void* (*kalloc)(size_t), void (*kfree)(void*));
proc*    proc_alloc(const char* name, void (*entry)(void*), void* arg);
void     scheduler(void);
void     yield(void);
