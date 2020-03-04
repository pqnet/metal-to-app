#include <stdint.h>
#include "multiboot.h"

struct initrd_info {
  char* command_line;
  char* start;
  char* end;
};

int setup_initrd(struct multiboot_info* multiboot, struct initrd_info* out_info);