#ifndef GDT_H
#define GDT_H
#include <assert.h>
#include <stdint.h>
#include <stdalign.h>

enum gdt_segment_type
{
  gdt_segment_type_data_r = 0,
  gdt_segment_type_data_ra = 1,
  gdt_segment_type_data_rw = 2,
  gdt_segment_type_data_rwa = 3,
  gdt_segment_type_data_rd = 4,
  gdt_segment_type_data_rda = 5,
  gdt_segment_type_data_rwd = 6,
  gdt_segment_type_data_rwda = 7,
  gdt_segment_type_code_x = 8,
  gdt_segment_type_code_xa = 9,
  gdt_segment_type_code_xr = 10,
  gdt_segment_type_code_xra = 11,
  gdt_segment_type_code_xc = 12,
  gdt_segment_type_code_xca = 13,
  gdt_segment_type_code_xrc = 14,
  gdt_segment_type_code_xrca = 15,
};

struct gdt_segment_descriptor
{
  uint16_t limit_low : 16; // 0
  uint16_t base_low;       // 16
  uint8_t base_mid : 8;    // 16
  unsigned type : 4;       // 40
  unsigned descriptor_type : 1;
  unsigned dpl : 2;
  unsigned present : 1;
  unsigned limit_high : 4; // 48
  unsigned available : 1;
  unsigned long_mode : 1;
  unsigned operation_size : 1;
  unsigned granularity : 1;
  uint8_t base_high : 8; // 56
} __attribute__((packed));
static_assert(sizeof(struct gdt_segment_descriptor) == 8, "Wrong size for gdt_segment_descriptor");

struct gdt_tss_descriptor
{
  uint16_t limit_low : 16; // 0
  uint16_t base_low;       // 16
  uint8_t base_mid : 8;    // 16
  unsigned type : 4;       // 40
  unsigned descriptor_type : 1;
  unsigned dpl : 2;
  unsigned present : 1;
  unsigned limit_high : 4; // 48
  unsigned available : 1;
  unsigned long_mode : 1;
  unsigned operation_size : 1;
  unsigned granularity : 1;
  uint8_t base_high : 8; // 56
  uint32_t base_over;    // 64
  uint32_t reserved;     // 96
} __attribute__((packed));
static_assert(sizeof(struct gdt_tss_descriptor) == 16, "Wrong size for gdt_tss_descriptor");


struct GDT
{
  struct gdt_segment_descriptor zero;
  struct gdt_segment_descriptor CS64;
  struct gdt_segment_descriptor DS;
  struct gdt_segment_descriptor CS32U;
  struct gdt_segment_descriptor DSU;
  struct gdt_segment_descriptor CS64U;
  struct gdt_tss_descriptor tss;
} __attribute__((packed));

struct descriptor_register
{
  alignas(8) uint32_t _padding1;
  uint16_t _padding2;
  uint16_t size;
  void *base;
} __attribute__((packed));
static_assert(sizeof(struct descriptor_register) == 16, "Wrong size for descriptor_register");

extern struct GDT gdt;

void init_gdt();
void init_tss();
void load_gdt();
void load_tss();

#endif // GDT_H
