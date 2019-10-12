#include <stddef.h>
#include <stdint.h>
#include "print.h"
#include "interrupts.h"

struct multiboot_info{
    uint32_t flags;
};

void cstart(struct multiboot_info* multiboot) {
    print("Hello ");
    println("World!");
    load_interrupts();
    asm volatile("int $200");
    asm volatile("hlt");
    //cstart(multiboot);
    //while (1)
    //{
    //}
    
}