#include <stddef.h>
#include <stdalign.h>
#include "frame.h"
#include "interrupts.h"
#include "memory.h"

alignas(0x1000) struct free_frame
{
    union {
        struct free_frame *nextFreePage;
        uint8_t memory[0x1000];
    };
};
struct free_frame *freemem_start;
struct free_frame *freemem_end; // first non-free address after last page
struct free_frame *freelist;

void init_frame_allocator(linear_address fm_start, linear_address fm_end)
{
    freemem_start = linearAddressToPtr(
        ((fm_start + 0xfff) / 0x1000) * 0x1000);
    freemem_end = linearAddressToPtr(
        (fm_end / 0x1000) * 0x1000);
    freelist = NULL;
}
static inline struct free_frame *frame_alloc_internal()
{
    if (freelist != NULL)
    {
        struct free_frame *freePage = freelist;
        freelist = freePage->nextFreePage;
        return freePage;
    }
    if (freemem_start == freemem_end)
    {
        panic();
    }
    return freemem_start++;
}

linear_address frame_alloc()
{
    return pointerToLinearAddres(frame_alloc_internal());
}
linear_address frame_alloc_zero()
{
    struct free_frame *frame = frame_alloc_internal();
    for (int i = 0; i < 0x1000; i++)
    {
        frame->memory[i] = 0;
    }
    return pointerToLinearAddres(frame);
}

void frame_dealloc(linear_address frame_address)
{
    struct free_frame *freePage = linearAddressToPtr(frame_address);
    freePage->nextFreePage = freelist;
    freelist = freePage;
}