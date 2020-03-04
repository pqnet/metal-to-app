/**
 * case 1. I'm in kernel, and I want to switch to a specific task // NOT SUPPORTED
 * case 2. I'm in the scheduler, and I want to switch to the next useful task
 * case 3. I'm preempted, and I need to switch to the scheduler task
 */
.global asm_get_cpuid
asm_get_cpuid:
    // (TODO)
    mov $0, %rax
    ret
.global asm_reschedule
// This is more complicated than it needs to be, saving everything because it could be called in preemption.
// We might write a faster version that only saves/restores the bare minimum when the thread is yielding voluntarily
// interrupt calling convention
asm_reschedule:
    push %rbp // rsp -=8
    mov %rsp, %rbp // use RBP to come back after "magic"
    push %rax
    push %rbx
    // ensure DS = SS = kernel segment
    mov %ss, %ax
    mov %ax, %ds
    // do the magic
    // this will put the thread id in %rbx
    call asm_get_cpuid
    mov %rax, %rbx
    // point %rbx to the start of current_task
    mov $current_task_table, %rax
    mov (%rax,%rbx, 0x8), %rax
    // stack now points to the end of current_task's registers
    lea 0xa0(%rax), %rsp
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    // save rbx
    mov -0x10(%rbp), %rax
    push %rax
    // save rax
    mov -0x8(%rbp), %rax
    push %rax
    // save rbp
    mov 0x0(%rbp), %rax
    push %rax
    // save RIP
    mov 0x08(%rbp), %rax
    push %rax
    // save CS
    mov 0x10(%rbp), %rax
    push %rax
    // save RFLAGS
    mov 0x18(%rbp), %rax
    push %rax
    // save RSP
    mov 0x20(%rbp), %rax
    push %rax
    // save SS
    mov 0x28(%rbp), %rax
    push %rax
    // restore a sane stack for calling scheduler
    mov %rbp, %rsp
    // load new task
    mov %rbx, %rdi
    call scheduler // calling convention: first argument %rdi, return value %rax
    // stack now points to the start of this task
    mov %rax, %rsp
    // i have to load the new address space into CR3
    mov 0xa8(%rax), %rbx // WARNING: this lookup assumes the current address space have the new task mapped in
    mov %rbx, %cr3
    // load SS
    pop %rbx
    mov %rbx, 0x28(%rbp)
    // load RSP
    pop %rbx
    mov %rbx, 0x20(%rbp)
    // load RFLAGS
    pop %rbx
    mov %rbx, 0x18(%rbp)
    // load CS
    pop %rbx
    mov %rbx, 0x10(%rbp)
    // load RIP
    pop %rbx
    mov %rbx, 0x8(%rbp)
    // load rbp
    pop %rbx
    mov %rbx, 0x0(%rbp)
    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    // magic is done, let's go back to sanity
    mov %rbp, %rsp
    // use RBP as scratch register, to load DS
    mov %rax, %rbp
    mov 0x28(%rsp), %rax // assume DS = SS in the interrupt stack frame
    mov %rax, %ds
    mov %rbp, %rax
    pop %rbp
    iretq