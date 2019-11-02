#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>
#include <assert.h>

struct interrupt_frame {
    void* RIP;
    uint64_t CS;
    uint64_t RFLAGS;
    void* RSP;
    uint64_t SS;
};
static_assert ( sizeof (struct interrupt_frame) == 40, "Wrong interrupt frame size");

enum gate_type {
    interrupt_gate = 14,
    trap_gate = 15,
};
void load_interrupt_fn(void(*fnAddr)(struct interrupt_frame* frame),unsigned entry_no, enum gate_type type);
static inline void load_interrupt_fn_error(void(*fnAddr)(struct interrupt_frame* frame, uint64_t error_code),unsigned entry_no, enum gate_type type) {
    load_interrupt_fn((void(*)(struct interrupt_frame* frame))fnAddr,entry_no, type);
};
void load_interrupts();
void enable_interrupts();

#endif // INTERRUPTS_H