#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include "print.h"
#include "interrupts.h"
#include "exceptions.h"
#include "scheduler.h"

#include "multiboot.h"
#include "memory.h"

#include "initrd.h"
#include "elf.h"
#include "frame.h"
#include "scheduler.h"
#include "userspace.h"

#include "syscall.h"
#include "gdt.h"

noreturn void idle_loop();
noreturn void cstart(struct multiboot_info *multiboot);

noreturn void idle_loop()
{
    for (;;)
    {
        asm volatile(
            "int $201\n"
            "hlt");
    }
}

noreturn void cstart(struct multiboot_info *multiboot)
{
    createKernelAddressSpace();
    setup_frame_allocator(multiboot);

    init_gdt();
    init_tss();

    load_gdt();
    load_tss();

    load_exceptions(); // load default CPU exception handlers into IDT
    load_interrupts(); // load hardware interrupt handlers into IDT and configure PIC
    enable_interrupts();

    init_scheduler();
    setup_syscall();

    struct initrd_info m[10];
    int processes = setup_initrd(multiboot, m);

    for (int i = 0; i < processes; i++) {
        struct task *initrd_task = make_userspace_process(m[i].start, m[i].end, m[i].command_line);
        schedule(i, initrd_task, 0);
    }

    idle_loop();
}
