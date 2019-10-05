OUTPUT_FORMAT(elf64-x86-64)
/*OUTPUT_FORMAT(elf32-x86-64) */
SECTIONS {
    /* kernel starts at -2gb  = - 80000000*/
    .KERNEL_BASE = 0xffffffff80000000;
    ENTRY(_start)
    . = 0x100000;
    .text0 : {
        helloworld.o(.multiboot)
        helloworld.o(.text)
    }
    . = 8 * 0x200000;
    .data0 : { helloworld.o(.data) }
    .bss0 : { helloworld.o(.bss) }
    . = .KERNEL_BASE + ADDR(.text0);
    . = ALIGN(0x200000);
    .text : AT(ADDR(.text) - .KERNEL_BASE)  {
        *(.text*)
    }
    . = .KERNEL_BASE + ADDR(.bss0);
    . = ALIGN(0x200000);
    .rodata : AT(ADDR(.rodata) - .KERNEL_BASE) {
        *(.rodata*)
    }
    .data : AT(ADDR(.data) - .KERNEL_BASE) {
        *(.data*)
    }
    .bss : AT(ADDR(.bss) - .KERNEL_BASE){
        *(.bss*)
    }
}