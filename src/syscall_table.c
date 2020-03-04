#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "print.h"
#include "interrupts.h"
#include "scheduler.h"
#include "syscall.h"


typedef uint64_t (*syscall_signature)();

static_assert(sizeof(syscall_signature) == 8, "Expected sizeof(syscall_signature) == 8");

syscall_signature syscall_table[256];

uint64_t syscall_write(uint64_t fd, char *buffer, uint64_t len)
{
  println(buffer);
  return 0;
}

uint64_t syscall_abort()
{
  println("We should abort current process. We can't.");
  panic();
}

uint64_t syscall_yield() {
  yield();
  return 0;
}

uint64_t syscall_exit(uint64_t error_code)
{
  // TODO implement
  println("Exit");
  suspend(NULL);
  return 0;
}

void load_syscall_table()
{
  syscall_table[0x01] = syscall_exit;
  syscall_table[0x11] = syscall_abort;
  syscall_table[0x04] = syscall_write;
  syscall_table[0x99] = syscall_yield;
}
