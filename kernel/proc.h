#pragma once

#include "types.h"

// --- 調整パラメータ ---
#define NPROC        16
#define KSTACK_SIZE  4096
#define PNAME_LEN    16

// コンテクストスイッチのために保存するレジスタ群.
struct context
{
    uint64_t ra;   // return address
    uint64_t sp;   // stack pointer
    uint64_t s0;   // frame pointer
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
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
static inline void cxt_init(context *cxt, void (*entry)(void *), void *arg, void *stack_top);

void     proc_init(void* (*kalloc)(size_t), void (*kfree)(void*));
proc*    proc_alloc(const char* name, void (*entry)(void*), void* arg);
void     scheduler(void);
void     yield(void);
