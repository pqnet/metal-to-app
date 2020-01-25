#include "initrd.h"
#include "print.h"
#include "memory.h"

void setup_initrd(struct multiboot_info* multiboot, struct initrd_info* out_info) {
    print("mod count: ");
    printhex(multiboot->mods_count);
    println("");
    struct multiboot_boot_module_info* moduleInfo = linearAddressToPtr(multiboot->mods_addr);
    for (int i = 0; i < multiboot->mods_count; i++) {
      print("mod addr:");
      char* start = linearAddressToPtr(moduleInfo[i].mod_start);
      char* end = linearAddressToPtr(moduleInfo[i].mod_end);
      char* command_line = linearAddressToPtr(moduleInfo[i].string);
      *end='\0';
      printhex((uintptr_t)start);
      print(", ");
      printhex((uintptr_t)end);
      print(" : ");
      print(command_line);
      print(" : ");
      print(" \"");
      print(start);
      println("\"");
      out_info->start = start;
      out_info->end = end;
      out_info->command_line = command_line;
    }
}