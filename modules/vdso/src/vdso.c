
#include "vdso.h"

int64_t syscall(
    uint64_t syscall_number,
    uint64_t arg0,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5)
{
  asm volatile(
      "mov %1, %%r10\n"
      "mov %5, %%r8\n"
      "mov %6, %%r9\n"
      "syscall\n"
      : "+a"(syscall_number), "+c"(arg3)
      : "D"(arg0), "S"(arg1), "d"(arg2), "r"(arg4), "r"(arg5)
      : "r8", "r9", "r10", "r11");
  return syscall_number;
}

void abort()
{
  syscall(0x11, 0, 0, 0, 0, 0, 0);
  while(1){}
}

void _exit(int status)
{
  syscall(0x01, status, 0, 0, 0, 0, 0);
  while(1){}
}

int64_t write(int fd, const void *buf, size_t count)
{
  return syscall(0x04, fd, (uint64_t)buf, count, 0, 0, 0);
}

void yield() {
  syscall(0x99, 0, 0, 0, 0, 0, 0);
}