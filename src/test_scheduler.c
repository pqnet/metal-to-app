#include "test_scheduler.h"
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "print.h"
#include "scheduler.h"
#include "frame.h"
#include "memory.h"
#include "address_space.h"

struct task task0;
struct task task1;
struct task task2;
struct task task3;

alignas(16) uint64_t stack0[0x200];
alignas(16) uint64_t stack1[0x200];
alignas(16) uint64_t stack2[0x200];
alignas(16) uint64_t stack3[0x200];

noreturn void run_scheduler(void *arg)
{
    schedule(1, &task1, 0);
    schedule(2, &task2, 0);
    schedule(3, &task3, 0);
    for (;;)
    {
        println("I am the scheduler. I run once");
        suspend();
    }
}

noreturn void print_task(void *arg)
{
    char *string = (char *)arg;
    for (;;)
    {
        println(string);
        yield();
    }
}

void test_scheduler()
{
    struct task *task1_data = linearAddressToPtr(frame_alloc_zero());
    struct pagetable_entry *task1_mem = create_address_space();
    void *task_stack_ptr = (void *)0x40000000;
    linear_address task1_stack = frame_alloc_zero();
    mmap(task1_mem, task1_stack, (char *)task_stack_ptr - DEFAULT_FRAME_SIZE, true);

    make_task(
        task1_data,
        task_stack_ptr,
        &print_task,
        "hello task 1",
        NULL,
        NULL,
        pointerToLinearAddres(task1_mem));
    make_task(
        &task2,
        stack2 + sizeof(stack2),
        &print_task,
        "hello task 2",
        NULL,
        NULL,
        0);
    make_task(
        &task3,
        stack3 + sizeof(stack3),
        &print_task,
        "hello task 3",
        NULL,
        NULL,
        0);
    make_task(
        &task0,
        stack0 + sizeof(stack3),
        &run_scheduler,
        NULL,
        NULL,
        NULL,
        0);
    schedule(0, &task0, 0);
    yield();
}