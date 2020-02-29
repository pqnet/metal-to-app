#include <stdint.h>
#include "syscall.h"

extern void syscall_entry_asm();
extern char CS64[1];
extern char CS32U[1];
extern char GDT[1];

static inline void writemsr(uint32_t regaddr, uint64_t value)
{
  uint32_t value_low = value & 0xffffffff;
  uint32_t value_high = (value >> 32) & 0xffffffff;
  asm volatile(
      "wrmsr"
      :
      : "c"(regaddr), "a"(value_low), "d"(value_high));
}
static inline uint64_t readmsr(uint32_t regaddr)
{
  uint32_t value_low, value_high;
  asm volatile("rdmsr"
               : "=a"(value_low), "=d"(value_high)
               : "c"(regaddr));
  return ((uint64_t)value_high << 32LU) + value_low;
}

void setup_syscall()
{
  // IA32_EFER.SCE = 1 done in bootstrap
  const uint32_t STAR_ADDR = 0xC0000081;
  const uint32_t LSTAR_ADDR = 0xC0000082;
  const uint32_t SFMASK_ADDR = 0xC0000084;
  uint64_t star = 0;
  // user code and stack segment used by SYSRET at 48:63
  star += ((&CS32U - &GDT) & 0xffff);
  star <<= 16;
  // kernel code and stack segment used by SYSCALL at 32:47
  star += (&CS64 - &GDT) & 0xffff;
  star <<= 32;
  star += readmsr(STAR_ADDR) & 0xffffffff;
  writemsr(STAR_ADDR, star);
  writemsr(LSTAR_ADDR, (uint64_t)syscall_entry_asm);
  writemsr(SFMASK_ADDR, readmsr(SFMASK_ADDR) & 0xffffffff00000000);
  // fill in the callback table
  load_syscall_table();
}
