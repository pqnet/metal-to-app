#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include "memory.h"

#define TASK_FLAG_MASK_CAN_EXECUTE 1UL

struct task
{
    alignas(16)
        uint64_t SS;
    void *RSP;
    uint64_t RFLAGS;
    uint64_t CS;
    void *RIP;
    void *rbp;
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
    uint64_t task_flags;
    linear_address address_space;
};
static_assert(offsetof(struct task, task_flags) == 0xa0, "Wrong interrupt frame size");
static_assert(offsetof(struct task, address_space) == 0xa8, "Wrong interrupt frame size");

struct task *make_task(
    struct task *task,
    void *stack,
    void (*entry)(void *),
    void *arg,
    linear_address address_space,
    bool user_mode);

struct scheduling_entry
{
    alignas(16) uint64_t flags;
    struct task *task;
};
static_assert(alignof(struct scheduling_entry) == 16, "scheduling entry should be aligned to 16 byte boundaries");
static_assert(sizeof(struct scheduling_entry) == 16, "scheduling entry should be tighly packed");

struct scheduling_entry schedule(int priority, struct task *task, uint64_t flags);
void init_scheduler();
void yield();
void suspend(struct task* task);
void resume(struct task* task);

#endif // SCHEDULER_H