#include "interrupts.h"
#include "print.h"

__attribute__ ((interrupt))
void double_fault(struct interrupt_frame * frame, uint64_t error_code) {
    println("Double fault!");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void invalid_tss(struct interrupt_frame * frame, uint64_t error_code) {
    print("Invalid TSS code 0x");
    printhex(error_code);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void general_protection(struct interrupt_frame * frame, uint64_t error_code) {
    print("General protection fault code 0x");
    printhex(error_code);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void stack_segment_fault(struct interrupt_frame * frame, uint64_t error_code) {
    print("Stack-Segment Fault code 0x");
    printhex(error_code);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void segment_not_present(struct interrupt_frame * frame, uint64_t error_code) {
    print("Segment not present fault code 0x");
    printhex(error_code);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void page_fault(struct interrupt_frame * frame, uint64_t error_code) {
    print("Page fault code 0x");
    printhex(error_code);

    print(", address 0x");
    uint64_t cr2;
    asm volatile("mov %%cr2, %0": "=r"(cr2));
    printhex(cr2);

    print(", instruction: 0x");
    printhex((uintptr_t)frame->RIP);
    println("");

    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void invalid_opcode(struct interrupt_frame * frame) {
    print("Invalid opcode at 0x");
    printhex((uintptr_t)frame->RIP);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

__attribute__ ((interrupt))
void alignment_check(struct interrupt_frame * frame, uint64_t error_code) {
    print("Alignment check failed code 0x");
    printhex(error_code);
    println("");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

void load_exceptions() {
    load_interrupt_fn(invalid_opcode,6,interrupt_gate);
    load_interrupt_fn_error(double_fault,8,interrupt_gate);
    load_interrupt_fn_error(invalid_tss,10,interrupt_gate);
    load_interrupt_fn_error(segment_not_present,11,interrupt_gate);
    load_interrupt_fn_error(stack_segment_fault,12,interrupt_gate);
    load_interrupt_fn_error(general_protection,13,interrupt_gate);
    load_interrupt_fn_error(page_fault,14,interrupt_gate);
    load_interrupt_fn_error(alignment_check,17,interrupt_gate);
}