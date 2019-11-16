#include <assert.h>
#include <stdalign.h>
#include <stdint.h>

struct task {
    alignas(16)
    uint64_t SS;
    void* RSP;
    uint64_t RFLAGS;
    uint64_t CS;
    void* RIP;
    // no hole
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

struct task* make_task (
    struct task* task,
    void* stack,
    void (*entry)(void*),
    void* arg,
    void(*cleanup)(void*,struct task*, uint64_t),
    void* cleanup_arg);

struct task* schedule(int priority, struct task* task, uint64_t flags);
void init_scheduler();
void yield();