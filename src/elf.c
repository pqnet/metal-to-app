#include "elf.h"
#include "frame.h"
#include <string.h>

void copy_section(struct pagetable_entry* memory_space, char* vaddr, char* section_start, ptrdiff_t memsz, ptrdiff_t filesz) {
  while(memsz > 0) {
    size_t dest_offset = ((uintptr_t)vaddr) % DEFAULT_FRAME_SIZE;
    char* aligned_dest = vaddr - dest_offset;

    linear_address new_frame = frame_alloc_zero();
    mmap(memory_space, new_frame, aligned_dest, true);

    size_t to_copy = filesz + dest_offset > DEFAULT_FRAME_SIZE ?  DEFAULT_FRAME_SIZE - dest_offset : filesz;
    if (to_copy > 0 ) {
      memcpy(linearAddressToPtr(new_frame + dest_offset), section_start, to_copy);
      section_start = section_start + to_copy;
      filesz -= to_copy;
    }
    vaddr = aligned_dest + DEFAULT_FRAME_SIZE;
    memsz = memsz + dest_offset - DEFAULT_FRAME_SIZE;
  }
}

void *load_elf(char *start, char *end, struct pagetable_entry *memory_space)
{
  void *entry = (void *)*(uint64_t *)(start + 0x18);
  ptrdiff_t programHeaderOffset = *(uint64_t *)(start + 0x20);
  size_t programHeaderSize = *(uint16_t *)(start + 0x36);
  size_t programHeaderCount = *(uint16_t *)(start + 0x38);
  for (size_t i = 0; i < programHeaderCount; i++)
  {
    char *programHeader = start + programHeaderOffset + (programHeaderSize * i);
    uint32_t headerType = *(uint32_t *)(programHeader + 0x0);

    // uint32_t headerFlags = *(uint32_t *)(programHeader + 0x4);
    uint64_t off = *(uint64_t *)(programHeader + 0x8);
    char *vaddr = (char *)*(uint64_t *)(programHeader + 0x10);
    // uint32_t paddr = *(uint64_t*)(programHeader + 0x18);
    uint64_t filesz = *(uint64_t *)(programHeader + 0x20);
    uint64_t memsz = *(uint64_t *)(programHeader + 0x28);
    // uint64_t align = *(uint64_t *)(programHeader + 0x30);

    if (headerType == 1)
    {
      copy_section(memory_space, vaddr, start + off, memsz, filesz);
    }
  }
  return entry;
}
