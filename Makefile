windows/img.iso: helloworld iso/boot/grub/grub.cfg
	cp helloworld iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o windows/img.iso iso

helloworld: helloworld.s
	clang-9 -g -nostdlib -Wl,-z,max-page-size=4096 -Wl,-n -Wl,--build-id=none $^ -o $@
 
