#ifndef IDT_H
#define IDT_H
#include <assert.h>

struct idt_entry
{
    unsigned offset_lo : 16;
    unsigned segment_sel : 16;
    unsigned ist : 3;
    unsigned _padding1 : 5;
    unsigned type : 4;
    unsigned _padding2 : 1;
    unsigned dpl : 2;
    unsigned present : 1;
    unsigned offset_mid : 16;
    unsigned offset_high : 32;
    unsigned _reserved : 32;
};
static_assert((16 == sizeof(struct idt_entry)), "Wrong idt_entry size");

#endif // IDT_H