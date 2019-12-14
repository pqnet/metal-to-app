#include "address_space.h"
#include "frame.h"
struct pagetable_entry *resolve_pageentry(struct pagetable_entry *root, uint64_t ptr)
{
    // 0-11 bit linear
    // 12-20 bit l1 entry
    // 21-29 bit l2 entry
    // 30-38 bit l3 entry
    // 39-47 bit l4 entry
    int l4_entry_idx = (ptr >> 39) & 0x1ff;
    int l3_entry_idx = (ptr >> 30) & 0x1ff;
    int l2_entry_idx = (ptr >> 21) & 0x1ff;
    int l1_entry_idx = (ptr >> 12) & 0x1ff;
    struct pagetable_entry *l4_entry = root + l4_entry_idx;
    if (l4_entry->big)
    {
        return l4_entry;
    }
    if (!l4_entry->present) {
        l4_entry->address = frame_alloc_zero();
        l4_entry->present = true;
        l4_entry->writable = true;
    }
    struct pagetable_entry *l3_entry =
        (struct pagetable_entry *)linearAddressToPtr((l4_entry->address >> 12) << 12) + l3_entry_idx;
    if (l3_entry->big)
    {
        return l3_entry;
    }
    if (!l3_entry->present) {
        l3_entry->address = frame_alloc_zero();
        l3_entry->present = true;
        l3_entry->writable = true;
    }
    struct pagetable_entry *l2_entry =
        (struct pagetable_entry *)linearAddressToPtr((l3_entry->address >> 12) << 12) + l2_entry_idx;
    if (l2_entry->big)
    {
        return l2_entry;
    }
    if (!l2_entry->present) {
        l2_entry->address = frame_alloc_zero();
        l2_entry->present = true;
        l2_entry->writable = true;
    }
    struct pagetable_entry *l1_entry =
        (struct pagetable_entry *)linearAddressToPtr((l2_entry->address >> 12) << 12) + l1_entry_idx;

    return l1_entry;
}

void mmap(struct pagetable_entry *pagetable_root, linear_address page_start, void *mapping_start)
{
    struct pagetable_entry* entry = resolve_pageentry(pagetable_root, (uint64_t)mapping_start);
    entry->address = page_start;
    entry->present = true;
    entry->writable = true;
}

linear_address munmap(struct pagetable_entry *pagetable_root, void *mapping_start) {
    struct pagetable_entry* entry = resolve_pageentry(pagetable_root, (uint64_t)mapping_start);
    linear_address ret = (entry->address >> 12) << 12;
    entry->writable = false;
    entry->present = false;
    entry->address = 0;
    return ret;
}

struct pagetable_entry* create_address_space() {
    struct pagetable_entry* entry = linearAddressToPtr(frame_alloc_zero());
    init_address_space(entry);
    return entry;
}
