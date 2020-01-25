#ifndef PRINT_H
#define PRINT_H
#include <stdint.h>

extern void print(char message[]);
extern void println(char message[]);

static inline void printhex(uint64_t number) {
    char buffer[2 + 16 + 1] = "0x";
    buffer[2 + 16] = '\0';

    for(int n = 0; n < 16; n++) {
        char nibbleN = (number >> (4 * (15-n)) ) & 0xf;
        buffer[2+n] = nibbleN + '0';
        if(nibbleN > 9) {
            buffer[2+n] = nibbleN - 10 + 'a';
        }
    }
    print(buffer);
}
#endif // PRINT_H