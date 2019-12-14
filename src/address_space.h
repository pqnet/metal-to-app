#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

#include "memory.h"

struct pagetable_entry* create_address_space();

// maps one page of memory into an address
void mmap(struct pagetable_entry* pagetable_root, linear_address page_start, void* mapping_start);
linear_address munmap(struct pagetable_entry* pagetable_root, void* mapping_start);

#endif // ADDRESS_SPACE_H