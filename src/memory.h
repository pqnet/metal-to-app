#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uintptr_t linear_address;

struct pagetable_entry
{
    union {
        linear_address address;
        struct
        {
            bool present : 1;
            bool writable : 1;
            bool usermode : 1;
            bool : 1;
            bool : 1;
            bool accessed : 1;
            bool dirty : 1;
            bool big : 1;
            uint32_t: 24;
            uint32_t: 32;
        } __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

static_assert(8 == sizeof(struct pagetable_entry), "pagetable entry not packed");

void createKernelAddressSpace();

void* linearAddressToPtr(linear_address a);
linear_address pointerToLinearAddres(void* ptr);

void init_address_space(struct pagetable_entry root_pagetable[]);

#endif // MEMORY_H
