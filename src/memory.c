#include "memory.h"

#include <stdalign.h>

#include "interrupts.h"

alignas(0x1000) struct pagetable_entry kernel_pml4[512];
alignas(0x1000) struct pagetable_entry kernel_pagedir[512];
alignas(0x1000) struct pagetable_entry kernel_pagedirtable[512];
alignas(0x1000) struct pagetable_entry kernel_linear[512];
linear_address kernel_address_space;

/*
static inline void reload_tlb(linear_address a) {
    asm volatile("INVLPG (%0)"::"r"(a): "memory");
}
*/
void* linearAddressToPtr(linear_address a) {
    return (char*)LINEAR_START + a;
}

linear_address pointerToLinearAddress(void* ptr) {
    if (ptr > (void*)KERNEL_BASE  && ptr < DYNAMIC_START) {
        return (char*)ptr - KERNEL_BASE;
    } else if (ptr > LINEAR_START && ptr < LINEAR_END) {
        return (char*)ptr - (char*)LINEAR_START;
    } else {
        // TODO do linear lookup
        panic();
    }
}

void createKernelAddressSpace() {
    kernel_address_space = pointerToLinearAddress(kernel_pml4); 
    for (unsigned i = 0; i < 512; i++) {
        kernel_linear[i].address = i * addrshift1G;
        kernel_linear[i].present = true;
        kernel_linear[i].writable = true;
        kernel_linear[i].big = true;
    }
    for (unsigned i = 0; i < 512; i++) {
        kernel_pagedirtable[i].address = 0;
    }
    kernel_pagedirtable[510].address = pointerToLinearAddress(kernel_pagedir);
    kernel_pagedirtable[510].present = true;
    kernel_pagedirtable[510].writable = true;
    kernel_pagedirtable[510].usermode = true;

    for (unsigned i = 0x00; i < 0x10; i++) {
        kernel_pagedir[i].address = i * addrshift2M;
        kernel_pagedir[i].present = true;
        kernel_pagedir[i].writable = false;
        kernel_pagedir[i].big = true;
        kernel_pagedir[i].usermode = false;
    }
    for (unsigned i = 0x10; i < 0x200; i++) {
        kernel_pagedir[i].address = 0;
    }

    for (unsigned i = 0x08; i < 0x10; i++) {
        kernel_pagedir[i].writable = true;
        kernel_pagedir[i].usermode = false;
    }
    kernel_pagedir[0].writable = true;
    kernel_pagedir[0].usermode = false;
    // TODO do not allow usermode processes to write here
    // pdkernel[0].usermode = false;

    init_address_space(kernel_pml4);
    asm volatile("mov %0, %%cr3"::"r"(kernel_address_space));
}

void init_address_space(struct pagetable_entry root_pagetable[]) {
    for (int i = 0; i < 512; i++) {
        root_pagetable[i].address = 0;
    }
    // map linear addresses into entry 256
    root_pagetable[256].address = pointerToLinearAddress(kernel_linear);
    root_pagetable[256].present = true;
    root_pagetable[256].writable = true;
    // map kernel memory into entry 511
    root_pagetable[511].address = pointerToLinearAddress(kernel_pagedirtable);
    root_pagetable[511].present = true;
    root_pagetable[511].writable = true;
    root_pagetable[511].usermode = true;
}