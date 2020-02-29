
.global syscall_entry_asm
syscall_entry_asm:
  push %rcx
  push %r11
  mov %r10, %rcx
  mov syscall_table(,%rax,8), %rax
  call *%rax
  pop %r11
  pop %rcx
  sysretq