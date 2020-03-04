#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../../vdso/src/vdso.h"

void _start(char *cmdline)
{
  char buf[32] = "hello my name is ";
  for (int i = 17; i < 31; i++)
  {
    int j = i - 17;
    buf[i] = cmdline[j];
    if (cmdline[j] == 0)
    {
      buf[i] = ' ';
      j--;
    }
  }
  buf[31] = '\0';
  while (1)
  {
    write(1, buf, 31);
    yield();
  }
  //abort();
  _exit(0);
}