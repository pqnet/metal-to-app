#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include "print.h"
#include "interrupts.h"
#include "exceptions.h"
#include "scheduler.h"
#include "frame.h"

#include "test_scheduler.h"

struct multiboot_info{
    uint32_t flags; // 0
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline; // 16
    uint32_t mods_count;
    uint32_t mods_addr; // 24
    uint32_t syms[4]; // 28
    uint32_t mmap_length; // 44
    uint32_t mmap_addr; // 48
};

noreturn void idle_loop();
noreturn void idle_loop() {
    for(;;) {
        println("Idle loop");
        asm volatile("hlt");
    }
}
struct mmap_info {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

extern char FREEMEMORY_START[1];

void setup_frame_allocator(struct multiboot_info* multiboot) {
    printhex(multiboot->flags);
    println(" flags");
    printhex(multiboot->mem_lower);
    println(" mem_lower");
    printhex(multiboot->mem_upper);
    println(" mem_upper");
    printhex(multiboot->boot_device);
    println(" boot_device");
    printhex(multiboot->cmdline);
    println(" cmdline");
    printhex(((uint64_t)multiboot->syms[0]<<32ULL) + multiboot->syms[1]);
    printhex(((uint64_t)multiboot->syms[2]<<32ULL) + multiboot->syms[3]);
    println(" syms");
    printhex(multiboot->mmap_length);
    println(" mmap_length");
    printhex(multiboot->mmap_addr);
    println(" mmap_addr");
    
    char* mmap_startb = (char*) (uintptr_t)multiboot->mmap_addr;
    char* mmap_endb = mmap_startb + multiboot->mmap_length;
    struct mmap_info* mmap_biggest_map = NULL;

    while(mmap_startb < mmap_endb) {
        struct mmap_info* mmap_entry = (struct mmap_info*) mmap_startb;
        printhex(mmap_entry->base_addr);
        print(" ");
        printhex(mmap_entry->length);
        print(" ");
        printhex(mmap_entry->type);
        println("");
        if (mmap_entry->type == 1 &&
            (mmap_biggest_map == NULL || mmap_entry->length > mmap_biggest_map->length)) {
            mmap_biggest_map = mmap_entry;
        }
        mmap_startb += mmap_entry->size + sizeof (uint32_t);
    }
    uintptr_t bigarea_start = (uintptr_t)FREEMEMORY_START;
    // bigarea_start = ((bigarea_start + 0x200000 - 1) / 0x200000) * 0x200000;
    if (mmap_biggest_map->base_addr > bigarea_start) {
        bigarea_start = mmap_biggest_map->base_addr;
    }
    uintptr_t bigarea_end = mmap_biggest_map->base_addr + mmap_biggest_map->length;
    print("bigger memory area starts at: ");
    printhex(bigarea_start);
    print(" ends at: ");
    printhex(bigarea_end);
    println("");

    init_frame_allocator(bigarea_start, bigarea_end);
}


noreturn void cstart(struct multiboot_info* multiboot);
noreturn void cstart(struct multiboot_info* multiboot) {
    load_exceptions(); // load default CPU exception handlers into IDT
    load_interrupts(); // load hardware interrupt handlers into IDT and configure PIC
    enable_interrupts();
    setup_frame_allocator(multiboot);

    init_scheduler();

    //test_scheduler();
    idle_loop();
}
