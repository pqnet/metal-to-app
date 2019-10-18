#include "interrupts.h"
#include "print.h"

__attribute__ ((interrupt))
void double_fault(struct interrupt_frame * frame, uint64_t error_code) {
    println("Double fault!");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}



void load_exceptions() {
    load_interrupt_fn_error(double_fault,8,interrupt_gate);
}