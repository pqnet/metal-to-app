#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

struct mmap_info {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));


struct multiboot_info {
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

void setup_frame_allocator(struct multiboot_info* multiboot);
#endif // MULTIBOOT_H