#include "multiboot.h"
#include "frame.h"
#include "stddef.h"
#include "print.h"

extern char FREEMEMORY_START[1];
extern char KERNEL_BASE[1];
void setup_frame_allocator(struct multiboot_info* multiboot) {
    // printhex(multiboot->flags);
    // println(" flags");
    // printhex(multiboot->mem_lower);
    // println(" mem_lower");
    // printhex(multiboot->mem_upper);
    // println(" mem_upper");
    // printhex(multiboot->boot_device);
    // println(" boot_device");
    // printhex(multiboot->cmdline);
    // println(" cmdline");
    // printhex(((uint64_t)multiboot->syms[0]<<32ULL) + multiboot->syms[1]);
    // printhex(((uint64_t)multiboot->syms[2]<<32ULL) + multiboot->syms[3]);
    // println(" syms");
    // printhex(multiboot->mmap_length);
    // println(" mmap_length");
    // printhex(multiboot->mmap_addr);
    // println(" mmap_addr");
    
    char* mmap_startb = (char*) (uintptr_t)multiboot->mmap_addr + (uintptr_t)KERNEL_BASE;
    char* mmap_endb = mmap_startb + multiboot->mmap_length;
    struct mmap_info* mmap_biggest_map = NULL;

    while(mmap_startb < mmap_endb) {
        struct mmap_info* mmap_entry = (struct mmap_info*) mmap_startb;
        // printhex(mmap_entry->base_addr);
        // print(" ");
        // printhex(mmap_entry->length);
        // print(" ");
        // printhex(mmap_entry->type);
        // println("");
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
    // print("bigger memory area starts at: ");
    // printhex(bigarea_start);
    // print(" ends at: ");
    // printhex(bigarea_end);
    // println("");

    init_frame_allocator(bigarea_start, bigarea_end);
}