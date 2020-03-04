#include "memory.h"

#include <stdalign.h>

#include "interrupts.h"

extern char KERNEL_BASE[1];

alignas(0x1000) struct pagetable_entry pml4[512];
alignas(0x1000) struct pagetable_entry pdkernel[512];
alignas(0x1000) struct pagetable_entry pdpthigh[512];
alignas(0x1000) struct pagetable_entry pdptlinear1[512];
linear_address kernel_address_space;

static void * const LINEAR_START = (void*)0xFFFF800000000000;
const linear_address addrshift4K = 1UL<<12; 
const linear_address addrshift2M = addrshift4K<<9;
const linear_address addrshift1G = addrshift2M<<9;
const linear_address addrshift512G = addrshift1G<<9;
static void * const LINEAR_END = LINEAR_START + addrshift512G;

static void * const DYNAMIC_START = KERNEL_BASE + addrshift1G;

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
    kernel_address_space = pointerToLinearAddress(pml4); 
    for (unsigned i = 0; i < 512; i++) {
        pdptlinear1[i].address = i * addrshift1G;
        pdptlinear1[i].present = true;
        pdptlinear1[i].writable = true;
        pdptlinear1[i].big = true;
    }
    for (unsigned i = 0; i < 512; i++) {
        pdpthigh[i].address = 0;
    }
    pdpthigh[510].address = pointerToLinearAddress(pdkernel);
    pdpthigh[510].present = true;
    pdpthigh[510].writable = true;
    pdpthigh[510].usermode = true;

    for (unsigned i = 0x00; i < 0x10; i++) {
        pdkernel[i].address = i * addrshift2M;
        pdkernel[i].present = true;
        pdkernel[i].writable = false;
        pdkernel[i].big = true;
        pdkernel[i].usermode = false;
    }
    for (unsigned i = 0x10; i < 0x200; i++) {
        pdkernel[i].address = 0;
    }

    for (unsigned i = 0x08; i < 0x10; i++) {
        pdkernel[i].writable = true;
        pdkernel[i].usermode = false;
    }
    pdkernel[0].writable = true;
    pdkernel[0].usermode = false;
    // TODO do not allow usermode processes to write here
    // pdkernel[0].usermode = false;

    init_address_space(pml4);
    asm volatile("mov %0, %%cr3"::"r"(kernel_address_space));
}

void init_address_space(struct pagetable_entry root_pagetable[]) {
    for (int i = 0; i < 512; i++) {
        root_pagetable[i].address = 0;
    }
    // map linear addresses into entry 256
    root_pagetable[256].address = pointerToLinearAddress(pdptlinear1);
    root_pagetable[256].present = true;
    root_pagetable[256].writable = true;
    // map kernel memory into entry 511
    root_pagetable[511].address = pointerToLinearAddress(pdpthigh);
    root_pagetable[511].present = true;
    root_pagetable[511].writable = true;
    root_pagetable[511].usermode = true;
}