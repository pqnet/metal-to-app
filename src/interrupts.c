#include <stdint.h>
#include <string.h>
#include "print.h"
#include "interrupts.h"
#include "exceptions.h"
#include "portio.h"

extern char IDT[1];
extern char KERNEL_BASE[1];
extern char CS64[1];
extern char GDT[1];


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

void load_interrupt_fn(void(*fnAddr)(struct interrupt_frame*),unsigned entry_no, enum gate_type type) {
    struct idt_entry* entry = (void*)IDT + (uintptr_t)KERNEL_BASE;
    entry += entry_no;
    memset(entry, 0, sizeof(struct idt_entry));
    uintptr_t offset = (uintptr_t)fnAddr;
    entry->offset_lo = offset & 0xffff;
    entry->offset_mid = (offset >> 16) & 0xffff;
    entry->offset_high = (offset >> 32) & 0xffffffff;
    entry->type = type;
    entry->segment_sel = &CS64 - &GDT;
    entry->ist = 0; // TODO handle stack selection (when coming from userspace)
    entry->dpl = 0; // descriptor privilege level
    entry->present = 1;
}

__attribute__ ((interrupt))
void ih_panic(struct interrupt_frame * frame) {
    println("KERNEL PANIC!");
    // no need to loop here, because interrupts are disabled as this is set as interrupt gate
    asm volatile ("hlt");
}

static inline void eoi(unsigned char irqNum) {
    outb(0x20,0x20);

    if(irqNum > 8) {
        outb(0xa0,0x20);
    }
}
static int timerCnt = 0;

__attribute__ ((interrupt))
void irq0(struct interrupt_frame * frame) {
    if(timerCnt++ % 182 == 0) { // 1 second == 18.2065 ticks
        println("timer");
    }
    eoi(0);
}
__attribute__ ((interrupt))
void irq1(struct interrupt_frame * frame) {
    println("keyboard");
    eoi(1);
}
__attribute__ ((interrupt))
void irq3(struct interrupt_frame * frame) {
    println("COM2/4");
    eoi(3);
}
__attribute__ ((interrupt))
void irq4(struct interrupt_frame * frame) {
    println("COM1/3");
    eoi(4);
}
__attribute__ ((interrupt))
void irq5(struct interrupt_frame * frame) {
    println("LPT2");
    eoi(5);
}
__attribute__ ((interrupt))
void irq6(struct interrupt_frame * frame) {
    println("FLOPPY");
    eoi(6);
}
__attribute__ ((interrupt))
void irq7(struct interrupt_frame * frame) {
    println("LPT1/spurious");
    eoi(7);
}
__attribute__ ((interrupt))
void irq8(struct interrupt_frame * frame) {
    println("CMOS RTC");
    eoi(8);
}
__attribute__ ((interrupt))
void irq9(struct interrupt_frame * frame) {
    println("free/scsi/nic");
    eoi(9);
}
__attribute__ ((interrupt))
void irq10(struct interrupt_frame * frame) {
    println("free/scsi/nic");
    eoi(10);
}
__attribute__ ((interrupt))
void irq11(struct interrupt_frame * frame) {
    println("free/scsi/nic");
    eoi(11);
}
__attribute__ ((interrupt))
void irq12(struct interrupt_frame * frame) {
    println("mouse");
    eoi(12);
}
__attribute__ ((interrupt))
void irq13(struct interrupt_frame * frame) {
    println("fpu/coprocessor");
    eoi(13);
}
__attribute__ ((interrupt))
void irq14(struct interrupt_frame * frame) {
    println("primary ATA");
    eoi(14);
}
__attribute__ ((interrupt))
void irq15(struct interrupt_frame * frame) {
    println("secondary ATA");
    eoi(15);
}

void enable_interrupts() {
    // start initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // give offset in vector table
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // let them know each other
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // more information (8086 mode)
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // set mask
    outb(0x21, 0xff);
    outb(0xA1, 0xff);

    asm volatile ("sti": : :"cc");
}

void load_interrupts() {
    load_interrupt_fn(irq0, 0x20, interrupt_gate);
    load_interrupt_fn(irq1, 0x21, interrupt_gate);
    // load_interrupt_fn(irq2, 0x22, interrupt_gate);
    load_interrupt_fn(irq3, 0x23, interrupt_gate);
    load_interrupt_fn(irq4, 0x24, interrupt_gate);
    load_interrupt_fn(irq5, 0x25, interrupt_gate);
    load_interrupt_fn(irq6, 0x26, interrupt_gate);
    load_interrupt_fn(irq7, 0x27, interrupt_gate);
    load_interrupt_fn(irq8, 0x28, interrupt_gate);
    load_interrupt_fn(irq9, 0x29, interrupt_gate);
    load_interrupt_fn(irq10, 0x2a, interrupt_gate);
    load_interrupt_fn(irq11, 0x2b, interrupt_gate);
    load_interrupt_fn(irq12, 0x2c, interrupt_gate);
    load_interrupt_fn(irq13, 0x2d, interrupt_gate);
    load_interrupt_fn(irq14, 0x2e, interrupt_gate);
    load_interrupt_fn(irq15, 0x2f, interrupt_gate);
    load_interrupt_fn(ih_panic, 200, interrupt_gate);
}