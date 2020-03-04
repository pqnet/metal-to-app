#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uintptr_t linear_address;

extern char KERNEL_BASE[1];

static void * const LINEAR_START = (void*)0xFFFF800000000000;
static const linear_address addrshift4K = 1UL<<12; 
static const linear_address addrshift2M = addrshift4K<<9;
static const linear_address addrshift1G = addrshift2M<<9;
static const linear_address addrshift512G = addrshift1G<<9;
static void * const LINEAR_END = LINEAR_START + addrshift512G;

static void * const DYNAMIC_START = KERNEL_BASE + addrshift1G;

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
extern linear_address kernel_address_space;

void* linearAddressToPtr(linear_address a);
linear_address pointerToLinearAddress(void* ptr);

void init_address_space(struct pagetable_entry root_pagetable[]);

#endif // MEMORY_H
