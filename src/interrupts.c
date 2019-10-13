#include <stdint.h>
#include <string.h>
#include "print.h"
#include "interrupts.h"

extern char IDT;
extern char KERNEL_BASE;
extern char CS64;
extern char GDT;


struct idt_entry {
    unsigned offset_lo : 16;
    unsigned segment_sel : 16;
    unsigned ist: 3;
    unsigned _padding1 : 5;
    unsigned type : 4;
    unsigned _padding2 : 1;
    unsigned dpl : 2;
    unsigned present: 1;
    unsigned offset_mid: 16;
    unsigned offset_high: 32;
    unsigned _reserved: 32;
};
_Static_assert ( (16 == sizeof (struct idt_entry)), "Wrong idt entry size");

struct interrupt_frame {
    void* RIP;
    uint64_t CS;
    uint64_t RFLAGS;
    void* RSP;
    uint64_t SS;
};
_Static_assert ( (40 == sizeof (struct interrupt_frame)), "Wrong interrupt frame size");

void load_interrupt_fn(void(*fnAddr)(),unsigned entry_no) {
    struct idt_entry* entry = (void*)&IDT;
    entry += entry_no;
    memset(entry, 0, sizeof(struct idt_entry));
    uintptr_t offset = (uintptr_t)fnAddr;
    entry->offset_lo = offset & 0xffff;
    entry->offset_mid = (offset >> 16) & 0xffff;
    entry->offset_high = (offset >> 32) & 0xffffffff;
    entry->type = 14; // 14 = interrupt gate, 15 = trap gate
    entry->segment_sel = &CS64 - &GDT;
    entry->ist = 0; // TODO handle stack selection (when coming from userspace)
    entry->dpl = 0; // descriptor privilege level
    entry->present = 1;
}

__attribute__ ((interrupt))
void panic(struct interrupt_frame * frame) {
    println("KERNEL PANIC!");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

void enable_interrupts() {
    asm volatile ("sti": : :"cc");
}

void load_interrupts() {
    load_interrupt_fn(panic, 200);
    enable_interrupts();
}