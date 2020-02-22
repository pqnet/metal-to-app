#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

#include "memory.h"

struct pagetable_entry* create_address_space();
static inline void destroy_address_space(struct pagetable_entry* address_space) {};

// maps one page of memory into an address
void mmap(struct pagetable_entry* pagetable_root, linear_address page_start, void* mapping_start, bool usermode);
linear_address munmap(struct pagetable_entry* pagetable_root, void* mapping_start);

#endif // ADDRESS_SPACE_H