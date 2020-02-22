#include <stddef.h>

#include "tss.h"
#include "descriptor.h"

extern char GDT[1];
extern char IST[1];
extern char TSS[1];
extern char TSSD[1];
extern char KERNEL_BASE[1];

void load_tss()
{
  struct descriptor_large *tss_descriptor = (struct descriptor_large *)(&KERNEL_BASE + (ptrdiff_t)&TSSD);
  const uintptr_t tss_base = (uintptr_t)&KERNEL_BASE + (ptrdiff_t)&TSS;
  tss_descriptor->limit_low = 100;
  tss_descriptor->base_low = tss_base & 0xffff;
  tss_descriptor->base_mid = tss_base >> 16 & 0xff;
  tss_descriptor->type = 9;
  tss_descriptor->descriptor_type = 0; // system descriptor
  tss_descriptor->dpl = 0;             // TODO check what is it for
  tss_descriptor->present = 1;
  tss_descriptor->limit_high = 0;
  tss_descriptor->long_mode = 0;
  tss_descriptor->operation_size = 0;
  tss_descriptor->granularity = 0;
  tss_descriptor->base_high = tss_base >> 24 & 0xff;
  tss_descriptor->base_over = tss_base >> 32;
  tss_descriptor->reserved = 0;

  uint16_t idt_offset = &TSSD - &GDT; // offset inside GDT
  asm volatile(
      "ltr %0"
      :
      : "r"(idt_offset));
}

void load_ist0()
{
  void **ist = (void **)(&KERNEL_BASE + (ptrdiff_t)&IST);
  asm volatile("mov %%rsp, %0"
               : "=rm"(ist[0]));
  ist[0] -= 32;
  // printhex((uint64_t)ist[0]);
}