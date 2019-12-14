#include <stddef.h>
#include <stdalign.h>
#include "frame.h"
#include "interrupts.h"
#include "memory.h"

linear_address freemem_start;
linear_address freemem_end; // first non-free address after last page

alignas(0x1000)
struct free_page {
    struct free_page* nextFreePage;
};
struct free_page* freelist;

void init_frame_allocator(linear_address fm_start, linear_address fm_end) {
    freemem_start = ((fm_start + 0xfff) / 0x1000)* 0x1000;
    freemem_end = (fm_end / 0x1000) * 0x1000;
    freelist = NULL;
}

linear_address frame_alloc() {
    if (freelist !== NULL) {
        struct free_page* freePage = freelist;
        freelist = freePage->nextFreePage;
        return pointerToLinearAddres(freePage);
    }
    if (freemem_start == freemem_end) {
        panic();
    }
    linear_address ret = freemem_start;
    freemem_start += 0x1000;
    return ret;
}

void frame_dealloc(linear_address frame_address) {
    struct free_page* freePage = linearAddressToPtr(frame_address);
    freePage->nextFreePage = freelist;
    freelist = freePage;
}