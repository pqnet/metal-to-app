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
    enable_interrupts();
    setup_frame_allocator(multiboot);

    init_scheduler();

    test_scheduler();
    idle_loop();
}
