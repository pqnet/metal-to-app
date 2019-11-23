#include "memory.h"

extern char KERNEL_BASE[1];
extern char PD[1];
extern char PDPT[1];
extern char PDPT2[1];
extern char PML4[1];
#define pd ((struct pagetable_entry*)(PD+(uintptr_t)KERNEL_BASE))
#define pdpt ((struct pagetable_entry*)(PDPT+(uintptr_t)KERNEL_BASE))
#define pdpt2 ((struct pagetable_entry*)(PDPT2+(uintptr_t)KERNEL_BASE))
#define pml4 ((struct pagetable_entry*)(PML4+(uintptr_t)KERNEL_BASE))
/*
static inline void reload_tlb(linear_address a) {
    asm volatile("INVLPG (%0)"::"r"(a): "memory");
}
*/
void* linearAddressToPtr(linear_address a) {
    return NULL;
}

linear_address pointerToLinearAddres(void* ptr) {
    return 0;
}

void forgetLinearAddresses() {
    pml4->address = 0;
    asm volatile("mov %0, %%cr3"::"r"(PML4));
}