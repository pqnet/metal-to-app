// .section .multiboot
.align 8
MULTIBOOT:
// 0x4000D8
.int 0x1BADB002
// 0x4000DC
.int 3
// 0x4000E0
.int 0xE4524FFE - 3 // 0xE4524FFE = -0x1BADB002 with 2 complement 
// .section .text
HELLO: // 0x4000E4
//.ascii "H\x0fe\x0fl\x0fl\x0fo\x0f,\x0f \x0fw\x0fo\x0fr\x0fl\x0fd\x0f!\x0f\n\x0f\0"
.ascii "hello world!\n\0\0\0"

.globl _start
_start:
    .code32
    jmp go64
_hello32:
    movl $HELLO, %esi
    movl $0xB8000, %edi
loop:
    xor %eax, %eax
    movb (%esi), %al
    movb %al, (%edi)
    or %al, %al
    jz halt
    inc %esi
    inc %edi
    movb $0xf, %al
    movb %al, (%edi)
    inc %edi
    jmp loop
halt:
    hlt
go64:
    // disable paging (not necessary, grub does it for us)
    mov %cr0, %eax
    andl $0x7fffffff, %eax
    mov %eax, %cr0
    // enable PAE
    mov %cr4, %eax
    orl $1<<5, %eax
    mov %eax, %cr4
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
    jmpl $(CS64 - GDT),$_start64
return32:
    hlt
.code64
_start64:
_hello64:
    movq $HELLO, %rsi
    movq $0xB8010, %rdi
loop64:
    xor %eax, %eax
    movb (%rsi), %al
    movb %al, (%rdi)
    or %al, %al
    jz halt64
    inc %rsi
    inc %rdi
    movb $0xf, %al
    movb %al, (%rdi)
    inc %rdi
    jmp loop64
halt64:
    // hlt
go32:
    // disable paging
    mov %cr0, %rax
    mov $0xffffffff7fffffff, %rbx
    andq %rbx, %rax
    mov %rax, %cr0
.code32
    // mov (($CS32 - $GDT)/8), %al
    // jmpl (($CS32 - $GDT)>>$3),$_start
    jmpl $(CS32 - GDT),$_hello32
.section .data, "w"
.align 0x200000
GDT:
// first descriptor in GDT is not used
.quad 0
CS32:
// type code/data  presence limit 64bit
.int 0xffff, 8<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<22 + 1<<23
CS64:
.int 0xffff, 8<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<21 +1<<23
DS32: 
.int 0xffff, 2<<8 + 1<<12 + 1<<15 + 0xf <<16 + 1<<22 + 1<<23
.align 0x1000,0
.quad 0,0,0,0
ENDGDT:
// align the GDTR to the second word
.align 4
.short 0
GDTR:
.short (ENDGDT - GDT) - 1
.int GDT

.align 0x1000,0
PML4:
.quad PDPT + 1 + 2

.align 0x1000,0
PDPT:
.quad PD + 1 + 2

.align 0x1000,0
PD:
.quad 0 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 1 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 2 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 3 * 0x200000 + 1 + 0 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 4 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 5 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 6 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 7 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 8 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.quad 9 * 0x200000 + 1 + 2 + 0 + 0 + 0 + 0 + 0 + 1<<7 + 0
.align 0x200000,0
stack_begin:
