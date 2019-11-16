#include "scheduler.h"
#include <stdnoreturn.h>
#include <assert.h>
#include <stddef.h>

#include "interrupts.h"

struct scheduling_entry
{
    alignas(16)
        uint64_t flags;
    struct task *task;
};
static_assert(alignof(struct scheduling_entry) == 16, "scheduling entry should be aligned to 16 byte boundaries");
static_assert(sizeof(struct scheduling_entry) == 16, "scheduling entry should be tighly packed");

struct task *current_task_table[16];
struct scheduling_entry scheduling_vector[16][256];

struct task *get_current_task(uint64_t cpu_id)
{
    return current_task_table[cpu_id];
}

struct task idle_loop_task;

void start_task(
    uint64_t (*entry)(void *),
    void *arg,
    void (*cleanup)(void *, struct task *, uint64_t),
    void *cleanup_arg,
    struct task *task)
{
    uint64_t res = entry(arg);
    cleanup(cleanup_arg, task, res);
}

uint64_t default_rflags = 0x246; // TODO

struct task *make_task(
    struct task *task,
    void *stack,
    void (*entry)(void *),
    void *arg,
    void (*cleanup)(void *, struct task *, uint64_t),
    void *cleanup_arg)
{
    task->RIP = start_task;
    task->CS = 0x10;//get_current_task(0)->CS;
    task->RFLAGS = default_rflags;
    task->SS = 0x18;//get_current_task(0)->SS;
    task->RSP = stack;
    task->rdi = (uintptr_t)entry;
    task->rsi = (uintptr_t)arg;
    task->rdx = (uintptr_t)cleanup;
    task->rcx = (uintptr_t)cleanup_arg;
    task->r8 = (uintptr_t)task;
    return task;
}

__attribute__((interrupt)) extern void asm_reschedule(struct interrupt_frame *frame);

struct task *scheduler(uint64_t cpu_id)
{
    struct scheduling_entry *scheduling_v = &scheduling_vector[cpu_id][0];
    struct task *next_task = NULL;
    for (int16_t pos = -128; pos < 0; pos++)
    {
        next_task = scheduling_v[128 + pos].task;
        if (next_task != NULL)
        {
            break;
        }
    }
    if (next_task == 0)
    {
        return &idle_loop_task;
    }
    return next_task;
}

void init_scheduler()
{
    current_task_table[0] = &idle_loop_task;
    /*for(int i = 0; i < 16; i++) {
        for (int j = 0; j < 256; j++) {
            scheduling_vector[i][j] = NULL;
        }
    }*/
    load_interrupt_fn(asm_reschedule, 201, trap_gate);
}
void yield()
{
    asm volatile(
        "sti\n"
        "int $201\n" ::
            : "cc");
}

struct task* schedule(int priority, struct task* task, uint64_t flags) {
    struct task* ret = scheduling_vector[0][128 + priority].task;
    scheduling_vector[0][128 + priority].task = task;
    scheduling_vector[0][128 + priority].flags = flags;
    return ret;
}
