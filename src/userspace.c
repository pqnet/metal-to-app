#include "userspace.h"
#include "interrupts.h"
#include "elf.h"
#include "frame.h"
#include "scheduler.h"
#include "memory.h"
#include "address_space.h"

extern char GDT[1];
extern char CS64U[1];
extern char DS32U[1];

/*
__attribute__((interrupt))
void ih_switch_to_usermode(struct interrupt_frame * frame) {
  frame->CS = &CS64U - &GDT + 3;
  frame->SS = &DS32U - &GDT + 3;
}
*/
void cleanup(void* this, struct task *cleanup_task, uint64_t retval)
{
  struct pagetable_entry* address_space = linearAddressToPtr(cleanup_task->address_space);
  linear_address stack = munmap(address_space, (void *)(0x40000000));
  destroy_address_space(address_space);
  frame_dealloc(stack);
  frame_dealloc(pointerToLinearAddress(cleanup_task));
}

struct task *make_userspace_process(char *elfStart, char *elfEnd)
{
  struct pagetable_entry *initrd_address_space = create_address_space();
  void *entry = load_elf(elfStart, elfEnd, initrd_address_space);
  void *initrd_stack_addr = (void *)(0x40000000);
  linear_address initrd_stack = frame_alloc_zero();
  struct task *ret = linearAddressToPtr(frame_alloc_zero());
  mmap(initrd_address_space, initrd_stack, (char *)initrd_stack_addr - DEFAULT_FRAME_SIZE, true);
  make_task(ret, initrd_stack_addr - 8, entry, NULL, NULL /* cleanup */, NULL, pointerToLinearAddress(initrd_address_space));
  ret->SS = (&DS32U - &GDT) + 3;
  ret->CS = (&CS64U - &GDT) + 3;
  return ret;
}