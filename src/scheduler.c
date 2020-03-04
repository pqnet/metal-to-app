#include "scheduler.h"
#include <stdnoreturn.h>
#include <assert.h>
#include <stddef.h>

#include "interrupts.h"
#include "memory.h"
#include "gdt.h"

static const uint64_t MAX_SUPPORTED_CPUS = 16;

struct task *current_task_table[MAX_SUPPORTED_CPUS];

struct scheduling_entry scheduling_vector[MAX_SUPPORTED_CPUS][256];

struct task *get_current_task(uint64_t cpu_id)
{
    return current_task_table[cpu_id];
}

struct task idle_loop_task;

const uint64_t default_rflags = 0x246; // TODO

struct task *make_task(
    struct task *task,
    void *stack,
    void (*entry)(void *),
    void *arg,
    linear_address address_space,
    bool user_mode)
{
    task->RIP = entry;
    task->CS = user_mode ? offsetof(struct GDT, CS64U) + 3 : offsetof(struct GDT, CS64);
    task->RFLAGS = default_rflags;
    task->SS = user_mode ? offsetof(struct GDT, DSU) + 3 : offsetof(struct GDT, DS);
    task->RSP = stack;
    task->rdi = (uintptr_t)arg;
    task->task_flags = TASK_FLAG_MASK_CAN_EXECUTE;
    task->address_space = address_space ? address_space : kernel_address_space;
    return task;
}

__attribute__((interrupt)) extern void asm_reschedule(struct interrupt_frame *frame);
extern uint16_t asm_get_cpuid();

static inline struct task *get_if_can_execute(struct scheduling_entry *entry)
{
    if (entry->task != NULL &&
        (entry->task->task_flags & TASK_FLAG_MASK_CAN_EXECUTE) == TASK_FLAG_MASK_CAN_EXECUTE)
    {
        return entry->task;
    }
    return NULL;
}
struct task *scheduler(uint64_t cpu_id)
{
    struct scheduling_entry *scheduling_v = &scheduling_vector[cpu_id][0];
    struct task *next_task = NULL;
    static int16_t round_schedule;
    for (int16_t pos = -128; pos < 0; pos++)
    {
        next_task = get_if_can_execute(scheduling_v + 128 + pos);
        if (next_task != NULL)
        {
            return current_task_table[cpu_id] = next_task;
        }
    }
    for (int16_t pos = round_schedule % 128; pos != (round_schedule + 127) % 128; pos = (pos + 1) % 128)
    {
        // TODO grab a task atomically to prevent other threads to run it
        next_task = get_if_can_execute(scheduling_v + 128 + pos);
        if (next_task != NULL)
        {
            round_schedule = pos + 1;
            return current_task_table[cpu_id] = next_task;
        }
    }

    return current_task_table[cpu_id] = &idle_loop_task;
}

void suspend(struct task *suspend_task)
{

    uint16_t cpu_id = asm_get_cpuid();
    if (suspend_task == NULL)
    {
        suspend_task = get_current_task(cpu_id);
    }
    else
    {
        // TODO worry about atomicity when the task may be running on another CPU
        // and signal that cpu to yield the task after marking it as not executable
    }
    suspend_task->task_flags |= TASK_FLAG_MASK_CAN_EXECUTE;
    suspend_task->task_flags ^= TASK_FLAG_MASK_CAN_EXECUTE;
    yield();
}

void init_scheduler()
{
    idle_loop_task.address_space = kernel_address_space;
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
        "int $201\n" ::
            : "cc");
}

void resume(struct task *resume_task)
{
    resume_task->task_flags |= TASK_FLAG_MASK_CAN_EXECUTE;
}

struct scheduling_entry schedule(int priority, struct task *task, uint64_t flags)
{
    struct scheduling_entry ret = scheduling_vector[0][128 + priority];
    scheduling_vector[0][128 + priority].task = task;
    scheduling_vector[0][128 + priority].flags = flags;
    return ret;
}
