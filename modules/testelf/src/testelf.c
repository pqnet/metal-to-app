#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../vdso/src/vdso.h"

void _start() {
  write(1, "hello syscall", 13);
  //abort();
  _exit(0);
}