#include "map-dynamic.h"
#include "memory.h"
#include "frame.h"
#include "address_space.h"

bool handle_dynamic_fault(void *address)
{
  if ((intptr_t)address > (intptr_t)DYNAMIC_START && (intptr_t)address < 0)
  {
    const linear_address new_page = frame_alloc_zero();
    void *mapping_start = (void *)((intptr_t)address / DEFAULT_FRAME_SIZE * DEFAULT_FRAME_SIZE);
    mmap(linearAddressToPtr(kernel_address_space), new_page, mapping_start, false);
  }
  return false;
}