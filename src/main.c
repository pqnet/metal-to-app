#include <stddef.h>
#include <stdint.h>
#include "print.h"
#include "interrupts.h"

struct multiboot_info{
    uint32_t flags;
};


void idle_loop() __attribute__((noreturn));
void idle_loop() {
    for(;;) {
        asm volatile("hlt");
    }
}

void cstart(struct multiboot_info* multiboot)  __attribute__((noreturn));
void cstart(struct multiboot_info* multiboot) {
    print("Hello ");
    println("World!");
    load_interrupts();
    asm volatile("int $200");
    idle_loop();
}