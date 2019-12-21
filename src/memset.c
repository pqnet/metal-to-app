#include <string.h>

void* memset(void* memory, int value, size_t size) {
    for (size_t i = 0; i < size; i++)
    {
        ((unsigned char*)memory)[i] = value;
    }
    return memory;
}