#include <stdint.h>

// a frame is a 4k physical page
uintptr_t frame_alloc();
void frame_dealloc(uintptr_t frame_address);
void init_frame_allocator(uintptr_t fm_start, uintptr_t fm_end);