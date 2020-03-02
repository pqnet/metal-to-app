QEMU='/mnt/c/Program Files/qemu/qemu-system-x86_64.exe'
QFLAGS=-m 1g -s -d cpu_reset -monitor stdio -display gtk,zoom-to-fit=on
QDIR=windows
test: $(QDIR)/img.iso
	cd $(QDIR) ; $(QEMU) $(QFLAGS) -cdrom img.iso