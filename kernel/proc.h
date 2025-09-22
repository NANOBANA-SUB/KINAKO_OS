#pragma once

#include "types.h"

// --- 調整パラメータ ---
#define NPROC        16
#define KSTACK_SIZE  8192
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
typedef struct context context;

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
typedef enum procstate procstate;

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
    uint8_t kstack[KSTACK_SIZE];

    // コンテクスト(コンテクストスイッチで保存・復元するレジスタ)
    context     ctx;

    // 親子関係
    struct proc* parent;

    // ユーザデータ
    void*       arg;

    // リンク
    struct proc* next;

    // コンテクストスイッチ時のスタックポインタ
    vaddr_t sp;
};
typedef struct proc proc;

struct proctable
{
    proc    procs[NPROC];
    proc*   free_list;      // 未使用リスト
    proc*   run_queue_head; // 単純なFIFO
    proc*   run_queue_tail;
    proc*   current;        // 実行中
    int     next_pid;
};
typedef struct proctable proctable;

extern proctable g_proctab;
extern proc procs[NPROC];

// API
struct proc *create_process(uint64_t pc);
void context_switch(uint64_t *prev_sp, uint64_t *next_sp);