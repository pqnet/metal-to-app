#include "scheduler.h"
#include <stdnoreturn.h>
#include <assert.h>
#include <stdalign.h>
#include <stddef.h>

#include "interrupts.h"

struct task {
    alignas(16)
    void* RIP;
    uint64_t CS;
    uint64_t RFLAGS;
    void* RSP;
    uint64_t SS;
    void* rbp;
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t scheduler_info[2];
};
static_assert ( sizeof (struct task) == 0xb0, "Wrong interrupt frame size");
struct scheduling_entry {
    alignas(16)
    uint64_t flags;
    struct task* task;
};
static_assert(alignof(struct scheduling_entry) == 16, "scheduling entry should be aligned to 16 byte boundaries");
static_assert(sizeof(struct scheduling_entry) == 16, "scheduling entry should be tighly packed");


struct task* current_task_table[16];
struct scheduling_entry scheduling_vector[16][256];

struct task* get_current_task(uint64_t cpu_id) {
    return current_task_table[cpu_id];
}

struct task idle_loop_task;
void start_task (
    uint64_t(*entry)(void*),
    void* arg,
    void(*cleanup)(void*, struct task*, uint64_t),
    void* cleanup_arg,
    struct task* task) {
    cleanup(cleanup_arg, task, entry(arg));
}

uint64_t default_rflags = 0; // TODO


void make_task (
    struct task* task,
    void* stack,
    void (*entry)(void*),
    void* arg,
    void(*cleanup)(void*,struct task*, uint64_t),
    void* cleanup_arg) {
    task->RIP = start_task;
    task->CS = get_current_task(0)->CS;
    task->RFLAGS = default_rflags;
    task->SS = get_current_task(0)->SS;
    task->RSP = stack;
    task->rdi = entry;
    task->rsi = arg;
    task->rdx = cleanup;
    task->rcx = cleanup_arg;
    task->r8 = task;
}


__attribute__ ((interrupt))
extern void asm_reschedule(struct interrupt_frame* frame);


struct task* scheduler(uint64_t cpu_id) {
    struct scheduling_entry* scheduling_v = &scheduling_vector[cpu_id][0];
    struct task* next_task = NULL; 
    for(int16_t pos = -128; pos < 0; pos++) {
        next_task = scheduling_v[pos].task;
        if (next_task != NULL) {
            break;
        }
    }
    if (next_task == 0) {
        return &idle_loop_task;
    }
    return next_task;
}

void init_scheduler() {
    load_interrupt_fn(asm_reschedule, 201, trap_gate);
}