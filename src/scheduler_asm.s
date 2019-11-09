/**
 * case 1. I'm in kernel, and I want to switch to a specific task // NOT SUPPORTED
 * case 2. I'm in the scheduler, and I want to switch to the next useful task
 * case 3. I'm preempted, and I need to switch to the scheduler task
 */

// assume asm_switch_task is called in SYS-V calling convention
// void switch_task(interrupt_frame* frame, task_switch_info* tsi)
.global asm_reschedule
asm_reschedule:
    push %rbp // rsp -=8
    mov %rsp, %rbp // use RBP to come back after "magic"
    // clear interrupt flag 
    push %rax
    push %rbx
    // do the magic
    // this will put the thread id in %rbx
    mov $0, %rbx
    // point %rbx to the start of current_task
    mov $current_task_table, %rax
    mov (%rax,%rbx, 0x8), %rax
    // stack now points to the end of current_task
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
    mov -0x10(%rbp), %rbx
    push %rbx
    // save rax
    mov -0x8(%rbp), %rbx
    push %rbx
    // save rbp
    mov 0x0(%rbp), %rbx
    push %rbx
    // save SS
    mov 0x8(%rbp), %rbx
    push %rbx
    // save RSP
    mov 0x10(%rbp), %rbx
    push %rbx
    // save RFLAGS
    mov 0x18(%rbp), %rbx
    push %rbx
    // save CS
    mov 0x20(%rbp), %rbx
    push %rbx
    // save RIP
    mov 0x28(%rbp), %rbx
    push %rbx
    // load new task
    mov %rbx, %rdi
    call scheduler
    // stack now points to the start of this task
    mov %rax, %rsp
    // load RIP
    pop %rbx
    mov %rbx, 0x28(%rbp)
    // load CS
    pop %rbx
    mov %rbx, 0x20(%rbp)
    // load RFLAGS
    pop %rbx
    mov %rbx, 0x18(%rbp)
    // load RSP
    pop %rbx
    mov %rbx, 0x10(%rbp)
    // load SS
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
    pop %rbp
    iret