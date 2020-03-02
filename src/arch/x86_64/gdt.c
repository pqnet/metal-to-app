#include "gdt.h"

#include <stddef.h>
#include <string.h>

#include "memory.h"
#include "frame.h"

static void init_segment_descriptor(
    struct gdt_segment_descriptor *desc,
    enum gdt_segment_type type,
    unsigned privilege_level)
{
  // base should be zero, limit should be -1
  desc->base_high = 0;
  desc->base_low = 0;
  desc->limit_high = -1;
  desc->limit_low = -1;
  desc->type = type;
  desc->descriptor_type = 1; // mem segment
  desc->dpl = privilege_level;
  desc->present = 1;
  desc->operation_size = !(type & gdt_segment_type_code_x);
  desc->granularity = 1;
  desc->long_mode = !!(type & gdt_segment_type_code_x);
  desc->available = 0; // can be used freely by OS
}

static void init_tss_descriptor(struct gdt_tss_descriptor *descriptor, void *tss_base_ptr)
{
  const uintptr_t tss_base = (uintptr_t)tss_base_ptr;
  descriptor->limit_low = 100;
  descriptor->base_low = tss_base & 0xffff;
  descriptor->base_mid = tss_base >> 16 & 0xff;
  descriptor->type = 9;
  descriptor->descriptor_type = 0; // system descriptor
  descriptor->dpl = 0;             // TODO check what is it for
  descriptor->present = 1;
  descriptor->limit_high = 0;
  descriptor->long_mode = 0;
  descriptor->operation_size = 0;
  descriptor->granularity = 0;
  descriptor->base_high = tss_base >> 24 & 0xff;
  descriptor->base_over = tss_base >> 32;
  descriptor->reserved = 0;
}

void start_tss(uint16_t gdt_offset)
{
  asm volatile(
      "ltr %0"
      :
      : "r"(gdt_offset));
}

void load_kernel_stack(void **ist)
{
  asm volatile("mov %%rsp, %0"
               : "=rm"(ist[0]));
  ist[0] -= 32;
}

struct GDT gdt;

struct TSS
{
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint32_t reserved1;
  uint32_t reserved2;
  void *ist1;
  void *ist2;
  void *ist3;
  void *ist4;
  void *ist5;
  void *ist6;
  void *ist7;
  uint32_t reserved3;
  uint32_t reserved4;
} __attribute__((packed));

static_assert(sizeof(struct TSS) == 36 + 64, "Wrong size for TSS");

struct TSS tss;

void init_gdt()
{
  *(uint64_t *)&(gdt.zero) = 0;
  init_segment_descriptor(&gdt.CS64, gdt_segment_type_code_xr, 0);
  init_segment_descriptor(&gdt.DS, gdt_segment_type_data_rw, 0);
  init_segment_descriptor(&gdt.CS32U, gdt_segment_type_code_xr, 0);
  gdt.CS32U.long_mode = 0;
  gdt.CS32U.operation_size = 1;
  init_segment_descriptor(&gdt.DSU, gdt_segment_type_data_rw, 3);
  init_segment_descriptor(&gdt.CS64U, gdt_segment_type_code_xr, 3);
  init_tss_descriptor(&gdt.tss, &tss);
}

void init_tss()
{
  memset(&tss, 0, sizeof(tss));
}

void load_gdt()
{
  struct descriptor_register toLoad;
  toLoad.base = &gdt;
  toLoad.size = sizeof(struct GDT);
  asm volatile(
      "lgdt (%0)\n"
      "mov %1, %%ax\n"
      "movw %%ax, %%ds\n"
      "movw %%ax, %%ss\n"
      "pushq %2\n"
      "pushq $end\n"
      "lretq\n"
      "end:\n"
      :
      :
      "r"(&toLoad.size),
      "i"(offsetof(struct GDT, DS)),
      "i"(offsetof(struct GDT, CS64))
      : "rax", "memory");
}

extern char STACK_BOTTOM[];
void load_tss()
{
  tss.ist1 = &STACK_BOTTOM;
  start_tss(offsetof(struct GDT, tss));
}