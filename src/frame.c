#include "frame.h"
#include "interrupts.h"

uintptr_t freemem_start;
uintptr_t freemem_end; // first non-free address after last page
uintptr_t freelist;

void init_frame_allocator(uintptr_t fm_start, uintptr_t fm_end) {
    freemem_start = fm_start;
    freemem_end = fm_end;
    freelist = 0;
}

uintptr_t frame_alloc() {
    if (freemem_start == freemem_end) {
        // panic!
        interrupt(200);
    }
    uintptr_t ret = freemem_start;
    freemem_start += 0x1000;
    return ret;
}

void frame_dealloc(uintptr_t frame_address) {
    // nop
}