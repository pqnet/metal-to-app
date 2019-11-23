.section .multiboot, "d"
.int 0x1BADB002
.int 3
.int 0xE4524FFE - 3 // 0xE4524FFE = -0x1BADB002 with 2 complement

.section .text
.align 8,0
MULTIBOOT_INFO:
.quad 0
.code32
.globl _start
_start:
    mov %ebx, MULTIBOOT_INFO
    // disable paging (not necessary, grub does it for us)
    mov %cr0, %eax
    andl $0x7fffffff, %eax
    mov %eax, %cr0
    // enable PAE
    mov %cr4, %eax
    orl $1<<5, %eax
    mov %eax, %cr4
    // configure 4-level paging
    movl $PML4, %eax
    mov %eax, %cr3
    // enable long mode addressing
    mov $0xC0000080, %ecx // EFER MSR address 
    rdmsr
    orl $1 << 8, %eax
    wrmsr
    // enable paging
    mov %cr0, %eax
    or $1<<31, %eax
    mov %eax, %cr0
    // we are now in compatibility mode
    // load new GDT
    mov $GDTR, %eax
    lgdtl (%eax)
    mov $(DS32 - GDT), %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    jmpl $(CS64 - GDT),$bootstrap64
.code64
bootstrap64:
    mov MULTIBOOT_INFO, %rdi
    mov $.stack_bottom, %rbp
    mov %rbp, %rsp
    // load new IDT
    mov $IDTR, %rax
    lidt (%rax)
    movabs $cstart, %rbx
    call *%rbx
go32:
    // disable interrupts
    cli
    // disable paging
    mov %cr0, %rax
    mov $0xffffffff7fffffff, %rbx
    andq %rbx, %rax
    mov %rax, %cr0
.code32
    hlt

.section .data, "w"
.align 0x1000,0
.globl GDT;
GDT:
// first descriptor in GDT is not used
.quad 0
CS32:
// type code/data  presence limit 64bit
.int 0xffff, 8<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<22 + 1<<23
.globl CS64;
CS64:
.int 0xffff, 8<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<21 + 1<<23
DS32: 
.int 0xffff, 2<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<22 + 1<<23
.align 0x1000,0
//.quad 0,0,0,0
ENDGDT:
IDT:
.rept 256
.int 0, 0, 0, 0
.endr
ENDIDT:
.globl IDT;
.align 0x1000,0

// align the GDTR to the second word of the second int
.align 8
.int 0
.short 0
GDTR:
.short (ENDGDT - GDT) - 1
.int GDT

// align the IDTR to the second word of the second int
.align 8
.int 0
.short 0
IDTR:
.short (ENDIDT - IDT) - 1
.int IDT


// page tables
.align 0x1000,0
PML4:
.quad PDPT + 1 + 2 // first 512 gb 
.rept 510
.quad 0
.endr
.quad PDPT2 + 1 + 2 // last 512 gb
.align 0x1000,0
PDPT: // covers first 512 gb of addresses
.quad PD + 1 + 2
.align 0x1000,0
PDPT2: // covers last 512 gb of addresses
.rept 510
.quad 0
.endr
.quad PD + 1 + 2 // 1gb 
.align 0x1000,0
PD: // covers 1 gb of addresses
.quad 0x0 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x1 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x2 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x3 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x4 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x5 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x6 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x7 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x8 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0x9 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xa * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xb * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xc * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xd * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xe * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 0xf * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.align 0x1000,0
