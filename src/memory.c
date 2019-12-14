#include "memory.h"
#include "interrupts.h"

extern char KERNEL_BASE[1];
extern char PD[1];
extern char PDPT[1];
extern char PDPT2[1];
extern char PML4[1];
#define pdkernel ((struct pagetable_entry*)(PD+(uintptr_t)KERNEL_BASE))
#define pdptlinear1 ((struct pagetable_entry*)(PDPT+(uintptr_t)KERNEL_BASE))
#define pdpthigh ((struct pagetable_entry*)(PDPT2+(uintptr_t)KERNEL_BASE))
#define pml4 ((struct pagetable_entry*)(PML4+(uintptr_t)KERNEL_BASE))

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

linear_address pointerToLinearAddres(void* ptr) {
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
    pml4[0].address = 0;

    pml4[256].address = pointerToLinearAddres(pdptlinear1);
    pml4[256].present = true;
    pml4[256].writable = true;
    for (unsigned i = 0; i < 512; i++) {
        pdptlinear1[i].address = i * addrshift1G;
        pdptlinear1[i].present = true;
        pdptlinear1[i].writable = true;
        pdptlinear1[i].big = true;
    }
    asm volatile("mov %0, %%cr3"::"r"(PML4));
}

void init_address_space(struct pagetable_entry root_pagetable[]) {
    // map linear addresses into entry 256
    root_pagetable[256].address = pointerToLinearAddres(pdptlinear1);
    root_pagetable[256].present = true;
    root_pagetable[256].writable = true;
    // map kernel memory into entry 511
    root_pagetable[511].address = pointerToLinearAddres(pdpthigh);
    root_pagetable[511].present = true;
    root_pagetable[511].writable = true;
}