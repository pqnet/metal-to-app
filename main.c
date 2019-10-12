#include <stddef.h>
#include <stdint.h>
struct multiboot_info{
    uint32_t flags;
};

extern int write(ptrdiff_t screen_offset, char* message);

static ptrdiff_t current_offset = 80;
void print(char message[]) {
    size_t writtenChars = write(current_offset, message);
    current_offset += writtenChars;
    current_offset = current_offset % (80 * 25);
}
void println(char message[]) {
    print(message);
    current_offset = (current_offset + 79)/80 * 80;
}
void cstart(struct multiboot_info* multiboot) {
    println("Hello");
    println("World!");
    cstart(multiboot);
}