#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "print.h"
#include "interrupts.h"
#include "exceptions.h"
#include "scheduler.h"

#include "test_scheduler.h"
#include "multiboot.h"
#include "memory.h"

#include "initrd.h"
#include "elf.h"
#include "frame.h"
#include "scheduler.h"

noreturn void idle_loop();
noreturn void idle_loop()
{
    for (;;)
    {
        println("Idle loop");
        asm volatile("hlt");
    }
}

struct task initrd_task;

noreturn void cstart(struct multiboot_info *multiboot);
noreturn void cstart(struct multiboot_info *multiboot)
{
    createKernelAddressSpace();
    load_exceptions(); // load default CPU exception handlers into IDT
    load_interrupts(); // load hardware interrupt handlers into IDT and configure PIC
    enable_interrupts();    
    setup_frame_allocator(multiboot);

    init_scheduler();

    struct initrd_info m;
    setup_initrd(multiboot,&m);

    struct pagetable_entry* initrd_address_space = create_address_space();
    void* entry = load_elf(m.start,m.end,initrd_address_space);
    void* initrd_stack_addr = (void*)(0x40000000);
    linear_address initrd_stack = frame_alloc_zero();
    mmap(initrd_address_space,initrd_stack,(char*)initrd_stack_addr - DEFAULT_FRAME_SIZE, true);
    make_task(&initrd_task,initrd_stack_addr,entry,NULL,NULL,NULL,pointerToLinearAddres(initrd_address_space));
    schedule(0,&initrd_task, 0);
    yield();
    
    // test_scheduler();
    idle_loop();
}
