#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

struct descriptor_small {
  uint16_t limit_low: 16; // 0
  uint16_t base_low; // 16
  uint8_t  base_mid: 8; // 16
  unsigned type: 4; // 40
  unsigned descriptor_type: 1;
  unsigned dpl: 2;
  unsigned present: 1;
  unsigned limit_high: 4; // 48
  unsigned available: 1;
  unsigned long_mode: 1;
  unsigned operation_size: 1;
  unsigned granularity: 1;
  uint8_t  base_high :8; // 56
} __attribute__((packed));

struct descriptor_large {
  uint16_t limit_low: 16; // 0
  uint16_t base_low; // 16
  uint8_t  base_mid: 8; // 16
  unsigned type: 4; // 40
  unsigned descriptor_type: 1;
  unsigned dpl: 2;
  unsigned present: 1;
  unsigned limit_high: 4; // 48
  unsigned available: 1;
  unsigned long_mode: 1;
  unsigned operation_size: 1;
  unsigned granularity: 1;
  uint8_t  base_high :8; // 56
  uint32_t base_over; // 64
  uint32_t reserved; // 96
} __attribute__((packed));

static_assert(sizeof (struct descriptor_small) == 8, "Wrong size for descriptor_small");
static_assert(sizeof (struct descriptor_large) == 16, "Wrong size for descriptor_large");
enum mem_segment_type {
  st_data_r = 0,
  st_data_rw = 2,
  st_code_x = 8,
  st_code_xr = 10,
  st_code_xc = 12,
  st_code_xrc = 14,
};

void init_long_mem_descriptor(struct descriptor_small* desc, enum mem_segment_type type, unsigned privilege_level) {
  // base should be zero, limit should be -1
  desc->base_high = 0;
  desc->base_low = 0;
  desc->limit_high = -1;
  desc->limit_low = -1;
  desc->type = type;
  desc->descriptor_type = 1; // mem segment
  desc->dpl = privilege_level;
  desc->present = 1;
  desc->operation_size = !(type & st_code_x);
  desc->granularity = 1;
  desc->long_mode = (type & st_code_x);
  desc->available = 0; // can be used freely by OS
}


#endif // DESCRIPTOR_H