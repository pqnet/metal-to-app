
#include <stdint.h>
#include "address_space.h"

void* load_elf(char* start, char* end, struct pagetable_entry* memory_space);