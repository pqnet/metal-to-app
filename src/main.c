#include <stddef.h>
#include <stdint.h>
#include "print.h"
#include "interrupts.h"
#include "exceptions.h"

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
    load_exceptions(); // load default CPU exception handlers into IDT
    load_interrupts(); // load hardware interrupt handlers into IDT and configure PIC
    // enable_interrupts();
    keyboard_test_loop();

    idle_loop();
}