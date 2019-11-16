#include "test_scheduler.h"
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "print.h"
#include "scheduler.h"

struct task task1;
struct task task2;
struct task task3;

alignas(16) uint64_t stack1[0x200];
alignas(16) uint64_t stack2[0x200];
alignas(16) uint64_t stack3[0x200];
alignas(16) uint64_t stack4[0x200];

static void run_scheduler() {
    struct task* oldTask = schedule(-1, NULL, 0);
    if (oldTask== &task1) {
        schedule(-1, &task2, 0);
    } else if (oldTask == &task2) {
        schedule(-1, &task3, 0);
    } else if (oldTask == &task3) {
        schedule(-1, &task1, 0);
    }
    yield();
}
noreturn void print_task(void* arg) {
    char* string = (char*)arg;
    for (;;) {
        println(string);
        run_scheduler();
    }
}

void test_scheduler() {
        make_task(
        &task1,
        stack1 + sizeof(stack1),
        &print_task,
        "hello task 1",
        NULL,
        NULL);
    make_task(
        &task2,
        stack2 + sizeof(stack2),
        &print_task,
        "hello task 2",
        NULL,
        NULL);
    make_task(
        &task3,
        stack3 + sizeof(stack3),
        &print_task,
        "hello task 3",
        NULL,
        NULL);
    
    schedule(-1, &task1,0);
    yield();
}