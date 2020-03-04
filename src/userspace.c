#include "userspace.h"

#include <stddef.h>
#include <string.h>

#include "interrupts.h"
#include "elf.h"
#include "frame.h"
#include "scheduler.h"
#include "memory.h"
#include "address_space.h"
#include "gdt.h"

/*
__attribute__((interrupt))
void ih_switch_to_usermode(struct interrupt_frame * frame) {
  frame->CS = &CS64U - &GDT + 3;
  frame->SS = &DS32U - &GDT + 3;
}
*/
void setup(void *t, struct task *cleanup_task)
{
}
void cleanup(void *t, struct task *cleanup_task, uint64_t retval)
{
  struct pagetable_entry *address_space = linearAddressToPtr(cleanup_task->address_space);
  linear_address stack = munmap(address_space, (void *)(0x40000000));
  destroy_address_space(address_space);
  frame_dealloc(stack);
  frame_dealloc(pointerToLinearAddress(cleanup_task));
}

struct task *make_userspace_process(char *elfStart, char *elfEnd, char *command_line)
{
  struct pagetable_entry *initrd_address_space = create_address_space();
  void *entry = load_elf(elfStart, elfEnd, initrd_address_space);
  void *initrd_stack_addr = (void *)(0x7FFFFFFFF000);
  linear_address initrd_stack = frame_alloc_zero();
  struct task *ret = linearAddressToPtr(frame_alloc_zero());
  mmap(initrd_address_space, initrd_stack, (char *)initrd_stack_addr - DEFAULT_FRAME_SIZE, true);
  // copy command line to the bottom of the stack
  unsigned argSize = strlen(command_line);
  unsigned paddedArgSize = (argSize + 15) /16 * 16;
  memcpy(linearAddressToPtr(initrd_stack + DEFAULT_FRAME_SIZE - paddedArgSize), command_line, argSize+1);
  make_task(ret, initrd_stack_addr - paddedArgSize, entry, initrd_stack_addr - paddedArgSize, pointerToLinearAddress(initrd_address_space), true);
  return ret;
}