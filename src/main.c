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
#include "userspace.h"

#include "tss.h"

noreturn void idle_loop();
noreturn void idle_loop()
{
    for (;;)
    {
        println("Idle loop");
        asm volatile("hlt");
    }
}

noreturn void cstart(struct multiboot_info *multiboot);
noreturn void cstart(struct multiboot_info *multiboot)
{
    createKernelAddressSpace();
    load_exceptions(); // load default CPU exception handlers into IDT
    load_interrupts(); // load hardware interrupt handlers into IDT and configure PIC
    load_tss();
    load_ist0();
    enable_interrupts();

    setup_frame_allocator(multiboot);

    init_scheduler();
    
    struct initrd_info m;
    setup_initrd(multiboot,&m);

    struct task* initrd_task =  make_userspace_process(m.start,m.end);
    schedule(0,initrd_task, 0);
    

    yield();
    
    // test_scheduler();
    idle_loop();
}
